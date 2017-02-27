
#include <EM_Plan>
#include <defMacro>
#include <defPictures>
#include <convertEnums>
#include <TIdent>
#include <TCarryPlan>
#include <TModulePlans>

TCarryPlan::TCarryPlan(TCarryTask *crytsk, int id, int n, QString nm, TAbstractObject *parent) : TAbstractPlan(id,n,nm,parent), fCarryTask(crytsk)
{
    fPlProcedures.setAutoDelete(true);
}
//-----------------------------------------------------------------------------

TCarryPlan::TCarryPlan(const TCarryPlan &crt) : TAbstractPlan(crt), fCarryTask(crt.fCarryTask)
{
    fPlProcedures.setAutoDelete(true);
    foreach (TCarryProcedure *pr,crt.fPlProcedures) fPlProcedures.append(new TCarryProcedure(*pr));
}
//-----------------------------------------------------------------------------

TCarryPlan::~TCarryPlan()
{

}
//-----------------------------------------------------------------------------

TCarryTask *TCarryPlan::carryTask() const
{
    return fCarryTask;
}
//-----------------------------------------------------------------------------

TCarryPlan &TCarryPlan::operator=(const TCarryPlan &crt)
{
    if (&crt==this) return *this;
    TAbstractPlan::operator=(crt);
    fCarryTask = crt.fCarryTask;
    fPlProcedures.clear();
    foreach (TCarryProcedure *pr,crt.fPlProcedures) fPlProcedures.append(new TCarryProcedure(*pr));
    return *this;
}
//-----------------------------------------------------------------------------

void TCarryPlan::reset(bool thisonly)
{
    if (!thisonly) TAbstractPlan::reset(true);
    fCarryTask = NULL;
    fPlProcedures.clear();
}
//-----------------------------------------------------------------------------

TCarryProcedureList &TCarryPlan::procedures() const
{
    return const_cast<TCarryProcedureList&>(fPlProcedures);
}
//-----------------------------------------------------------------------------

void TCarryPlan::insertProcedure(TCarryProcedure *pr)
{
    if (pr && fPlProcedures.indexOf(pr)==-1) fPlProcedures.append(pr);
}
//-----------------------------------------------------------------------------

void TCarryPlan::removeProcedure(TCarryProcedure *pr)
{
    if (pr) fPlProcedures.removeOne(pr);
}
//-----------------------------------------------------------------------------

void TCarryPlan::clearProcedures()
{
    fPlProcedures.clear();
}
//-----------------------------------------------------------------------------

TCarryProcedure *TCarryPlan::findProcedure(int n)
{
    foreach (TCarryProcedure *pr,fPlProcedures)
        if (pr->num()==n) return pr;
    return NULL;
}
//-----------------------------------------------------------------------------

TCarryProcedure *TCarryPlan::findProcedure(QString nm, bool onscrnm)
{
    foreach (TCarryProcedure *pr,fPlProcedures)
        if (onscrnm)
        {
            if (pr->scrName()==nm) return pr;
        }
        else if (pr->name()==nm) return pr;
    return NULL;
}
//-----------------------------------------------------------------------------

int TCarryPlan::nextProcedureNum()
{
  int nextN(0);
    foreach (TCarryProcedure *pr,fPlProcedures)
        if (pr->num()>nextN) nextN = pr->num();
    return ++nextN;
}
//-----------------------------------------------------------------------------

