#include <TAppCarryPlan>

TAppCarryPlan::TAppCarryPlan(QStringList &params, int n, QString nm) : TAppPlan(params, n, nm)
{

}
//-----------------------------------------------------------------------------

TAppCarryPlan::~TAppCarryPlan()
{

}
//-----------------------------------------------------------------------------

TAppCarryPlan *TAppCarryPlan::instance(QStringList &params, int n, QString nm)
{
    if (!TAbstractApp::project) TAbstractApp::project = new TAppCarryPlan(params, n, nm);
    return (TAppCarryPlan*)TAbstractApp::project;
}

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
