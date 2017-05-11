#include <defMacro>
#include <TModuleEmployees>
#include <TModulePlans>
#include <QMessageBox>
#include <QtCore>
#include <QProgressDialog>
#include <QProgressBar>

// Терминология:
// Номер рабочего часа года (м.б. от 0 до 365*8). В году 365 суток по 8 часов
// Номер дня года (м.б. от 0 до 365)
// Астрономическое время в формате QDateTime. Минуты и секунды игнорируются. Всегда равны 0
// Номер рабочего часа суток (м.б. от 0 до 7). Количество рабочих часов в сутках = kolhours
// Начало рабочего дня beginworkday (по - умолчанию = 9 часов)
// Рабочий день по - умолчанию с 9:00 до 17:00. 8 рабочих часов начинаются в: 9, 10, 11, 12, 13, 14, 15, 16 часов
// Минимально разрешенное время начала продукции - гарантированное время, раньше которого выполнение продукции не начнется. Позже - может.
// Максимально разрешенное время конца продукции - время, до которого продукции должна быть выполнена. Но это не гарантируется
// В объекте tasks времена начала и конца всегда хранятся, как НАЧАЛО астрономического часа. Минуты и секунды не участвуют
// Прямое планирование (обычное), когда работы планируются слева - направо (найти начало, а затем конец). М.б. задано минимально разрешенное
//время начала продукции
// Обратное планирование, когда работы планируются справа - налево. Сначала находится конец работы, а затем начало работы. Используется только
//тогда, когда задано максимальное время конца продукции
// Планирование проводится в условных единицах времени. Поэтому, при длительности работы 1 единица, время начала работы м.б. равно времени конца

// объявить глобальные переменные (задать неименованное пространство имен. Скрыть видимость переменных из вне)
namespace {

    int   tbeginplan      = 0;       // номер рабочего часа года (начиная с 0), с которого начнется планирования
    int   numerror        = 0;       // порядковый номер ошибки для сообщения
    int   kolworkhoursday = 8;       // количество рабочих часов в сутках
    int   beginworkday    = 9;       // начало рабочего дня 9:00. Первый рабочий час будет начинаться в 9:00. Последний, 8-ой
                                     //рабочий час будет начинаться в (beginworkday + kolhours - 1) в 16:00
    QDate beginyearplan;             // начало года планирования
    QProgressDialog* pprd;           // ProgressDialog
    QProgressBar*    pprb;           // ProgressBar
    QString begin         = "begin"; // константы для удобства вызова функций
    QString end           = "end";   // константы для удобства вызова функций
    int   flprint         = 3;       // флаг вывода отладочной информации в консоль: 0-ничего; 1-ошибки,план; 2-ошибки,план,job; 3-ошибки,план,job,tasks

    struct executor_type { // структура исполнителя
        int  id;           // id исполнителя
        bool external;     // исполнитель внешний (true) или внутренний (false)
        QVector<int>rest;  // вектор нерабочих ЧАСОВ исполнителя (отпуск, командировка, праздничные и вых. дни). Начиная с 0
    };

    struct job_type { // структура работы
        int                    id;           // id работы
        bool                   changes;      // true - признак того, что продукция была изменена. Хранится в начальной работе продукции
        int                    priority;     // приоритет проекта, которому принадлежит работа. 0 - низший приоритет (по - умолчанию)
        int                    projectnum;   // номер проекта, которому принадлежит работа (только для формирования сообщения об ошибке)
        int                    productnum;   // номер продукции, которой принадлежит работа (только для формирования сообщения об ошибке)
        int                    procedurenum; // номер процедуры, которой принадлежит работа (только для формирования сообщения об ошибке)
        int                    num;          // номер работы (только для формирования сообщения об ошибке)
        int                    beginmin;     // минимально разрешенное время (рабочий час года) начала работы (из "Планировать проект с...") (от 0 до 365*8)
                                             //По умолчанию = 0
        int                    begin;        // время (рабочий час года) начала выполнения работы (от 0 до 365*8)
        int                    time;         // полное время выполнения работы в часах
        int                    end;          // время (рабочий час года) окончания работы (с учетом нерабочих дней) (от 0 до 365*8)
        int                    endmax;       // максимально разрешенное время (рабочий час года) конца работы (из ...) (от 0 до 365*8). Если
                                             //задан 0 (по - умолчанию), то считать, что ограничения по концу - нет
        executor_type          executor;     // назначенный исполнитель (из списка people)
        QVector<executor_type> people;       // список возможных исполнителей работы
        bool                   protection;   // признак защиты работы от редактирования (true - защищена; false - можно редактировать)
    };

    QVector<QVector<QVector<job_type>>> job;  // вектор работ, содержащий всю основную информацию по всем работам
    QVector<executor_type> executor;          // вектор исполнители (общий список из people всех работ)
}

// объявить прототипы функций
void      Progress();
QDateTime NumWorkHourYearToDateTime(int &n, QString &str);
int       DateTimeToNumWorkHourYear(QDateTime &dt, QString &str);
int       TestTCarryTaskBeforePlanning(TCarryTaskList &tasks, QString &errs);
int       TestTCarryTaskAfterPlanning(TCarryTaskList &tasks, QString &errs);
int       TestData(QString &errs);
int       TestPlan(QString &errs);
void      FindBeginEndJob(executor_type &executor, int &tstart, int &time, int &tbegin, int &tend);
void      FindEndBeginJob(executor_type &executor, int &tstart, int &time, int &tbegin, int &tend);
job_type  FindJob(int &code);
void      SortingVectorJob();
void      FillVectorJob(TCarryTaskList &tasks);
void      PrinTCarryTask(TCarryTaskList &tasks);
void      PrintPlan();
void      PrintJob();
void      WritePlan(TCarryTaskList &tasks);
int       PlanJobBeginEnd(int &l, int &m, int &n, QString &errs);
int       PlanJobEndBegin(int &l, int &m, int &n, QString &errs);
int       PlanProductBeginEnd(int &l, QString &errs);
int       PlanProductEndBegin(int &l, QString &errs);
int       Plan(TCarryTaskList &tasks, QString &errs);

void Progress() // настройка ProgressBar
{
    // Создать ProgressDialog (pprd) и внутри него ProgressBar (pprb). Кн. Cancel для прекращения планирования, если оно длится очень долго.

    pprd = new QProgressDialog("Выполнение...", "&Cancel", 0, 0); // создать новый ProgressDialog
    pprb = new QProgressBar();             // создать новый ProgressBar
    pprb->setAlignment(Qt::AlignCenter);   // выровнять шкалу ProgressBar внутри ProgressDialog по центру
    pprb->setTextVisible(false);           // не показывать цифры процентов выполнения, т.к. количество итераций заранее не известно
    pprb->setMaximum(0);                   // установить максимум ProgressBar (, что бы был просто бегущий индикатор без цифр)
    pprb->setMinimum(0);                   // установить минимум ProgressBar (, что бы был просто бегущий индикатор без цифр)
    pprb->setValue(0);                     // установить начальное значение ProgressBar (, что бы был просто бегущий индикатор без цифр)
    pprd->setBar(pprb);                    // установить ProgressBar внутрь ProgressDialog
    pprd->setMinimumDuration(300);         // не показывать окно ProgressDialog, если программа выполняется менее 300 милисекунд
    pprd->setWindowTitle("Планирование");  // заголовок окна ProgressDialog
    pprd->setCancelButtonText("Прервать"); // надпись на кнопке Cancel
}

QDateTime NumWorkHourYearToDateTime(int &n, QString &str) // перейти от номера рабочего часа года к QDateTime
{
    // По номеру рабочего часа (n) года (от 0 до 365*8) найти астрономическое время начала в формате QDateTime.
    // Если str == "begin", то найти астрономическое время начала рабочего часа,
    //если str == "end", то найти астрономическое время конца часа, которое заменяется НАЧАЛОМ СЛЕДУЮЩЕГО рабочего часа.
    //Например: time = 2; begin = 3; end = 4, тогда время начала будет  11:00, а время конца 13:00
    // Эта функция используется только при записи результатов планирования в объект tasks

    int numday  = n / kolworkhoursday; // номер дня года, начиная с 0
    int numhour = n % kolworkhoursday; // номер рабочего часа суток (м.б. от 0 до 7 (kolhours-1))

    QDate date = beginyearplan.addDays(numday); // найденная астрономическая дата в формате QDate (beginyearplan - нач. года планирования)
    QTime time(beginworkday + numhour,0);       // найденное астрономическое время начала часа в формате QTime. Минуты всегда = 0

    QDateTime datetime(date, time); // найденное астрономическое время начала часа в формате QDateTime

    // Проверка. Если нужен конец часа, то к найденному datetime прибавить 1 час (3600 секунд)
    if(str=="end") datetime = datetime.addSecs(3600) ; // прибавить 1 час, что бы было время конца часа

    return datetime;
}

int DateTimeToNumWorkHourYear(QDateTime &dt, QString &str) // перейти от даты QDateTime к номеру рабочего часа года
{
    // По дате в формате QDateTime найти номер рабочего часа года (от 0 до 365*8)
    // Если str == "begin", dt это астрономическое время начала рабочего часа,
    //если str == "end", то dt это астрономическое время НАЧАЛА следующего рабочего часа после последнего часа работы.

    int n;    // номер рабочего часа суток (от 0 до 7)

    QDate day = dt.date();  // дата
    QTime time = dt.time(); // время

    //Проверка. Если dt - это время конца астрономического часа, то перейти ко времени начала часа
    if(str=="end") time = time.addSecs(-3600); // надо уменьшить время на 1 час (3600 секунд)

    int deyofyear = day.dayOfYear() - 1; // номер дня года, начиная с 0
    int hour      = time.hour();         // номер часа в сутках (от 0 до 23)

    // по номеру часа суток (от 0 до 23) перейти к номеру рабочего часа суток (от 0 до 7)
    if(hour <  beginworkday)  n = 0; // До начала рабочего дня с 0:00 до 8:00. Если 0,1,2,3,4,5,6,7,8, то будет 0-ой рабочий час
    else if(hour > (beginworkday + kolworkhoursday - 1)) n = kolworkhoursday; // После рабочего дня с 17:00 до 23:00. Если 17,18,19,20,21,22,23, то будет 0-ой час завтра
    else n = hour - beginworkday; // В течении рабочего дня c 9:00 до 16:00. Если 9,10,11,12,13,14,15,16 , то будет: 0, 1, 2, 3, 4, 5, 6 или 7

    int num = deyofyear * kolworkhoursday + n; // номер рабочего часа года (от 0 до 365*8)

    return num;
}

