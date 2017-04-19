#include <QStack>
#include <defMacro>
#include <defPictures>
#include <gen>
#include <convertEnums>
#include <TIdent>
#include <TModulePlans>
#include <TModuleUnits>
#include <TModuleEmployees>
#include <TModuleEmployees>
#include <EM_Plan>
#include <EM_ExtProc>
#include <EM_Calendar>

TModulePlans::TModulePlans(TProjModuleType t, int id, int n, QString nm, TAbstractObject *parent) : TAbstractModule(t, id, n, nm, parent), fCompanyCalendar(new TCompanyCalendar(QDate::currentDate().year())), fCurrentCarryTaskPackage(NULL), fCurrentGenPlan(NULL)
{
    fPlExternProcedureTemplates.setAutoDelete(true);
    fPlPlanTemplates.setAutoDelete(true);
    fPlCarryTasks.setAutoDelete(true);
}
//-----------------------------------------------------------------------------

TModulePlans::~TModulePlans()
{
    DELETE(fCompanyCalendar);
    DELETE(fCurrentCarryTaskPackage);
    DELETE(fCurrentGenPlan);
}
//-----------------------------------------------------------------------------

void TModulePlans::reset(bool thisonly)
{
    if (!thisonly) TAbstractModule::reset(true);
    fPlExternProcedureTemplates.clear();
    fPlPlanTemplates.clear();
    fCompanyCalendar->fromDB(gen::intToStr(QDate::currentDate().year()));
    fPlCarryTasks.clear();
    DELETE(fCurrentCarryTaskPackage);
    DELETE(fCurrentGenPlan);
}
//-----------------------------------------------------------------------------

bool TModulePlans::init()
{
    bool res(true);

    if (!fromDB("ExternProcedures")) res = false;
    if (!fromDB("Temlates")) res = false;
    if (!fromDB("CompanyCalendar")) res = false;
    if (!fromDB("CarryTasks")) res = false;

    // проверить параметры запуска и создать (при необходимости) пакет проектов

    return res;
}
//-----------------------------------------------------------------------------

TExternProcedureTemplateList &TModulePlans::externProcedureTemplates() const
{
    return const_cast<TExternProcedureTemplateList&>(fPlExternProcedureTemplates);
}
//-----------------------------------------------------------------------------

TExternProcedureTemplate *TModulePlans::findExternProcedureTemplate(int n)
{
    foreach (TExternProcedureTemplate *epr, fPlExternProcedureTemplates)
        if (epr->num() == n) return epr;
    return NULL;
}
//-----------------------------------------------------------------------------

TExternProcedureTemplate *TModulePlans::findExternProcedureTemplate(QString nm, bool onnm)
{
    foreach (TExternProcedureTemplate *epr, fPlExternProcedureTemplates)
        if (onnm)
        {
            if (epr->name() == nm) return epr;
        }
        else if (epr->scrName() == nm) return epr;
    return NULL;
}
//-----------------------------------------------------------------------------

TAbstractPlanList &TModulePlans::planTemplates() const
{
    return const_cast<TAbstractPlanList&>(fPlPlanTemplates);
}
//-----------------------------------------------------------------------------

void TModulePlans::insertPlanTemplate(TAbstractPlan *pl)
{
    if (pl && fPlPlanTemplates.indexOf(pl) == -1) fPlPlanTemplates.append(pl);
}
//-----------------------------------------------------------------------------

