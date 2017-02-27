#include <QMessageBox>
#include <defMacro>
#include <defPictures>
#include <qtools>
#include <TModulePlans>
#include <TModuleUnits>
#include <TModuleEmployees>
#include <TIdent>
#include <WPlan>
#include <WGantDiagramm>
#include <TfrmMainUkpCarryPlan>

WPlan *wPlan(NULL);

extern TfrmMainUkpCarryPlan *frmMainUkpCarryPlan; // from fmainukpcarryplan.cpp
extern WGantDiagramm *wGantDiagramm;   // from wgantdiagramm.cpp

namespace
{

    const int TW_COL_COUNT(8), TW_COL_WIDTHS[] = { 800,60,120,120,120,200,60,120 };
    const QString TW_COL_NAMES[] = { "Наименование","Изм.","Продолжит.","Начало","Окончание","Должн. лицо","Сохр.","..." };

}

WPlan::WPlan(QWidget *parent) : QFrame(parent)
{
    setupUi(this);

    if (QTreeWidgetItem *hdrIt = twProjects->headerItem())
        for (int i=0; i<TW_COL_COUNT; i++)
        {
            hdrIt->setText(i,TW_COL_NAMES[i]);
            hdrIt->setTextAlignment(i,Qt::AlignCenter);
        }
    twProjects->setIconSize(QSize(36,36));
    twProjects->setUniformRowHeights(true);
    twProjects->setSortingEnabled(true);
    connect(twProjects,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(selectPlanElement(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(twProjects,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(onItemChanged(QTreeWidgetItem*,int)));

    dtEnd->setDate(QDate(QDate::currentDate().year(),1,1));

    connect(pbTemplatesOk,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbDtEndOk,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbPeriodQuery,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbPeriodOk,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbEmployeesOk,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbEmployeesCalendar,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbEmployeesReset,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbChangeYes,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbChangeNo,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbChangeReset,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbCheckAll,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbUncheckAll,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbExpand,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbCollapse,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbDiagr,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbPlan,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbCancel,SIGNAL(clicked()),this,SLOT(resetPlan()));
    connect(pbApply,SIGNAL(clicked()),this,SLOT(resetPlan()));

    fillForm();

}
//-----------------------------------------------------------------------------

WPlan::~WPlan()
{
}
//-----------------------------------------------------------------------------

void WPlan::fillForm()
{
  MODULE(Plans);
    modPlans->reflectCarryTasksToTree(modPlans->carryTasks(),*twProjects);
    twProjects->sortItems(0,Qt::AscendingOrder);
    qtools::expand(*twProjects);
    modPlans->reflectTemplatesToCb(modPlans->planTemplates(),*cbTemplates);
    for (int i=0; i<TW_COL_COUNT; i++) twProjects->setColumnWidth(i,TW_COL_WIDTHS[i]);
}
//-----------------------------------------------------------------------------

void WPlan::resetPlan()
{
    if (QPushButton *btn = dynamic_cast<QPushButton*>(sender())) resetPlan(*btn);
}
//-----------------------------------------------------------------------------

void WPlan::resetPlan(const QPushButton &btn)
{
    if (&btn==pbTemplatesOk)
    {
      QString sErr("");
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
          QStringList scrnms = qtools::hierarchyTexts(*curIt);
          MODULE(Plans);
            if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(),scrnms))
            {
                if (TCarryPlan *plan = dynamic_cast<TCarryPlan*>(plEl))
                {
                  QTreeWidgetItem *topIt(curIt->parent());
                  QString tskTitle(topIt ? topIt->text(0) : "");
                  MODULE(Units);
                    plan->fillFromTemplate((TCarryPlan*)modPlans->findPlanTemplate(cbTemplates->currentText(),true),*modUnits->selfUnit());
                    modPlans->setWorkIdsForPlan();
                    modPlans->reflectCarryTasksToTree(modPlans->carryTasks(),*twProjects);
                    qtools::expand(*twProjects,0);
                    for (int i=0; i<twProjects->topLevelItemCount(); i++)
                        if (QTreeWidgetItem *topIt = twProjects->topLevelItem(i))
                            if (topIt->text(0)==tskTitle)
                            {
                                for (int j=0; j<topIt->childCount(); j++)
                                    if (QTreeWidgetItem *it = topIt->child(j)) it->setExpanded(true);
                                topIt->setExpanded(true);
                                break;
                            }
                }
                else sErr = "Узел дерева не является планом";
            }
            else sErr = "Отсутствует идентификатор узла";
        }
        else sErr = "Не выбран узел дерева";
        if (!sErr.isEmpty())
        {
          QMessageBox mb(QMessageBox::Question,tr("Ошибка"),sErr,QMessageBox::Ok,this);
            mb.setButtonText(QMessageBox::Ok,tr("Принять"));
            mb.exec();
        }
    }
    else if (&btn==pbDtEndOk)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
          QStringList scrnms = qtools::hierarchyTexts(*curIt);
          MODULE(Plans);
            if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(),scrnms)) plEl->setDtPlanEnd(QDateTime(dtEnd->date(),QTime(23,59,59)));
        }
    }
    else if (&btn==pbPeriodQuery)
    {

    }
    else if (&btn==pbPeriodOk)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
          QStringList scrnms = qtools::hierarchyTexts(*curIt);
          MODULE(Plans);
            if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(),scrnms)) plEl->setPlanPeriod(sbPeriod->value());
        }
    }
    else if (&btn==pbEmployeesOk)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
          QStringList scrnms = qtools::hierarchyTexts(*curIt);
          MODULE(Plans);
            if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(),scrnms))
            {
              QStringList chkEmpls;
                for (int i=0; i<lwEmployees->count(); i++)
                    if (QListWidgetItem *it = lwEmployees->item(i))
                        if (it->checkState()==Qt::Checked) chkEmpls<<it->text();
                plEl->clearPossibleEmployeess();
              MODULE(Employees);
                foreach (const QString &scrnm,chkEmpls)
                    if (TEmployee *empl = modEmployees->findEmployee(scrnm,true)) plEl->insertPossibleEmployee(empl);
                modEmployees->reflectEmployeesToLw(plEl->possibleEmployees(),*lwEmployees);
                teParams->setText(plEl->toHtml(!cbIsShort->isChecked()));
            }
        }
    }
    else if (&btn==pbEmployeesCalendar)
    {

    }
    else if (&btn==pbEmployeesReset)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
          QStringList scrnms = qtools::hierarchyTexts(*curIt);
          MODULE(Plans);
            if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(),scrnms))
            {
                plEl->clearPossibleEmployeess();
              TEmployeeRole frstRl = plEl->firstTemplateRole();
                if (frstRl.type()!=eltNone && frstRl.unitId()==0) // свое подразделение
                {
                  MODULE(Units);
                  TEmployeeRoleList rls;
                    rls<<frstRl;
                    if (TEmployeeList *posempl = modUnits->selfUnit()->findEmployees(rls))
                        foreach (TEmployee *empl,*posempl) plEl->insertPossibleEmployee(empl);
                }
              MODULE(Employees);
                modEmployees->reflectEmployeesToLw(plEl->possibleEmployees(),*lwEmployees);
                teParams->setText(plEl->toHtml(!cbIsShort->isChecked()));
            }
        }
    }
    else if (&btn==pbChangeYes || &btn==pbChangeNo)
    {
      QList<QTreeWidgetItem*> checkedItems;
        if (qtools::treeToList(*twProjects,checkedItems,Qt::Checked))
        {
          MODULE(Plans);
          bool setvol(&btn==pbChangeYes);
            foreach (QTreeWidgetItem *it,checkedItems)
            {
              QStringList scrnms = qtools::hierarchyTexts(*it);
                if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(),scrnms))
                {
                    plEl->setVolatile(setvol);
                    it->setIcon(1,ICONPIX(plEl->isVolatile() ? PIX_CHANGE : ""));
                }
            }
        }
    }
    else if (&btn==pbChangeReset)
    {
      QList<QTreeWidgetItem*> checkedItems;
        if (qtools::treeToList(*twProjects,checkedItems))
        {
          MODULE(Plans);
            foreach (QTreeWidgetItem *it,checkedItems)
            {
              QStringList scrnms = qtools::hierarchyTexts(*it);
                if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(),scrnms))
                {
                    plEl->setVolatile(false);
                    it->setIcon(1,ICONPIX(""));
                }
            }
        }
    }
    else if (&btn==pbCheckAll || &btn==pbUncheckAll)
    {
        for (int i=0; i<twProjects->topLevelItemCount(); i++)
            if (QTreeWidgetItem *it = twProjects->topLevelItem(i)) it->setCheckState(0,&btn==pbCheckAll ? Qt::Checked : Qt::Unchecked);
    }
    else if (&btn==pbExpand || &btn==pbCollapse)
    {
        qtools::expand(*twProjects,&btn==pbExpand ? -1 : 0);
    }
    else if (&btn==pbDiagr)
    {
        if (!wGantDiagramm) wGantDiagramm = new WGantDiagramm(this);
        wGantDiagramm->setGeometry(frmMainUkpCarryPlan->centralWidgetGlobalRect());
        //wGantDiagramm->setGeometry(frmMainUkpCarryPlan->formRect());
      MODULE(Plans);
        wGantDiagramm->prepare(modPlans->carryTasks(),TGantGraphicsView::cdPlan);
        wGantDiagramm->exec();
    }
    else if (&btn==pbPlan)
    {
      MODULE(Plans);
      QString errs("");
        if (!modPlans->createCarryPlans(modPlans->carryTasks(),errs))
        {
          QMessageBox mb(QMessageBox::Critical,tr("Ошибка планирования"),errs,QMessageBox::Ok,this);
            mb.setButtonText(QMessageBox::Ok,tr("Принять"));
            mb.exec();
        }
        else
        {
            fillForm();
          QMessageBox mb(QMessageBox::Information,tr("Планирование"),"Выполнено успешно",QMessageBox::Ok,this);
            mb.setButtonText(QMessageBox::Ok,tr("Принять"));
            mb.exec();
        }
    }
    else if (&btn==pbCancel)
    {

    }
    else if (&btn==pbApply)
    {
      QString sErr("");
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
          QStringList scrnms = qtools::hierarchyTexts(*curIt);
          MODULE(Plans);
            if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(),scrnms))
            {
                if (TCarryPlan *plan = dynamic_cast<TCarryPlan*>(plEl))
                {
                    if (!plan->toDB("plan"))  sErr = "План не сохранен";
                }
                else sErr = "Не выбран план";
            }
            else sErr = "Не найден элемент";
        }
        else sErr = "Не выбран план";
      bool wasErr(!sErr.isEmpty());
      QMessageBox mb(wasErr ? QMessageBox::Warning : QMessageBox::Information,wasErr ? "Ошибка" : "Успешно",wasErr ? sErr : "План сохранен",QMessageBox::Ok,this);
        mb.setButtonText(QMessageBox::Ok,tr("Принять"));
        mb.exec();
    }
}
//-----------------------------------------------------------------------------