int TestTCarryTaskBeforePlanning(TCarryTaskList &tasks, QString &errs) // проверить объект tasks до планирования
{
    // Проверить данные объекта tasks до начала планирования, что бы выявить ошибки, которые не позволят начать планирование
    bool flerror = false; // флаг наличия ошибки в объекте tasks

    // Проверка. Есть ли проект?
    if(!tasks.count()){
        flerror = true; // признак наличия ошибки
        numerror ++; // порядковый номер ошибки
        errs += QString("<br><b>%1</b>. Не найден проект").arg(numerror);
    }

    int kolplan = 0; // счетчик считанных планов. Если он останется = 0, то прервать планирование
    foreach (TCarryTask *curTask,tasks) // цикл перебора проектов
    {
        // Проверка. Есть ли проект?
        if(!curTask){
            flerror = true; // признак наличия ошибки
            numerror ++; // порядковый номер ошибки
            errs += QString("<br><b>%1</b>. Не найден проект").arg(numerror);
        }

        for(int i=0; i<=1; i++){ // цикл перебора продукций (План выпуска продукции (i=0) и План ОРД (i=1))
            TCarryPlan *plan;

            // Проверка. Это план выпуска продукции или ОРД?
            if(i==0) plan = curTask->carryPlan(); // план выпуска продукции
            else     plan = curTask->ordPlan();   // план ОРД

            if(!plan) continue; // пропустить пустую продукцию (пустой план)

            kolplan++; // счетчик считанных планов. Если счетчик останется нулевым, то прекратить планирование

            //??? Не ясно до конца, какой год брать? Когда в базе будут планы разных годов?
            // Проверка. Есть ли год планирования проекта? Если нет, то прекратить планирование
            if(!curTask->year()){
                flerror = true; // признак наличия ошибки
                numerror ++; // порядковый номер ошибки
                errs += QString("<br><b>%1</b>. Пустой год планирования в проекте: %2").arg(numerror).arg(curTask->num());
            }

            // Проверка. Минимально возможное время начала продукции д.б. меньше максимально возможного времени конца продукции.
            //С этой ошибкой планирование выполнится, но заданное время конца проекта будет проигнорировано
            if(curTask->dtMinBegin()){ // если минимально возможное время начала проекта задано
                if(curTask->dtMaxEnd()){ // если максимально возможное время конца проекта задано
                    if(curTask->dtMinBegin() > curTask->dtMaxEnd()){ // если время "начала" больше времени "конца"
                        qDebug()<<"ПРЕДУПРЕЖДЕНИЕ. Заданное время начала проекта больше (равно) времени конца. curTask->dtMinBegin()="<<QDateTime(curTask->dtMinBegin()->date(), curTask->dtMinBegin()->time()).toString("d.M.yyyy")
                               <<"curTask->dtMaxEnd()="<<QDateTime(curTask->dtMaxEnd()->date(), curTask->dtMaxEnd()->time()).toString("d.M.yyyy");
                        //flerror = true; // признак наличия ошибки
                        //numerror ++; // порядковый номер ошибки
                        //errs += QString("<br><b>%1</b>. Заданное время начала больше заданного времени конца проекта: %2").arg(numerror).arg(curTask->num());
                    }
                }
            }

            // Проверка. Максимально возможное время конца продукции не может быть меньше текущего времени.
            //С этой ошибкой планирование выполнится, но заданное время конца проекта будет проигнорировано
            if(curTask->dtMaxEnd()){ // если максимально возможное время конца проекта задано
                QDateTime datetimecurrent(QDate::currentDate(), QTime::currentTime()); // текущая дата-время
                if(*(curTask->dtMaxEnd()) < datetimecurrent){ // если время "конца" меньше текущей даты
                    qDebug()<<"ПРЕДУПРЕЖДЕНИЕ. Текущее время превышает заданное время конца проекта. Тек. время="<<QDateTime(QDate::currentDate(), QTime::currentTime()).toString("d.M.yyyy")
                           <<"curTask->dtMaxEnd()="<<QDateTime(curTask->dtMaxEnd()->date(), curTask->dtMaxEnd()->time()).toString("d.M.yyyy");
                    //flerror = true; // признак наличия ошибки
                    //numerror ++; // порядковый номер ошибки
                    //errs += QString("<br><b>%1</b>. Текущее время превышает заданное время конца проекта: %2").arg(numerror).arg(curTask->num());
                }
            }

            foreach (TCarryProcedure *proc,plan->procedures()) // цикл перебора процедур
            {
                // Проверка. Процедура не должна быть пустой. Должна содержать хотя бы одну задачу
//               if(proc->works().count(>employees().empty()){
//                    flerror = true; // признак наличия ошибки
//                    numerror ++; // порядковый номер ошибки
//                    errs += QString("<br><b>%1</b>. Пустой список исполнителей в процедуре: %2.%3.%4").arg(numerror).arg(curTask->num()).arg(plan->num()).arg(proc->num());
//                }


                // Проверка. Если список возможных исполнителей процедуры пуст, то прекратить планирование
                if(proc->possibleEmployees().empty()){
                    flerror = true; // признак наличия ошибки
                    numerror ++; // порядковый номер ошибки
                    errs += QString("<br><b>%1</b>. Пустой список исполнителей в процедуре: %2.%3.%4").arg(numerror).arg(curTask->num()).arg(plan->num()).arg(proc->num());
                }

                // Проверка. Если у процедуры есть назначенный исполнитель, то этот исполнитель должен иметь календарь
                if(proc->employee()){
                    if(!proc->employee()->calendar()){ // проверка на пустой календарь
                        flerror = true; // признак наличия ошибки
                        numerror ++; // порядковый номер ошибки
                        errs += QString("<br><b>%1</b>. Отсутствует календарь исполнителя: %2 в "
                                        "процедуре: %3.%4.%5").arg(numerror).arg(proc->employee()->name()).arg(curTask->num()).arg(plan->num()).arg(proc->num());
                    }
                }

                // Проверка. Каждый возможный исполнитель процедуры должен иметь календарь
                foreach (TEmployee *empl, proc->possibleEmployees()) { // цикл перебора возможных исполнителей процедуры
                    if(!empl->calendar()){ // проверка на пустой календарь
                        flerror = true; // признак наличия ошибки
                        numerror ++; // порядковый номер ошибки
                        errs += QString("<br><b>%1</b>. Отсутствует календарь возможного исполнителя: %2 в "
                                        "процедуре: %3.%4.%5").arg(numerror).arg(empl->name()).arg(curTask->num()).arg(plan->num()).arg(proc->num());
                        //qDebug()<<"Нет календаря у исполнителя:"<<empl->scrName();
                    }
                    else{
                        //qDebug()<<"Есть календарь у исполнителя:"<<empl->name()<<" , id:"<<empl->id();
                    }

                    // Проверка. Каждый возможный исполнитель должен иметь доступ к календарю предприятия
                    //if(empl->calendar()->fCompanyCalendar->fWeekEnds.empty()){
                    //if(empl->calendar()->
                    //{
                    //}
                }

                foreach (TCarryWork *work,proc->works()) // цикл перебора работ
                {
                    if(work){ // проверка на нулевой указатель
                        // Проверка. Любая работа должна иметь time - длительность
                        if(!work->planPeriod()){
                            flerror = true; // признак наличия ошибки
                            numerror ++; // порядковый номер ошибки
                            errs += QString("<br><b>%1</b>. Не определена длительность работы: %2.%3.%4").arg(numerror).arg(curTask->num()).arg(plan->num()).arg(proc->num()).arg(work->num());
                        }
                    }
                    else { // пустая работа
                        flerror = true; // признак наличия ошибки
                        numerror ++; // порядковый номер ошибки
                        errs += QString("<br><b>%1</b>. Пустая работа: %2.%3.%4").arg(numerror).arg(curTask->num()).arg(plan->num()).arg(proc->num());
                    }
                } // конец цикла перебора работ
            } // конец цикла перебора процедур
        } // конец цикла перебора 2-ух планов: продукции и ОРД
    } // конец цикла перебора продукций

    // Проверка. Был ли считан хотя бы один план?
    if(!kolplan){
        flerror = true; // признак наличия ошибки
        numerror ++; // порядковый номер ошибки
        errs += QString("<br><b>%1</b>. Не найден ни один план").arg(numerror);
    }

    // Проверка. Были ли ошибки при проверке tasks?
    if(flerror) return 1; // есть ошибки. Планирование отменяется
    else return 0;        // норм. завершение функции. Ошибок - нет
}

int TestTCarryTaskAfterPlanning(TCarryTaskList &tasks, QString &errs) // проверить объект tasks после планирования
{
    // Проверить данные объекта tasks после записи в него результатов планирования
    bool flerror = false; // признак наличия ошибки

    foreach (TCarryTask *curTask,tasks) // цикл перебора проектов
    {
        for(int i=0; i<=1; i++){ // цикл перебора продукций (План выпуска продукции (i=0) и План ОРД (i=1))
            TCarryPlan *plan;

            // Проверка. Это план выпуска продукции или ОРД?
            if(i==0) plan = curTask->carryPlan(); // план выпуска продукции
            else     plan = curTask->ordPlan();   // план ОРД

            if(!plan) continue; // пропустить пустую продукцию (пустой план)

            foreach (TCarryProcedure *proc,plan->procedures()) // цикл перебора процедур
            {

                foreach (TCarryWork *work,proc->works()) // цикл перебора работ
                {
                    // Проверка. Время конца работы д.б. больше времени начала работы (равенство не допускается)
                    QDateTime tbegin = QDateTime(work->dtPlanBegin()->date(), work->dtPlanBegin()->time());
                    QDateTime tend   = QDateTime(work->dtPlanEnd()->date(),   work->dtPlanEnd()->time());

                    if(tbegin >= tend){
                        flerror = true; // признак наличия ошибки
                        numerror ++; // порядковый номер ошибки
                        errs += QString("<br><b>%1</b>. Время начала работы больше или равно времени конца "
                                        "работы: %2.%3.%4.%5").arg(numerror).arg(curTask->num()).arg(plan->num()).arg(proc->num()).arg(work->num());
                    }

                } // конец цикла перебора работ
            } // конец цикла перебора процедур
        } // конец цикла перебора 2-ух планов: продукции и ОРД
    } // конец цикла перебора продукций

    // Проверка. Были ли ошибки при проверке tasks?
    if(flerror) return 1; // есть ошибки. Планирование отменяется
    else return 0;        // норм. завершение функции. Ошибок - нет
}