void TModulePlans::removePlanTemplate(TAbstractPlan *pl)
{
    if (pl)
    {
        EM_YearPlan *templates(NULL);
        try
        {
            EM_YearPlanDic &planDic = EM_YearPlanDic::Instance();
            QSharedPointer<EM_YearPlan> shpTemplates = planDic.by(YEAR_PLAN_TYPE_CARRYTEMPLATE);
            templates = shpTemplates.data();
            try
            {
                templates->fromDB();
                EM_BasePlanItem *root = templates->getRoot(), *remItem(NULL);
                for(EM_BasePlanItem::iterator tmplIter = root->begin(); tmplIter != root->end(); ++tmplIter)
                    if ((*tmplIter)->getID() == pl->id())
                    {
                        remItem = *tmplIter;
                        break;
                    }
                if (remItem)
                    try
                    {
                        templates->rem(remItem);
                    }
                    catch (CommonException::OpenDBException &e)
                    {
                        INSERT_ERROR(QString("EM_YearPlan::rem(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
                    }
                    catch (CommonException::SQLException &e)
                    {
                        INSERT_ERROR(QString("EM_YearPlan::rem(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
                    }
                    catch (CommonException::NullParamException &e)
                    {
                        INSERT_ERROR(QString("EM_YearPlan::rem(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
                    }
            }
            catch (CommonException::OpenDBException &e)
            {
                INSERT_ERROR(QString("EM_YearPlan::fromDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
            }
            catch (CommonException::SQLException &e)
            {
                INSERT_ERROR(QString("EM_YearPlan::fromDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
            }
        }
        catch (CommonException::OpenDBException &e)
        {
            INSERT_ERROR(QString("EM_YearPlanDic::Instance(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
        }
        catch (CommonException::SQLException &e)
        {
            INSERT_ERROR(QString("EM_YearPlanDic::Instance(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
        }
        fPlPlanTemplates.removeOne(pl);
    }
}
//-----------------------------------------------------------------------------

void TModulePlans::clearPlanTemplates()
{
    fPlPlanTemplates.clear();
}
//-----------------------------------------------------------------------------

TAbstractPlan *TModulePlans::findPlanTemplate(int id, bool onnum)
{
    foreach (TAbstractPlan *pl, fPlPlanTemplates)
        if (onnum)
        {
            if (pl->num() == id) return pl;
        }
        else if (pl->id() == id) return pl;
    return NULL;
}
//-----------------------------------------------------------------------------

TAbstractPlan *TModulePlans::findPlanTemplate(QString nm, bool onscrnm)
{
    foreach (TAbstractPlan *pl, fPlPlanTemplates)
        if (onscrnm)
        {
            if (pl->scrName() == nm) return pl;
        }
        else if (pl->name() == nm) return pl;
    return NULL;
}
//-----------------------------------------------------------------------------

TCompanyCalendar *TModulePlans::companyCalendar() const
{
    return fCompanyCalendar;
}
//-----------------------------------------------------------------------------

void TModulePlans::reloadCompanyCalendar(int year)
{
    if (year) fCompanyCalendar->fromDB(gen::intToStr(year));
}
//-----------------------------------------------------------------------------

TCarryTaskList &TModulePlans::carryTasks() const
{
    return const_cast<TCarryTaskList&>(fPlCarryTasks);
}
//-----------------------------------------------------------------------------

void TModulePlans::insertTask(TCarryTask *tsk)
{
    if (tsk && fPlCarryTasks.indexOf(tsk) == -1) fPlCarryTasks.append(tsk);
}
//-----------------------------------------------------------------------------

void TModulePlans::removeTask(TCarryTask *tsk)
{
    if (tsk) fPlCarryTasks.removeOne(tsk);
}
//-----------------------------------------------------------------------------


void TModulePlans::clearTasks()
{
    fPlCarryTasks.clear();
}
//-----------------------------------------------------------------------------

TCarryTask *TModulePlans::findTask(int id, bool onnum)
{
    foreach (TCarryTask *tsk, fPlCarryTasks)
        if (onnum)
        {
            if (tsk->num() == id) return tsk;
        }
        else if (tsk->id() == id) return tsk;
    return NULL;
}
//-----------------------------------------------------------------------------

TCarryTask *TModulePlans::findTask(QString nm, bool onscrnm)
{
    foreach (TCarryTask *tsk, fPlCarryTasks)
        if (onscrnm)
        {
            if (tsk->scrName() == nm) return tsk;
        }
        else if (tsk->name() == nm) return tsk;
    return NULL;
}
//-----------------------------------------------------------------------------

TCarryTask *TModulePlans::findTask(TProductionType tp, QString nm)
{
    foreach (TCarryTask *tsk, fPlCarryTasks)
        if (tsk->productionType() == tp && tsk->name() == nm) return tsk;
    return NULL;
}
//-----------------------------------------------------------------------------

TCarryTaskPackage *TModulePlans::currentCarryTaskPackage() const
{
    return fCurrentCarryTaskPackage;
}
//-----------------------------------------------------------------------------

void TModulePlans::setCurrentCarryTaskPackage(TCarryTaskPackage *pkg)
{
    if (fCurrentCarryTaskPackage == pkg) return;
    DELETE(fCurrentCarryTaskPackage);
    fCurrentCarryTaskPackage = pkg;
}
//-----------------------------------------------------------------------------

TGenPlan *TModulePlans::currentGenPlan() const
{
    return fCurrentGenPlan;
}
//-----------------------------------------------------------------------------

void TModulePlans::setCurrentGenPlan(TGenPlan *pl)
{
    if (fCurrentGenPlan == pl) return;
    DELETE(fCurrentGenPlan);
    fCurrentGenPlan = pl;
}
//-----------------------------------------------------------------------------

// ... найти по наименованию (по умолчанию) или по отображаемому наименованию
TCarryTask *TModulePlans::findTask(const TCarryTaskList &tsks, QString nm, bool onscrnm)
{
    foreach (TCarryTask *tsk, tsks)
        if (onscrnm)
        {
            if (tsk->scrName() == nm) return tsk;
        }
        else if (tsk->name() == nm) return tsk;
    return NULL;
}
//-----------------------------------------------------------------------------

TAbstractPlanElement *TModulePlans::findCarryElement(const TCarryTaskList &tasks, const QStringList &scrnms)
{
    if (scrnms.count())
    {
        const QString &str0 = scrnms.at(0); // проект
        if (TCarryTask *tsk = findTask(tasks, str0, true))
        {
            if (scrnms.count() > 1)
            {
                const QString &str1 = scrnms.at(1); // план
                TCarryPlan *plan(NULL);
                if (TCarryPlan *pl = tsk->ordPlan())
                {
                    if (pl->scrName() == str1) plan = pl;
                }
                if (!plan)
                    if (TCarryPlan *pl = tsk->carryPlan())
                    {
                        if (pl->scrName() == str1) plan = pl;
                    }
                if (plan)
                {
                    if (scrnms.count() > 2)
                    {
                        const QString &str2 = scrnms.at(2); // процедура
                        if (TCarryProcedure *proc = plan->findProcedure(str2, true))
                        {
                            if (scrnms.count() > 3)
                            {
                                const QString &str3 = scrnms.at(3); // работа
                                if (TCarryWork *wrk = proc->findWork(str3, true)) return wrk;
                            }
                            else return proc;
                        }
                    }
                    else return plan;
                }
            }
            else return tsk;
        }
    }
    return NULL;
}
//-----------------------------------------------------------------------------

void TModulePlans::fillTasks(QList<int> tskids)
{
    MODULE(Employees);
    QDateTime dt;
    fPlCarryTasks.clear();
    if (!tskids.count()) // все проекты
    {
        int curYear(QDate::currentDate().year());
        //??? find param -year=<number>
        try
        {
            EM_YearPlan *yearPlan = new EM_YearPlan(curYear);
            if (yearPlan != 0)
            {
                yearPlan->fromDB();
                yearPlan->printPlanTree();
                EM_BasePlanItem *root = yearPlan->getRoot();
                PR3(4, "NODE_TYPE: %1, SUID: %2, (%3)", root->getType(), root->getID(), root->getFullTitle());
                for(EM_BasePlanItem::iterator projIter = root->begin(); projIter != root->end(); ++projIter)
                {
                    PR(0, "for projIter");
                    EM_BasePlanItem *proj = *projIter;
                    PR3(4, "NODE_TYPE: %1, SUID: %2, (%3)", proj->getType(), proj->getID(), proj->getFullTitle());
                    if (EM_ProjectPlanItem *curPlan = dynamic_cast<EM_ProjectPlanItem*>(*projIter)) // проект
                    {
                        //PR(4,"if EM_ProjectPlanItem");
                        TCarryTask *newCarryTask(new TCarryTask(curYear, curPlan->getProduction()->getProductionType(), curPlan->getID(), curPlan->getNum(), curPlan->getShortTitle())); // curPlan->SUID()
                        newCarryTask->setScrName(QString("%1. %2").arg((int)newCarryTask->productionType()).arg(newCarryTask->name()));
                        newCarryTask->setISort(newCarryTask->num());
                        newCarryTask->setCondition(curPlan->getState());
                        newCarryTask->setProblem(curPlan->getProblem());
                        newCarryTask->setSourcesTitle(curPlan->getSrcTitle());
                        newCarryTask->setResultsTitle(curPlan->getResTitle());
                        TEmployeeRole rl(curPlan->getTemplEmployee());
                        rl.setUnitId(curPlan->getOshsItemID());
                        newCarryTask->setSingleTemplateRole(rl);
                        newCarryTask->setUnitId(rl.unitId()); // тролько для проекта
                        if (EM_User *user = curPlan->getEmployee()) newCarryTask->setEmployee(modEmployees->findEmployee(user->SUID()));
                        newCarryTask->setTemplatePeriod(curPlan->getTypalDuration());
                        newCarryTask->setPlanPeriod(curPlan->getTypalDuration()); // ??? должно быть плановое
                        dt = curPlan->planBegin();
                        if (dt.isValid()) newCarryTask->setDtPlanBegin(dt);
                        dt = curPlan->planEnd();
                        if (dt.isValid()) newCarryTask->setDtPlanEnd(dt);
                        dt = curPlan->realBegin();
                        if (dt.isValid()) newCarryTask->setDtRealBegin(dt);
                        dt = curPlan->realEnd();
                        if (dt.isValid()) newCarryTask->setDtRealEnd(dt);
                        newCarryTask->setStatuses(curPlan->getStatus());
                        newCarryTask->setDescr(curPlan->getDescr());
                        newCarryTask->setPriority(curPlan->getPriority());
                        newCarryTask->setCarryOutPercent(curPlan->getProgress());
                        if (curPlan->getTimeBegin().isValid()) newCarryTask->setDtMinBegin(curPlan->getTimeBegin());
                        if (curPlan->getTimeEnd().isValid()) newCarryTask->setDtMaxEnd(curPlan->getTimeEnd());
                        newCarryTask->setSaved(true);
                        for(EM_BasePlanItem::iterator planIter = curPlan->begin(); planIter != curPlan->end(); ++planIter)
                        {
                            PR(8, "for planIter");
                            EM_BasePlanItem *pl = *planIter;
                            PR3(12, "NODE_TYPE: %1, SUID: %2, (%3)", pl->getType(), pl->getID(), pl->getFullTitle());
                            if (EM_User *user = pl->getEmployee())
                            {
                            PR1(16, "USER: %1", user->getDescr());
                              MODULE(Units);
                                if (!modUnits->selfUnit()->isInternalEmployee(user->SUID())) continue;
                            }
                            else PR(16, "USER: unknown");
                            if (EM_PlanItem *curPlan = dynamic_cast<EM_PlanItem*>(*planIter)) // план
                            {
                                //PR(12,"if EM_PlanItem");
                                TCarryPlan *newCarryPlan(new TCarryPlan(newCarryTask, curPlan->getID(), curPlan->getNum(), curPlan->getShortTitle())); // curPlan->SUID()
                                newCarryPlan->setScrName(QString("%1.%2. %3").arg(newCarryPlan->num() / 10).arg(newCarryPlan->num() % 10).arg(newCarryPlan->name()));
                                newCarryPlan->setISort(newCarryPlan->num());
                                newCarryPlan->setCondition(curPlan->getState());
                                newCarryPlan->setProblem(curPlan->getProblem());
                                newCarryPlan->setSourcesTitle(curPlan->getSrcTitle());
                                newCarryPlan->setResultsTitle(curPlan->getResTitle());
                                TEmployeeRole rl(curPlan->getTemplEmployee());
                                rl.setUnitId(curPlan->getOshsItemID());
                                newCarryPlan->setSingleTemplateRole(rl);
                                if (EM_User *user = curPlan->getEmployee()) newCarryPlan->setEmployee(modEmployees->findEmployee(user->SUID()));
                                newCarryPlan->setTemplatePeriod(curPlan->getTypalDuration());
                                newCarryPlan->setPlanPeriod(curPlan->getDuration());
                                dt = curPlan->planBegin();
                                if (dt.isValid()) newCarryPlan->setDtPlanBegin(dt);
                                dt = curPlan->planEnd();
                                if (dt.isValid()) newCarryPlan->setDtPlanEnd(dt);
                                dt = curPlan->realBegin();
                                if (dt.isValid()) newCarryPlan->setDtRealBegin(dt);
                                dt = curPlan->realEnd();
                                if (dt.isValid()) newCarryPlan->setDtRealEnd(dt);
                                newCarryPlan->setStatuses(curPlan->getStatus());
                                newCarryPlan->setDescr(curPlan->getDescr());
                                newCarryPlan->setCarryOutPercent(curPlan->getProgress());
                                newCarryPlan->setSaved(true);
                                for(EM_BasePlanItem::iterator prIter = curPlan->begin(); prIter != curPlan->end(); ++prIter)
                                {
                                    //PR(16,"for prIter");
                                    if (EM_StagePlanItem *curPr = dynamic_cast<EM_StagePlanItem*>(*prIter)) // процедура
                                    {
                                        //PR(20,"if EM_StagePlanItem");
                                        TCarryProcedure *newCarryProcedure(new TCarryProcedure(newCarryPlan, curPr->getID(), curPr->getNum(), curPr->getShortTitle())); // curPr->SUID()
                                        newCarryProcedure->setISort(newCarryProcedure->num());
                                        newCarryProcedure->setCondition(curPr->getState());
                                        newCarryProcedure->setProblem(curPr->getProblem());
                                        newCarryProcedure->setSourcesTitle(curPr->getSrcTitle());
                                        newCarryProcedure->setResultsTitle(curPr->getResTitle());
                                        TEmployeeRole rl(curPr->getTemplEmployee());
                                        rl.setUnitId(curPr->getOshsItemID());
                                        newCarryProcedure->setSingleTemplateRole(rl);
                                        if (EM_User *user = curPr->getEmployee()) newCarryProcedure->setEmployee(modEmployees->findEmployee(user->SUID()));
                                        newCarryProcedure->setTemplatePeriod(curPr->getTypalDuration());
                                        newCarryProcedure->setPlanPeriod(curPr->getDuration());
                                        dt = curPr->planBegin();
                                        if (dt.isValid()) newCarryProcedure->setDtPlanBegin(dt);
                                        dt = curPr->planEnd();
                                        if (dt.isValid()) newCarryProcedure->setDtPlanEnd(dt);
                                        dt = curPr->realBegin();
                                        if (dt.isValid()) newCarryProcedure->setDtRealBegin(dt);
                                        dt = curPr->realEnd();
                                        if (dt.isValid()) newCarryProcedure->setDtRealEnd(dt);
                                        newCarryProcedure->setStatuses(curPr->getStatus());
                                        newCarryProcedure->setDescr(curPr->getDescr());
                                        newCarryProcedure->setExternProcedureNum(curPr->getExtProcNum());
                                        newCarryProcedure->setCarryOutPercent(curPr->getProgress());
                                        newCarryProcedure->setSaved(true);
                                        for(EM_BasePlanItem::iterator wrkIter = curPr->begin(); wrkIter != curPr->end(); ++wrkIter)
                                        {
                                            //PR(24,"for wrkIter");
                                            if (EM_TaskPlanItem *curWrk = dynamic_cast<EM_TaskPlanItem*>(*wrkIter)) // работа
                                            {
                                                //PR(28,"if EM_TaskPlanItem");
                                                TCarryWork *newCarryWork(new TCarryWork(newCarryProcedure, emtNone, curWrk->getID(), curWrk->getNum(), curWrk->getShortTitle())); // curWrk->SUID()
                                                newCarryWork->setISort(newCarryWork->num());
                                                newCarryWork->setScrName(QString("%1. %2").arg(newCarryWork->num()).arg(newCarryWork->name()));
                                                newCarryWork->setCondition(curWrk->getState());
                                                newCarryWork->setProblem(curWrk->getProblem());
                                                newCarryWork->setSourcesTitle(curWrk->getSrcTitle());
                                                newCarryWork->setResultsTitle(curWrk->getResTitle());
                                                newCarryWork->setExternProcedureNum(curWrk->getExtProcNum());
                                                TEmployeeRole rl(curWrk->getTemplEmployee());
                                                rl.setUnitId(curWrk->getOshsItemID());
                                                newCarryWork->setSingleTemplateRole(rl);
                                                if (EM_User *user = curWrk->getEmployee()) newCarryWork->setEmployee(modEmployees->findEmployee(user->SUID()));
                                                newCarryWork->setTemplatePeriod(curWrk->getTypalDuration());
                                                newCarryWork->setPlanPeriod(curWrk->getDuration());
                                                dt = curWrk->planBegin();
                                                if (dt.isValid()) newCarryWork->setDtPlanBegin(dt);
                                                dt = curWrk->planEnd();
                                                if (dt.isValid()) newCarryWork->setDtPlanEnd(dt);
                                                dt = curWrk->realBegin();
                                                if (dt.isValid()) newCarryWork->setDtRealBegin(dt);
                                                dt = curWrk->realEnd();
                                                if (dt.isValid()) newCarryWork->setDtRealEnd(dt);
                                                newCarryWork->setStatuses(curWrk->getStatus());
                                                newCarryWork->setDescr(curWrk->getDescr());
                                                newCarryWork->setExternalModule(curWrk->getExtModuleType());
                                                newCarryWork->setOptional(curWrk->isOptional());
                                                newCarryWork->setPresent(curWrk->isPresent());
                                                newCarryWork->setControl(curWrk->isTester());
                                                newCarryWork->setCarryOutPercent(curWrk->getProgress());
                                                newCarryWork->setSaved(true);
                                                newCarryProcedure->insertWork(newCarryWork);
                                            }
                                        }
                                        qStableSort(newCarryProcedure->works().begin(), newCarryProcedure->works().end(), obj::FSortMinToMaxInt<TCarryWork*>());
                                        newCarryPlan->insertProcedure(newCarryProcedure);
                                    }
                                }
                                qStableSort(newCarryPlan->procedures().begin(), newCarryPlan->procedures().end(), obj::FSortMinToMaxInt<TCarryProcedure*>());
                                if (newCarryPlan->name().contains("ОРД")) newCarryTask->setOrdPlan(newCarryPlan);
                                else newCarryTask->setCarryPlan(newCarryPlan);
                            }
                        }
                        if (newCarryTask->ordPlan() || newCarryTask->carryPlan()) fPlCarryTasks.append(newCarryTask);
                        else DELETE(newCarryTask);
                    }
                    qStableSort(fPlCarryTasks.begin(), fPlCarryTasks.end(), obj::FSortMinToMaxInt<TCarryTask*>());
                }
                delete yearPlan;
            }
        }
        catch (CommonException::OpenDBException &e)
        {
            INSERT_ERROR(QString("%1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
        }
        catch (CommonException::SQLException &e)
        {
            INSERT_ERROR(QString("%1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
        }
    }
    else
    {
        // ??? где отсеиваются чужие проекты
    }
}
//-----------------------------------------------------------------------------

void TModulePlans::reflectTemplatesToCb(const TAbstractPlanList &plans, QComboBox &cb)
{
    cb.clear();
    QStringList scrnms;
    foreach (TAbstractPlan * pl, plans) scrnms << pl->scrName();
    qSort(scrnms);
    cb.addItems(scrnms);
    for (int i = 0; i < cb.count(); i++)
    {
        bool issaved(false);
        if (TAbstractPlan *pl = findPlanTemplate(cb.itemText(i), true)) issaved = pl->isSaved();
        cb.setItemIcon(i, ICONPIX(issaved ? "" : PIX_QUESTION));
    }
}
//-----------------------------------------------------------------------------

// 0. Наименование                    - ICON(PIX_LEVEL_) + TEXT
// 1. Признак невозможности изменения - ICON(PIX_CROSS)
// 2. Продолжительность               -                    TEXT
// 3. Начало                          -                    TEXT
// 4. Окончание                       -                    TEXT
// 5. Должностное лицо                -                    TEXT
// 6. Признак (не)сохраненности       - ICON(PIX_DB)
// (если onlycheckedplans==true, то непомеченные планы () план не отображать)
void TModulePlans::reflectCarryTasksToTree(const TCarryTaskList &tsks, QTreeWidget &tw, bool onlycheckedplans, int rowheight)
{
    tw.clear();
    foreach (TCarryTask *tsk, tsks)
    {
        bool isReflect(false);
        TCarryPlan *plans[2] = { tsk->ordPlan(), tsk->carryPlan() };
        if (onlycheckedplans) // проверка наличия отображаемых планов
        {
            for (int i = 0; i < 2; i++)
                if (TCarryPlan *plan = plans[i])
                    if (plan->isChecked())
                    {
                        isReflect = true;
                        break;
                    }
        }
        if (onlycheckedplans && !isReflect) continue;
        QTreeWidgetItem *twiTsk(new QTreeWidgetItem(&tw));
        twiTsk->setText(0, tsk->scrName());
        twiTsk->setIcon(0, ICONPIX(PIX_LEVEL2));
        twiTsk->setIcon(1, ICONPIX(tsk->isVolatile() ? "" : PIX_CROSS));
        twiTsk->setText(2, "");
        twiTsk->setText(3, "");
        twiTsk->setText(4, "");
        twiTsk->setText(5, "");
        twiTsk->setIcon(6, ICONPIX(tsk->isSaved() ? "" : PIX_DB));
        twiTsk->setData(0, Qt::UserRole, qVariantFromValue(TIdent(tsk->id(), tsk->num(), tsk->name(), (int)petProject)));
        twiTsk->setFlags(twiTsk->flags() | Qt::ItemIsTristate);
        twiTsk->setCheckState(0, Qt::Unchecked);
        if (rowheight)
        {
            QSize sz = twiTsk->sizeHint(0);
            sz.setHeight(rowheight);
            twiTsk->setSizeHint(0, sz);
        }
        for (int i = 0; i < 2; i++)
            if (TCarryPlan *plan = plans[i])
            {
                if (onlycheckedplans && !plan->isChecked()) continue;
                QTreeWidgetItem *twiPl(new QTreeWidgetItem(twiTsk));
                twiPl->setText(0, plan->scrName());
                twiPl->setIcon(0, ICONPIX(PIX_LEVEL3));
                twiPl->setIcon(1, ICONPIX(tsk->isVolatile() ? "" : PIX_CROSS));
                twiPl->setText(2, gen::intToStr(plan->planPeriod()));
                twiPl->setText(3, plan->dtPlanBegin() ? plan->dtPlanBegin()->toString("hh:mm dd.MM") : "");
                twiPl->setText(4, plan->dtPlanEnd() ? plan->dtPlanEnd()->toString("hh:mm dd.MM") : "");
                twiPl->setText(5, "");
                twiPl->setIcon(6, ICONPIX(plan->isSaved() ? "" : PIX_DB));
                twiPl->setData(0, Qt::UserRole, qVariantFromValue(TIdent(plan->id(), plan->num(), plan->name(), (int)petPlan)));
                twiPl->setFlags(twiPl->flags() | Qt::ItemIsTristate);
                twiPl->setCheckState(0, Qt::Unchecked);
                if (rowheight)
                {
                    QSize sz = twiPl->sizeHint(0);
                    sz.setHeight(rowheight);
                    twiPl->setSizeHint(0, sz);
                }
                foreach (TCarryProcedure *pr, plan->procedures())
                {
                    QTreeWidgetItem *twiPr(new QTreeWidgetItem(twiPl));
                    twiPr->setText(0, pr->scrName());
                    twiPr->setIcon(0, ICONPIX(PIX_LEVEL4));
                    twiPr->setIcon(1, ICONPIX(tsk->isVolatile() ? "" : PIX_CROSS));
                    twiPr->setText(2, gen::intToStr(pr->planPeriod()));
                    twiPr->setText(3, pr->dtPlanBegin() ? pr->dtPlanBegin()->toString("hh:mm dd.MM") : "");
                    twiPr->setText(4, pr->dtPlanEnd() ? pr->dtPlanEnd()->toString("hh:mm dd.MM") : "");
                    twiPr->setText(5, pr->employee() ? pr->employee()->name() : "");
                    twiPr->setIcon(6, ICONPIX(pr->isSaved() ? "" : PIX_DB));
                    twiPr->setData(0, Qt::UserRole, qVariantFromValue(TIdent(pr->id(), pr->num(), pr->name(), (int)petProcedure)));
                    twiPr->setFlags(twiPr->flags() | Qt::ItemIsTristate);
                    twiPr->setCheckState(0, Qt::Unchecked);
                    if (rowheight)
                    {
                        QSize sz = twiPr->sizeHint(0);
                        sz.setHeight(rowheight);
                        twiPr->setSizeHint(0, sz);
                    }
                    foreach (TCarryWork *wrk, pr->works())
                    {
                        QTreeWidgetItem *twiWrk(new QTreeWidgetItem(twiPr));
                        twiWrk->setText(0, wrk->scrName());
                        twiWrk->setIcon(0, ICONPIX(PIX_LEVEL5));
                        twiWrk->setIcon(1, ICONPIX(tsk->isVolatile() ? "" : PIX_CROSS));
                        twiWrk->setText(2, gen::intToStr(wrk->planPeriod()));
                        twiWrk->setText(3, wrk->dtPlanBegin() ? wrk->dtPlanBegin()->toString("hh:mm dd.MM") : "");
                        twiWrk->setText(4, wrk->dtPlanEnd() ? wrk->dtPlanEnd()->toString("hh:mm dd.MM") : "");
                        twiWrk->setText(5, wrk->employee() ? wrk->employee()->name() : "");
                        twiWrk->setIcon(6, ICONPIX(wrk->isSaved() ? "" : PIX_DB));
                        twiWrk->setData(0, Qt::UserRole, qVariantFromValue(TIdent(wrk->id(), wrk->num(), wrk->name(), (int)petWork)));
                        twiWrk->setFlags(twiWrk->flags() | Qt::ItemIsUserCheckable);
                        twiWrk->setCheckState(0, Qt::Unchecked);
                        if (rowheight)
                        {
                            QSize sz = twiWrk->sizeHint(0);
                            sz.setHeight(rowheight);
                            twiWrk->setSizeHint(0, sz);
                        }
                    }
                }
            }
    }
}
//-----------------------------------------------------------------------------

void TModulePlans::reflectExternProceduresToCb(const TExternProcedureTemplateList &extprocs, QComboBox &cb, bool withempty)
{
    cb.clear();
    if (withempty) cb.addItem("");
    foreach (TExternProcedureTemplate * extproc, extprocs) cb.addItem(extproc->scrName());
}
//-----------------------------------------------------------------------------

// Назначить id для элементов c id==0. После записи в БД id изменятся
// (для выполнения createCarryPlans)
void TModulePlans::setElementIdsForPlan()
{
    int maxId(0);
    foreach(TCarryTask *tsk, fPlCarryTasks) // найти наибольший id элемента
    {
        if (tsk->id() > maxId) maxId = tsk->id();
        TCarryPlanList plans;
        plans.setAutoDelete(false);
        if (tsk->ordPlan()) plans.append(tsk->ordPlan());
        if (tsk->carryPlan()) plans.append(tsk->carryPlan());
        foreach (TCarryPlan *plan, plans)
        {
            if (plan->id() > maxId) maxId = plan->id();
            foreach (TCarryProcedure *proc, plan->procedures())
            {
                if (proc->id() > maxId) maxId = proc->id();
                foreach (TCarryWork *wrk, proc->works())
                    if (wrk->id() > maxId) maxId = wrk->id();
            }
        }
    }
    foreach(TCarryTask * tsk, fPlCarryTasks) // назначить id для элемента c id==0
    {
        if (!tsk->id()) tsk->setId(++maxId);
        TCarryPlanList plans;
        plans.setAutoDelete(false);
        if (tsk->ordPlan()) plans.append(tsk->ordPlan());
        if (tsk->carryPlan()) plans.append(tsk->carryPlan());
        foreach (TCarryPlan *plan, plans)
        {
            if (!plan->id()) plan->setId(++maxId);
            foreach (TCarryProcedure *proc, plan->procedures())
            {
                if (!proc->id()) proc->setId(++maxId);
                foreach (TCarryWork *wrk, proc->works())
                    if (!wrk->id()) wrk->setId(++maxId);
            }
        }
    }
}
//-----------------------------------------------------------------------------

QString TModulePlans::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TModulePlans::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

// param: ExternProcedures, Temlates, CompanyCalendar, CarryTasks
bool TModulePlans::toDB(QString param)
{
    if (param == QString("ExternProcedures"))
    {

        return true;
    }
    else if (param == QString("Temlates"))
    {

        return true;
    }
    else if (param == QString("CompanyCalendar"))
    {

        return true;
    }
    else if (param == QString("CarryTasks"))
    {

        return true;
    }
    else return false;
}
//-----------------------------------------------------------------------------

// param: ExternProcedures, Temlates, CompanyCalendar, CarryTasks
bool TModulePlans::fromDB(QString param)
{
    //.........................................................................
    if (param == QString("ExternProcedures"))
    {
        fPlExternProcedureTemplates.clear();
        try
        {
            EM_ExtProcDic &procDic = EM_ExtProcDic::Instance();
            for (QMap<int, EM_ExtProc*>::const_iterator it = procDic.begin(); it != procDic.end(); ++it)
                if (EM_ExtProc *extProc = procDic.by(it.key()))
                {
                    TExternProcedureTemplate *pr(new TExternProcedureTemplate(extProc->getNum(), extProc->getTitle(), extProc->getModules()));
                    //QString mods("");
                    //foreach (TExternalModuleType t,pr->externalModules()) mods += QString("%1,").arg((int)t);
                    //  if (!mods.isEmpty()) mods.remove(mods.length()-1,1);
                    //PR2(4,"ExtProc: %1 <%2>",pr->scrName(),mods);
                    fPlExternProcedureTemplates.append(pr);
                }
        }
        catch (CommonException::OpenDBException &e)
        {
            INSERT_ERROR(QString("%1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
            return false;
        }
        catch (CommonException::SQLException &e)
        {
            INSERT_ERROR(QString("%1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
            return false;
        }
        return true;
    }
    //.........................................................................
    else if (param == QString("Temlates"))
    {
        MODULE(Employees);
        QDateTime dt;
        fPlPlanTemplates.clear();
        try
        {
            EM_YearPlan *templates = new EM_YearPlan(YEAR_PLAN_TYPE_CARRYTEMPLATE);
            if (templates != 0)
            {
                templates->fromDB();
                //PR1(4,"templates: %1",templates->getTitle());
                //templates->printPlanTree();
                EM_BasePlanItem *root = templates->getRoot();
                for(EM_BasePlanItem::iterator tmplIter = root->begin(); tmplIter != root->end(); ++tmplIter)
                {
                    //PR(0,"for tmplIter");
                    if (EM_PlanItem *curTmpl = dynamic_cast<EM_PlanItem*>(*tmplIter)) // шаблон
                    {
                        //PR(4,"if EM_PlanItem");
                        TCarryPlan *newCarryPlan(new TCarryPlan(NULL, curTmpl->getID(), curTmpl->getNum(), curTmpl->getShortTitle()));
                        newCarryPlan->setScrName(QString("%1.%2. %3").arg(newCarryPlan->num() / 10).arg(newCarryPlan->num() % 10).arg(newCarryPlan->name()));
                        if (newCarryPlan->num() == 11) PR1(12, "TEMPLATE: newCarryPlan: %1", newCarryPlan->scrName());
                        newCarryPlan->setCondition(curTmpl->getState());
                        newCarryPlan->setProblem(curTmpl->getProblem());
                        newCarryPlan->setSourcesTitle(curTmpl->getSrcTitle());
                        newCarryPlan->setResultsTitle(curTmpl->getResTitle());
                        TEmployeeRole rl(curTmpl->getTemplEmployee());
                        rl.setUnitId(curTmpl->getOshsItemID());
                        newCarryPlan->setSingleTemplateRole(rl);
                        if (EM_User *user = curTmpl->getEmployee()) newCarryPlan->setEmployee(modEmployees->findEmployee(user->SUID()));
                        newCarryPlan->setTemplatePeriod(curTmpl->getTypalDuration());
                        newCarryPlan->setPlanPeriod(curTmpl->getDuration());
                        dt = curTmpl->planBegin();
                        if (dt.isValid()) newCarryPlan->setDtPlanBegin(dt);
                        dt = curTmpl->planEnd();
                        if (dt.isValid()) newCarryPlan->setDtPlanEnd(dt);
                        dt = curTmpl->realBegin();
                        if (dt.isValid()) newCarryPlan->setDtRealBegin(dt);
                        dt = curTmpl->realEnd();
                        if (dt.isValid()) newCarryPlan->setDtRealEnd(dt);
                        newCarryPlan->setStatuses(curTmpl->getStatus());
                        newCarryPlan->setDescr(curTmpl->getDescr());
                        newCarryPlan->setSaved(true);
                        for(EM_BasePlanItem::iterator prIter = curTmpl->begin(); prIter != curTmpl->end(); ++prIter)
                        {
                            //PR(8,"for prIter");
                            if (EM_StagePlanItem *curPr = dynamic_cast<EM_StagePlanItem*>(*prIter)) // процедура
                            {
                                //PR(12,"if EM_StagePlanItem");
                                TCarryProcedure *newCarryProcedure(new TCarryProcedure(newCarryPlan, curPr->getID(), curPr->getNum(), curPr->getShortTitle()));
                                if (newCarryPlan->num() == 11) PR1(12, "TEMPLATE: newCarryProcedure: %1", newCarryProcedure->scrName());
                                newCarryProcedure->setISort(newCarryProcedure->num());
                                newCarryProcedure->setCondition(curPr->getState());
                                newCarryProcedure->setProblem(curPr->getProblem());
                                newCarryProcedure->setSourcesTitle(curPr->getSrcTitle());
                                newCarryProcedure->setResultsTitle(curPr->getResTitle());
                                TEmployeeRole rl(curPr->getTemplEmployee());
                                rl.setUnitId(curPr->getOshsItemID());
                                if (newCarryPlan->num() == 11) PR2(16, "rl: TP[%1], [%2]", rl.type(), rl.unitId());
                                newCarryProcedure->setSingleTemplateRole(rl);
                                if (EM_User *user = curPr->getEmployee()) newCarryProcedure->setEmployee(modEmployees->findEmployee(user->SUID()));
                                newCarryProcedure->setTemplatePeriod(curPr->getTypalDuration());
                                newCarryProcedure->setPlanPeriod(curPr->getDuration());
                                dt = curPr->planBegin();
                                if (dt.isValid()) newCarryProcedure->setDtPlanBegin(dt);
                                dt = curPr->planEnd();
                                if (dt.isValid()) newCarryProcedure->setDtPlanEnd(dt);
                                dt = curPr->realBegin();
                                if (dt.isValid()) newCarryProcedure->setDtRealBegin(dt);
                                dt = curPr->realEnd();
                                if (dt.isValid()) newCarryProcedure->setDtRealEnd(dt);
                                newCarryProcedure->setStatuses(curPr->getStatus());
                                newCarryProcedure->setDescr(curPr->getDescr());
                                newCarryProcedure->setExternProcedureNum(curPr->getExtProcNum());
                                //PR3(4,"proc: %1, TemplEmployee: %2, OshsItemID: %3",newCarryProcedure->scrName(),curPr->getTemplEmployee(),curPr->getOshsItemID());
                                newCarryProcedure->setSaved(true);
                                for(EM_BasePlanItem::iterator wrkIter = curPr->begin(); wrkIter != curPr->end(); ++wrkIter)
                                {
                                    //PR(16,"for wrkIter");
                                    if (EM_TaskPlanItem *curWrk = dynamic_cast<EM_TaskPlanItem*>(*wrkIter)) // работа
                                    {
                                        //PR(20,"if EM_TaskPlanItem");
                                        TCarryWork *newCarryWork(new TCarryWork(newCarryProcedure, emtNone, curWrk->getID(), curWrk->getNum(), curWrk->getShortTitle()));
                                        newCarryWork->setISort(newCarryWork->num());
                                        newCarryWork->setScrName(QString("%1. %2").arg(newCarryWork->num()).arg(newCarryWork->name()));
                                        newCarryWork->setCondition(curWrk->getState());
                                        newCarryWork->setProblem(curWrk->getProblem());
                                        newCarryWork->setSourcesTitle(curWrk->getSrcTitle());
                                        newCarryWork->setResultsTitle(curWrk->getResTitle());
                                        newCarryWork->setExternProcedureNum(curWrk->getExtProcNum());
                                        TEmployeeRole rl(curWrk->getTemplEmployee());
                                        rl.setUnitId(curWrk->getOshsItemID());
                                        newCarryWork->setSingleTemplateRole(rl);
                                        if (EM_User *user = curWrk->getEmployee()) newCarryWork->setEmployee(modEmployees->findEmployee(user->SUID()));
                                        newCarryWork->setTemplatePeriod(curWrk->getTypalDuration());
                                        if (!newCarryWork->templatePeriod()) newCarryWork->setTemplatePeriod(1);
                                        newCarryWork->setPlanPeriod(curWrk->getDuration());
                                        dt = curWrk->planBegin();
                                        if (dt.isValid()) newCarryWork->setDtPlanBegin(dt);
                                        dt = curWrk->planEnd();
                                        if (dt.isValid()) newCarryWork->setDtPlanEnd(dt);
                                        dt = curWrk->realBegin();
                                        if (dt.isValid()) newCarryWork->setDtRealBegin(dt);
                                        dt = curWrk->realEnd();
                                        if (dt.isValid()) newCarryWork->setDtRealEnd(dt);
                                        newCarryWork->setStatuses(curWrk->getStatus());
                                        newCarryWork->setDescr(curWrk->getDescr());
                                        newCarryWork->setExternalModule(curWrk->getExtModuleType());
                                        newCarryWork->setOptional(curWrk->isOptional());
                                        newCarryWork->setPresent(curWrk->isPresent());
                                        newCarryWork->setControl(curWrk->isTester());
                                        newCarryWork->setSaved(true);
                                        newCarryProcedure->insertWork(newCarryWork);
                                    }
                                }
                                qStableSort(newCarryProcedure->works().begin(), newCarryProcedure->works().end(), obj::FSortMinToMaxInt<TCarryWork*>());
                                newCarryPlan->insertProcedure(newCarryProcedure);
                            }
                        }
                        qStableSort(newCarryPlan->procedures().begin(), newCarryPlan->procedures().end(), obj::FSortMinToMaxInt<TCarryProcedure*>());
                        newCarryPlan->recalcPeriod(true);
                        fPlPlanTemplates.append(newCarryPlan);
                    }
                }
#if defined(Q_OS_WIN32)
                delete templates;
#endif
            }
        }
        catch (CommonException::OpenDBException &e)
        {
            INSERT_ERROR(QString("%1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
            return false;
        }
        catch (CommonException::SQLException &e)
        {
            INSERT_ERROR(QString("%1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
            return false;
        }
        return true;
    }
    //.........................................................................
    else if (param == QString("CompanyCalendar"))
    {
        fCompanyCalendar->fromDB(""); // на текущий год (первоначально устанавливается в конструкторе)
        // календари сотрудников своего и подчиненных подразделений
        try
        {
            EM_CalendarDic &dic = EM_CalendarDic::Instance();
            MODULE(Units);
            QStack<TUnit*> unStack;
            unStack.push(modUnits->selfUnit());
            while (!unStack.isEmpty())
            {
                TUnit &curUn = *unStack.pop();
                PR1(0, "Calendar Unit: %1", curUn.name());
                foreach (TEmployee *empl, curUn.employees())
                {
                    PR1(4, "Calendar Employee: %1", empl->scrName());
                    TEmployeeCalendar *clndr(new TEmployeeCalendar(fCompanyCalendar));
                    try
                    {
                        QList<QSharedPointer<EM_CalendarUserItem> > shPeriods = dic.get(QDateTime(QDate(fCompanyCalendar->year(), 1, 1), QTime(0, 0)), QDateTime(QDate(fCompanyCalendar->year() + 1, 1, 1), QTime(0, 0)), empl->id());
                        foreach (QSharedPointer<EM_CalendarUserItem> shIt, shPeriods)
                            if (EM_CalendarUserItem *cdrIt = shIt.data())
                                clndr->insertWorkPeriod(new TWorkPeriod(cdrIt->getType(), cdrIt->getBeginDate().date().dayOfYear() - 1, cdrIt->getEndDate().date().dayOfYear() - 1));
                    }
                    catch (AddressBookException::UserNotFoundException &e)
                    {
                        INSERT_ERROR(QString("EM_CalendarDic::get(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
                        return false;
                    }
                    catch (CommonException::OpenDBException &e)
                    {
                        INSERT_ERROR(QString("EM_CalendarDic::get(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
                        return false;
                    }
                    catch (CommonException::SQLException &e)
                    {
                        INSERT_ERROR(QString("EM_CalendarDic::get(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
                        return false;
                    }
                    empl->setCalendar(clndr);
                }
                foreach (TUnit * subUn, curUn.subUnits()) unStack.push(subUn);
            }
        }
        catch (CommonException::OpenDBException &e)
        {
            INSERT_ERROR(QString("EM_CalendarDic::Instance(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
            return false;
        }
        catch (CommonException::SQLException &e)
        {
            INSERT_ERROR(QString("EM_CalendarDic::Instance(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
            return false;
        }
        return true;
    }
    //.........................................................................
    else if (param == QString("CarryTasks"))
    {
        QList<int> tskids; // пустой список - грузятся все проекты
        fillTasks(tskids);
        return true;
    }
    else return false;
}
//-----------------------------------------------------------------------------
