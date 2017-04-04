#include <defMacro>
#include <TModuleEmployees>
#include <TModulePlans>
#include <QMessageBox>
#include <QtCore>
//#include <TWorkCalendar> //??? Надо ли???

// Терминология:
// Номер рабочего часа года (м.б. от 0 до 365*8). Год состоит из 8-ми часовых суток
// Номер дня года (м.б. от 0 до 365)
// Астрономическое время в формате QDateTime. Минуты и секунды игнорируются
// Номер рабочего часа суток (м.б. от 0 до 7). Количество рабочих часов в сутках = kolhours
// Начало рабочего дня beginworkday = 9 часов
// Рабочий день с 9:00 до 17:00. 8 рабочих часов начинаются в: 9, 10, 11, 12, 13, 14, 15, 16 часов

// объявить глобальные переменные (задать неименованное пространство имен. Скрыть видимость переменных из вне)
namespace {

    int tbeginplan      = 0;       // номер рабочего часа года (начиная с 0), с которого начнется планирования
    int numerror        = 0;       // порядковый номер ошибки для сообщения
    int kolworkhoursday = 8;       // количество рабочих часов в сутках
    int beginworkday    = 9;       // начало рабочего дня 9:00. Первый рабочий час будет начинаться в 9:00. Последний
                                   //рабочий час будет начинаться в (beginworkday + kolhours - 1) в 16:00
    int flprint         = 1;       // флаг вывода отладочной информации в консоль: 0-ничего; 1-ошибки, план; 2-ошибки, план, job, tasks
    QDate beginyearplan(2017,1,1); // начало года планирования

    struct executor_type { // структура исполнителя
        int  id;           // id исполнителя
        bool external;     // исполнитель внешний (true) или внутренний (false)
        QVector<int>rest;  // вектор нерабочих дней исполнителя (отпуск, командировка, праздничные и вых. дни). Начиная с 0
    };

    struct job_type { // структура работы
        int                    id;           // id работы
        int                    priority;     // приоритет проекта, которому принадлежит работа. 0 - низший приоритет
        int                    projectnum;   // номер проекта, которой принадлежит работа (только для формирования сообщения об ошибке)
        int                    productnum;   // номер продукции, которой принадлежит работа (только для формирования сообщения об ошибке)
        int                    procedurenum; // номер процедуры, которой принадлежит работа (только для формирования сообщения об ошибке)
        int                    num;          // номер работы (только для формирования сообщения об ошибке)
        int                    begin;        // время начала выполнения работы
        int                    time;         // полное время выполнения работы
        int                    end;          // время окончания работы (с учетом нерабочих дней)
        executor_type          executor;     // назначенный исполнитель (из списка people)
        QVector<executor_type> people;       // возможные исполнители работы
        bool                   protection;   // признак защиты работы от редактирования (true - защищена; false - можно редактировать)

    };

    QVector<QVector<QVector<job_type>>> job;  // вектор работ, содержащий всю основную информацию по всем работам
    QVector<executor_type> executor;          // вектор исполнители (данные о всех возможных исполнителях всех планируемых работ)
    QVector<int>holidey;                      // вектор праздничные и выходных дней (календарь предприятия). Начиная с 0
}

// объявить прототипы функций
void      FillHoliday();
int       TestTCarryTask(TCarryTaskList &tasks, QString &errs);
int       Plan(TCarryTaskList &tasks, QString &errs);
void      FindBeginEndJob(executor_type &executor, int &tstart, int &time, int &tbegin, int &tend);
int       TestPlan(QString &errs);
int       TestData(QString &errs);
void      PrintPlan();
void      PrintJob();
void      PrinTCarryTask(TCarryTaskList &tasks);
job_type  FindJob(int &code);
void      SortingVectorJob();
void      FillVectorJob(TCarryTaskList &tasks);
void      WritePlan(TCarryTaskList &tasks);
void      PlanJob(int &l, int &m, int &n);
int       Plan(TCarryTaskList &tasks, QString &errs);
void      SetProtection();
QDateTime NumWorkHourYearToDateTime(int &n);
int       DateTimeToNumWorkHourYear(QDateTime &dt);


