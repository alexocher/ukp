#include <defMacro>
#include <convertEnums>
#include <TModuleUnits>
#include <TCarryProcedure>
#include <TCarryWork>

TCarryWork::TCarryWork(TCarryProcedure *crypr, TExternalModuleType emt, int id, int n, QString nm, TAbstractObject *parent) : TAbstractPlanElement(id,n,nm,parent), fCarryProcedure(crypr), fExternalModule(emt), fIsOptional(false), fIsPresent(false)
{
    setScrName(QString("%1. %2").arg(fNum,2,10,QChar(' ')).arg(fName));
}
//-----------------------------------------------------------------------------

TCarryWork::TCarryWork(const TCarryWork &wrk) : TAbstractPlanElement(wrk), fCarryProcedure(wrk.fCarryProcedure), fExternalModule(wrk.fExternalModule), fIsOptional(wrk.fIsOptional), fIsPresent(wrk.fIsPresent)
{
    setScrName(QString("%1. %2").arg(fNum,2,10,QChar(' ')).arg(fName));
}
//-----------------------------------------------------------------------------

TCarryWork::~TCarryWork()
{

}
//-----------------------------------------------------------------------------

TCarryProcedure *TCarryWork::carryProcedure() const
{
    return fCarryProcedure;
}
//-----------------------------------------------------------------------------

TCarryWork &TCarryWork::operator=(const TCarryWork &wrk)
{
    if (&wrk==this) return *this;
    TAbstractPlanElement::operator=(wrk);
    fCarryProcedure = wrk.fCarryProcedure;
    fExternalModule = wrk.fExternalModule;
    fIsOptional = wrk.fIsOptional;
    fIsPresent = wrk.fIsPresent;
    return *this;
}
//-----------------------------------------------------------------------------

void TCarryWork::reset(bool thisonly)
{
    if (!thisonly) TAbstractPlanElement::reset(true);
    fCarryProcedure = NULL;
    fExternalModule = emtNone;
    fIsOptional = false;
    fIsPresent = false;
}
//-----------------------------------------------------------------------------

TExternalModuleType TCarryWork::externalModule() const
{
    return fExternalModule;
}
//-----------------------------------------------------------------------------

void TCarryWork::setExternalModule(TExternalModuleType emt)
{
    fExternalModule = emt;
}
//-----------------------------------------------------------------------------

bool TCarryWork::isOptional() const
{
    return fIsOptional;
}
//-----------------------------------------------------------------------------

void TCarryWork::setOptional(bool opt)
{
    fIsOptional = opt;
}
//-----------------------------------------------------------------------------

bool TCarryWork::isPresent() const
{
    return fIsPresent;
}
//-----------------------------------------------------------------------------

void TCarryWork::setPresent(bool pr)
{
    fIsPresent = pr;
}
//-----------------------------------------------------------------------------

QString TCarryWork::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TCarryWork::toHtml(bool fullinfo)
{
  QString sHtml("");

    sHtml += QString("Процедура: <b>%1</b>").arg(fCarryProcedure ? fCarryProcedure->scrName() : "---");
    sHtml += QString("<br>Тип вн. модуля: <b>%1</b>").arg(convertEnums::enumToStr(fExternalModule));
    sHtml += QString("<br>Обязательность: <b>%1</b>").arg(fIsOptional ? "Нет" : "Да");

    //fIsPresent;    // Признак присутствия в плане (для плана)

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

bool TCarryWork::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TCarryWork::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
