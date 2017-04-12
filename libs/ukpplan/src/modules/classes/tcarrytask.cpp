#include <defMacro>
#include <convertEnums>
#include <TCarryTask>
#include <EM_Plan>

TCarryTask::TCarryTask(int year, TProductionType prtp, int id, int n, QString nm,
                       TAbstractObject *parent) : TAbstractPlanElement(id, n, nm, parent), fYear(year), fPriority(0), fProductionType(prtp),
    fOrdPlan(NULL), fCarryPlan(NULL), fDtMinBegin(NULL), fDtMaxEnd(NULL)
{
}
//-----------------------------------------------------------------------------

TCarryTask::~TCarryTask()
{
    DELETE(fOrdPlan);
    DELETE(fCarryPlan);
    DELETE(fDtMinBegin);
    DELETE(fDtMaxEnd);
}
//-----------------------------------------------------------------------------

int TCarryTask::year() const
{
    return fYear;
}
//-----------------------------------------------------------------------------

//void TCarryTask::setYear(int year)
//{
//    fYear = year;
//}
////-----------------------------------------------------------------------------

void TCarryTask::reset(bool thisonly)
{
    if (!thisonly) TAbstractPlanElement::reset(true);
    fYear = 0;
    fPriority = 0;
    fProductionType = prtNone;
    DELETE(fOrdPlan);
    DELETE(fCarryPlan);
}
//-----------------------------------------------------------------------------

int TCarryTask::priority() const
{
    return fPriority;
}
//-----------------------------------------------------------------------------

void TCarryTask::setPriority(int pr)
{
    fPriority = pr;
}
//-----------------------------------------------------------------------------

TProductionType TCarryTask::productionType() const
{
    return fProductionType;
}
//-----------------------------------------------------------------------------

void TCarryTask::setProductionType(TProductionType prtp)
{
    fProductionType = prtp;
}
//-----------------------------------------------------------------------------

TCarryPlan *TCarryTask::ordPlan() const
{
    return fOrdPlan;
}
//-----------------------------------------------------------------------------

void TCarryTask::setOrdPlan(TCarryPlan *pl)
{
    DELETE(fOrdPlan);
    fOrdPlan = pl;
}
//-----------------------------------------------------------------------------

TCarryPlan *TCarryTask::carryPlan() const
{
    return fCarryPlan;
}
//-----------------------------------------------------------------------------

void TCarryTask::setCarryPlan(TCarryPlan *pl)
{
    DELETE(fCarryPlan);
    fCarryPlan = pl;
}
//-----------------------------------------------------------------------------

QDateTime *TCarryTask::dtMinBegin() const
{
    return fDtMinBegin;
}
//-----------------------------------------------------------------------------

void TCarryTask::setDtMinBegin(const QDateTime &dt)
{
    if (dt.isValid())
    {
        DELETE(fDtMinBegin);
        fDtMinBegin = new QDateTime(dt);
    }
}
//-----------------------------------------------------------------------------

QDateTime *TCarryTask::dtMaxEnd() const
{
    return fDtMaxEnd;
}
//-----------------------------------------------------------------------------

void TCarryTask::setDtMaxEnd(const QDateTime &dt)
{
    if (dt.isValid())
    {
        DELETE(fDtMaxEnd);
        fDtMaxEnd = new QDateTime(dt);
    }
}
//-----------------------------------------------------------------------------

