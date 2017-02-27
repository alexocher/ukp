#include <TAbstractPlan>

TAbstractPlan::TAbstractPlan(int id, int n, QString nm, TAbstractObject *parent) : TAbstractPlanElement(id,n,nm,parent)
{

}
//-----------------------------------------------------------------------------

TAbstractPlan::TAbstractPlan(const TAbstractPlan &ap) : TAbstractPlanElement(ap)
{

}
//-----------------------------------------------------------------------------

TAbstractPlan::~TAbstractPlan()
{

}
//-----------------------------------------------------------------------------

TAbstractPlan &TAbstractPlan::operator=(const TAbstractPlan &ap)
{
    if (&ap==this) return *this;
    TAbstractPlanElement::operator=(ap);
    return *this;
}
//-----------------------------------------------------------------------------

void TAbstractPlan::reset(bool thisonly)
{
    if (!thisonly) TAbstractPlanElement::reset(true);
}
//-----------------------------------------------------------------------------

QString TAbstractPlan::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TAbstractPlan::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TAbstractPlan::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TAbstractPlan::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
