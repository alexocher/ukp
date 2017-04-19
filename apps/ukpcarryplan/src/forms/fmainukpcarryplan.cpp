
#include <QMessageBox>
#include <QMdiArea>
#include <defMacro>
#include <defPictures>
#include <common>
#include <enums>
#include <TfrmMainUkpCarryPlan>
#include <TModulePlans>
#include <TMdiSubwindow>
#include <WCompanyCalendar>
#include <WEmployeeCalendar>
#include <WPlan>
#include <WTemplate>
#include <WOrgShtat>
#include <WProduction>
#include <TfrmDialog_NNm>

TfrmMainUkpCarryPlan *frmMainUkpCarryPlan(NULL);

extern WCompanyCalendar *wCompanyCalendar; // from wcompanycalendar.cpp
extern WEmployeeCalendar *wEmployeeCalendar; // from wemployeecalendar.cpp
extern WPlan *wPlan;                   // from wplan.cpp
extern WTemplate *wTemplate;           // from wtemplate.cpp
extern WOrgShtat *wOrgShtat;           // from worgshtat.cpp
extern WProduction *wProduction;       // from wproduction.cpp

namespace
{

    QAction *actPlan(NULL),
            *actTemplate(NULL),
            *actOrgShtat(NULL),
            *actProduction(NULL),
            *actCompanyCalendar(NULL),
            *actEmployeeCalendar(NULL),
            *actTileHorizontal(NULL),
            *actTileVertical(NULL),
            *actCascade(NULL),
            *actCloseAll(NULL),
            *actExit(NULL),
            *actAddProject(NULL);

    QMdiArea *MDI_AREA(NULL);

    QList<TMdiSubWindow*> SUB_WINDOWS;

    TMdiSubWindow *swCompanyCalendar(NULL),
                  *swEmployeeCalendar(NULL),
                  *swPlan(NULL),
                  *swTemplate(NULL),
                  *swOrgShtat(NULL),
                  *swProduction(NULL);

}

TfrmMainUkpCarryPlan::TfrmMainUkpCarryPlan(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);

    createControls();

    showMaximized(); // showFullScreen();

/*
    if (!wGantDiagramm) wGantDiagramm = new WGantDiagramm(this);
    wGantDiagramm->setGeometry(frmMainUkpCarryPlan->centralWidgetGlobalRect());
    //wGantDiagramm->setGeometry(frmMainUkpCarryPlan->formRect());
    MODULE(Plans);
    wGantDiagramm->prepare(modPlans->carryTasks(), TGantGraphicsView::cdPlan, TGantGraphicsView::svDay);
    wGantDiagramm->exec();
*/

}
//-----------------------------------------------------------------------------

TfrmMainUkpCarryPlan::~TfrmMainUkpCarryPlan()
{
}
//-----------------------------------------------------------------------------

const QRect TfrmMainUkpCarryPlan::centralWidgetGlobalRect()
{
  const QWidget &cw = *((QMainWindow*)this)->centralWidget();
    return QRect(mapToGlobal(cw.pos()),cw.size());
}
//-----------------------------------------------------------------------------

const QRect TfrmMainUkpCarryPlan::formRect()
{
    return QRect(pos(),size());
}
//-----------------------------------------------------------------------------

