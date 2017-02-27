
#include <TAbstractModule>

TAbstractModule::TAbstractModule(TProjModuleType t, int id, int n, QString nm, TAbstractObject *parent) : TAbstractObject(id,n,nm,parent), fType(t)
{
}
//-----------------------------------------------------------------------------

TAbstractModule::~TAbstractModule()
{
}
//-----------------------------------------------------------------------------

void TAbstractModule::reset(bool thisonly)
{
    if (!thisonly) TAbstractObject::reset(true);
    fType = pmtNone;
}
//-----------------------------------------------------------------------------

TProjModuleType TAbstractModule::type() const
{
    return fType;
}
//-----------------------------------------------------------------------------

bool TAbstractModule::init()
{
    return true;
}
//-----------------------------------------------------------------------------

QString TAbstractModule::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TAbstractModule::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TAbstractModule::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TAbstractModule::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