int TestData(QString &errs) // тестирование векторов executor и job
{
    // Проверка на корректность данных, уже считанных из tasks в вектор job

    // Проверка. Есть ли что то в векторе job?
    if(!job.size()){
        numerror ++; // порядковый номер ошибки
        errs += QString("<br><b>%1</b>. Не найден ни одн план (продукция)").arg(numerror);
        return 1;
    }

    // Проверка на существование вектора об исполнителях executor
    if(executor.size() == 0) {
        if(flprint>=1) qDebug()<<"Ошибка. Вектор executor (сведения об исполнителях) пустой";
        numerror ++; // порядковый номер ошибки
        errs += QString("<br><b>%1</b>. Не найден ни один исполнитель").arg(numerror);
        return 1;
    }

    // Проверка id исполнителя в векторе executor не должен быть равен 0
    for(int i=0; i<(int)executor.size(); ++i){
        if(executor[i].id == 0) {
            if(flprint>=1) qDebug()<<"Ошибка. Вектор executor, поле id=0";
            numerror ++; // порядковый номер ошибки
            errs += QString("<br><b>%1</b>. Обнаружен исполнитель с пустым идентификатором").arg(numerror);
            return 2;
        }
    }

    // Проверка на уникальность id всех возможных исполнителей
    for(int i=0; i<(int)executor.size(); ++i){ // цикл перебора всех исполнителей
        int tekid = executor[i].id;
        for(int j=0; j<(int)executor.size(); ++j){ // цикл поиска текущего кода tekcode
            if(j==i) continue; // пропустить проверямого исполнителя
            if(executor[j].id==tekid) { // обнаружена ошибка - повторение кода
                if(flprint>=1) qDebug()<<"Ошибка. Нарушение уникальности id исполнителей: "<<QString::number(tekid);
                numerror ++; // порядковый номер ошибки
                errs += QString("<br><b>%1</b>. Нарушена уникальность id исполнителей").arg(numerror);
                return 3;
            }
        } // конец цикла поиска текущего кода tekcode
    } // конец цикла перебора всех исполнителей

    // Проверка заполнения данных об работах
    if(job.size()==0) {
        qDebug("Ошибка. Вектор job (сведения о работах) пустой");
        errs += QString("<br> Не найдена ни одна работа");
    }
    for(int i=0; i<(int) job.size(); ++i){ // цикл перебора продукций
        for(int j=0; j<(int) job[i].size(); ++j){ // цикл перебора процедур текущей продукции
            for(int k=0; k<(int) job[i][j].size(); ++k){ // цикл перебора работ текущей продукции

            // Работа должна иметь "продолжительность"
            if(!job[i][j][k].time) {
                if(flprint>=1) qDebug()<<"Ошибка. Вектор job, поле time=0 (пустое). i: "<<QString::number(i)<<", j: "<<QString::number(j);
                numerror ++; // порядковый номер ошибки
                errs += QString("<br><b>%1</b>. Не найдена продолжительность работы: %2.%3.%4.%5").arg(numerror).arg(job[i][j][k].projectnum).
                        arg(job[i][j][k].productnum).arg(job[i][j][k].procedurenum).arg(job[i][j][k].num);
                return 6;
            }

            // Работа должна иметь список возможных исполнителей
            if(!job[i][j][k].people.size()) {
                if(flprint>=1) qDebug()<<"Ошибка. Вектор job, поле people пустое. i: "<<QString::number(i)<<", j: "<<QString::number(j);
                numerror ++; // порядковый номер ошибки
                errs += QString("<br><b>%1</b>. Пустой список возможных исполнителей работы: %2.%3.%4.%5").arg(numerror).arg(job[i][j][k].projectnum).
                        arg(job[i][j][k].productnum).arg(job[i][j][k].procedurenum).arg(job[i][j][k].num);
                return 7;
            }

            // Время конца работы д.б. больше или равно времени начала
            if(job[i][j][k].begin > job[i][j][k].end) {
                if(flprint>=1) qDebug()<<"Ошибка. Время конца меньше начала работы. i: "<<QString::number(i)<<", j: "<<QString::number(j);
                numerror ++; // порядковый номер ошибки
                errs += QString("<br><b>%1</b>. Время начала больше времени конца работы: %2.%3.%4.%5").arg(numerror).arg(job[i][j][k].projectnum).
                        arg(job[i][j][k].productnum).arg(job[i][j][k].procedurenum).arg(job[i][j][k].num);
                return 11;
            }

            // Проверка защищенных работ. У этих работ д.б. заполнены поля: begin, end, executor
            //if(job[i][j][k].protection) {
            //    // Д.б. заполнен исполнитель (поле executor)
            //    if(job[i][j][k].executor.id == 0) {
            //        if(flprint>=1) qDebug()<<"Ошибка. Защищенная работа. Вектор job, поле executor.code=0 (пустое). i: "<<QString::number(i)<<", j: "<<QString::number(j);
            //        numerror ++; // порядковый номер ошибки
            //        errs += QString("<br><b>%1</b>. В защищенной работе пустой исполнитель").arg(numerror);
            //        return 10;
            //    }
            //}

            } // конец цикла перебора работ
        } // конец цикла перебора продукций
    } // конец цикла перебора продукций

    // Проверка на уникальность id всех работ
    for(int i=0; i<(int)job.size(); ++i){ // цикл перебора продукций
        for(int j=0; j<(int)job[i].size(); ++j){ // цикл перебора процедур текущей продукции
            for(int k=0; k<(int)job[i][j].size(); ++k){ // цикл перебора работ текущей процедуры
                int tekid = job[i][j][k].id;
                for(int ii=0; ii<(int)job.size(); ++ii){ // цикл перебора продукций
                    for(int jj=0; jj<(int)job[ii].size(); ++jj){ // цикл перебора процедур текущей продукции
                        for(int kk=0; kk<(int)job[ii][jj].size(); ++kk){ // цикл перебора работ текущей процедуры
                            if((j==jj) && (i==ii) && (k==kk)) continue; // пропустить проверямую работу
                            if(job[ii][jj][kk].id==tekid) {
                                if(flprint>=1) qDebug()<<"Ошибка. Вектор job, поле id, нарушение уникальности. id: "<<QString::number(tekid);
                                numerror ++; // порядковый номер ошибки
                                errs += QString("<br><b>%1</b>. Нарушение уникальности id работы: "
                                                "%2.%3.%4.%5").arg(numerror).arg(job[i][j][k].projectnum).arg(job[i][j][k].productnum).
                                                                             arg(job[i][j][k].procedurenum).arg(job[i][j][k].num);
                                return 12;
                            }
                        } // конец цикла перебора работ
                    } // конец цикла перебора процедур
                } // конец цикла перебора продукции
            } // конец цикла перебора работ
        } // конец цикла перебора процедур
    } // конец цикла перебора продукций

    return 0; // нормальное завершение функции. Ошибок в исходных данных не обнаружено
}

int TestPlan(QString &errs) // протестировать  сформированный план
{
    // Тестирование сформированного плана на ошибки. Тестирование вектора job после планирования

    // Каждая последующая работа должна начинается после завершения предыдущей (в рамках одной продукции).
    //Проверка принципа последовательности выполнения работ одной продукции
    int endold; // конец предыдущей (старой) работы
    for(int i=0; i<(int)job.size(); ++i){ // цикл перебора продукций
        for(int j=0; j<(int)job[i].size(); ++j){ // цикл перебора процедур
            for(int k=0; k<(int)job[i][j].size(); ++k) { // цикл перебора работ

                // Найти конец начальной работы продукции (у нее нет предшествующей работы)
                if((j==0) && (k==0)){
                    endold = job[i][j][k].end; // запомнить конец начальной работы продукции
                    continue; // перейти к следующей работе
                }

                // Проверка нарушения последовательности выполнения работ
                if(job[i][j][k].begin < endold){
                    QMessageBox::information(0,"Отладка", "Нарушение последовательности работ в плане");
                    numerror ++; // порядковый номер ошибки
                    errs += QString("<br><b>%1</b>. Нарушение последовательности работы:"
                                    "%2.%3.%4.%5").arg(numerror).arg(job[i][j][k].projectnum).arg(job[i][j][k].productnum).
                                                                 arg(job[i][j][k].procedurenum).arg(job[i][j][k].num);
                    return 1;
                }
                endold = job[i][j][k].end; // запомнить конец предыдущей работы
            } // конец цикла перебора работ
        } // конец цикла перебора процедур
    } // конец цикла перебора продукций

    // Проверка на дублирование одновременно выполняемых работ одним исполнителем. Каждый исполнитель в любой момент времени
    //может выполнять только одну работу
    for(int i=0; i<(int)job.size(); ++i){ // цикл перебора продукций
        for(int j=0; j<(int)job[i].size(); ++j) { // цикл перебора процедур
            for(int k=0; k<(int)job[i][j].size(); ++k) { // цикл перебора работ

                executor_type executor = job[i][j][k].executor; // запомнить проверямого исполнителя
                int begin = job[i][j][k].begin;                 // запомнить начало проверяемой работы
                int end   = job[i][j][k].end;                   // запомнить конец проверяемой работы

                if(executor.external) continue; // пропустить внешнего исполнителя (для него не надо проверять дублирование)

                // перебрать все работы для проверки
                for(int ii=0; ii<(int)job.size(); ++ii){ // цикл перебора продукций
                    for(int jj=0; jj<(int)job[ii].size(); ++jj) { // цикл перебора продукций
                        for(int kk=0; kk<(int)job[ii][jj].size(); ++kk) { // цикл перебора работ
                            if((i==ii) && (j==jj) && (k==kk)) continue; // пропустить проверямую работу (не проверять саму с собой)
                            if(job[ii][jj][kk].executor.id != executor.id) continue; // пропустить ненужного исполнителя
                            if(((begin>=job[ii][jj][kk].begin) && (begin<=job[ii][jj][kk].end)) || // точка begin проверяемого интервала попала внутрь текущего интервала
                               ((end  >=job[ii][jj][kk].begin) && (end  <=job[ii][jj][kk].end)) || // точка end   проверяемого интервала попала внутрь текущего интервала
                               ((begin<=job[ii][jj][kk].begin) && (end  >=job[ii][jj][kk].end))){  // проверяемый интервал полностью накрыл текущий интервал
                                QMessageBox::information(0,"Отладка", "Дублирование работ одним исполнителем id=" + executor.id);
                                numerror ++; // порядковый номер ошибки
                                errs += QString("<br><b>%1</b>. Дублирование исполнителя в работе:"
                                                "%2.%3.%4.%5").arg(numerror).arg(job[i][j][k].projectnum).arg(job[i][j][k].productnum).
                                                                             arg(job[i][j][k].procedurenum).arg(job[i][j][k].num);
                                return 2;
                            }
                        } // конец цикла перебора работ
                    } // конец цикла перебора продукций
                } // конец цикла перебора продукции

            } // конец цикла перебора работ
        } // конец цикла перебора процедур
    } // конец цикла перебора продукций

    // Конец спланированной работы должен превышать начало работы (равенство начала и конца допускается, если работа длится 1 час)
    for(int i=0; i<(int)job.size(); ++i){ // цикл перебора продукций
        for(int j=0; j<(int)job[i].size(); ++j){ // цикл перебора процедур
            for(int k=0; k<(int)job[i][j].size(); ++k) { // цикл перебора работ
                if(job[i][j][k].begin > job[i][j][k].end){
                    numerror ++; // порядковый номер ошибки
                    errs += QString("<br><b>%1</b>. Начало работы больше или равно концу работы:"
                                    "%2.%3.%4.%5").arg(numerror).arg(job[i][j][k].projectnum).arg(job[i][j][k].productnum).
                                                                 arg(job[i][j][k].procedurenum).arg(job[i][j][k].num);
                    return 1;
                }
            } // конец цикла перебора работ
        } // конец цикла перебора процедур
    } // конец цикла перебора продукций


    return 0; // ошибок нет
}