void TfrmMainUkpCarryPlan::createControls()
{

    actPlan = mainToolBar->addAction(ICONPIX(PIX_PLAN),"&Планы",this,SLOT(clickAction()));
    actTemplate = mainToolBar->addAction(ICONPIX(PIX_TEMPLATE),"&Шаблоны",this,SLOT(clickAction()));
    mainToolBar->addSeparator();
    actOrgShtat = mainToolBar->addAction(ICONPIX(PIX_ORGSHTAT),"&Оргштат",this,SLOT(clickAction()));
    actProduction = mainToolBar->addAction(ICONPIX(PIX_PRODUCTION),"Продук&ция",this,SLOT(clickAction()));
    mainToolBar->addSeparator();
    actCompanyCalendar = mainToolBar->addAction(ICONPIX(PIX_WORKCALENDAR),"Календарь предпри&ятия",this,SLOT(clickAction()));
    actEmployeeCalendar = mainToolBar->addAction(ICONPIX(PIX_EMPLOYEECALENDAR),"Календарь сотрудников",this,SLOT(clickAction()));
    mainToolBar->addSeparator();
    actTileHorizontal = mainToolBar->addAction(ICONPIX(PIX_TILEHORIZONTAL),"Расположить &горизонтально",this,SLOT(clickAction()));
    actTileVertical = mainToolBar->addAction(ICONPIX(PIX_TILEVERTICAL),"Расположить вер&тикально",this,SLOT(clickAction()));
    actCascade = mainToolBar->addAction(ICONPIX(PIX_CASCADE),"Расположить ка&скадом",this,SLOT(clickAction()));
    actCloseAll = mainToolBar->addAction(ICONPIX(PIX_CLOSEALL),"&Закрыть все панели",this,SLOT(clickAction()));
    mainToolBar->addSeparator();
    actAddProject = mainToolBar->addAction(ICONPIX(PIX_QUESTION),"&Добавить проект",this,SLOT(clickAction()));
    mainToolBar->addSeparator();
    actExit = mainToolBar->addAction(ICONPIX(PIX_EXIT),"В&ыйти",this,SLOT(clickAction()));
    actExit->setShortcut(QKeySequence("Ctrl+Q"));

  QMenu *topMenu = mainMenuBar->addMenu("&Файл");
    topMenu->addAction(actExit);
    topMenu = mainMenuBar->addMenu("&Планирование");
    topMenu->addAction(actPlan);
    topMenu->addAction(actTemplate);
    topMenu = mainMenuBar->addMenu("&Данные");
    topMenu->addAction(actOrgShtat);
    topMenu->addAction(actProduction);
    topMenu = mainMenuBar->addMenu("&Календарь");
    topMenu->addAction(actCompanyCalendar);
    topMenu->addAction(actEmployeeCalendar);
    topMenu = mainMenuBar->addMenu("&Вид");
    topMenu->addAction(actTileHorizontal);
    topMenu->addAction(actTileVertical);
    topMenu->addAction(actCascade);
    topMenu->addAction(actCloseAll);
    topMenu = mainMenuBar->addMenu("&Справка");
    topMenu->addAction(actAddProject);

    MDI_AREA = new QMdiArea(this);
    setCentralWidget(MDI_AREA);
    MDI_AREA->setDocumentMode(true);

}
//-----------------------------------------------------------------------------

void TfrmMainUkpCarryPlan::clickAction()
{
    if (QAction *act = dynamic_cast<QAction*>(sender())) clickAction(*act);
}
//-----------------------------------------------------------------------------