void WPlan::selectPlanElement(QTreeWidgetItem *curIt, QTreeWidgetItem *) // prev
{
  QString params("");
    if (curIt)
    {
      QStringList scrnms = qtools::hierarchyTexts(*curIt);
      MODULE(Plans);
        if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(),scrnms))
        {
            params = plEl->toHtml(!cbIsShort->isChecked());
          bool isPlan(false), isProcedure(false), isWork(false); // isTask(false),
            //if (dynamic_cast<TCarryTask*>(plEl)) isTask = true; // TCarryTask *tsk =
            //else
            if (dynamic_cast<TCarryPlan*>(plEl)) isPlan = true; // TCarryPlan *plan =
            else if (TCarryProcedure *pr = dynamic_cast<TCarryProcedure*>(plEl))
            {
              MODULE(Employees);
                modEmployees->reflectEmployeesToLw(pr->possibleEmployees(),*lwEmployees);
                isProcedure = true;
            }
            else if (dynamic_cast<TCarryWork*>(plEl)) isWork = true; // TCarryWork *wrk =
            if (!isProcedure) lwEmployees->clear();
            pbTemplatesOk->setEnabled(isPlan);
            pbDtEndOk->setEnabled(isPlan);
            pbPeriodQuery->setEnabled(isWork);
            pbPeriodOk->setEnabled(isWork);
            pbEmployeesOk->setEnabled(isProcedure);
            pbEmployeesCalendar->setEnabled(isProcedure);
            pbEmployeesReset->setEnabled(isProcedure);
        }
    }
    teParams->setText(params);
}
//-----------------------------------------------------------------------------

void WPlan::onItemChanged(QTreeWidgetItem *it, int col)
{
    if (!it || col) return;
    if (qtools::level(*it)!=1) return;
    //PR(0,it->text(0));
  QStringList scrnms = qtools::hierarchyTexts(*it);
  MODULE(Plans);
    if (TCarryPlan *plan = dynamic_cast<TCarryPlan*>(modPlans->findCarryElement(modPlans->carryTasks(),scrnms)))
    {
        plan->setChecked(it->checkState(0)==Qt::Checked);
    //PR(4,plan->isChecked() ? "Да" : "Нет");
    }
}
//-----------------------------------------------------------------------------
