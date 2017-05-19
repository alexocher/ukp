#include <QMessageBox>
#include <defMacro>
#include <defPictures>
#include <qtools>
#include <gen>
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

    const int TW_COL_COUNT(8), TW_COL_WIDTHS[] = { 800, 60, 120, 150, 150, 200, 60, 120 };
    const QString TW_COL_NAMES[] = { "Наименование", "Изм.", "Продолжит.", "Начало", "Окончание", "Должн. лицо", "Сохр.", "..." };

}

WPlan::WPlan(QWidget *parent) : QFrame(parent)
{
    setupUi(this);

    if (QTreeWidgetItem *hdrIt = twProjects->headerItem())
        for (int i = 0; i < TW_COL_COUNT; i++)
        {
            hdrIt->setText(i, TW_COL_NAMES[i]);
            hdrIt->setTextAlignment(i, Qt::AlignCenter);
        }
    twProjects->setIconSize(QSize(36, 36));
    twProjects->setUniformRowHeights(true);
    twProjects->setSortingEnabled(true);
    connect(twProjects, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(selectPlanElement(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(twProjects, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onItemChanged(QTreeWidgetItem*, int)));

    dtBegin->setDate(QDate::currentDate());
    dtEnd->setDate(QDate::currentDate());

    connect(pbPriorityOk, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbCurrentDateBegin, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbCurrentDateEnd, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbDtBeginOk, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbDtEndOk, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbDtBeginCancel, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbDtEndCancel, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbTemplatesOk, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbPeriodQuery, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbPeriodOk, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbEmployeesOk, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbEmployeesCalendar, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbEmployeesReset, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbEmployeesAdd, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbChangeYes, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbChangeNo, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbChangeReset, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbCheckAll, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbUncheckAll, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbExpand, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbCollapse, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbDiagr, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbPlan, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbCancel, SIGNAL(clicked()), this, SLOT(resetPlan()));
    connect(pbApply, SIGNAL(clicked()), this, SLOT(resetPlan()));

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
    modPlans->reflectCarryTasksToTree(modPlans->carryTasks(), *twProjects);
    twProjects->sortItems(0, Qt::AscendingOrder);
    qtools::expand(*twProjects);
    modPlans->reflectTemplatesToCb(modPlans->planTemplates(), *cbTemplates);
    for (int i = 0; i < TW_COL_COUNT; i++) twProjects->setColumnWidth(i, TW_COL_WIDTHS[i]);
}
//-----------------------------------------------------------------------------

void WPlan::resetPlan()
{
    if (QPushButton *btn = dynamic_cast<QPushButton*>(sender())) resetPlan(*btn);
}
//-----------------------------------------------------------------------------

void WPlan::resetPlan(const QPushButton &btn)
{
    if (&btn == pbPriorityOk)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
            QStringList scrnms = qtools::hierarchyTexts(*curIt);
            MODULE(Plans);
            if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(), scrnms))
            {
                if (TCarryTask *tsk = dynamic_cast<TCarryTask*>(plEl)) tsk->setPriority(sbPriority->value());
                teParams->setText(plEl->toHtml(!cbIsShort->isChecked()));
            }
        }
    }
    else if (&btn == pbCurrentDateBegin)
    {
        dtBegin->setDate(QDate::currentDate());
    }
    else if (&btn == pbCurrentDateEnd)
    {
        dtEnd->setDate(QDate::currentDate());
    }
    else if (&btn == pbDtBeginOk)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
            QStringList scrnms = qtools::hierarchyTexts(*curIt);
            MODULE(Plans);
            if (TCarryTask *tsk = dynamic_cast<TCarryTask*>(modPlans->findCarryElement(modPlans->carryTasks(), scrnms)))
            {
                tsk->setDtMinBegin(QDateTime(dtBegin->date(), QTime(9, 0, 0)));
                teParams->setText(tsk->toHtml(!cbIsShort->isChecked()));
            }
        }
    }
    else if (&btn == pbDtEndOk)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
            QStringList scrnms = qtools::hierarchyTexts(*curIt);
            MODULE(Plans);
            if (TCarryTask *tsk = dynamic_cast<TCarryTask*>(modPlans->findCarryElement(modPlans->carryTasks(), scrnms)))
            {
                tsk->setDtMaxEnd(QDateTime(dtEnd->date(), QTime(9, 0, 0)));
                teParams->setText(tsk->toHtml(!cbIsShort->isChecked()));
            }
        }
    }
    else if (&btn == pbDtBeginCancel)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
            QStringList scrnms = qtools::hierarchyTexts(*curIt);
            MODULE(Plans);
            if (TCarryTask *tsk = dynamic_cast<TCarryTask*>(modPlans->findCarryElement(modPlans->carryTasks(), scrnms)))
            {
                tsk->setDtMinBegin(QDateTime());
                dtBegin->setDate(QDate(2000,1,1));
                teParams->setText(tsk->toHtml(!cbIsShort->isChecked()));
            }
        }
    }
    else if (&btn == pbDtEndCancel)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
            QStringList scrnms = qtools::hierarchyTexts(*curIt);
            MODULE(Plans);
            if (TCarryTask *tsk = dynamic_cast<TCarryTask*>(modPlans->findCarryElement(modPlans->carryTasks(), scrnms)))
            {
                tsk->setDtMaxEnd(QDateTime());
                dtEnd->setDate(QDate(2000,1,1));
                teParams->setText(tsk->toHtml(!cbIsShort->isChecked()));
            }
        }
    }
    else if (&btn == pbTemplatesOk)
    {
        QString sErr("");
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
            QStringList scrnms = qtools::hierarchyTexts(*curIt);
            MODULE(Plans);
            if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(), scrnms))
            {
                if (TCarryPlan *plan = dynamic_cast<TCarryPlan*>(plEl))
                {
                    QTreeWidgetItem *topIt(curIt->parent());
                    QString tskTitle(topIt ? topIt->text(0) : "");
                    plan->fillFromTemplate((TCarryPlan*)modPlans->findPlanTemplate(cbTemplates->currentText(), true));
                    modPlans->setElementIdsForPlan();
                    modPlans->reflectCarryTasksToTree(modPlans->carryTasks(), *twProjects);
                    qtools::expand(*twProjects, 0);
                    for (int i = 0; i < twProjects->topLevelItemCount(); i++)
                        if (QTreeWidgetItem *topIt = twProjects->topLevelItem(i))
                            if (topIt->text(0) == tskTitle)
                            {
                                for (int j = 0; j < topIt->childCount(); j++)
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
            QMessageBox mb(QMessageBox::Question, tr("Ошибка"), sErr, QMessageBox::Ok, this);
            mb.setButtonText(QMessageBox::Ok, tr("Принять"));
            mb.exec();
        }
    }
    else if (&btn == pbPeriodQuery)
    {

    }
    else if (&btn == pbPeriodOk)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
            QStringList scrnms = qtools::hierarchyTexts(*curIt);
            MODULE(Plans);
            if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(), scrnms))
            {
                plEl->setPlanPeriod(sbPeriod->value());
                curIt->setText(2, gen::intToStr(sbPeriod->value()));
                teParams->setText(plEl->toHtml(!cbIsShort->isChecked()));
            }
        }
    }
    else if (&btn == pbEmployeesOk)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
            QStringList scrnms = qtools::hierarchyTexts(*curIt);
            MODULE(Plans);
            if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(), scrnms))
            {
                QStringList chkEmpls;
                for (int i = 0; i < lwEmployees->count(); i++)
                    if (QListWidgetItem *it = lwEmployees->item(i))
                        if (it->checkState() == Qt::Checked) chkEmpls << it->text();
                plEl->clearPossibleEmployeess();
                MODULE(Employees);
                foreach (const QString &scrnm, chkEmpls)
                    if (TEmployee *empl = modEmployees->findEmployee(scrnm, true)) plEl->insertPossibleEmployee(empl);
                modEmployees->reflectEmployeesToLw(plEl->possibleEmployees(), *lwEmployees);
                teParams->setText(plEl->toHtml(!cbIsShort->isChecked()));
            }
        }
    }
    else if (&btn == pbEmployeesCalendar)
    {

    }
    else if (&btn == pbEmployeesReset)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
            QStringList scrnms = qtools::hierarchyTexts(*curIt);
            MODULE(Plans);
            if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(), scrnms))
            {
                plEl->clearPossibleEmployeess();
                MODULE(Employees);
                TEmployeeRole frstRl = plEl->firstTemplateRole();
                if (frstRl.type() != eltNone)
                {
                    TEmployeeRoleList rls;
                    rls << frstRl;
                    if (TEmployeeList *posempl = modEmployees->findEmployees(frstRl.unitId(), rls))
                        foreach (TEmployee * empl, *posempl) plEl->insertPossibleEmployee(empl);
                }
                modEmployees->reflectEmployeesToLw(plEl->possibleEmployees(), *lwEmployees);
                teParams->setText(plEl->toHtml(!cbIsShort->isChecked()));
            }
        }
    }
    else if (&btn == pbEmployeesAdd)
    {
        if (QTreeWidgetItem *curIt = twProjects->currentItem())
        {
            QStringList scrnms = qtools::hierarchyTexts(*curIt);
            MODULE(Plans);
            if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(), scrnms))
            {
                MODULE(Employees);
                if (TEmployee *empl = modEmployees->findEmployee(cbEmployees->currentData().toInt()))
                {
                    plEl->insertPossibleEmployee(empl);
                    modEmployees->reflectEmployeesToLw(plEl->possibleEmployees(), *lwEmployees);
                }
                teParams->setText(plEl->toHtml(!cbIsShort->isChecked()));
            }
        }
    }
    else if (&btn == pbChangeYes || &btn == pbChangeNo)
    {
        QList<QTreeWidgetItem*> checkedItems;
        if (qtools::treeToList(*twProjects, checkedItems, Qt::Checked))
        {
            MODULE(Plans);
            bool setvol(&btn == pbChangeYes);
            foreach (QTreeWidgetItem *it, checkedItems)
            {
                QStringList scrnms = qtools::hierarchyTexts(*it);
                if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(), scrnms))
                {
                    plEl->setVolatile(setvol);
                    it->setIcon(1, ICONPIX(plEl->isVolatile() ? "" : PIX_CROSS));
                }
            }
        }
    }
    else if (&btn == pbChangeReset)
    {
        QList<QTreeWidgetItem*> checkedItems;
        if (qtools::treeToList(*twProjects, checkedItems))
        {
            MODULE(Plans);
            foreach (QTreeWidgetItem *it, checkedItems)
            {
                QStringList scrnms = qtools::hierarchyTexts(*it);
                if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(), scrnms))
                {
                    plEl->setVolatile(true);
                    it->setIcon(1, ICONPIX(""));
                }
            }
        }
    }
    else if (&btn == pbCheckAll || &btn == pbUncheckAll)
    {
        for (int i = 0; i < twProjects->topLevelItemCount(); i++)
            if (QTreeWidgetItem *it = twProjects->topLevelItem(i)) it->setCheckState(0, &btn == pbCheckAll ? Qt::Checked : Qt::Unchecked);
    }
    else if (&btn == pbExpand || &btn == pbCollapse)
    {
        qtools::expand(*twProjects, &btn == pbExpand ? -1 : 0);
    }
    else if (&btn == pbDiagr)
    {
        if (!wGantDiagramm) wGantDiagramm = new WGantDiagramm(this);
        wGantDiagramm->setGeometry(frmMainUkpCarryPlan->centralWidgetGlobalRect());
        //wGantDiagramm->setGeometry(frmMainUkpCarryPlan->formRect());
        MODULE(Plans);
        wGantDiagramm->prepare(modPlans->carryTasks(), TGantGraphicsView::cdPlan, TGantGraphicsView::svDay);
        wGantDiagramm->exec();
    }
    else if (&btn == pbPlan)
    {
        MODULE(Plans);
        QString errs("");
        if (!modPlans->createCarryPlans(modPlans->carryTasks(), errs))
        {
            QMessageBox mb(QMessageBox::Critical, tr("Ошибка планирования"), errs, QMessageBox::Ok, this);
            mb.setButtonText(QMessageBox::Ok, tr("Принять"));
            mb.exec();
        }
        else
        {
            fillForm();
            QMessageBox mb(QMessageBox::Information, tr("Планирование"), "Выполнено успешно", QMessageBox::Ok, this);
            mb.setButtonText(QMessageBox::Ok, tr("Принять"));
            mb.exec();
        }
    }
    else if (&btn == pbCancel)
    {

    }
    else if (&btn == pbApply)
    {
        QString sErr("");
        MODULE(Plans);
        int n(0);
        foreach (TCarryTask *tsk, modPlans->carryTasks())
        {
            TCarryPlan *plans[2] = { tsk->ordPlan(), tsk->carryPlan() };
            for (int i=0; i<2; i++)
                if (TCarryPlan *plan = plans[i])
                    // if (!plan->isSaved()) ??? пока сохраняется все
                        if (!plan->toDB("plan"))  sErr += QString("%1. План не сохранен (%2)\n").arg(++n).arg(plan->scrName());
        }
        bool wasErr(!sErr.isEmpty());
        QMessageBox mb(wasErr ? QMessageBox::Warning : QMessageBox::Information, wasErr ? "Ошибка" : "Успешно", wasErr ? sErr : "Сохранено", QMessageBox::Ok, this);
        mb.setButtonText(QMessageBox::Ok, tr("Принять"));
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
        if (TAbstractPlanElement *plEl = modPlans->findCarryElement(modPlans->carryTasks(), scrnms))
        {
            params = plEl->toHtml(!cbIsShort->isChecked());
            bool isTask(false), isPlan(false), isProcedure(false), isWork(false);
            MODULE(Employees);
            if (TCarryTask *tsk = dynamic_cast<TCarryTask*>(plEl))
            {
                isTask = true;
                sbPriority->setValue(tsk->priority());
                dtBegin->setDate(tsk->dtMinBegin() ? tsk->dtMinBegin()->date() : QDate(QDate::currentDate().year(), 1, 1));
                dtEnd->setDate(tsk->dtMaxEnd() ? tsk->dtMaxEnd()->date() : QDate(QDate::currentDate().year(), 1, 1));
            }
            else if (dynamic_cast<TCarryPlan*>(plEl)) isPlan = true; // TCarryPlan *plan =
            else if (TCarryProcedure *pr = dynamic_cast<TCarryProcedure*>(plEl))
            {
                modEmployees->reflectEmployeesToLw(pr->possibleEmployees(), *lwEmployees);
                isProcedure = true;
            }
            else if (TCarryWork *wrk = dynamic_cast<TCarryWork*>(plEl))
            {
                isWork = true;
                sbPeriod->setValue(wrk->planPeriod());
            }
            if (!isProcedure) lwEmployees->clear();
            pbPriorityOk->setEnabled(isTask);
            sbPriority->setEnabled(isTask);
            pbCurrentDateBegin->setEnabled(isTask);
            pbCurrentDateEnd->setEnabled(isTask);
            pbDtBeginOk->setEnabled(isTask);
            pbDtEndOk->setEnabled(isTask);
            pbDtBeginCancel->setEnabled(isTask);
            pbDtEndCancel->setEnabled(isTask);
            dtBegin->setEnabled(isTask);
            dtEnd->setEnabled(isTask);
            pbTemplatesOk->setEnabled(isPlan);
            pbPeriodQuery->setEnabled(isWork);
            pbPeriodOk->setEnabled(isWork);
            sbPeriod->setEnabled(isWork);
            pbEmployeesOk->setEnabled(isProcedure);
            pbEmployeesCalendar->setEnabled(isProcedure);
            pbEmployeesReset->setEnabled(isProcedure);
            pbEmployeesAdd->setEnabled(isProcedure);
            if (isProcedure)
            {
                TEmployeeRole frstRl = plEl->firstTemplateRole();
                modEmployees->reflectEmployeesToCb(frstRl.unitId(), *cbEmployees);
            }
            else cbEmployees->clear();
        }
    }
    teParams->setText(params);
}
//-----------------------------------------------------------------------------

void WPlan::onItemChanged(QTreeWidgetItem *it, int col)
{
    if (!it || col) return;
    if (qtools::level(*it) != 1) return;
    //PR(0,it->text(0));
    QStringList scrnms = qtools::hierarchyTexts(*it);
    MODULE(Plans);
    if (TCarryPlan *plan = dynamic_cast<TCarryPlan*>(modPlans->findCarryElement(modPlans->carryTasks(), scrnms)))
    {
        plan->setChecked(it->checkState(0) == Qt::Checked);
        //PR(4,plan->isChecked() ? "Да" : "Нет");
    }
}
//-----------------------------------------------------------------------------
