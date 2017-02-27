#include <TGenPlan>

TGenPlan::TGenPlan(int year, int id, int n, QString nm, TAbstractObject *parent) : TAbstractPlan(id,n,nm,parent), fYear(year)
{

}
//-----------------------------------------------------------------------------

TGenPlan::~TGenPlan()
{

}
//-----------------------------------------------------------------------------

void TGenPlan::reset(bool thisonly)
{
    if (!thisonly) TAbstractPlan::reset(true);
    fYear = 0;
}
//-----------------------------------------------------------------------------

int TGenPlan::year() const
{
    return fYear;
}
//-----------------------------------------------------------------------------

void TGenPlan::setYear(int yr)
{
    fYear = yr;
}
//-----------------------------------------------------------------------------

QString TGenPlan::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TGenPlan::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TGenPlan::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TGenPlan::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