QDateTime NumWorkHourYearToDateTime(int &n) // перейти от номера рабочего часа года к QDateTime
{
    // По номеру рабочего часа года (от 0 до 368*8) найти астрономическую дату начала астрономического часа в формате QDateTime.

    int numday  = n / kolworkhoursday; // номер дня года, начиная с 0
    int numhour = n % kolworkhoursday; // номер рабочего часа суток (м.б. от 0 до 7 (kolhours-1))

    QDate date = beginyearplan.addDays(numday); // найденная астрономическая дата в формате QDate (beginyearplan - нач. года планирования)
    QTime time(beginworkday + numhour,0);       // найденное астрономическое время начала часа в формате QTime. Минуты всегда = 0

    QDateTime datetime(date, time); // найденное астрономическое время начала часа в формате QDateTime

    return datetime;
}

int DateTimeToNumWorkHourYear(QDateTime &dt) // перейти от даты QDateTime к номера рабочего часа года
{
    // По дате в формате QDateTime найти номер рабочего часа года (от 0 до 365*8)

    int n;    // номер рабочего часа суток (от 0 до 7)

    QDate day = dt.date();  // дата
    QTime time = dt.time(); // время

    int deyofyear = day.dayOfYear() - 1; // номер дня года, начиная с 0
    int hour      = time.hour();         // номер часа в сутках (от 0 до 23)

    // Найти номер рабочего часа в сутках (от 0 до 7)
    if(hour <  beginworkday)  n = 0; //если до начала рабочего дня (до 9 часов), то будет 0-ой рабочий час
    else if(hour > (beginworkday + kolworkhoursday - 1)) n = kolworkhoursday - 1; //если после окончания рабочего дня (после 16 часов), то будет 7
    else n = hour - beginworkday; // в течении рабочего дня (c 9 до 16) будет: 0, 1, 2, 3, 4, 5, 6 или 7

    // Найти номер рабочего часа года (от 0 до 365*8)
    int num = deyofyear * kolworkhoursday + n;

    return num;
}

void SetProtection() // установить признак защиты работ
{
    // Установить принудительно признак защиты работ, которые по плану завершены или продолжаются на момент планирования.
    //Для всех остальных работ признак защиты принудительно снять. Это необходимо в том случае, когда, например,
    //появляется новая продукция с высоким приоритетом, которой надо выделить максимум ресурсов, а старая защищенная работа
    //держит ресурсы. И эти ресурсы не могут быть выделены новой приоритетной работе

    qDebug()<<"работает установка защиты";

    for(int i=0; i<(int) job.size(); ++i){ // цикл перебора продукций
        for(int j=0; j<(int) job[i].size(); ++j){ // цикл перебора процедур текущей продукции
            for(int k=0; k<(int) job[i][j].size(); ++k){ // цикл перебора работ текущей продукции

                job[i][j][k].protection = false; // вначале, по-умолчанию, снять защиту с текущей работы

                // Проверка. Если работа еще не спланирована, то снять с нее защиту
                if(job[i][j][k].end == 0) {
                    job[i][j][k].protection = false;
                    continue;
                }

                // Проверка. Если сейчас работа уже завершена, то защитить ее от возможного изменения (работа в прошлом)
                if(job[i][j][k].end <= tbeginplan) {
                    job[i][j][k].protection = true;
                    continue;
                }

                // Проверка. Если работа сейчас выполняется, то защитить ее от возможного изменения
                if((job[i][j][k].begin <= tbeginplan) && (job[i][j][k].end >= tbeginplan)){
                    job[i][j][k].protection = true;
                    continue;
                }

            } // конец цикла перебора работ текущей продукции
        } // конец цикла перебора процедур текущей продукции
    } // конец цикла перебора продукций
}