QString TCarryTask::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TCarryTask::toHtml(bool) // fullinfo
{
    QString sHtml("");
    sHtml += QString("Год: <b>%1</b>").arg(fYear);
    sHtml += QString("<br>Продукция: <b>%1</b>").arg(convertEnums::enumToStr(fProductionType));
    sHtml += QString("<br>ОРД: <b>%1</b>").arg(fOrdPlan ? (fOrdPlan->procedures().count() ? "заполнен" :
             "не заполнен") : "нет");
    sHtml += QString("<br>План: <b>%1</b>").arg(fCarryPlan ? (fCarryPlan->procedures().count() ? "заполнен" :
             "не заполнен") : "нет");

    sHtml += QString("<br>Приоритет: <b>%1</b>").arg(fPriority);
    sHtml += QString("<br>Сосотояние: <b>%1</b>").arg(convertEnums::enumToStr(fCondition));
    if (fProblem != copNone) sHtml += QString("<br>Проблемы: <b>%1</b>").arg(convertEnums::enumToStr(fProblem));
    sHtml += QString("<br>Исх. данные: <b>%1</b>").arg(fSourcesTitle.isEmpty() ? "---" : fSourcesTitle);
    //TAbstractAttachmentList fSources;  // Список исходных данных
    sHtml += QString("<br>Результаты: <b>%1</b>").arg(fResultsTitle.isEmpty() ? "" : fResultsTitle);
    //TAbstractAttachmentList fResults;  // Список результатов
    TEmployeeRole erl = firstTemplateRole();
    if (erl.type() != eltNone)
    {
        sHtml += QString("<br>Типовое ДЛ: <b>%1</b>").arg(convertEnums::enumToStr(erl.type()));
        MODULE(Units);
        if (TUnit *un = modUnits->findUnit(erl.unitId())) sHtml += QString("<br>Внешнее подразд.: <b>%1</b>").arg(
                        un->scrName());
    }
    sHtml += QString("<br>Возможные ДЛ: <b>%1</b>").arg(fPossibleEmployees.count());
    int n(0);
    foreach (TEmployee *empl, fPossibleEmployees)
        sHtml += QString("<br><b>%1. %2</b>").arg(++n).arg(empl->scrName());
    sHtml += QString("<br>Закрепленное ДЛ: <b>%1</b>").arg(fEmployee ? fEmployee->scrName() : "---");
    sHtml += QString("<br>Продолж. тип.: <b>%1</b>").arg(fTemplatePeriod);
    sHtml += QString("<br>Продолж. план.: <b>%1</b>").arg(fPlanPeriod);
    sHtml += QString("<br>Tн (план): <b>%1</b>").arg(fDtPlanBegin ? fDtPlanBegin->toString("dd.MM.yy") : "---");
    sHtml += QString("<br>Tк (план): <b>%1</b>").arg(fDtPlanEnd ? fDtPlanEnd->toString("dd.MM.yy") : "---");
    sHtml += QString("<br>Tн (факт): <b>%1</b>").arg(fDtRealBegin ? fDtRealBegin->toString("dd.MM.yy (hh:mm)") :
             "---");
    sHtml += QString("<br>Tк (факт): <b>%1</b>").arg(fDtRealEnd ? fDtRealEnd->toString("dd.MM.yy (hh:mm)") : "---");
    sHtml += QString("<br>Статусы: <b>%1</b>").arg(fStatuses.count());
    n = 0;
    foreach (TStatus st, fStatuses)
        sHtml += QString("<br><b>%1. %2</b>").arg(++n).arg(convertEnums::enumToStr(st));
    sHtml += QString("<br>Сохранено: <b>%1</b>").arg(fIsSaved ? "Да" : "Нет");
    sHtml += QString("<br>Можно изменять: <b>%1</b>").arg(fIsVolatile ? "Да" : "Нет");

    sHtml += QString("<br>Планировать с: <b>%1</b>").arg(fDtMinBegin ? fDtMinBegin->toString("dd.MM.yy") :
             "---");
    sHtml += QString("<br>Выполнить к: <b>%1</b>").arg(fDtMaxEnd ? fDtMaxEnd->toString("dd.MM.yy") : "---");

    return sHtml;
}
//-----------------------------------------------------------------------------