void FindBeginEndJob(executor_type &executor, int &tstart, int &time, int &tbegin, int &tend) // Прямое планирование. Найти начало и конец работы
{
    // Для исполнителя executor найти время начала и время конца работы с учетом нерабочих часов (праздников, выходных, отпусков и т.д.)
    //    Входные параметры:
    //tstart - время, начиная с которого, надо искать допустимое начало работы tbegin;
    //time   - заданное полное время выполнения работы;
    //    Выходные параметры:
    //tbegin - найденное время начала работы с учетом нерабочих часов;
    //tend   - найденное время конца работы с учетом нерабочих часов.

    int sum=0; // счетчик найденных рабочих часов
    int ttek = tstart; // проверяемый час
    while(sum <= time){ // цикл, пока не найдено нужное количество рабочих часов time
        // проверка. Является ли час ttek нерабочим?
        int kol = executor.rest.count(ttek); // поиск ttek - нужного часа среди списка нерабочих часов
        if(kol == 0) { // проверяемый час - рабочий? (т.к. он не найден среди списка нерабочих часов)
            ++sum; // счетчик найденных рабочих часов
            if(sum == 1)    tbegin = ttek; // запомнить первый найденный рабочий час, как tbegin
            if(sum == time) tend   = ttek; // запомнить последний найденный рабочий час, как tend
        }
        ++ttek; // переход к следующему часу для его проверки. Двигаемся слева направо
    }
}

void FindEndBeginJob(executor_type &executor, int &tstart, int &time, int &tbegin, int &tend) // Обратное планирование. Найти конец и начало работы
{
    // Для исполнителя executor найти время конца и время начала работы с учетом нерабочих часов (праздников, выходных, отпусков и т.д.)
    //    Входные параметры:
    //tstart - время, начиная с которого, надо искать допустимое начало работы tbegin;
    //time   - заданное полное время выполнения работы;
    //    Выходные параметры:
    //tbegin - найденное время начала работы с учетом нерабочих часов;
    //tend   - найденное время конца работы с учетом нерабочих часов.

    int sum=0; // счетчик найденных рабочих часов
    int ttek = tstart; // проверяемый час
    while(sum <= time){ // цикл, пока не найдено нужное количество рабочих часов time
        // проверка. Является ли час ttek нерабочим?
        int kol = executor.rest.count(ttek); // поиск ttek - нужного часа среди списка нерабочих часов
        if(kol == 0) { // проверяемый час - рабочий? (т.к. он не найден среди списка нерабочих часов)
            ++sum; // счетчик найденных рабочих часов

            //if(sum == 1)    tbegin = ttek; // запомнить первый найденный рабочий час, как tbegin
            if(sum == 1)    tend = ttek; // запомнить найденный рабочий час, как tend

            //if(sum == time) tend   = ttek; // запомнить последний найденный рабочий час, как tend
            if(sum == time) tbegin   = ttek; // запомнить найденный рабочий час, как tbegin
        }
        //++ttek; // переход к следующему часу для его проверки. Двигаемся слева направо
        --ttek; // переход к следующему часу для его проверки. Двигаемся справа налево
    }
}

job_type FindJob(int &id) // найти работу по id
{
    // найти работу в векторе job по ее идентификатору id. Если работа не найдена, то вернуть пустую работу
    for(int i=0; i<(int)job.size(); i++){ // цикл перебора продукции
        for(int j=0; j<(int)job[i].size(); j++){ // цикл перебора процедур
            for(int k=0; k<(int)job[i][j].size(); k++){ // цикл перебора работ
                if(id == job[i][j][k].id) return job[i][j][k];
            } // конец цикла перебора работ
        } // конец цикла перебора процедур
    } // конец цикла перебора продукции
    // не найдена работа

    // Не найдена работа. Это ошибка. Работа по id должна быть найдена всегда
    qDebug()<<"Ошибка. В объекте tasks не найдена работа с id: " + QString::number(id);
    job_type wortempty; // сформировать и вернуть пустую работу
    return wortempty;
}

void SortingVectorJob() // сортировка вектора job
{
    // сортировка вектора job в соответствии с приоритетами продукций (в порядке убывания приоритета)
    //"Самая верхняя" продукция (для i=0) должна иметь высший приоритет. "Самая нижняя" продукция с наименьшим приритетом будет
    //получать ресурсы исполнителей по остаточному принципу. Чем "выше" приоритет продукция, тем быстрее она будет выполнена

    int kol; // количество перестановок местами продукций при нарушении приоритета
    do {
        kol = 0; // количество перестановок
        for(int i=0; i<=(int)job.size() - 2; ++i){ // цикл перебора продукций (до предпоследней включительно)
            // Проверка. Есть нарушение приоритета? Если Да, то переставить продукции местами
            if(job[i][0][0].priority < job[i+1][0][0].priority){ // проритет взять из начальной работы каждой продукции
                kol = kol + 1;
                QVector<QVector<job_type>> producttemp = job[i]; // запомнить i-ую продукцию
                job[i] = job[i+1];                               // заменить i-ую продукцию на (i+1)-ую продукцию
                job[i+1] = producttemp;                          // получить (i+1)-ую продукцию
            }
        } // конец цикла перебора продукций (до предпоследней)
    } while(kol > 0);
}

void FillVectorJob(TCarryTaskList &tasks) // заполнить данные о работах
{
    // функция "заполнить вектор работ" извлекает из принятого параметра tasks все необходимые сведения по всеми проектам,
    //продукциям, процедурам и работам. Заполняет вектор job, необходимый для планирования.
    // Вектор job[i][j][k] - трехмерный динамический массив, где i - индекс продукции, j - индекс процедуры, k - индекс работы.

    job.clear();      // предварительно очистить вектор работ
    executor.clear(); // предварительно очистить вектор исполнителей

    // Проверка. Есть ли tasks
    if(!tasks.count()) return;

    // прочитать данные tasks и записать их в рабочий вектор job, который является основным при планировании
    foreach (TCarryTask *curTask, tasks){ // цикл перебора проектов

        // Проверка. Есть ли проект?
        if(!curTask) continue;

        // План выпуска продукции и План выпуска ОРД - это две разные продукции. Надо загрузить последовательно обе продукции.
        //В первом проходе считывается продукция "План выпуска продукции", а во втором проходе считывается продукция "План ОРД"
        for(int i=0; i<=1; i++){ // цикл перебора продукций (двух планов): План выпуска продукции (i=0) и План ОРД (i=1)
            TCarryPlan *plan;

            // Проверка продукции. Это план выпуска продукции или план ОРД? Каждый план это продукция
            if(i==0) plan = curTask->carryPlan(); // план выпуска продукции
            else     plan = curTask->ordPlan();   // план ОРД

            // Проверка. Есть ли план (продукция)?
            if(!plan) continue;

            // Проверка. Если нет плана (продукции), то пропустить его
            if(!plan->procedures().size()) continue; // пропустить план (продукцию), в котором нет процедур

            QVector<QVector<job_type>> tempvectorproc; // временный вектор процедур
            foreach (TCarryProcedure *proc, plan->procedures()){ // цикл перебора процедур

                // Проверка. Есть ли процедура?
                if(!proc) continue;

                QVector<job_type> tempvectorwork; // временный вектор работ
                foreach (TCarryWork *work, proc->works()){ // цикл перебора работ

                    // Проверка. Есть ли работа?
                    if(!work) continue;

                    // Проверка "плохой" работы. Если в защищенной работе не заполнены: начало или конец или исполнитель, то
                    //такую работу пропустить и не загружать в вектор job. Такая работа не будет участвовать в планировании и
                    //эта работа не будет проверяться на дублирование
                    if(!work->isVolatile()){ // работа защищена (false)

                        if(!work->dtPlanBegin()){
                            if(flprint>=2) qDebug()<<"Обнаружена защищенная работа id:"<<work->id()<<"без времени начала. Работа не будет участвовать в планировании";
                            continue; // пропустить защищенную работу с незаполненной датой начала. Не учитывать и не планировать такую работу
                        }
                        if(!work->dtPlanEnd()){
                            if(flprint>=2) qDebug()<<"Обнаружена защищенная работа id:"<<work->id()<<"без времени окончания. Работа не будет участвовать в планировании";
                            continue;   // пропустить защищенную работу с незаполненной датой конца. Не учитывать и не планировать такую работу
                        }
                        if(!proc->employee()) {
                            if(flprint>=2) qDebug()<<"Обнаружена защищенная работа id:"<<work->id()<<"без исполнителя. Работа не будет участвовать в планировании";
                            continue;    // пропустить защищенную работу с незаполненным исполнителем. Не учитывать и не планировать такую работу
                        }
                    }

                    // Сформировать текущую работу jobtemp для записи ее в вектор работ job
                    job_type jobtemp;

                    // Заполнить реквизиты работы jobtemp
                    jobtemp.id           = work->id();          // id идентификатор работы (из БД. Нужен для записи результатов планирования)
                    jobtemp.priority     = curTask->priority(); // приоритет работы взять из приоритета проекта
                    jobtemp.num          = work->num();         // номер работы
                    jobtemp.procedurenum = proc->num() ;        // номер процедуры, которой принадлежит данная работа
                    jobtemp.productnum   = plan->num();         // номер продукции, которой принадлежит данная работа
                    jobtemp.projectnum   = curTask->num();      // номер проекта, которому принадлежит данная работа

                    QDateTime datetime;
                    // Заполнить минимально разрешенное время начала работы (от 0 до 365*8)
                    if(curTask->dtMinBegin()){ // проверка. Задана ли минимально разрешенное время начала продукции?
                        datetime = QDateTime(curTask->dtMinBegin()->date(), curTask->dtMinBegin()->time());
                        jobtemp.beginmin = DateTimeToNumWorkHourYear(datetime, begin);
                    }
                    else jobtemp.beginmin = 0;

                    // Заполнить максимально разрешенное время конца работы (от 0 до 365*8)
                    if(curTask->dtMaxEnd()){ // проверка. Задана ли максимально разрешенное время конца продукции?
                        datetime = QDateTime(curTask->dtMaxEnd()->date(), curTask->dtMaxEnd()->time());
                        jobtemp.endmax = DateTimeToNumWorkHourYear(datetime, end);
                    }
                    else jobtemp.endmax = 0;

                    //+??? УДАЛИТЬ ПРОБА ИСПОЛЬЗОВАНИЯ МАКСИМАЛЬНО ЗАДАННОГО КОНЦА ПРОДУКЦИИ
                    //QDate date(2017, 9, 30);
                    //QTime time(0,0);
                    //QDateTime dt(date, time);
                    //jobtemp.endmax = DateTimeToNumWorkHourYear(dt, end);
                    //-

                    jobtemp.time         = work->planPeriod();  // время выполнение работы
                    jobtemp.protection   = !work->isVolatile(); // признак защиты работы

                    // Заполнить список возможных исполнителей работы jobtemp.people
                    foreach (TEmployee *empl, proc->possibleEmployees()) { // цикл перебора возможных исполнителей
                        int idtek = empl->id();   // id текущего исполнителя
                        executor_type executortemp; // структура нового исполнителя, которую надо заполнить, а затем сохранить
                        executortemp.id       = idtek;                   // записать код текущего исполнителя
                        executortemp.external = empl->role().isExtern(); // записать признак исполнителя (внешний/внутренний)

                        //qDebug()<<"-------------------------------";
                        // Заполнить календарь возможных исполнителей процедуры нерабочими часами (не днями!) исполнителя
                        QList<int> calendar = *empl->calendar()->fullRestDates(); // календарь исполнителя, содержащий номера нерабочих дней года: выходные, праздники, отпуск (начиная с 0)
                        QList<int>::iterator p = calendar.begin(); // итератор для перебора списка calendar
                        while (p != calendar.end()) { // цикл перебора списка calendar (перебор номеров нерабочих дней года возможных исполнителей)

                            //
                            //QDate dt1(2017,1,1);
                            //int k=*p;
                            //QDate dt2 = dt1.addDays(k);
                            //qDebug()<<"empl->id()"<<empl->id()<<"*p="<<*p<<"k="<<k<<"dt2="<<dt2;
                            //

                            for(int j=0; j<kolworkhoursday; ++j){ // цикл перебора рабочих часов суток (от 0 до 7)
                                executortemp.rest.push_back(*p * kolworkhoursday + j); // календарь исполнителя, содержащий номера нерабочих часов года (по 8 часов в день)(начиная с 0)
                                //qDebug()<<"*p * kolworkhoursday + j"<<*p * kolworkhoursday + j;
                            } // конец цикла перебора рабочих часов суток
                            p++;
                        } // конец цикла перебора списка calendar


                        jobtemp.people.push_back(executortemp); // добавить текущего исполнителя executortemp в список возможных исполнителей работы
                        // Здесь же добавить текущего исполнителя executortemp в общий вектор всех исполнителей executor (исключив дублирование)
                        bool flfind = false; // признак, что исполнитель найден (true)
                        for(int i=0; i<(int)executor.size(); i++){ // цикл перебора вектора исполнителей
                            if(executor[i].id == idtek){ // исключить дублирование исполнителей в векторе executor
                                flfind = true;
                                break;
                            }
                        } // конец цикла перебора вектора исполнителей
                        if(!flfind) executor.push_back(executortemp); // доб. тек. исполнителя в вектор исполнителей executor
                    } // цикл перебора возможных исполнителей процедуры

                    // Заполнить оставшиеся реквизиты работы
                    if(jobtemp.protection){ // защищенная работа. В ней ничего менять нельзя
                        jobtemp.changes = false; // признак того, что в защищенной работе изменений не будет

                        // Заполнить время начала работы
                        QDateTime datetime = QDateTime(work->dtPlanBegin()->date(), work->dtPlanBegin()->time());
                        jobtemp.begin = DateTimeToNumWorkHourYear(datetime, begin);  // начало работы. Номер рабочего часа года (от 0 до 365*8)

                        // Заполнить время конца работы
                        datetime = QDateTime(work->dtPlanEnd()->date(), work->dtPlanEnd()->time());
                        jobtemp.end = DateTimeToNumWorkHourYear(datetime, end);  // начало работы. Номер рабочего часа года (от 0 до 365*8)

                        jobtemp.executor.id = proc->employee()->id(); // заполнить код исполнителя

                        // Заполнить календарь уже назначенного ранее исполнителя процедуры (и работы) нерабочими часами (не днями!) исполнителя
                        QList<int> calendar = *proc->employee()->calendar()->fullRestDates(); // календарь исполнителя, содержащий номера нерабочих дней года: выходные, праздники, отпуск (начиная с 0)
                        QList<int>::iterator p = calendar.begin(); // итератор для перебора списка calendar
                        while (p != calendar.end()) { // цикл перебора списка calendar (перебор номеров нерабочих дней года назначенного исполнителя)
                            for(int j=0; j<kolworkhoursday; ++j){ // цикл перебора рабочих часов суток (от 0 до 7)
                                jobtemp.executor.rest.push_back(*p * kolworkhoursday + j); // календарь исполнителя, содержащий номера нерабочих часов года (по 8 часов в день)(начиная с 0)
                            } // конец цикла перебора рабочих часов суток
                            p++;
                        } // конец цикла перебора списка calendar

                        jobtemp.executor.external = proc->employee()->role().isExtern(); // внешний или внутренний исполнитель
                    }
                    else{ // незащищенная работа обнуляется перед планированием
                        jobtemp.changes           = true;      // признак того, что работа будет изменена (и продукция тоже)
                        jobtemp.begin             = 0;         // время начала работы
                        jobtemp.end               = 0;         // время конца работы
                        jobtemp.executor.id       = 0;         // исполнителя нет
                        jobtemp.executor.rest     = {};        // исполнителя нет
                        jobtemp.executor.external = false;     // Пока так. Это поле будет заполнено позднее, при планировании
                    }

                    tempvectorwork.push_back(jobtemp); // записать текущую работу в вектор работ
                } // конец цикла перебора работ
                tempvectorproc.push_back(tempvectorwork); // записать заполненный вектор работ в вектор процедур
            } // конец цикла перебора процедур
            job.push_back(tempvectorproc); // записать заполненный вектор процедур в вектор продукций
        } // конец цикла перебора 2-ух планов: продукции и ОРД.
    } // конец цикла перебора продукций
}