void FillHoliday() // заполнить календарь предприятия рабочими часами
{
    // ДЛЯ ОТЛАДКИ ЗАПОЛНИТЬ СУББОТАМИ И ВОСКРЕСЕНЬЯМИ 2017 года ВЕКТОР holidey
    //Возможно, этот календарь не понадобится. Все нерабочие дни будут в календарях исполнителей

    // Заполнить вектор номерами выходных дней: суббота и воскресенье. Здесь же можно задать и номера праздничных дней
    holidey.clear() ;// очистить вектор holidey
    // Заполнить вектор выходных и праздничных дней (календарь предприятия)
    for(int i=-1; i<=365; i=i+7){ // на 2017 год. 1 января - вск. 0,  6,7,  13,14,  20,21,...
        if(i!=-1) holidey.push_back(i);
        holidey.push_back(i+1);
    }

    // В векторе holidey перейти от номеров дней года к номерам рабочих часов года
    QVector<int>holideyhours; // временный вектор праздничных часов
    for(int i=0; i<(int)holidey.size(); ++i){ // цикл перебора номеров выходных и праздничных дней
        for(int j=0; j<kolworkhoursday; ++j){ // цикл перебора рабочих часов суток
            holideyhours.push_back(holidey[i] * kolworkhoursday + j);
        } // конец цикла перебора рабочих часов суток
    } // конец цикла перебора номеров выходных и праздничных дней

    holidey.clear(); // очистить вектор от номеров дней года
    for(int i=0; i<(int)holideyhours.size(); i++) holidey.push_back(holideyhours[i]); // скопировать вектор holideyhours в вектор holidey
    // теперь в векторе holidey содержатся номера рабочих часов года, начиная с 0. Количество рабочих часов в сутках равно kolhours
}

int TestTCarryTask(TCarryTaskList &tasks, QString &errs) // проверить объект tasks
{
    // Проверить данные объекта tasks на ошибки, которые не позволят начать планирование
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
                // Проверка. Если список возможных исполнителей процедуры пуст, то прекратить планирование
                if(proc->possibleEmployees().empty()){
                    flerror = true; // признак наличия ошибки
                    numerror ++; // порядковый номер ошибки
                    errs += QString("<br><b>%1</b>. Пустой список исполнителей в процедуре: %2.%3.%4").arg(numerror).arg(curTask->num()).arg(plan->num()).arg(proc->num());
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

    // Проверка. Были ли ошибки при проверке tasks?
    if(flerror) return 1; // есть ошибки. Планирование отменяется
    else return 0;        // норм. завершение функции. Ошибок - нет
}

int TestData(QString &errs) // тестирование векторов executor и job
{
    // Проверка на корректность данных, без которых нельзя начинать планирование

    // Проверка на существование вектора об исполнителях executor
    if(executor.size() == 0) {
        if(flprint>=1) qDebug()<<"Ошибка. Вектор executor (сведения об исполнителях) пустой";
        numerror ++; // порядковый номер ошибки
        errs += QString("<br><b>%1</b>. Не найден ни один исполнитель");
        return 1;
    }

    // Проверка id исполнителя в векторе executor
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
                errs += QString("<br><b>%1</b>. Обнаружено пустое время работы").arg(numerror);
                return 6;
            }

            // Работа должна иметь список возможных исполнителей
            if(!job[i][j][k].people.size()) {
                if(flprint>=1) qDebug()<<"Ошибка. Вектор job, поле people пустое. i: "<<QString::number(i)<<", j: "<<QString::number(j);
                numerror ++; // порядковый номер ошибки
                errs += QString("<br><b>%1</b>. Пустой список возможных исполнителей процедуры").arg(numerror);
                return 7;
            }

            // Проверка защищенных работ. У этих работ д.б. заполнены поля: begin, end, executor
            if(job[i][j][k].protection) {
                // Д.б. заполнен исполнитель (поле executor)
                if(job[i][j][k].executor.id == 0) {
                    if(flprint>=1) qDebug()<<"Ошибка. Защищенная работа. Вектор job, поле executor.code=0 (пустое). i: "<<QString::number(i)<<", j: "<<QString::number(j);
                    numerror ++; // порядковый номер ошибки
                    errs += QString("<br><b>%1</b>. В защищенной работе пустой исполнитель").arg(numerror);
                    return 10;
                }
            }
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
                                qDebug()<<"tekid"<<tekid;
                                errs += QString("<br><b>%1</b>. "
                                                "Нарушение уникальности id работы: "
                                                "%2.%3.%4").arg(numerror).arg(job[i][j][k].productnum).
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
    // Тестирование сформированного плана на ошибки

    // Каждая последующая работа должна начинается после завершения предыдущей (в рамках одной продукции)
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

    // Проверка на дублирование одновременно выполняемых работ одним исполнителем
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
                                QMessageBox::information(0,"Отладка", "Дублирование работ одним исполнителем");
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

    return 0; // ошибок нет
}