bool TCarryTask::toDB(QString)
{
    EM_AddressBook *addrBook(NULL);
    try
    {
        EM_AddressBook &raddrBook = EM_AddressBook::Instance();
        addrBook = &raddrBook;
    }
    catch(CommonException::OpenDBException &e)
    {
        INSERT_ERROR(QString("EM_AddressBook::Instance(): %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
        return false;
    }
    catch(CommonException::SQLException &e)
    {
        INSERT_ERROR(QString("EM_AddressBook::Instance(): %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
        return false;
    }
    EM_ProductionDic *prodDic(NULL);
    try
    {
        EM_ProductionDic &rprodDic = EM_ProductionDic::Instance();
        prodDic = &rprodDic;
    }
    catch(CommonException::OpenDBException &e)
    {
        INSERT_ERROR(QString("EM_ProductionDic::Instance(): %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
        return false;
    }
    catch(CommonException::SQLException &e)
    {
        INSERT_ERROR(QString("EM_ProductionDic::Instance(): %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
        return false;
    }
    try
    {
        EM_YearPlanDic &planDic = EM_YearPlanDic::Instance();
        QSharedPointer<EM_YearPlan> shpYearPlan = planDic.by(fYear);
        EM_YearPlan *yearPlan = shpYearPlan.data();
        if (!yearPlan)
        {
            yearPlan = new EM_YearPlan(fYear);
            yearPlan->setTitle(QString("Годовой план %1 года").arg(fYear));
            shpYearPlan.reset(yearPlan);
        }
        EM_BasePlanItem *root(NULL);
        try // удалить старый проект, если он был в базе
        {
            yearPlan->fromDB(); // чтение годового плана
            root = yearPlan->getRoot();
            for (EM_BasePlanItem::iterator projIter = root->begin(); projIter != root->end(); ++projIter)
                if (EM_ProjectPlanItem *emProj = dynamic_cast<EM_ProjectPlanItem *>(*projIter)) // проект
                    if (emProj->getID() == id())
                    {
                        try
                        {
                            yearPlan->rem(emProj); // удаление проекта
                        }
                        catch (CommonException::OpenDBException &e)
                        {
                            INSERT_ERROR(QString("yearPlan->rem(emProj). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
                            return false;
                        }
                        catch (CommonException::SQLException &e)
                        {
                            INSERT_ERROR(QString("yearPlan->rem(emProj). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
                            return false;
                        }
                        catch (CommonException::NullParamException &e)
                        {
                            INSERT_ERROR(QString("yearPlan->rem(emProj). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
                            return false;
                        }
                        break;
                    }
        }
        catch (CommonException::OpenDBException &e)
        {
            INSERT_ERROR(QString("yearPlan->fromDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
            return false;
        }
        catch (CommonException::SQLException &e)
        {
            INSERT_ERROR(QString("yearPlan->fromDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
            return false;
        }
        // добавить новый
        EM_BasePlanItem *emProj = yearPlan->Instance(PROJECT);
        if (EM_ProjectPlanItem *projItem = dynamic_cast<EM_ProjectPlanItem *>(emProj))
        {
            projItem->setNum(num());
            projItem->setShortTitle(name());
            projItem->setFullTitle(scrName());
            projItem->setState(condition());
            projItem->setProblem(problem());
            if (dtPlanBegin()) projItem->planBegin(*dtPlanBegin());
            if (dtPlanEnd()) projItem->planBegin(*dtPlanEnd());
            if (dtRealBegin()) projItem->realBegin(*dtRealBegin());
            if (dtRealEnd()) projItem->realBegin(*dtRealEnd());
            projItem->setDescr(descr());
            if (employee())
            {
                if (EM_User *usr = addrBook->getUser(employee()->id())) projItem->setEmployee(usr); // external=false
            }
            projItem->setTypalDuration(templatePeriod());
            projItem->setDuration(planPeriod());
            projItem->setSrcTitle(sourcesTitle());
            projItem->setResTitle(resultsTitle());
            foreach (TStatus st, statuses()) projItem->addStatus(st);
            TEmployeeRole rl = firstTemplateRole();
            projItem->setTemplEmployee(rl.type());
            projItem->setOshsItemID(rl.unitId() ? rl.unitId() : unitId());
            projItem->setPriority(priority());
            if (EM_Production *prod = prodDic->by(productionType())) projItem->setProduction(prod);
            yearPlan->add(root, emProj);
            try
            {
                yearPlan->toDB();
            }
            catch (CommonException::OpenDBException &e)
            {
                INSERT_ERROR(QString("emPlan->toDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
                return false;
            }
            catch (CommonException::SQLException &e)
            {
                INSERT_ERROR(QString("emPlan->toDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
                return false;
            }
            PR1(4, "emProj->getID(): %1", emProj->getID());
            setId(emProj->getID());
            if (fOrdPlan) fOrdPlan->toDB("plan");
            if (fCarryPlan) fCarryPlan->toDB("plan");
        }
    }
    catch (CommonException::OpenDBException &e)
    {
        INSERT_ERROR(QString("EM_YearPlanDic::Instance(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
        return false;
    }
    catch (CommonException::SQLException &e)
    {
        INSERT_ERROR(QString("EM_YearPlanDic::Instance(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()), false);
        return false;
    }
    return true;
}
//-----------------------------------------------------------------------------

bool TCarryTask::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

//*****************************************************************************

TCarryTaskPackage::TCarryTaskPackage(int id, int n, QString nm, TAbstractObject *parent) :
    TAbstractObject(id, n, nm, parent)
{
    fPlTasks.setAutoDelete(false);
}
//-----------------------------------------------------------------------------

TCarryTaskPackage::~TCarryTaskPackage()
{
}
//-----------------------------------------------------------------------------

void TCarryTaskPackage::reset(bool thisonly)
{
    if (!thisonly) TAbstractObject::reset(true);
    fPlTasks.clear();
}
//-----------------------------------------------------------------------------

TCarryTaskList &TCarryTaskPackage::tasks() const
{
    return const_cast<TCarryTaskList &>(fPlTasks);
}
//-----------------------------------------------------------------------------

void TCarryTaskPackage::insertTask(TCarryTask *tsk)
{
    if (tsk && fPlTasks.indexOf(tsk) == -1) fPlTasks.append(tsk);
}
//-----------------------------------------------------------------------------

void TCarryTaskPackage::removeTask(TCarryTask *tsk)
{
    if (tsk) fPlTasks.removeOne(tsk);
}
//-----------------------------------------------------------------------------

void TCarryTaskPackage::clearTasks()
{
    fPlTasks.clear();
}
//-----------------------------------------------------------------------------

TCarryTask *TCarryTaskPackage::findTask(int id, bool onnum)
{
    foreach (TCarryTask *tsk, fPlTasks)
        if (onnum)
        {
            if (tsk->num() == id) return tsk;
        }
        else if (tsk->id() == id) return tsk;
    return NULL;
}
//-----------------------------------------------------------------------------

TCarryTask *TCarryTaskPackage::findTask(QString nm, bool onscrnm)
{
    foreach (TCarryTask *tsk, fPlTasks)
        if (onscrnm)
        {
            if (tsk->scrName() == nm) return tsk;
        }
        else if (tsk->name() == nm) return tsk;
    return NULL;
}
//-----------------------------------------------------------------------------

QString TCarryTaskPackage::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TCarryTaskPackage::toHtml(bool) // fullinfo
{
    return "";
}
//-----------------------------------------------------------------------------

bool TCarryTaskPackage::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TCarryTaskPackage::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