void PrinTCarryTask(TCarryTaskList &tasks) // вывести в консоль объект tasks
{
    // вывести в консоль содержимое tasks (только для отладки)
    foreach (TCarryTask *curTask,tasks) // цикл перебора проэктов
    {
        qDebug()<<"Проект id="<<curTask->id();
        qDebug()<<"Проект priority="<<curTask->priority();
        qDebug()<<"Проект dtMinBegin="<<curTask->dtMinBegin()->date();
        qDebug()<<"Проект dtMaxEnd="<<curTask->dtMaxEnd()->date();

        for(int i=0; i<=1; i++){ // цикл перебора двух продукций (планов): План выпуска продукции (i=0) и План ОРД (i=1)
            TCarryPlan *plan; // plan - это продукция

            // Проверка. Это план выпуска продукции или ОРД?
            if(i==0) plan = curTask->carryPlan(); // план выпуска продукции
            else     plan = curTask->ordPlan();   // план ОРД

            // Проверка. Есть ли продукция?
            if(!plan) continue; // пропустить пустую продукцию
            qDebug()<<"    Продукция (план) id="<<plan->id();
            qDebug()<<"    isSaved="<<plan->isSaved();


            foreach (TCarryProcedure *proc,plan->procedures()) // цикл перебора процедур продукции
            {
                qDebug()<<"        Процедура id="<<proc->id();
                qDebug()<<"            proc isExtern="<<proc->isExtern(); // признак того, что процедура внешняя (не известы возможные исполнители)

                // вывести в цикле id возможных исполнителей процедуры
                if(proc->possibleEmployees().empty())                     qDebug()<<"            possibleEmployees (people) = ...";
                else foreach (TEmployee *empl, proc->possibleEmployees()) qDebug()<<"            possibleEmployees (people) id="<<empl->id();

                // вывести назначенного исполнителя процедуры
                if(proc->employee()) {
                    qDebug()<<"            employee id="<<proc->employee()->id();
                    qDebug()<<"            employee isExtern="<<proc->employee()->role().isExtern();
                }
                else                 qDebug()<<"            employee id=..."; // не назначен исполнитель

                // вывести данные по работе
                foreach (TCarryWork *work,proc->works()) // цикл перебора работ
                {
                    if(work){ // проверка на нулевой указатель
                        qDebug()<<"            Работа  id="<<work->id();
                        qDebug()<<"            Работа num="<<work->num();

                        if(work->dtPlanBegin()) qDebug()<<"                fDtPlanBegin (begin)="<<work->dtPlanBegin()->toString("d.M.yyyy hh:mm");
                        else                    qDebug()<<"                fDtPlanBegin (begin)=...";

                        qDebug()<<"                fPlanPeriod (time)="<<work->planPeriod();

                        if(work->dtPlanBegin()) qDebug()<<"                fDtPlanEnd (end)="<<work->dtPlanEnd()->toString("d.M.yyyy hh:mm");
                        else                    qDebug()<<"                fDtPlanEnd (end)=...";

                        qDebug()<<"                !fIsVolatile (protection)="<<!work->isVolatile();
                    }
                    else {qDebug()<<"            Работа id=...";
                    QMessageBox::information(0,"Ошибка", "Обнаружена пустая работа в процедуре с id=" + proc->id());
                    }
                } // конец цикла перебора работ
            } // конец цикла перебора процедур
        } // конец цикла перебора продукции (планов)
    } // конец цикла перебора проектов
}

void PrintJob() // вывести в консоль данные по всем работам (вектор job) в виде таблицы
{
    qDebug()<<"----------";
    // вывести содержимое вектора job в консоль (для отладки). Это перечень работ по каждой продукции и каждой процедуре
    for(int i=0; i<(int)job.size(); i++){ // цикл перебора продукции-строк
        for(int j=0; j<(int)job[i].size(); j++){ // цикл перебора процедур
            for(int k=0; k<(int)job[i][j].size(); k++){ // цикл перебора работ
                QString b; // список кодов возможных исполнителей процедуры
                for(int l=0; l<(int)job[i][j][k].people.size(); ++l) b = b + QString::number(job[i][j][k].people[l].id) + " ";
                qDebug()<<"i:"                <<i
                        <<"j:"                <<j
                        <<"k:"                <<k
                        <<"changes:"          <<job[i][0][0].changes // признак того, что продукция была изменена. Хранится в начальной работе продукции
                        <<"priority:"         <<job[i][j][k].priority
                        <<"proj num:"         <<job[i][j][k].projectnum
                        <<"prod num:"         <<job[i][j][k].productnum
                        <<"proc num:"         <<job[i][j][k].procedurenum
                        <<"job num:"          <<job[i][j][k].num
                        <<"protection:"       <<job[i][j][k].protection
                        <<"beginmin:"         <<job[i][j][k].beginmin
                        <<"begin:"            <<job[i][j][k].begin
                        <<"time:"             <<job[i][j][k].time
                        <<"end:"              <<job[i][j][k].end
                        <<"endmax:"           <<job[i][j][k].endmax
                        <<"executor:"         <<job[i][j][k].executor.id
                        <<"executor.external:"<<job[i][j][k].executor.external
                        <<"people.code:"      <<b;
            } // конец цикла перебора работ
        } // конец цикла перебора процедур
    } // конец цикла продукций
}