// Сформировать план на основе шаблона, заполнив possibleEmployees из wrkun
bool TCarryPlan::fillFromTemplate(const TCarryPlan *tmpl, const TUnit &wrkun)
{
    if (!tmpl) return false;
    clearProcedures();
    foreach (TCarryProcedure *tmplpr,tmpl->procedures())
    {
      TCarryProcedure *newPr(new TCarryProcedure(*tmplpr));
        newPr->setISort(newPr->num());
        newPr->clearPossibleEmployeess();
      TEmployeeRole frstRl = tmplpr->firstTemplateRole();
        if (frstRl.type()!=eltNone && frstRl.unitId()==0) // свое подразделение
        {
          TEmployeeRoleList rls;
            rls<<frstRl;
            if (TEmployeeList *posempl = const_cast<TUnit&>(wrkun).findEmployees(rls))
                foreach (TEmployee *empl,*posempl) newPr->insertPossibleEmployee(empl);
        }
        foreach (TCarryWork *newWrk,newPr->works())
        {
            newWrk->setId(0);
            newWrk->setPlanPeriod(newWrk->templatePeriod());
            newWrk->setSaved(false);
            newWrk->setISort(newWrk->num());
            newWrk->setVolatile(true);
        }
        qStableSort(newPr->works().begin(),newPr->works().end(),obj::FSortMinToMaxInt<TCarryWork*>());
        newPr->setPlanPeriod(newPr->templatePeriod());
        newPr->setSaved(false);
        newPr->setVolatile(true);
        insertProcedure(newPr);
    }
    qStableSort(procedures().begin(),procedures().end(),obj::FSortMinToMaxInt<TCarryProcedure*>());
    setPlanPeriod(templatePeriod());
    recalcPeriod(false); // плановый
    setSaved(false);
    setVolatile(true);
    return true;
}
//-----------------------------------------------------------------------------

void TCarryPlan::recalcPeriod(bool ontempl)
{
  int sumPlanTime(0);
    foreach (TCarryProcedure *pr,fPlProcedures)
    {
      int sumProcTime(0);
        foreach (TCarryWork *wrk,pr->works()) sumProcTime += ontempl ? wrk->templatePeriod() : wrk->planPeriod();
        ontempl ? pr->setTemplatePeriod(sumProcTime) : pr->setPlanPeriod(sumProcTime);
        sumPlanTime += ontempl ? pr->templatePeriod() : pr->planPeriod();
    }
    ontempl ? setTemplatePeriod(sumPlanTime) : setPlanPeriod(sumPlanTime);
}
//-----------------------------------------------------------------------------

void TCarryPlan::reflectToTree(QTreeWidget &tw, bool astemplate)
{
    tw.clear();
    if (astemplate)
    {
      MODULE(Plans);
      QTreeWidgetItem *lastPr(NULL);
        foreach (TCarryProcedure *pr,fPlProcedures)
        {
          QTreeWidgetItem *twiPr(new QTreeWidgetItem(&tw,lastPr));
            twiPr->setText(0,pr->scrName());
            twiPr->setIcon(0,ICONPIX(PIX_LEVEL4));
            twiPr->setText(1,"");
            twiPr->setText(2,"");
          QString extNm("");
            if (pr->isExtern())
                if (TExternProcedureTemplate *extProc = modPlans->findExternProcedureTemplate(pr->externProcedureNum())) extNm = extProc->scrName();
            twiPr->setText(3,extNm);
            twiPr->setData(0,Qt::UserRole,qVariantFromValue(TIdent(pr->id(),pr->num(),pr->name(),(int)petProcedure)));
            twiPr->setFlags(twiPr->flags() | Qt::ItemIsTristate);
            twiPr->setCheckState(0,Qt::Unchecked);
          QTreeWidgetItem *lastWrk(NULL);
            foreach (TCarryWork *wrk,pr->works())
            {
              QTreeWidgetItem *twiWrk(new QTreeWidgetItem(twiPr,lastWrk));
                twiWrk->setText(0,wrk->scrName());
                twiWrk->setIcon(0,ICONPIX(PIX_LEVEL5));
                twiWrk->setText(1,"");
                twiWrk->setText(2,"");
                twiWrk->setText(3,"");
                twiWrk->setData(0,Qt::UserRole,qVariantFromValue(TIdent(wrk->id(),wrk->num(),wrk->name(),(int)petWork)));
                twiWrk->setFlags(twiWrk->flags() | Qt::ItemIsUserCheckable);
                twiWrk->setCheckState(0,Qt::Unchecked);
                lastWrk = twiWrk;
            }
            lastPr = twiPr;
        }
    }
    else
    {
        // ???
    }
}
//-----------------------------------------------------------------------------

