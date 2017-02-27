#include <TDocument>

TDocument::TDocument(TDocumentType dtp, TAttachmentType t, int id, int n, QString nm, TAbstractObject *parent) : TAbstractAttachment(t,id,n,nm,parent), fDocumentType(dtp)
{

}
//-----------------------------------------------------------------------------

TDocument::TDocument(const TDocument &dt) : TAbstractAttachment(dt), fDocumentType(dt.fDocumentType)
{

}
//-----------------------------------------------------------------------------

TDocument::~TDocument()
{

}
//-----------------------------------------------------------------------------

TDocument &TDocument::operator=(const TDocument &dt)
{
    if (&dt==this) return *this;
    TAbstractAttachment::operator=(dt);
    fDocumentType = dt.fDocumentType;
    return *this;
}
//-----------------------------------------------------------------------------

void TDocument::reset(bool thisonly)
{
    if (!thisonly) TAbstractAttachment::reset(true);
    fDocumentType = docNone;
}
//-----------------------------------------------------------------------------

TDocumentType TDocument::documentType() const
{
    return fDocumentType;
}
//-----------------------------------------------------------------------------

void TDocument::setDocumentType(TDocumentType dtp)
{
    fDocumentType = dtp;
}
//-----------------------------------------------------------------------------

QString TDocument::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TDocument::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TDocument::toDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------

bool TDocument::fromDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------
