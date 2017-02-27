
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
    for (int i=1; i<argc; i++) params<<QString(QByteArray(argv[i]));

    if (EM_User *selfUser = input2system(argc,argv,false)) params.append(QString("selfUser=%1").arg(selfUser->SUID()));
    else return -666;

  QTranslator qtTranslator;
    qtTranslator.load("qt_ru.qm");
    a.installTranslator(&qtTranslator);
    TAppCarryPlan *ukpApp = TAppCarryPlan::instance(params); // num, name
    if (!ukpApp->init())
    {
        QMessageBox::critical(NULL,"Ошибка","Ошибка инициализации");
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