QString TCarryPlan::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TCarryPlan::toHtml(bool fullinfo)
{
  QString sHtml("");

    sHtml += QString("Проект: <b>%1</b>").arg(fCarryTask ? fCarryTask->scrName() : "---");
    sHtml += QString("<br>Сосотояние: <b>%1</b>").arg(convertEnums::enumToStr(fCondition));
    if (fProblem!=copNone) sHtml += QString("<br>Проблемы: <b>%1</b>").arg(convertEnums::enumToStr(fProblem));
    sHtml += QString("<br>Исх. данные: <b>%1</b>").arg(fSourcesTitle.isEmpty() ? "---" : fSourcesTitle);
    //TAbstractAttachmentList fSources;  // Список исходных данных
    sHtml += QString("<br>Результаты: <b>%1</b>").arg(fResultsTitle.isEmpty() ? "" : fResultsTitle);
    //TAbstractAttachmentList fResults;  // Список результатов
  TEmployeeRole erl = firstTemplateRole();
    if (erl.type()!=eltNone)
    {
        sHtml += QString("<br>Типовое ДЛ: <b>%1</b>").arg(convertEnums::enumToStr(erl.type()));
      MODULE(Units);
        if (TUnit *un = modUnits->findUnit(erl.unitId())) sHtml += QString("<br>Внешнее подразд.: <b>%1</b>").arg(un->scrName());
    }
    sHtml += QString("<br>Возможные ДЛ: <b>%1</b>").arg(fPossibleEmployees.count());
  int n(0);
    foreach (TEmployee *empl,fPossibleEmployees)
        sHtml += QString("<br><b>%1. %2</b>").arg(++n).arg(empl->scrName());
    sHtml += QString("<br>Закрепленное ДЛ: <b>%1</b>").arg(fEmployee ? fEmployee->scrName() : "---");
    sHtml += QString("<br>Продолж. тип.: <b>%1</b>").arg(fTemplatePeriod);
    sHtml += QString("<br>Продолж. план.: <b>%1</b>").arg(fPlanPeriod);
    sHtml += QString("<br>Tн (план): <b>%1</b>").arg(fDtPlanBegin ? fDtPlanBegin->toString("MM.dd") : "---");
    sHtml += QString("<br>Tк (план): <b>%1</b>").arg(fDtPlanEnd ? fDtPlanEnd->toString("MM.dd") : "---");
    sHtml += QString("<br>Tн (факт): <b>%1</b>").arg(fDtRealBegin ? fDtRealBegin->toString("MM.dd (hh:mm)") : "---");
    sHtml += QString("<br>Tк (факт): <b>%1</b>").arg(fDtRealEnd ? fDtRealEnd->toString("MM.dd (hh:mm)") : "---");
    sHtml += QString("<br>Статусы: <b>%1</b>").arg(fStatuses.count());
    n = 0;
    foreach (TStatus st,fStatuses)
        sHtml += QString("<br><b>%1. %2</b>").arg(++n).arg(convertEnums::enumToStr(st));
    sHtml += QString("<br>Сохранено: <b>%1</b>").arg(fIsSaved ? "Да" : "Нет");
    sHtml += QString("<br>Можно изменять: <b>%1</b>").arg(fIsVolatile ? "Да" : "Нет");

    return sHtml;
}
//-----------------------------------------------------------------------------

