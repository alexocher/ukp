
#include <QApplication>
#include <QMessageBox>
#include <QTranslator>
#include <defMacro>
#include <TAppCarryPlan>
#include <TfrmMainUkpCarryPlan>

#if defined(Q_OS_WIN32)
#include "../LoginService/loginservice.h"
#else
#include <LoginService>
#endif

extern TfrmMainUkpCarryPlan *frmMainUkpCarryPlan; // from fmainukpdetale.cpp

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList params;
    for (int i = 1; i < argc; i++) params << QString(QByteArray(argv[i]));

    if (EM_User *selfUser = input2system(argc, argv, false)) params.append(QString("selfUser=%1").arg(selfUser->SUID()));
    else return -666;

    QTranslator qtTranslator;
    qtTranslator.load("qt_ru.qm");
    a.installTranslator(&qtTranslator);
    TAppCarryPlan *ukpApp = TAppCarryPlan::instance(params); // num, name
    if (!ukpApp->init())
    {
        QMessageBox::critical(NULL, "Ошибка", "Ошибка инициализации");
        ukpApp->printErrors();
        exit(911);
    }
    frmMainUkpCarryPlan = new TfrmMainUkpCarryPlan();
    frmMainUkpCarryPlan->show();
    ukpApp->run();
    int res(a.exec());
    DELETE(frmMainUkpCarryPlan);
    ukpApp->freeInstance();

    return res;
}
//-----------------------------------------------------------------------------

/*
TODO
 4.   Доработать форму диаграммы для отображения информации о выделенном на дереве узле.
 5.   Разработать интерфейс взаимодействия с Димой по синхронизации раскрытия узлов.
 7.2. % выполнения отображать полосой на выполняемых элементах.
 8.   Загружать планы в соответствии с текущим должностным лицом.
      (согласовать алгоритм, реализовать TUnit::isInternalEmployee(int id))
 9.   Сохранять все планы, которые изменялись.
10.   Проверить заполнение календаря сотрудника (empl->calendar()->fullRestDates()...).
16.2. Добавить в модель времена (начать не ранее..., окончить не позднее...)
21.   В шаблоне добавить признак контроля
22.   Разными цветами отображать для выполняемых работ:
      TCarryOutCondition cocGet      - Задача получена
                         cocCarryBeg - Приступил к выполнению
                         cocCarryEnd - Выполнил
                         cocProblem  - Проблемы
22.   Перечитать параметры при изменении чего-либо в форме планирования

CARRYOUT
 1.   Перейти в модели к заданию времени в часах.
 2.   Задавать продолжительность в чем-нибудь, а отображать во всем.
 3.   Доработать форму диаграммы для задания режима отображения в часах, днях, неделях, месяцах.
 6.   Ввести приоритет проекта. Целое. 0-(по-умолчанию), самый низкий приоритет. Чем больше число, тем выше приоритет проекта.
 7.1. Добавить в модель % выполнения и отображать полосой на выполняемых элементах.
11.   Выполняемая работа - другим цветом
12.   Заменить в проекте "Дата окончания" на "Дата начала (не ранее)".
14.   Изменять продолжительность работы на дереве после изменения
15.   Сохранять в диаграмме дату, установленную moveTo...
16.1. Добавить для проекта времена (начать не ранее..., окончить не позднее...)
17.   По умолчанию все можно изменять
18.   Отображать в дереве крестом то, что нельзя изменять
19.   Начало и окончание в форме планирования - до часов и минут
20.1. Задавать начало рабочего дня
20.2. Начало рабочего дня довести до Димы

CANCEL
13.   Установить "Нельзя изменять" после сохранения
*/