void TfrmMainUkpCarryPlan::clickAction(const QAction &act)
{
    if (&act==actPlan)
    {
        if (!swPlan)
        {
            swPlan = new TMdiSubWindow();
            wPlan = new WPlan(swPlan);
            swPlan->setWidget(wPlan);
            MDI_AREA->addSubWindow(swPlan);
            swPlan->setAttribute(Qt::WA_DeleteOnClose,false);
            //swPlan->resize(MDI_AREA->width()/2,MDI_AREA->height()/2);
            connect(swPlan,SIGNAL(sigHideSubWindow()),this,SLOT(getSigHideSubWindow()));
            wPlan->fillForm();
        }
        else
        {
            wPlan->fillForm();
            wPlan->show();
        }
        if (SUB_WINDOWS.indexOf(swPlan)==-1) SUB_WINDOWS.append(swPlan);
        swPlan->show();
    }
    else if (&act==actTemplate)
    {
        if (!swTemplate)
        {
            swTemplate = new TMdiSubWindow();
            wTemplate = new WTemplate(swTemplate);
            swTemplate->setWidget(wTemplate);
            MDI_AREA->addSubWindow(swTemplate);
            swTemplate->setAttribute(Qt::WA_DeleteOnClose,false);
            //swTemplate->resize(MDI_AREA->width()/2,MDI_AREA->height()/2);
            connect(swTemplate,SIGNAL(sigHideSubWindow()),this,SLOT(getSigHideSubWindow()));
        }
        else wTemplate->show();
        if (SUB_WINDOWS.indexOf(swTemplate)==-1) SUB_WINDOWS.append(swTemplate);
        swTemplate->show(); //Maximized();
    }
    else if (&act==actOrgShtat)
    {
        if (!swOrgShtat)
        {
            swOrgShtat = new TMdiSubWindow();
            wOrgShtat = new WOrgShtat(swOrgShtat);
            swOrgShtat->setWidget(wOrgShtat);
            MDI_AREA->addSubWindow(swOrgShtat);
            swOrgShtat->setAttribute(Qt::WA_DeleteOnClose,false);
            swOrgShtat->resize(MDI_AREA->width()/2,MDI_AREA->height()/2);
            connect(swOrgShtat,SIGNAL(sigHideSubWindow()),this,SLOT(getSigHideSubWindow()));
        }
        else wOrgShtat->show();
        wOrgShtat->fillForm();
        if (SUB_WINDOWS.indexOf(swOrgShtat)==-1) SUB_WINDOWS.append(swOrgShtat);
        swOrgShtat->show();
    }
    else if (&act==actProduction)
    {
        if (!swProduction)
        {
            swProduction = new TMdiSubWindow();
            wProduction = new WProduction(swProduction);
            swProduction->setWidget(wProduction);
            MDI_AREA->addSubWindow(swProduction);
            swProduction->setAttribute(Qt::WA_DeleteOnClose,false);
            swProduction->resize(MDI_AREA->width()/2,MDI_AREA->height()/2);
            connect(swProduction,SIGNAL(sigHideSubWindow()),this,SLOT(getSigHideSubWindow()));
        }
        else wProduction->show();
        wProduction->fillForm();
        if (SUB_WINDOWS.indexOf(swProduction)==-1) SUB_WINDOWS.append(swProduction);
        swProduction->show();
    }
    else if (&act==actCompanyCalendar)
    {
        if (!swCompanyCalendar)
        {
            swCompanyCalendar = new TMdiSubWindow();
            wCompanyCalendar = new WCompanyCalendar(swCompanyCalendar);
            swCompanyCalendar->setWidget(wCompanyCalendar);
            MDI_AREA->addSubWindow(swCompanyCalendar);
            swCompanyCalendar->setAttribute(Qt::WA_DeleteOnClose,false);
            swCompanyCalendar->resize(MDI_AREA->width()/2,MDI_AREA->height()/2);
            connect(swCompanyCalendar,SIGNAL(sigHideSubWindow()),this,SLOT(getSigHideSubWindow()));
        }
        else wCompanyCalendar->show();
        if (SUB_WINDOWS.indexOf(swCompanyCalendar)==-1) SUB_WINDOWS.append(swCompanyCalendar);
        swCompanyCalendar->show();
    }
    else if (&act==actEmployeeCalendar)
    {
        if (!swEmployeeCalendar)
        {
            swEmployeeCalendar = new TMdiSubWindow();
            wEmployeeCalendar = new WEmployeeCalendar(swEmployeeCalendar);
            swEmployeeCalendar->setWidget(wEmployeeCalendar);
            MDI_AREA->addSubWindow(swEmployeeCalendar);
            swEmployeeCalendar->setAttribute(Qt::WA_DeleteOnClose,false);
            swEmployeeCalendar->resize(MDI_AREA->width()/2,MDI_AREA->height()/2);
            connect(swEmployeeCalendar,SIGNAL(sigHideSubWindow()),this,SLOT(getSigHideSubWindow()));
        }
        else wEmployeeCalendar->show();
        if (SUB_WINDOWS.indexOf(swEmployeeCalendar)==-1) SUB_WINDOWS.append(swEmployeeCalendar);
        swEmployeeCalendar->show();
    }
    else if (&act==actTileHorizontal)
    {
      TMdiSubWindow *sw(SUB_WINDOWS.count() ? SUB_WINDOWS.first() : NULL);
        if (sw)
        {
          int dW(sw->width()-sw->widget()->width()), dH(sw->height()-sw->widget()->height());
            PR2(4,"w: %1, h: %2",sw->widget()->width(),sw->widget()->height());
            PR2(4,"W: %1, H: %2",sw->width(),sw->height());
            PR2(4,"D_W: %1, D_H: %2",dW,dH);
        }
    }
    else if (&act==actTileVertical)
    {
    }
    else if (&act==actCascade)
    {
        MDI_AREA->cascadeSubWindows();
    }
    else if (&act==actCloseAll)
    {
        foreach (QMdiSubWindow *w,SUB_WINDOWS) w->hide();
    }
    else if (&act==actAddProject)
    {
      TfrmDialog_NNm *dlg(new TfrmDialog_NNm());
        dlg->prepare("Добавить новый проект выпуска продукции",1,"Наименование проекта");
        if (dlg->exec()==QDialog::Accepted)
        {
          int tp(dlg->propNum());
            if (tp!=1 && tp!=2 && (tp<51 || tp>71))
            {
              QMessageBox mb(QMessageBox::Critical,tr("Ошибка"),tr("Задан недопустимый вид продукции.\nЗадайте один из номеров ( 1, 2, 51..71 )"),QMessageBox::Yes,this);
                mb.setButtonText(QMessageBox::Yes,tr("Принять"));
                mb.exec();
                delete dlg;
                return;
            }
          MODULE(Plans);
            if (modPlans->findTask((TProductionType)dlg->propNum(),dlg->propName()))
            {
              QMessageBox mb(QMessageBox::Critical,tr("Ошибка"),QString("Проект типа %1 с таким наименованием уже существует").arg(dlg->propNum()),QMessageBox::Yes,this);
                mb.setButtonText(QMessageBox::Yes,tr("Принять"));
                mb.exec();
                delete dlg;
                return;
            }
          TCarryTask *newCarryTask(new TCarryTask(QDate::currentDate().year(),(TProductionType)dlg->propNum(),0,dlg->propNum(),dlg->propName()));
            newCarryTask->setScrName(QString("%1. %2").arg(newCarryTask->num()).arg(newCarryTask->name()));
          TCarryPlan *newOrd(new TCarryPlan(newCarryTask,0,1,"Разработка ОРД")), *newPlan(new TCarryPlan(newCarryTask,0,1,"План выпуска продукции"));
            newOrd->setScrName(newOrd->name());
            newCarryTask->setOrdPlan(newOrd);
            newPlan->setScrName(newPlan->name());
            newCarryTask->setCarryPlan(newPlan);
            if (newCarryTask->toDB("")) modPlans->insertTask(newCarryTask);
            else delete newCarryTask; // message ???
        }
        delete dlg;
    }
    else if (&act==actExit)
    {
        qApp->quit();
    }
/*
    else if (&btn==pbRead_resources)
    {
      QVariantList varList;
      QList<QGraphicsItem *> items;
        items = scene->items();
        for (int i = items.length()-1; i >=0; i--)
        {
<<<<<<< HEAD
          QJsonDocument document = QJsonDocument::fromJson(QByteArray::fromStdString(filedata));
          //
          //
          QJsonObject root = document.object();
          QJsonValue pts = root.value("production_types");
            if (pts.isArray())
=======
            if (items[i]->type()== MyTextItem::Type)
>>>>>>> ab35ccf78e566e8c996672820a04fa306bd23ba3
            {
              QVariantMap bt1;
                bt1.insert("Type", items[i]->data(Type));
              QMap <QString, int> map;
              MyTextItem *tItem = qgraphicsitem_cast<MyTextItem *>(items[i]);
                map = tItem->fProperties;
              QMapIterator<QString, int> p(map);
                while (p.hasNext())
                {
                    p.next();
                    bt1.insert(p.key(),p.value());
                }
                buttons << bt1;
            }
        }
      QString fileName(QFileDialog::getSaveFileName(this,tr("Сохранение схемы"),QString(),tr("(*.json)")));
        if (!fileName.isEmpty())
        {
          QFile jsFile(fileName);
            if(jsFile.open(QIODevice::WriteOnly)) jsFile.write(QJsonDocument::fromVariant(varList).toJson());
        }
    }
*/
}
//-----------------------------------------------------------------------------