// template, plan
bool TCarryPlan::toDB(QString param)
{
  bool isTemplate(param==QString("template"));
  EM_AddressBook *addrBook(NULL);
    try
    {
      EM_AddressBook &raddrBook = EM_AddressBook::Instance();
        addrBook = &raddrBook;
    }
    catch(CommonException::OpenDBException &e)
    {
        INSERT_ERROR(QString("EM_AddressBook::Instance(): %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
    }
    catch(CommonException::SQLException &e)
    {
        INSERT_ERROR(QString("EM_AddressBook::Instance(): %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
    }
    try
    {
      EM_YearPlanDic &planDic = EM_YearPlanDic::Instance();
      planDic.reinit();
if (!isTemplate)
PR1(4,"carryTask()->year(): %1",carryTask()->year());
      QSharedPointer<EM_YearPlan> shpYearPlan = planDic.by(isTemplate ? YEAR_PLAN_TYPE_CARRYTEMPLATE : carryTask()->year());
      EM_YearPlan *yearPlan(shpYearPlan.data()); // ??? корректно проверить NULL
      EM_BasePlanItem *root(NULL);
      EM_ProjectPlanItem *currentProject(NULL);
        if (yearPlan)
        {
            //root = yearPlan->getRoot();
            // удалить старый план, если он был в базе
            try
            {
                yearPlan->fromDB(); // чтение годового плана
                root = yearPlan->getRoot();

                if (isTemplate)
                {
                    for (EM_BasePlanItem::iterator planIter=root->begin(); planIter!=root->end(); ++planIter)
                        if (EM_PlanItem *emPlan = dynamic_cast<EM_PlanItem*>(*planIter)) // план
                            if (emPlan->getID()==id())
                            {
                                try
                                {
                                    yearPlan->rem(emPlan); // удаление шаблона
                                }
                                catch (CommonException::OpenDBException &e)
                                {
                                    INSERT_ERROR(QString("yearPlan->rem(emPlan). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
                                }
                                catch (CommonException::SQLException &e)
                                {
                                    INSERT_ERROR(QString("yearPlan->rem(emPlan). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
                                }
                                catch (CommonException::NullParamException &e)
                                {
                                    INSERT_ERROR(QString("yearPlan->rem(emPlan). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
                                }
                                break;
                            }
                }
                else
                {
                    for (EM_BasePlanItem::iterator projIter=root->begin(); projIter!=root->end(); ++projIter)
                        if (EM_ProjectPlanItem *emProj = dynamic_cast<EM_ProjectPlanItem*>(*projIter)) // проект
                            if (emProj->getID()==carryTask()->id())
                            {
                                for (EM_BasePlanItem::iterator planIter=emProj->begin(); planIter!=emProj->end(); ++planIter)
                                    if (EM_PlanItem *emPlan = dynamic_cast<EM_PlanItem*>(*planIter)) // план
                                        if (emPlan->getID()==id())
                                        {
                                            try
                                            {
                                                yearPlan->rem(emPlan); // удаление плана
                                            }
                                            catch (CommonException::OpenDBException &e)
                                            {
                                                INSERT_ERROR(QString("yearPlan->rem(emPlan). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
                                            }
                                            catch (CommonException::SQLException &e)
                                            {
                                                INSERT_ERROR(QString("yearPlan->rem(emPlan). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
                                            }
                                            catch (CommonException::NullParamException &e)
                                            {
                                                INSERT_ERROR(QString("yearPlan->rem(emPlan). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
                                            }
                                            break;
                                        }
                                currentProject = emProj;
                                break;
                            }
                }
            }
            catch (CommonException::OpenDBException &e)
            {
                INSERT_ERROR(QString("yearPlan->fromDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
            }
            catch (CommonException::SQLException &e)
            {
                INSERT_ERROR(QString("yearPlan->fromDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
            }
        }
        else if (isTemplate) // еще нет шаблонов
        {
            yearPlan = new EM_YearPlan(YEAR_PLAN_TYPE_CARRYTEMPLATE);
            yearPlan->setTitle("Циклограммы производственных планов");
            root = yearPlan->getRoot();
            shpYearPlan.reset(yearPlan);
        }
        else // ошибка
        {
            INSERT_ERROR(QString("Ошибка. Отсутствует план план %1 года").arg(carryTask()->year()),false); return false;
        }

        // Заполнение нового
      EM_PlanItem *planItem = (EM_PlanItem*)yearPlan->Instance(CARRY_PLAN);
        planItem->setState(condition());
        planItem->setProblem(problem());
        planItem->setNum(num());
        planItem->setShortTitle(name());
        planItem->setFullTitle(scrName());
        planItem->setTypalDuration(templatePeriod());
        planItem->setDuration(planPeriod());
        if (dtPlanBegin()) planItem->planBegin(*dtPlanBegin());
        if (dtPlanEnd()) planItem->planEnd(*dtPlanEnd());
        if (dtRealBegin()) planItem->realBegin(*dtRealBegin());
        if (dtRealEnd()) planItem->realEnd(*dtRealEnd());
        planItem->setSrcTitle(sourcesTitle());
        planItem->setResTitle(resultsTitle());
        if (employee())
        {
            if (EM_User *usr = addrBook->getUser(employee()->id())) planItem->setEmployee(usr); // external=false
        }
      TEmployeeRole rl = firstTemplateRole();
        planItem->setTemplEmployee(rl.type());
        planItem->setOshsItemID(rl.unitId() ? rl.unitId() : unitId());
        foreach (TStatus st,statuses()) planItem->addStatus(st);
        planItem->setDescr(descr());
        yearPlan->add(currentProject ? currentProject : root,planItem);
        foreach (TCarryProcedure *pr,fPlProcedures)
        {
          EM_StagePlanItem *prItem = (EM_StagePlanItem*)yearPlan->Instance(STAGE);
            prItem->setState(pr->condition());
            prItem->setProblem(pr->problem());
            prItem->setNum(pr->num());
            prItem->setShortTitle(pr->name());
            prItem->setFullTitle(pr->scrName());
            prItem->setTypalDuration(pr->templatePeriod());
            prItem->setDuration(pr->planPeriod());
            if (pr->dtPlanBegin()) prItem->planBegin(*pr->dtPlanBegin());
            if (pr->dtPlanEnd()) prItem->planEnd(*pr->dtPlanEnd());
            if (pr->dtRealBegin()) prItem->realBegin(*pr->dtRealBegin());
            if (pr->dtRealEnd()) prItem->realEnd(*pr->dtRealEnd());
            prItem->setSrcTitle(pr->sourcesTitle());
            prItem->setResTitle(pr->resultsTitle());
            if (pr->employee())
            {
                if (EM_User *usr = addrBook->getUser(pr->employee()->id())) prItem->setEmployee(usr); // external=false
            }
          TEmployeeRole rl = pr->firstTemplateRole();
            prItem->setTemplEmployee(rl.type());
            prItem->setOshsItemID(rl.unitId() ? rl.unitId() : pr->unitId());
            foreach (TStatus st,pr->statuses()) prItem->addStatus(st);
            prItem->setDescr(pr->descr());
            prItem->setExtProcNum(pr->externProcedureNum());
            yearPlan->add(planItem,prItem);
            foreach (TCarryWork *wrk,pr->works())
            {
              EM_TaskPlanItem *wrkItem = (EM_TaskPlanItem*)yearPlan->Instance(TASK);
                wrkItem->setState(wrk->condition());
                wrkItem->setProblem(wrk->problem());
                wrkItem->setNum(wrk->num());
                wrkItem->setShortTitle(wrk->name());
                wrkItem->setFullTitle(wrk->scrName());
                wrkItem->setTypalDuration(wrk->templatePeriod());
                wrkItem->setDuration(wrk->planPeriod());
                if (wrk->dtPlanBegin()) wrkItem->planBegin(*wrk->dtPlanBegin());
                if (wrk->dtPlanEnd()) wrkItem->planEnd(*wrk->dtPlanEnd());
                if (wrk->dtRealBegin()) wrkItem->realBegin(*wrk->dtRealBegin());
                if (wrk->dtRealEnd()) wrkItem->realEnd(*wrk->dtRealEnd());
                wrkItem->setSrcTitle(wrk->sourcesTitle());
                wrkItem->setResTitle(wrk->resultsTitle());
                if (wrk->employee())
                {
                    if (EM_User *usr = addrBook->getUser(wrk->employee()->id())) wrkItem->setEmployee(usr); // external=false
                }
              TEmployeeRole rl = wrk->firstTemplateRole();
                wrkItem->setTemplEmployee(rl.type());
                wrkItem->setOshsItemID(rl.unitId() ? rl.unitId() : wrk->unitId());
                foreach (TStatus st,wrk->statuses()) wrkItem->addStatus(st);
                wrkItem->setDescr(wrk->descr());
                wrkItem->setOptional(wrk->isOptional());
                wrkItem->setPresent(wrk->isPresent());
                wrkItem->setExtModuleType(wrk->externalModule());
                yearPlan->add(prItem,wrkItem);
            }
        }

        // Сохранение нового
        try
        {
            yearPlan->toDB();
            setId(planItem->getID());
            setSaved(true);
        }
        catch(CommonException::OpenDBException &e)
        {
            INSERT_ERROR(QString("yearPlan->toDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
        }
        catch(CommonException::SQLException &e)
        {
            INSERT_ERROR(QString("yearPlan->toDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
        }
    }
    catch (CommonException::OpenDBException &e)
    {
        INSERT_ERROR(QString("EM_YearPlanDic::Instance(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
    }
    catch (CommonException::SQLException &e)
    {
        INSERT_ERROR(QString("EM_YearPlanDic::Instance(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
    }
    return true;
}
//-----------------------------------------------------------------------------

bool TCarryPlan::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