void FindBeginEndJob(executor_type &executor, int &tstart, int &time, int &tbegin, int &tend) // найти начало и конец работы
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
        ++ttek; // переход к следующему часу для его проверки
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
    //"Самая верхняя" продукция (для i=0) должна иметь высший приоритет. "Самая нижняя" продукция с наименьшим приритетом будет п
    //олучать ресурсы исполнителей по остаточному принципу, в последнюю очередь. Чем "выше" приоритет продукция, тем быстрее
    //она будет выполнена

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
    // функция "заполнить вектор работ" извлекает из принятого параметра tasks все необходимые сведения по всеми
    //работам, процедурам и продукциям. Заполняет вектор job, необходимый для планирования.
    // Вектор job[i][j][k] - трехмерный динамический массив, где i - индекс продукции, j - индекс процедуры, k - индекс работы.

    job.clear();      // предварительно очистить вектор работ
    executor.clear(); // предварительно очистить вектор исполнителей

    int priority = 0;      //??? приритет проекта. Только отладка

    // прочитать данные tasks и записать их в рабочий вектор job, который является основным при планировании
    foreach (TCarryTask *curTask, tasks){ // цикл перебора проектов

        // План выпуска продукции и План выпуска ОРД - это две разные продукции. Надо загрузить последовательно обе продукции.
        //В первом проходе считывается продукция "План выпуска продукции", а во втором проходе считывается продукция "План ОРД"
        for(int i=0; i<=1; i++){ // цикл перебора продукций (двух планов): План выпуска продукции (i=0) и План ОРД (i=1)
            TCarryPlan *plan;

            priority = priority + 1; //??? установить приоритет проекта. Только отладка

            // Проверка продукции. Это план выпуска продукции или план ОРД? Каждый план это продукция
            if(i==0) plan = curTask->carryPlan(); // план выпуска продукции
            else     plan = curTask->ordPlan();   // план ОРД

            // Проверка. Если нет плана (продукции), то пропустить его
            if(plan->procedures().size()==0)continue; // пропустить план (продукцию), в котором нет процедур

            QVector<QVector<job_type>> tempvectorproc;
            foreach (TCarryProcedure *proc, plan->procedures()){ // цикл перебора процедур

                QVector<job_type> tempvectorwork;
                foreach (TCarryWork *work, proc->works()){ // цикл перебора работ

                    // Проверка "плохой" работы. Если в защищенной работе не заполнены: начало или конец или исполнитель, то
                    //такую работу пропустить и не загружать в вектор job
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
                    jobtemp.priority     = priority;            // приоритет работы (соответствует приоритету продукции)
                    jobtemp.num          = work->num();         // номер работы
                    jobtemp.procedurenum = proc->num() ;        // номер процедуры, которой принадлежит данная работа
                    jobtemp.productnum   = plan->num();         // номер продукции, которой принадлежит данная работа
                    jobtemp.projectnum   = curTask->num();      // номер проекта, которому принадлежит данная работа
                    jobtemp.time         = work->planPeriod();  // время выполнение работы
                    jobtemp.protection   = !work->isVolatile(); // признак защиты работы

                    // Заполнить список возможных исполнителей работы jobtemp.people
                    foreach (TEmployee *empl, proc->possibleEmployees()) { // цикл перебора возможных исполнителей
                        int idtek = empl->id();   // id текущего исполнителя
                        executor_type executortemp; // структура нового исполнителя, которую надо заполнить, а затем сохранить
                        executortemp.id       = idtek;                   // записать код текущего исполнителя
                        executortemp.external = empl->role().isExtern(); // записать внешний/внутренний исполнитель
                        // Заполнить executortemp.rest (календарь исполнителя) нерабочими часами исполнителя
                        //???...
                        if(empl->calendar()){
                            qDebug()<<"*******************************************************календарь"<<empl->calendar();
                            //qDebug()<<"календарь"<<empl->calendar()->fullRestDates();
                            //QList<int> calendar;
                            //TEmployeeCalendar a=empl->calendar()->id();
                            //if(codetek==89){
                            //qDebug()<<"proba"<<empl->calendar()->fullRestDates()->size();
                            //qDebug()<<"proba"<<empl->calendar()->id();
                            //qDebug()<<"proba"<<empl->calendar()->name();
                            //TEmployeeCalendar calendar = empl->calendar();
                        }

                        // Скопировать нерабочие часы из календаря предприятия holidey в календарь возможного исполнителя
                        for(int i=0; i<(int)holidey.size(); i++) executortemp.rest.push_back(holidey[i]); // записать календарь исполнителя

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

                        jobtemp.begin             = work->dtPlanBegin()->date().dayOfYear() - 1;              // начало работы. Номер дня года, начиная с 0
                        // Перейти от номера дня года к номеру рабочего часа года
                        //???...jobtemp.begin = ...                                                           // начало работы. Номер часа года, начиная с 0

                        jobtemp.end               = work->dtPlanEnd()->date().dayOfYear() - 1;                // конец работы. Номер дня года, начиная с 0
                        // Перейти от номера дня года к номеру рабочего часа года
                        //???...jobtemp.begin = ...                                                           // конец работы. Номер часа года, начиная с 0


                        jobtemp.executor.id       = proc->employee()->id();                                   // код исполнителя

                        // Скопировать нерабочие часы из календаря предприятия holidey в календарь исполнителя
                        for(int i=0; i<(int)holidey.size(); i++) jobtemp.executor.rest.push_back(holidey[i]);

                        jobtemp.executor.external = proc->employee()->role().isExtern();                      // внешний или внутренний исполнитель
                    }
                    else{ // незащищенная работа обнуляется перед планированием
                        jobtemp.begin             = 0;     // начала работы
                        jobtemp.end               = 0;     // конец работы
                        jobtemp.executor.id       = 0;     // исполнителя нет
                        jobtemp.executor.rest     = {};    // исполнителя нет
                        jobtemp.executor.external = false; // Пока так. Это поле будет заполнено позднее, при планировании
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

        for(int i=0; i<=1; i++){ // цикл перебора двух продукций (планов): План выпуска продукции (i=0) и План ОРД (i=1)
            TCarryPlan *plan; // plan - это продукция

            // Проверка. Это план выпуска продукции или ОРД?
            if(i==0) plan = curTask->carryPlan(); // план выпуска продукции
            else     plan = curTask->ordPlan();   // план ОРД

            // Проверка. Есть ли продукция?
            if(!plan) continue; // пропустить пустую продукцию
            qDebug()<<"    Продукция (план) id="<<plan->id();


            foreach (TCarryProcedure *proc,plan->procedures()) // цикл перебора процедур
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
                        <<"priority:"         <<job[i][j][k].priority
                        <<"prod num:"         <<job[i][j][k].productnum
                        <<"proc num:"         <<job[i][j][k].procedurenum
                        <<"job num:"          <<job[i][j][k].num
                        <<"protection:"       <<job[i][j][k].protection
                        <<"begin:"            <<job[i][j][k].begin
                        <<"time:"             <<job[i][j][k].time
                        <<"end:"              <<job[i][j][k].end
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
    int kol=250; // кол. дней выводимых на одной строке экрана (при kol>300 будет переносе строк и потеряется наглядность на экране)

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
                        + "(proj:" + QString::number(job[i][j][k].projectnum)   // num проекта
                        + " prod:" + QString::number(job[i][j][k].productnum)   // num продукции
                        + " proc:" + QString::number(job[i][j][k].procedurenum) // num процедуры
                        + " job:"  + QString::number(job[i][j][k].num)          // num работы
                        + " ex:"   + QString::number(job[i][j][k].executor.id)  // id исполнителя
                        + " time:" + QString::number(job[i][j][k].time)         // время выполнения работы
                        + ")";
                qDebug()<<str;
            } // конец цикла перебора работ
        } // конец цикла перебора работ
    } // конец цикла перебора продукций
}

void WritePlan(TCarryTaskList &tasks) // записать результаты планирования в объект tasks
{
    // записать найденные результаты планирования в объект tasks:
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

            if(!plan->procedures().size())continue; // пропустить пустую продукцию

            // Время начала и конца текущей продукции (которые надо найти)
            int productbegin = 999999;
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

                    work->setDtPlanBegin(NumWorkHourYearToDateTime(jobbegin)); // записать время начала текущей работы в формате QDateTime
                    work->setDtPlanEnd(NumWorkHourYearToDateTime(jobend));     // записать время конца текущей работы в формате QDateTime

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
                proc->setDtPlanBegin(NumWorkHourYearToDateTime(procedurebegin)); // записать время начала текущей процедуры в формате QDateTime
                proc->setDtPlanEnd(NumWorkHourYearToDateTime(procedureend));     // записать время конца текущей процедуры формате QDateTime
            } // конец цикла перебора процедур
            plan->setDtPlanBegin(NumWorkHourYearToDateTime(productbegin)); // записать время начала текущей продукции в формате QDateTime
            plan->setDtPlanEnd(NumWorkHourYearToDateTime(productend));     // записать время конца текущей продукции в формате QDateTime
        } // конец плана (продукции)
        curTask->setDtPlanBegin(NumWorkHourYearToDateTime(curTaskbegin)); // записать время начала текущего проекта в формате QDateTime
        curTask->setDtPlanEnd(NumWorkHourYearToDateTime(curTaskend));     // записать время начала текущего проекта в формате QDateTime
    } // конец цикла перебора проектов

}

