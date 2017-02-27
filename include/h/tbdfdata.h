#ifndef TBDFDATA_H
#define TBDFDATA_H

#include <TAbstractAttachment>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT TBdfData : public TAbstractAttachment
{
private:

public:
    TBdfData(TAttachmentType t, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TBdfData(const TBdfData &att);
    ~TBdfData();


    TBdfData &operator=(const TBdfData &att);

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TBdfData*> TBdfDataList;

#endif // TBDFDATA_H
