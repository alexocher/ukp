#include <TAppPlan>

TAppPlan::TAppPlan(QStringList &params, int n, QString nm) : TAbstractApp(params,n,nm), fModulePlans(new TModulePlans(pmtPlans))
{
    fPlModules.append(fModulePlans);
}
//-----------------------------------------------------------------------------

TAppPlan::~TAppPlan()
{

}
//-----------------------------------------------------------------------------

TAppPlan *TAppPlan::instance(QStringList &params, int n, QString nm)
{
    if (!TAbstractApp::project) TAbstractApp::project = new TAppPlan(params,n,nm);
    return (TAppPlan*)TAbstractApp::project;
}
//-----------------------------------------------------------------------------

void TAppPlan::freeInstance()
{
    if (TAbstractApp::project) delete this;
}
//-----------------------------------------------------------------------------

bool TAppPlan::init()
{
    if (!TAbstractApp::init()) return false;
    fModulePlans->init();
    return true;
}
//-----------------------------------------------------------------------------

bool TAppPlan::run()
{
    return true;
}
//-----------------------------------------------------------------------------

void TAppPlan::reset(bool thisonly)
{
    if (!thisonly) TAbstractApp::reset(true);
}
//-----------------------------------------------------------------------------

QString TAppPlan::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TAppPlan::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TAppPlan::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TAppPlan::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