void PlanJob(int &l, int &m, int &n) // спланировать конкретную работу с конкретным исполнителем
{
    // Спланировать работу job[l][m][n] (найти время начала и конца с учетом всех ограничений)
    //Исполнитель job[l][m][n].executor уже записан в работе

    int tbegin, tend; // время начала и конца работы, которые надо найти

    // ПРОВЕРКА. Если работа защищенная, то ничего не делать
    if(job[l][m][n].protection) return;

    // Бесконечный цикл перебора возможных дней начала тек. работы job[l][m][n]. Из цикла выйдем тогда, когда
    //найдем самое раннее время начала тек. работы, удовлетворяющее всем ограничениям
    for(int ttek = tbeginplan;  ; ++ttek){ // бесконечный цикл перебора дней ttek, начиная с сегодняшнего дня

        tbegin = tend = 0; // время начала и конца работы, которые надо найти

        // ПРОВЕРКА. Если текущий день есть в списке нерабочих дней исполнителя job1[l][m][n].executor, то пропустить его
        if(job[l][m][n].executor.rest.count(ttek)) continue; // пропустить текущий день ttek

        // ПРОВЕРКА. Работа не может начаться ранее, чем завершится предыдущая (1-ый вариант проверки)
        //Если это не начальная работа процедуры, то для проверки нужна предыдущая работа той же процедуры
        if(n!=0) if(ttek <= job[l][m][n-1].end) continue; // пропустить текущий день ttek

        // ПРОВЕРКА. Работа не может начаться ранее, чем завершится предыдущая (2-ой вариант проверки)
        //Если это начальная работа процедуры, то для проверки нужна последняя работа предыдущей процедуры.
        if(m!=0 && n==0){  // это начальная работа процедуры. Надо найти конец последней работы предыдущей процедуры
            int klast = job[l][m-1].size()-1; // индекс последней работы в предыдущей процедуре
            if(ttek <= job[l][m-1][klast].end) continue; // пропустить текущий день ttek
        }

        // Найти время начала (tbegin) и конца (tend) работы job[l][m][n] с учетом нерабочих часов (начиная поиск со времени ttek) для
        //исполнителя job[l][m][n].executor. Задана продолжительность работы: job[l][m][n].time
        FindBeginEndJob(job[l][m][n].executor, ttek, job[l][m][n].time, tbegin, tend); // нашли tbegin и tend

        // ПРОВЕРКА. Если исполнитель внешний, то дублирование работ не проверять. Время начала и конца - найдены.
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

        // Если дошли до конца цикла перебора возможных дней начала ttek, значит все проверки пройдены. Даты начала и конца найдены.
        break; // выйти из цикла перебора дней, планирование работы job[l][m][n] - успешно завершено
    } // конец цикла перебора дней планирования

    // Из цикла вышли только тогда, когда работа спланирована
    job[l][m][n].begin = tbegin; // записать найденное время начала
    job[l][m][n].end   = tend;   // записать найденное время конца

    return;
}