void PrintPlan() // вывести в консоль сформированный план
{
    // Вывести сформированный план в консоль (только для отладки)
    qDebug("\n**********  Результаты планирования **********");
    int codetekold = 0;
    QString str="";
    //int kol=250; // кол. рабочих часов, выводимых на одной строке экрана (при kol>300 будет переносе строк и потеряется наглядность на экране)
    int kol=3920; // кол. рабочих часов, выводимых на одной строке экрана (при kol>300 будет переносе строк и потеряется наглядность на экране)

    // Сформировать и вывести информацию по работе (код исполнителя, код процедуры, код продукции, время  выполнения работы)
    for(int i=0; i<(int)job.size(); ++i){ // цикл перебора продукции
        for(int j=0; j<(int)job[i].size(); ++j){ // цикл перебора работ текущей продукции
            for(int k=0; k<(int)job[i][j].size(); ++k){ // цикл перебора работ
                int codetek = job[i][j][k].executor.id; // код текущего исполнителя
                // Сформировать и вывести строку-временную ось нерабочих дней исполнителя
                str="";
                for(int h=0; h<kol; ++h) { // цикл перебора дней, начиная с 0
                    int n = job[i][j][k].executor.rest.count(h); // найти номер дня (h) в векторе rest
                    if(n) str = str + "*"; // выходной день вывести, как "*"
                    else  str = str + "="; // рабочий день вывести, как "="
                }
                // если тек. исполнитель изменился, то вывести календарь текущего исполнителя (будет меньше осей времени, больше наглядности)
                if(codetekold != codetek) qDebug()<<(str + "calendar executor.code:" + QString::number(job[i][j][k].executor.id));
                codetekold = codetek; // запомнить код текущего исполнителя
                // Сформировать и вывести информационную строку по работе
                str="";
                for(int l=0; l<job[i][j][k].begin; ++l) str = str + "-"; // сформировать строку из дефисов, количеством job[i][j].begin
                str = str
                        + "(proj:"     + QString::number(job[i][j][k].projectnum)   // num проекта
                        + " prod:"     + QString::number(job[i][j][k].productnum)   // num продукции
                        + " proc:"     + QString::number(job[i][j][k].procedurenum) // num процедуры
                        + " job:"      + QString::number(job[i][j][k].num)          // num работы
                        + " ex:"       + QString::number(job[i][j][k].executor.id)  // id исполнителя
                        + " time:"     + QString::number(job[i][j][k].time)         // время выполнения работы
                        + " priority:" + QString::number(job[i][j][k].priority)     // приоритет работы (взят из приоритета проекта)
                        + ")";
                qDebug()<<str;
            } // конец цикла перебора работ
        } // конец цикла перебора работ
    } // конец цикла перебора продукций
}

void WritePlan(TCarryTaskList &tasks) // записать результаты планирования в объект tasks
{
    // записать найденные результаты планирования в объект tasks:
    // begin    в поле work->fDtPlanBegin
    // end      в поле work->fDtPlanEnd
    // executor в поле work->fEmployee
    // executor в поле proc->fEmployee

    foreach (TCarryTask *curTask,tasks) // цикл перебора проектов
    {
        // Время начала и конца текущего проекта (которые надо найти)
        int curTaskbegin = 999999;
        int curTaskend   = 0;

        for(int i=0; i<=1; i++){ // цикл перебора продукций (двух планов): План выпуска продукции (i=0) и План ОРД (i=1)
            TCarryPlan *plan; // plan - это продукция
            // Проверка. Это план выпуска продукции или ОРД?
            if(i==0) plan = curTask->carryPlan(); // план выпуска продукции
            else     plan = curTask->ordPlan();   // план ОРД

            if(!plan) continue; // пропустить пустую продукцию (план)

            if(!plan->procedures().size())continue; // пропустить продукцию, не содержащую процедуры

            // Время начала и конца текущей продукции (которые надо найти)
            int productbegin = 999999999;
            int productend   = 0;
            foreach (TCarryProcedure *proc,plan->procedures()) // цикл перебора процедур
            {
                // Время начала и конца текущей процедуры (которые надо найти)
                int procedurebegin = 999999;
                int procedureend   = 0;
                foreach (TCarryWork *work,proc->works()) // цикл перебора работ
                {
                    // зная id работы, найти из сформированного вектора job: время начала, конца и id исполнителя
                    int idtemp = work->id();                // id работы, который будем искать
                    job_type jobtemp = FindJob(idtemp);     // найти работу jobtemp в векторе job по ее id
                    int jobbegin     = jobtemp.begin;       // найденное время начала текущей работы
                    int jobend       = jobtemp.end;         // найденное время конца текущей работы
                    int executorid   = jobtemp.executor.id; // найденный исполнитель работы (он же исполнитель процедуры)

                    work->setDtPlanBegin(NumWorkHourYearToDateTime(jobbegin, begin)); // записать время начала текущей работы в формате QDateTime
                    work->setDtPlanEnd(NumWorkHourYearToDateTime(jobend, end));     // записать время конца текущей работы в формате QDateTime

                    // если есть изменения в работах продукции, то записать признак изменений в продукции. Для последующего сохранения продукции
                    if(jobtemp.changes) plan->setSaved(false); // т.е. продукция не сохранена в базе и ее надо сохранить

                    // зная id исполнителя найти его среди списка возможных исполнителей процедуры
                    foreach (TEmployee *empl, proc->possibleEmployees()) // цикл перебора возможных исполнителей процедуры
                    {
                        int tekid = empl->id(); // код текущего возможного исполнителя
                        if(executorid == tekid) { // проверка. Найден ли исполнитель?
                            work->setEmployee(empl); // записать найденного исполнителя для работы
                            proc->setEmployee(empl); // записать найденного исполнителя для процедуры
                            break;
                        }
                    } // цикл перебора возможных исполнителей процедуры
                    // Поиск времени начала и конца текущей процедуры
                    if(jobbegin < procedurebegin) procedurebegin = jobbegin;
                    if(jobend > procedureend) procedureend = jobend;
                    // Поиск времени начала и конца текущей продукции
                    if(jobbegin < productbegin) productbegin = jobbegin;
                    if(jobend > productend) productend = jobend;
                    // Поиск времени начала и конца текущего проекта
                    if(jobbegin < curTaskbegin) curTaskbegin = jobbegin;
                    if(jobend > curTaskend) curTaskend = jobend;
                } // конец цикла перебора работ

                proc->setDtPlanBegin(NumWorkHourYearToDateTime(procedurebegin, begin)); // записать время начала текущей процедуры в формате QDateTime
                proc->setDtPlanEnd(NumWorkHourYearToDateTime(procedureend, end));     // записать время конца текущей процедуры формате QDateTime
            } // конец цикла перебора процедур

            plan->setDtPlanBegin(NumWorkHourYearToDateTime(productbegin, begin)); // записать время начала текущей продукции в формате QDateTime
            plan->setDtPlanEnd(NumWorkHourYearToDateTime(productend, end));     // записать время конца текущей продукции в формате QDateTime
        } // конец плана (продукции)

        curTask->setDtPlanBegin(NumWorkHourYearToDateTime(curTaskbegin, begin)); // записать время начала текущего проекта в формате QDateTime
        curTask->setDtPlanEnd(NumWorkHourYearToDateTime(curTaskend, end));     // записать время начала текущего проекта в формате QDateTime
    } // конец цикла перебора проектов

}

int PlanJobBeginEnd(int &l, int &m, int &n, QString &errs) // прямое планирование работы
{
    // Прямое планирование. Спланировать работу job[l][m][n] (найти время начала и конца с учетом всех ограничений)
    //Исполнитель job[l][m][n].executor уже записан в работе

    int tbegin, tend; // время начала и конца работы, которые надо найти

    qApp->processEvents(); // показать ProgressDialog
    // Проверка. Нажата кн. Прервать? При долгом планировании.
    if(pprd->wasCanceled()){ // если нажата кн. Прервать
        numerror ++; // порядковый номер ошибки
        errs += QString("<br><b>%1</b>. Планирование прервано пользователем").arg(numerror);
        return 1;
    }

    // ПРОВЕРКА. Если работа защищенная, то ничего с ней не делать
    if(job[l][m][n].protection) return 0;

    // Бесконечный цикл перебора возможных дней начала тек. работы job[l][m][n]. Из цикла выйдем тогда, когда
    //найдем самое раннее время начала тек. работы, удовлетворяющее всем ограничениям
    for(int ttek = tbeginplan;  ; ++ttek){ // бесконечный цикл перебора рабочих часов года, начиная с текущего рабочего часа

        tbegin = tend = 0; // время начала и конца работы, которые надо найти

        // ПРОВЕРКА. Если текущий рабочий час года есть в списке нерабочих часов года исполнителя job[l][m][n].executor, то пропустить его
        if(job[l][m][n].executor.rest.count(ttek)) continue; // пропустить ttek

        // ПРОВЕРКА. Если текущий рабочий час года меньше минимально разрешенного времени начала работы, то пропустить его
        //По-умолчанию beginmin = 0, т.е., если минимално возможное время начала продукции (работы) не задано, то оно = 0
        if(ttek < job[l][m][n].beginmin) continue; // пропустить ttek

        // ПРОВЕРКА. Работа не может начаться ранее, чем завершится предыдущая (1-ый вариант проверки)
        //Если это не начальная работа процедуры, то для проверки нужна предыдущая работа той же процедуры
        if(n!=0) if(ttek <= job[l][m][n-1].end) continue; // пропустить ttek

        // ПРОВЕРКА. Работа не может начаться ранее, чем завершится предыдущая (2-ой вариант проверки)
        //Если это начальная работа процедуры, то для проверки нужна последняя работа предыдущей процедуры.
        if(m!=0 && n==0){  // это начальная работа процедуры. Надо найти конец последней работы предыдущей процедуры
            if(ttek <= job[l][m-1][job[l][m-1].size()-1].end) continue; // пропустить ttek
        }

        // Найти время начала (tbegin) и конца (tend) работы job[l][m][n] с учетом нерабочих часов (начиная поиск со времени ttek) для
        //исполнителя job[l][m][n].executor. Задана продолжительность работы: job[l][m][n].time
        FindBeginEndJob(job[l][m][n].executor, ttek, job[l][m][n].time, tbegin, tend); // нашли tbegin и tend

        // ПРОВЕРКА. Если исполнитель внешний,  то дублирование работ не проверять. Время начала и конца уже найдены.
        //Внешний(внутренний) исполнитель определяется, как: proc->employee()->role().isExtern()==true (внешний)
        if(job[l][m][n].executor.external) break; // внешний исполнитель. Не проверять на дублирование. Работа спланирована

        // ПРОВЕРКА предшествующих незащищенных работ. Если исполнитель внутренний, то проверить найденные tbegin и tend на
        //дублирование уже спланированных ранее незащищенных работ. Проверить незащищенные работы предшествующие индексу [l][m][n].
        bool flduplicate = false; // нет дублирования
        if(!job[l][m][n].executor.external){  // внутренний исполнитель
            for(int i=0; i<(int)job.size(); i++){ // цикл перебора продукции
                for(int j=0; j<(int)job[i].size(); j++){ // цикл перебора процедур
                    for(int k=0; k<(int)job[i][j].size(); k++){ // цикл перебора работ
                        if(job[i][j][k].executor.id != job[l][m][n].executor.id) continue; // пропустить работу с ненужным исполнителем
                        if(job[i][j][k].protection) continue; // пропустить защищенные работы, которые будут проверены в следующей проверке
                        // Проверка. Как только дойдем до тек. работы, проверку прервать
                        if((l==i) && (m==j) && (n==k)) // дошли до текущей работы, ее и последующие проверять не надо
                            goto metkastop1; // прервать все циклы, т.к. проверяем только прешествующие работы
                        // Проверка. Найдена предшествующая незащищенная работа с нужным исполнителем. Есть ли дублирование?
                        if(((tbegin >= job[i][j][k].begin) && (tbegin <= job[i][j][k].end)) || // точка tbegin проверяемого интервала попала внутрь текущего интервала
                           ((tend   >= job[i][j][k].begin) && (tend   <= job[i][j][k].end)) || // точка tend   проверяемого интервала попала внутрь текущего интервала
                           ((tbegin <= job[i][j][k].begin) && (tend   >= job[i][j][k].end))){  // проверяемый интервал полностью накрыл текущий интервал
                            flduplicate = true; // есть дублирование
                            goto metkastop1; // прервать все циклы, т.к. обнаружено дублирование
                        }
                    } // конец цикла перебора работ
                } // конец цикла перебора процедур
            } // конец цикла перебора продукции
metkastop1:; // метка досрочного выхода из цикла (,когда дошли до проверяемой работы или обнаружено дублирование
        }
        if(flduplicate) continue; // если есть дублирование, то пропустить текущий день ttek

        // ПРОВЕРКА всех защищенных оабот. Если исполнитель внутренний, то проверить найденные tbegin и tend на дублирование с защищенными
        //работами (по всем индексам работ: i, j, k). Проверить все только защищенные работы
        flduplicate = false; // нет дублирования
        if(!job[l][m][n].executor.external){  // внутренний исполнитель
            for(int i=0; i<(int)job.size(); i++){ // цикл перебора продукции
                for(int j=0; j<(int)job[i].size(); j++){ // цикл перебора процедур
                    for(int k=0; k<(int)job[i][j].size(); k++){ // цикл перебора работ
                        if(job[i][j][k].executor.id != job[l][m][n].executor.id) continue; // пропустить работу с ненужным исполнителем
                        if(!job[i][j][k].protection) continue; // пропустить незащищенную работу
                        // Проверка. Найдена защищенная работа job[i][j][k] с нужным исполнителем. Есть ли дублирование?
                        if(((tbegin >= job[i][j][k].begin) && (tbegin <= job[i][j][k].end)) || // точка tbegin проверяемого интервала попала внутрь текущего интервала
                           ((tend   >= job[i][j][k].begin) && (tend   <= job[i][j][k].end)) || // точка tend   проверяемого интервала попала внутрь текущего интервала
                           ((tbegin <= job[i][j][k].begin) && (tend   >= job[i][j][k].end))){  // проверяемый интервал полностью накрыл текущий интервал
                            flduplicate = true;  // есть дублирование
                            goto metkastop2; // прервать все циклы, т.к. обнаружено дублирование
                        }
                    } // конец цикла перебора работ
                } // конец цикла перебора процедур
            } // конец цикла перебора продукции
metkastop2:; // метка выхода из цикла
        }
        if(flduplicate) continue; // если есть дублирование, то пропустить текущий день ttek

        // Если дошли до конца цикла перебора ttek, значит все проверки пройдены. Даты начала и конца найдены.
        break; // выйти из цикла перебора рабочих часов, планирование работы job[l][m][n] - успешно завершено
    } // конец цикла перебора рабочих часов года

    // Из цикла вышли только тогда, когда работа спланирована, т.е. найдены tbegin и tend
    job[l][m][n].begin = tbegin; // записать найденное время начала
    job[l][m][n].end   = tend;   // записать найденное время конца

    return 0;
}