void TfrmMainUkpCarryPlan::getSigHideSubWindow()
{
    if (QWidget *w = dynamic_cast<QWidget*>(sender())) getSigHideSubWindow(*w);
}
//-----------------------------------------------------------------------------

void TfrmMainUkpCarryPlan::getSigHideSubWindow(const QWidget &w)
{
    if (&w==swCompanyCalendar)
    {
        SUB_WINDOWS.removeOne(swCompanyCalendar);
        PR(0,"swCompanyCalendar");
    }
    else if (&w==swEmployeeCalendar)
    {
        SUB_WINDOWS.removeOne(swEmployeeCalendar);
        PR(0,"swEmployeeCalendar");
    }
    else if (&w==swPlan)
    {
        SUB_WINDOWS.removeOne(swPlan);
        PR(0,"swPlan");
    }
    else if (&w==swTemplate)
    {
        SUB_WINDOWS.removeOne(swTemplate);
        //swTemplate = NULL;
        PR(0,"swTemplate");
    }
    else if (&w==swOrgShtat)
    {
        SUB_WINDOWS.removeOne(swOrgShtat);
        PR(0,"swOrgShtat");
    }
    else if (&w==swProduction)
    {
        SUB_WINDOWS.removeOne(swProduction);
        PR(0,"swProduction");
    }
}
//-----------------------------------------------------------------------------
