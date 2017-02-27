#ifndef TAPPPLAN_H
#define TAPPPLAN_H

#include <TAbstractApp>
#include <TModulePlans>
#include <defUkpPlan>

class UKPPLANSHARED_EXPORT TAppPlan : public TAbstractApp
{
private:
    TAppPlan(const TAppPlan &obj);
    TAppPlan &operator=(const TAppPlan &obj);

protected:
    TModulePlans       *fModulePlans;  // Модуль работы с планами

    TAppPlan(QStringList &params, int n=0, QString nm="");
    ~TAppPlan();

public:
    static TAppPlan *instance(QStringList &params, int n=0, QString nm="");

public: // TAbstractApp interface
    virtual void freeInstance();
    virtual bool init();
    virtual bool run();

public: // TAbstractObject interface
    virtual void reset(bool thisonly);
    virtual QString toStr();
    virtual QString toHtml(bool fullinfo=true);
    virtual bool toDB(QString param);
    virtual bool fromDB(QString param);

};

#endif // TAPPPLAN_H
