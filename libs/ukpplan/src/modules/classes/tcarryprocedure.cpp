#include <defMacro>
#include <convertEnums>
#include <TModuleUnits>
#include <TModulePlans>
#include <TCarryProcedure>

TExternProcedureTemplate::TExternProcedureTemplate(int n, const QString &nm, const TExternalModuleTypeList &extmods) : fNum(n), fName(nm), fExternalModules(extmods)
{
}
//-----------------------------------------------------------------------------

int TExternProcedureTemplate::num() const
{
    return fNum;
}
//-----------------------------------------------------------------------------

QString &TExternProcedureTemplate::name() const
{
    return const_cast<QString&>(fName);
}
//-----------------------------------------------------------------------------

TExternalModuleTypeList &TExternProcedureTemplate::externalModules() const
{
    return const_cast<TExternalModuleTypeList&>(fExternalModules);
}
//-----------------------------------------------------------------------------

QString TExternProcedureTemplate::scrName()
{
    return QString("ВП%1. %2").arg(fNum).arg(fName);
}
//-----------------------------------------------------------------------------

//*****************************************************************************

TCarryProcedure::TCarryProcedure(TCarryPlan *crypl, int id, int n, QString nm, TAbstractObject *parent) : TAbstractPlanElement(id,n,nm,parent), fCarryPlan(crypl), fExternProcedureNum(0)
{
    fPlWorks.setAutoDelete(true);
    setScrName(QString("%1. %2").arg(fNum,2,10,QChar(' ')).arg(fName));
}
//-----------------------------------------------------------------------------

TCarryProcedure::TCarryProcedure(const TCarryProcedure &crp) : TAbstractPlanElement(crp), fCarryPlan(crp.fCarryPlan), fExternProcedureNum(crp.fExternProcedureNum)
{
    fPlWorks.setAutoDelete(true);
    setScrName(QString("%1. %2").arg(fNum,2,10,QChar(' ')).arg(fName));
    foreach (TCarryWork *wrk,crp.fPlWorks) fPlWorks.append(new TCarryWork(*wrk));
}
//-----------------------------------------------------------------------------

TCarryProcedure::~TCarryProcedure()
{
}
//-----------------------------------------------------------------------------

TCarryPlan *TCarryProcedure::carryPlan() const
{
    return fCarryPlan;
}
//-----------------------------------------------------------------------------

TCarryProcedure &TCarryProcedure::operator=(const TCarryProcedure &crp)
{
    if (&crp==this) return *this;
    TAbstractPlanElement::operator=(crp);
    fCarryPlan = crp.fCarryPlan;
    fExternProcedureNum = crp.fExternProcedureNum;
    fPlWorks.clear();
    foreach (TCarryWork *wrk,crp.fPlWorks) fPlWorks.append(new TCarryWork(*wrk));
    return *this;
}
//-----------------------------------------------------------------------------

void TCarryProcedure::reset(bool thisonly)
{
    if (!thisonly) TAbstractPlanElement::reset(true);
    fCarryPlan = NULL;
    fExternProcedureNum = 0;
    fPlWorks.clear();
}
//-----------------------------------------------------------------------------

bool TCarryProcedure::isExtern() const
{
    return fExternProcedureNum;
}
//-----------------------------------------------------------------------------

int TCarryProcedure::externProcedureNum() const
{
    return fExternProcedureNum;
}
//-----------------------------------------------------------------------------

void TCarryProcedure::setExternProcedureNum(int n)
{
    fExternProcedureNum = n;
}
//-----------------------------------------------------------------------------

TCarryWorkList &TCarryProcedure::works() const
{
    return const_cast<TCarryWorkList&>(fPlWorks);
}
//-----------------------------------------------------------------------------

void TCarryProcedure::insertWork(TCarryWork *wrk)
{
    if (wrk && fPlWorks.indexOf(wrk)==-1) fPlWorks.append(wrk);
}
//-----------------------------------------------------------------------------

void TCarryProcedure::removeWork(TCarryWork *wrk)
{
    if (wrk) fPlWorks.removeOne(wrk);
}
//-----------------------------------------------------------------------------

void TCarryProcedure::clearWorks()
{
    fPlWorks.clear();
}
//-----------------------------------------------------------------------------

TCarryWork *TCarryProcedure::findWork(int n)
{
    foreach (TCarryWork *wrk,fPlWorks)
        if (wrk->num()==n) return wrk;
    return NULL;
}
//-----------------------------------------------------------------------------

TCarryWork *TCarryProcedure::findWork(QString nm, bool onscrnm)
{
    foreach (TCarryWork *wrk,fPlWorks)
        if (onscrnm)
        {
            if (wrk->scrName()==nm) return wrk;
        }
        else if (wrk->name()==nm) return wrk;
    return NULL;
}
//-----------------------------------------------------------------------------

int TCarryProcedure::nextWorkNum()
{
  int nextN(0);
    foreach (TCarryWork *wrk,fPlWorks)
        if (wrk->num()>nextN) nextN = wrk->num();
    return ++nextN;
}
//-----------------------------------------------------------------------------

QString TCarryProcedure::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TCarryProcedure::toHtml(bool fullinfo)
{
  QString sHtml("");

  MODULE(Plans);
    sHtml += QString("План: <b>%1</b>").arg(fCarryPlan ? fCarryPlan->scrName() : "---");
  TExternProcedureTemplate *ept = modPlans->findExternProcedureTemplate(fExternProcedureNum);
    sHtml += QString("<br>Внешняя процедура: <b>%1</b>").arg(ept ? ept->scrName() : "---");

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
    sHtml += QString("<br>Tн (план): <b>%1</b>").arg(fDtPlanBegin ? fDtPlanBegin->toString("dd.MM.yy") : "---");
    sHtml += QString("<br>Tк (план): <b>%1</b>").arg(fDtPlanEnd ? fDtPlanEnd->toString("dd.MM.yy") : "---");
    sHtml += QString("<br>Tн (факт): <b>%1</b>").arg(fDtRealBegin ? fDtRealBegin->toString("dd.MM.yy (hh:mm)") : "---");
    sHtml += QString("<br>Tк (факт): <b>%1</b>").arg(fDtRealEnd ? fDtRealEnd->toString("dd.MM.yy (hh:mm)") : "---");
    sHtml += QString("<br>Статусы: <b>%1</b>").arg(fStatuses.count());
    n = 0;
    foreach (TStatus st,fStatuses)
        sHtml += QString("<br><b>%1. %2</b>").arg(++n).arg(convertEnums::enumToStr(st));
    sHtml += QString("<br>Сохранено: <b>%1</b>").arg(fIsSaved ? "Да" : "Нет");
    sHtml += QString("<br>Можно изменять: <b>%1</b>").arg(fIsVolatile ? "Да" : "Нет");

    return sHtml;
}
//-----------------------------------------------------------------------------

bool TCarryProcedure::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TCarryProcedure::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
