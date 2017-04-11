#include <TAppCarryPlan>

TAppCarryPlan::TAppCarryPlan(QStringList &params, QTime workdaybegin, int n, QString nm) : TAppPlan(params, n, nm), fWorkDayBegin(workdaybegin)
{

}
//-----------------------------------------------------------------------------

TAppCarryPlan::~TAppCarryPlan()
{

}
//-----------------------------------------------------------------------------

TAppCarryPlan *TAppCarryPlan::instance(QStringList &params, QTime workdaybegin, int n, QString nm)
{
    if (!TAbstractApp::project) TAbstractApp::project = new TAppCarryPlan(params, workdaybegin, n, nm);
    return (TAppCarryPlan*)TAbstractApp::project;
}

const QTime &TAppCarryPlan::workDayBegin() const
{
    return fWorkDayBegin;
}
//-----------------------------------------------------------------------------

void TAppCarryPlan::freeInstance()
{
    if (TAbstractApp::project) delete this;
}
//-----------------------------------------------------------------------------

bool TAppCarryPlan::init()
{
    if (!TAppPlan::init()) return false;
    return true;
}
//-----------------------------------------------------------------------------

bool TAppCarryPlan::run()
{
    return true;
}
//-----------------------------------------------------------------------------

void TAppCarryPlan::reset(bool thisonly)
{
    if (!thisonly) TAbstractApp::reset(true);
}
//-----------------------------------------------------------------------------

QString TAppCarryPlan::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TAppCarryPlan::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TAppCarryPlan::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TAppCarryPlan::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
