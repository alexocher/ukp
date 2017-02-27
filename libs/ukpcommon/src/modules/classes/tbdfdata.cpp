#include <TBdfData>

TBdfData::TBdfData(TAttachmentType t, int id, int n, QString nm, TAbstractObject *parent) : TAbstractAttachment(t,id,n,nm,parent)
{

}
//-----------------------------------------------------------------------------

TBdfData::TBdfData(const TBdfData &dt) : TAbstractAttachment(dt)
{

}
//-----------------------------------------------------------------------------

TBdfData::~TBdfData()
{

}
//-----------------------------------------------------------------------------

TBdfData &TBdfData::operator=(const TBdfData &dt)
{
    if (&dt==this) return *this;
    TAbstractAttachment::operator=(dt);
    return *this;
}
//-----------------------------------------------------------------------------

void TBdfData::reset(bool thisonly)
{
    if (!thisonly) TAbstractAttachment::reset(true);
}
//-----------------------------------------------------------------------------

QString TBdfData::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TBdfData::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TBdfData::toDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------

bool TBdfData::fromDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------