int PlanJobEndBegin(int &l, int &m, int &n, QString &errs) //НОВЫЙ БЫСТРЫЙ ВАРИАНТ обратное планирование работы
{
    // Обратное планирование. Спланировать работу job[l][m][n] (найти время конца и начала с учетом всех ограничений)
    //Исполнитель job[l][m][n].executor уже записан в работе.
    //Планировать с конца, т.е. вначале найти конец работы, и только потом найти начало работы с учетом ограничений

    int tbegin, tend; // время начала и конца работы, которые надо найти

    qApp->processEvents(); // показать ProgressDialog
    // Проверка. Нажата кн. Прервать? При долгом планировании.
    if(pprd->wasCanceled()){ // если нажата кн. Прервать
        numerror ++; // порядковый номер ошибки
        errs += QString("<br><b>%1</b>. Планирование прервано пользователем").arg(numerror);
        return 1;
    }

    // ПРОВЕРКА. Если работа защищенная, то ничего с ней не делать
    if(job[l][m][n].protection) return 0;

    // Бесконечный цикл перебора возможных часов КОНЦА тек. работы job[l][m][n] (справа налево). Из цикла выйдем тогда, когда
    //найдем самое позднее время НАЧАЛА тек. работы, удовлетворяющее всем ограничениям. Будем стремиться к тому, что бы не просто
    //"растянуть" работу и выполнить ее к заданному сроку. А выполнить работу за минимальное время к заданному сроку

    int endmax = job[l][m][n].endmax; //максимально заданное время конца продукции

    for(int ttek = endmax;  ; --ttek){ // бесконечный цикл перебора рабочих часов года (справа налево), начиная с максимально заданного времени конца продукции
        tbegin = tend = 0; // время начала и конца работы, которые надо найти

        // ПРОВЕРКА. Если текущий рабочий час года есть в списке нерабочих часов года исполнителя job[l][m][n].executor, то пропустить его
        if(job[l][m][n].executor.rest.count(ttek)) continue; // пропустить ttek

        // ПРОВЕРКА. Если текущий рабочий час года больше максимально разрешенного времени конца работы, то пропустить его
        if(ttek >= job[l][m][n].endmax) continue; // пропустить ttek. Нужна ли эта проверка???

        // ПРОВЕРКА. Работа не может закончиться позднее, чем начнется последующая (1-ый вариант проверки)
        //Если это не последняя работа процедуры, то для проверки нужна следующая работа той же процедуры
        int klast = job[l][m].size()-1; // индекс k последней работы в текущей процедуре, которой принадлежит работа job[l][m][n]
        if(n!=klast) if(ttek >= job[l][m][n+1].begin) continue; // пропустить ttek


        // ПРОВЕРКА. Работа не может закончиться позднее, чем начнется последующая (2-ой вариант проверки)
        //Если это последняя работа не последней процедуры, то для проверки нужна начальная работа следующей процедуры.
        if(n==klast && m!=job[l].size()-1)  {  // это последняя работа процедуры, но процедура не последняя. Надо найти начало начальной работы следующей процедуры
            if(ttek >= job[l][m+1][0].begin) continue; // пропустить ttek
        }

        // ПРОВЕРКА. Последняя работа последней процедуры - не проверяется, т.к. у нее нет последующих работ

        // Найти время начала (tbegin) и конца (tend) работы job[l][m][n] с учетом нерабочих часов (начиная поиск со времени ttek) для
        //исполнителя job[l][m][n].executor. Задана продолжительность работы: job[l][m][n].time. Планирование обратное, т.е. справа налево
        FindEndBeginJob(job[l][m][n].executor, ttek, job[l][m][n].time, tbegin, tend); // нашли tbegin и tend

        // ПРОВЕРКА. Если исполнитель внешний,  то дублирование работ не проверять. Время начала и конца уже найдены.
        //Внешний(внутренний) исполнитель определяется, как: proc->employee()->role().isExtern()==true (внешний)
        if(job[l][m][n].executor.external) break; // внешний исполнитель. Не проверять на дублирование. Работа спланирована

        // ПРОВЕРКА предшествующих незащищенных работ. Если исполнитель внутренний, то проверить найденные tbegin и tend на
        //дублирование уже спланированных ранее незащищенных работ. Проверить незащищенные работы предшествующие индексу [l][m][n].
        bool flduplicate = false; // нет дублирования
        if(!job[l][m][n].executor.external){  // внутренний исполнитель
            for(int i=0; i<(int)job.size(); i++){ // цикл перебора продукции (слева направо)
                for(int j=job[i].size()-1; j>=0; j--){ // цикл перебора процедур (справо налево)
                    for(int k=job[i][j].size()-1; k>=0; k--){ // цикл перебора работ (справо налево)
                        if(job[i][j][k].executor.id != job[l][m][n].executor.id) continue; // пропустить работу с ненужным исполнителем
                        if(job[i][j][k].protection) continue; // пропустить защищенные работы, которые будут проверены в следующей проверке
                        // Проверка. Как только дойдем до тек. работы, проверку прервать
                        if((l==i) && (m==j) && (n==k)) // дошли до текущей работы, ее и последующие проверять не надо
                            goto metkastop1; // прервать все циклы, т.к. проверяем только прешествующие работы
                        // Проверка. Найдена предшествующая незащищенная работа с нужным исполнителем. Есть ли дублирование?
                        if(((tbegin >= job[i][j][k].begin) && (tbegin <= job[i][j][k].end)) || // точка tbegin проверяемого интервала попала внутрь текущего интервала
                           ((tend   >= job[i][j][k].begin) && (tend   <= job[i][j][k].end)) || // точка tend   проверяемого интервала попала внутрь текущего интервала
                           ((tbegin <= job[i][j][k].begin) && (tend   >= job[i][j][k].end))){  // проверяемый интервал полностью накрыл текущий интервал
                            flduplicate = true; // есть дублирование
                            goto metkastop1; // прервать все циклы, т.к. обнаружено дублирование
                        }
                    } // конец цикла перебора работ
                } // конец цикла перебора процедур
            } // конец цикла перебора продукции
metkastop1:; // метка досрочного выхода из цикла (,когда дошли до проверяемой работы или обнаружено дублирование
        }
        if(flduplicate) continue; // если есть дублирование, то пропустить текущий день ttek

        // ПРОВЕРКА всех защищенных работ. Если исполнитель внутренний, то проверить найденные tbegin и tend на дублирование с защищенными
        //работами (по всем индексам работ: i, j, k). Проверить все только защищенные работы
        flduplicate = false; // нет дублирования
        if(!job[l][m][n].executor.external){  // внутренний исполнитель
            for(int i=0; i<(int)job.size(); i++){ // цикл перебора продукции (слева направо)
                for(int j=job[i].size()-1; j>=0; j--){ // цикл перебора процедур (справо налево)
                    for(int k=job[i][j].size()-1; k>=0; k--){ // цикл перебора работ (справо налево)
                        if(job[i][j][k].executor.id != job[l][m][n].executor.id) continue; // пропустить работу с ненужным исполнителем
                        if(!job[i][j][k].protection) continue; // пропустить незащищенную работу
                        // Проверка. Найдена защищенная работа job[i][j][k] с нужным исполнителем. Есть ли дублирование?
                        if(((tbegin >= job[i][j][k].begin) && (tbegin <= job[i][j][k].end)) || // точка tbegin проверяемого интервала попала внутрь текущего интервала
                           ((tend   >= job[i][j][k].begin) && (tend   <= job[i][j][k].end)) || // точка tend   проверяемого интервала попала внутрь текущего интервала
                           ((tbegin <= job[i][j][k].begin) && (tend   >= job[i][j][k].end))){  // проверяемый интервал полностью накрыл текущий интервал
                            flduplicate = true;  // есть дублирование
                            goto metkastop2; // прервать все циклы, т.к. обнаружено дублирование
                        }
                    } // конец цикла перебора работ
                } // конец цикла перебора процедур
            } // конец цикла перебора продукции
metkastop2:; // метка выхода из цикла
        }
        if(flduplicate) continue; // если есть дублирование, то пропустить текущий день ttek

        // Если дошли до конца цикла перебора ttek, значит все проверки пройдены. Даты начала и конца найдены.
        break; // выйти из цикла перебора рабочих часов, планирование работы job[l][m][n] - успешно завершено
    } // конец цикла перебора рабочих часов года

    // Из цикла вышли только тогда, когда работа спланирована, т.е. найдены tbegin и tend
    job[l][m][n].begin = tbegin; // записать найденное время начала
    job[l][m][n].end   = tend;   // записать найденное время конца

    return 0;
}

