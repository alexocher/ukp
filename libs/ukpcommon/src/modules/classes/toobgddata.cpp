#include <TOobgdData>

TOobgdData::TOobgdData(TAttachmentType t, int id, int n, QString nm, TAbstractObject *parent) : TAbstractAttachment(t,id,n,nm,parent)
{

}
//-----------------------------------------------------------------------------

TOobgdData::TOobgdData(const TOobgdData &dt) : TAbstractAttachment(dt)
{

}
//-----------------------------------------------------------------------------

TOobgdData::~TOobgdData()
{

}
//-----------------------------------------------------------------------------

TOobgdData &TOobgdData::operator=(const TOobgdData &dt)
{
    if (&dt==this) return *this;
    TAbstractAttachment::operator=(dt);
    return *this;
}
//-----------------------------------------------------------------------------

void TOobgdData::reset(bool thisonly)
{
    if (!thisonly) TAbstractAttachment::reset(true);
}
//-----------------------------------------------------------------------------

QString TOobgdData::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TOobgdData::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TOobgdData::toDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------

bool TOobgdData::fromDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------
