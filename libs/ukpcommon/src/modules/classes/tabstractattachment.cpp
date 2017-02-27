#include <TAbstractAttachment>

TAbstractAttachment::TAbstractAttachment(TAttachmentType t, int id, int n, QString nm, TAbstractObject *parent) : TAbstractObject(id,n,nm,parent), fType(t)
{

}
//-----------------------------------------------------------------------------

TAbstractAttachment::TAbstractAttachment(const TAbstractAttachment &att) : TAbstractObject(att), fType(att.fType)
{

}
//-----------------------------------------------------------------------------

TAbstractAttachment::~TAbstractAttachment()
{

}
//-----------------------------------------------------------------------------

TAttachmentType TAbstractAttachment::type() const
{
    return fType;
}
//-----------------------------------------------------------------------------

TAbstractAttachment &TAbstractAttachment::operator=(const TAbstractAttachment &att)
{
    if (&att==this) return *this;
    fType = att.fType;
    return *this;
}
//-----------------------------------------------------------------------------

void TAbstractAttachment::reset(bool thisonly)
{
    if (!thisonly) TAbstractObject::reset(true);
    fType = attNone;
}
//-----------------------------------------------------------------------------

QString TAbstractAttachment::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TAbstractAttachment::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TAbstractAttachment::toDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------

bool TAbstractAttachment::fromDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------