int PlanProductBeginEnd(int &i, QString &errs){ // прямое планирование продукции
    // Спланировать продукцию у которой не задано максимально разрешенное время конца.
    //При этом минимальное разрешенное время начала м.б. задано или не задано

    for(int j=0; j<(int)job[i].size(); j++){ // цикл перебора процедур
        // Каждая процедура м.б. выполнена несколькими исполнителями. Проверить каждого. Кто быстрее выполнит, того и выбрать
        int endmin = 999999999; // минимальное время конца процедуры
        executor_type executormin = job[i][j][0].people[0]; // пока лучший исполнитель (из начальной работы процедуры)
        for(int indexecutor = 0; indexecutor<(int)job[i][j][0].people.size(); indexecutor++){ // цикл перебора возможных исполнителей тек. процедуры
            // назначить текущего исполнителя: job[i][j][k].people[indexecutor] всем работам текущей процедуры
            for(int k=0; k<(int)job[i][j].size(); k++) job[i][j][k].executor = job[i][j][k].people[indexecutor];
            // спланировать все работы текущей процедуры: job[i][j] текущим исполнителем: job[i][j][k].people[indexecutor]
            for(int k=0; k<(int)job[i][j].size(); k++){
                if(PlanJobBeginEnd(i, j, k, errs) !=0) return 1; // планирование работ процедуры
            }
            // Определить время окончания текущей процедуры, как время конца последней работы в текущей процедуре
            int klast = job[i][j].size()-1; // индекс k последней работы в текущей процедуре
            int end = job[i][j][klast].end; // время конца последней работы текущей процедуры (т.е. время конца тек. процедуры)
            // Проверка. Если это лучшее время выполнения всей тек. процедуры, то запомнить это время и этого исполнителя
            if(end < endmin){
                endmin      = job[i][j][klast].end;
                executormin = job[i][j][klast].executor;
            }
        } // конец цикла перебора возможных исполнителей процедуры. Найден лучший исполнитель executormin
        // назначить найденного лучшего исполнителя: executormin всем работам текущей процедуры
        for(int k=0; k<(int)job[i][j].size(); k++) job[i][j][k].executor = executormin;
        // Окончательно (еще раз) спланировать все работы текущей процедуры: job[i][j] теперь уже с лучшим исполнителем
        for(int k=0; k<(int)job[i][j].size(); k++){
            if(PlanJobBeginEnd(i, j, k, errs) != 0) return 1; // планирование работ процедуры
        }
    } // конец цикла перебора процедур
    return 0;
}

int PlanProductEndBegin(int &i, QString &errs){ // обратное планирование продукции
    // Спланировать продукцию у которой задано максимально разрешенное время конца продукции
    //При этом минимальное разрешенное время начала м.б. задано или не задано

    //for(int j=0; j<(int)job[i].size(); j++){ // цикл перебора процедур
    for(int j=job[i].size()-1; j>=0; j--){ // обратный цикл перебора процедур i-ой продукции

        // Каждая процедура м.б. выполнена несколькими исполнителями. Проверить каждого. Кто позже начнет выполнять процедуру и
        //завершит выполнение процедуры к заданному сроку, того и выбрать. Т.е., кто выполнить работу за минимальное время к заданному
        //сроку.

        //int endmin = 999999999; // минимальное время конца процедуры
        int beginmax = 0; // максимальное время начала процедуры. Т.е. максимальное время, когда можно начать продукцию и завершить ее к заданному времени

        //executor_type executormin = job[i][j][0].people[0]; // пока лучший исполнитель (из начальной работы процедуры)
        executor_type executormax = job[i][j][0].people[0]; // пока лучший исполнитель (из начальной работы процедуры), который начнет выполнение процедуры
        //в максимальное время и выполнит ее к заданному сроку

        for(int indexecutor = 0; indexecutor<(int)job[i][j][0].people.size(); indexecutor++){ // цикл перебора возможных исполнителей тек. процедуры
            // назначить текущего исполнителя: job[i][j][k].people[indexecutor] всем работам текущей процедуры

            //for(int k=0; k<(int)job[i][j].size(); k++) job[i][j][k].executor = job[i][j][k].people[indexecutor];
            for(int k=job[i][j].size()-1; k>=0; k--) job[i][j][k].executor = job[i][j][k].people[indexecutor];

            // спланировать все работы текущей процедуры: job[i][j] текущим исполнителем: job[i][j][k].people[indexecutor]

            //for(int k=0; k<(int)job[i][j].size(); k++){
            for(int k=job[i][j].size()-1; k>=0; k--){ // цикл обратного перебора работ

                //if(PlanJobBeginEnd(i, j, k, errs) !=0) return 1; // планирование работ процедуры
                if(PlanJobEndBegin(i, j, k, errs) !=0) return 1; // обратное планирование работ процедуры

            } // конец цикла обратного перебора работ

            // Определить время окончания текущей процедуры, как время конца последней работы в текущей процедуре
            // Определить время начала текущей процедуры, как время начала 0-ой работы в текущей процедуре

            //int klast = job[i][j].size()-1; // индекс k последней работы в текущей процедуре
            //int end = job[i][j][klast].end; // время конца последней работы текущей процедуры (т.е. время конца тек. процедуры)
            int begin = job[i][j][0].begin; // время начала 0-ой работы текущей процедуры (т.е. время начала тек. процедуры)


            // Проверка. Если это лучшее (самое позднее!) время начала выполнения тек. процедуры, то запомнить это время и этого исполнителя
            //if(end < endmin){
            if(begin > beginmax){

                //endmin      = job[i][j][klast].end;
                beginmax      = job[i][j][0].begin; // запомнить начало процедуры

                //executormin = job[i][j][klast].executor;
                executormax = job[i][j][0].executor;
            }

        } // конец цикла перебора возможных исполнителей процедуры. Найден лучший исполнитель executormin

        // назначить найденного лучшего исполнителя: executormin всем работам текущей процедуры
        // назначить найденного лучшего исполнителя: executormax всем работам текущей процедуры


        //for(int k=0; k<(int)job[i][j].size(); k++) job[i][j][k].executor = executormin;
        for(int k=job[i][j].size()-1; k>=0; k--) job[i][j][k].executor = executormax;

        // Окончательно (еще раз) спланировать все работы текущей процедуры: job[i][j] теперь уже с лучшим исполнителем

        //for(int k=0; k<(int)job[i][j].size(); k++){
        for(int k=job[i][j].size()-1; k>=0; k--){

            //if(PlanJobBeginEnd(i, j, k, errs) != 0) return 1; // планирование работ процедуры
            if(PlanJobEndBegin(i, j, k, errs) != 0) return 1; // планирование работ процедуры

        }
    } // конец обратного цикла перебора процедур i-ой продукции
    return 0;
}

int Plan(TCarryTaskList &tasks, QString &errs) // основная процедура
{
    //??? Не ясно до конца, как работать с началом года? Если разные проекты в базе будут иметь разные года?
    // Пока год планирования взять из начального проекта
    beginyearplan = QDate(tasks[0]->year(), 1, 1); // начало года планирования в формате QDate (нужен для функции addDays())

    // Найти tbeginplan - время начала планирования. Это начальный час, с которого будем планировать. Это текущий рабочий час года (аналог текущей даты)
    QDateTime currentDateTime(QDate::currentDate(), QTime::currentTime()); // текущие: дата и время
    tbeginplan = DateTimeToNumWorkHourYear(currentDateTime, begin);        // текущий рабочий час года (от 0 до 365*8)

    if(TestTCarryTaskBeforePlanning(tasks, errs)) return 5; // проверить содержимое объекта tasks до начала планирования
    if(flprint>=3) PrinTCarryTask(tasks);                   // для отладки вывести в консоль содержимое объекта tasks до начала планирования
    FillVectorJob(tasks);                                   // заполнить вектор job данными из объекта tasks
    SortingVectorJob();                                     // сортировка вектора job в соответствии с приоритетами проектов
    if(flprint>=2) PrintJob();                              // для отладки вывести в консоль данные вектора job в табличной форме до начала планирования
    if(TestData(errs)) return 1;                            // проверить данные в векторе job до начала планирования
    Progress();                                             // Создать ProgressDialog и внутри него ProgressBar

    // Планирование продукций
    for(int i=0; i<(int)job.size(); i++){ // цикл перебора продукций
        // Проверка. Заданно ли максимально разрешенное время конца продукции?
        if(job[i][0][0].endmax){ // Да, есть максимально разрешенное время конца продукции
            if(PlanProductEndBegin(i, errs)) return 1;   // обратное планирование продукции (задано максимально разрешенное время конца). Достоинство - быстрота
        }
        else {
            if(PlanProductBeginEnd(i, errs)) return 1;  // прямое планирование продукции (не задано максимально разрешенноое время конца)
        }
    } // конец цикла перебора продукций

    delete pprb; // удалить ProgressBar
    delete pprd; // удалить ProgressDialog

    if(TestPlan(errs)) return 1;                           // проверить сформированный план на грубые ошибки
    if(flprint>=2) PrintJob();                             // для отладки вывести данные вектора job после планирования
    if(flprint>=1) PrintPlan();                            // для отладки вывести в консоль сформированный план
    WritePlan(tasks);                                      // записать результаты планирования в объект tasks
    if(TestTCarryTaskAfterPlanning(tasks, errs)) return 7; // проверить содержимое объекта tasks после записи в него результатов планирования
    if(flprint>=3) PrinTCarryTask(tasks);                  // для отладки вывести содержимое объекта tasks после записи в него результатов планирования

    return 0; // успешное завершение планирования
}

// Основная функция планирования
// tasks - список проектов      - ВХОД и ВЫХОД
// ers  - сообщение об ошибках - ВЫХОД
// ВОЗВРАТ - признак успешности
bool TModulePlans::createCarryPlans(TCarryTaskList &tasks, QString &errs)
{
    numerror = 0; // обнулить порядковый номер ошибки

    errs = "";
    if (!tasks.count()){
        errs += QString("<br>Ошибка. Список проектов пуст");
        return false;
    }

    int kodvozvrata = Plan(tasks, errs);
    if(kodvozvrata) return false;
    else return true;
}
