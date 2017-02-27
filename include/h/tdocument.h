#ifndef TDOCUMENT_H
#define TDOCUMENT_H

#include <TAbstractAttachment>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT TDocument : public TAbstractAttachment
{
private:
    TDocumentType       fDocumentType; // Тип документа

public:
    TDocument(TDocumentType dtp, TAttachmentType t, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TDocument(const TDocument &att);
    ~TDocument();

    TDocumentType documentType() const; // Тип документа
    void setDocumentType(TDocumentType dtp);

    TDocument &operator=(const TDocument &att);

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TDocument*> TDocumentList;

#endif // TDOCUMENT_H
