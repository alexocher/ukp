#ifndef TOOBGDDATA_H
#define TOOBGDDATA_H

#include <TAbstractAttachment>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT TOobgdData : public TAbstractAttachment
{
private:

public:
    TOobgdData(TAttachmentType t, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TOobgdData(const TOobgdData &att);
    ~TOobgdData();


    TOobgdData &operator=(const TOobgdData &att);

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TOobgdData*> TOobgdDataList;

#endif // TOOBGDDATA_H