int Plan(TCarryTaskList &tasks, QString &errs) // основная процедура
{

    // По текущей дате получить tbeginplan - номер сегодняшнего текущего дня года, начиная с которого будет проводиться планирование
    tbeginplan = QDate::currentDate().dayOfYear() - 1; // "-1", т.к. в программе нумерация дней начинается с 0
    //??? НЕВЕРНО здесь должен быть текущий час года. Какой?
    //...

    FillHoliday();                               // заполнить календарь предприятия (выходные и праздничные дне)
    if(TestTCarryTask(tasks, errs)!=0) return 5; // проверить содержимое объекта tasks до начала планирования
    if(flprint>=2) PrinTCarryTask(tasks);        // вывести в консоль содержимое объекта tasks до начала планирования
    FillVectorJob(tasks);                        // заполнить вектор job данными из объекта tasks

    // Диалог. Установить принудительно защиту для всех работ, которые звершены или продолжаются. И снять защиту с остальных работ
    QMessageBox* pmbx = new QMessageBox(QMessageBox::Question, "Выберите вариант ответа",
                                        "Установить автоматически признак НЕ ИЗМЕНЯТЬ для выполненных и текущих работ. И признак МОЖНО ИЗМЕНЯТЬ для будущих работ?",
                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    int n = pmbx->exec();
    delete pmbx;
    if(n == QMessageBox::Yes)SetProtection();  // Нажата кнопка Yes. Установить защиту для завершенных и текущих работ и снять защиту с остальных работ

    SortingVectorJob();                          // сортировка вектора job в соответствии с приоритетами проектов
    if(flprint>=2) PrintJob();                   // вывести в консоль данные вектора job в табличной форме до начала планирования
    if(TestData(errs)!=0) return 1;              // проверить данные в векторе job

    // Планирование. Цикл перебор всех работ попроцедурно, т.к. работы одной процедуры исполняет один исполнитель из списка возможных
    for(int i=0; i<(int)job.size(); i++){ // цикл перебора продукций
        for(int j=0; j<(int)job[i].size(); j++){ // цикл перебора процедур
            // Каждая процедура м.б. выполнена несколькими исполнителями. Проверить каждого. Кто быстрее выполнит, того и выбрать
            int endmin = 999999999; // минимальное время конца процедуры
            executor_type executormin = job[i][j][0].people[0]; // лучший исполнитель (из начальной работы процедуры)
            for(int indexecutor = 0; indexecutor<(int)job[i][j][0].people.size(); indexecutor++){ // цикл перебора возможных исполнителей тек. процедуры
                // назначить текущего исполнителя: job[i][j][k].people[indexecutor] всем работам текущей процедуры
                for(int k=0; k<(int)job[i][j].size(); k++) job[i][j][k].executor = job[i][j][k].people[indexecutor];
                // спланировать все работы текущей процедуры: job[i][j] текущим исполнителем: job[i][j][k].people[indexecutor]
                for(int k=0; k<(int)job[i][j].size(); k++) PlanJob(i, j, k); // планирование работ процедуры
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
            for(int k=0; k<(int)job[i][j].size(); k++) PlanJob(i, j, k); // планирование работ процедуры
        } // конец цикла перебора процедур
    } // конец цикла перебора продукций

    if(TestPlan(errs)!=0) return 1;       // проверить сформированный план на грубые ошибки
    if(flprint>=2) PrintJob();            // вывести данные вектора job после планирования
    if(flprint>=1) PrintPlan();           // вывести в консоль сформированный план
    WritePlan(tasks);                     // записать результаты планирования в объект tasks
    if(flprint>=2) PrinTCarryTask(tasks); // вывести содержимое объекта tasks после записи в него результатов планирования
    return 0; // успешное завершение планирования
}

void TModulePlans::testPlanData() // УДАЛИТЬ. ЭТО СТАРАЯ ФУНКЦИЯ ПО КНОПКЕ "ЦЕНТРИРОВАТЬ"
{ // не удалять. Будет ошибка
    QMessageBox::information(0,"Отладка", "Работает пустая процедура, которую надо удалить");
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
