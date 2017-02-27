#ifndef TABSTRACTATTACHMENT_H
#define TABSTRACTATTACHMENT_H

#include <enums>
#include <TAbstractObject>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT TAbstractAttachment : public TAbstractObject
{
private:
    TAttachmentType     fType;         // Тип вложения

public:
    explicit TAbstractAttachment(TAttachmentType t, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TAbstractAttachment(const TAbstractAttachment &att);
    ~TAbstractAttachment();

    TAttachmentType type() const;      // Тип вложения

    TAbstractAttachment &operator=(const TAbstractAttachment &att);

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TAbstractAttachment*> TAbstractAttachmentList;

#endif // TABSTRACTATTACHMENT_H
