#ifndef TABSTRACTMODULE_H
#define TABSTRACTMODULE_H

#include <enums>
#include <TAbstractObject>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT TAbstractModule : public TAbstractObject
{
    Q_OBJECT
private:
    TAbstractModule(const TAbstractModule &obj);
    TAbstractModule &operator=(const TAbstractModule &obj);

protected :
    TProjModuleType     fType;         // Тип модуля

public :
    TAbstractModule(TProjModuleType t, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    ~TAbstractModule();

    TProjModuleType type() const;      // Тип модуля

    virtual bool init()=0;             // Инициализация модуля

public: // TAbstractObject interface
    virtual void reset(bool thisonly)=0;
    virtual QString toStr()=0;
    virtual QString toHtml(bool fullinfo=true)=0;
    virtual bool toDB(QString param)=0;
    virtual bool fromDB(QString param)=0;

};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TAbstractModule*> TModuleList;

#endif // TABSTRACTMODULE_H
