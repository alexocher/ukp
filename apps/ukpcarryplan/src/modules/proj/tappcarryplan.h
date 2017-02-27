#ifndef TAPPCARRYPLAN_H
#define TAPPCARRYPLAN_H

#include <TAppPlan>

class TAppCarryPlan : public TAppPlan
{
private:
    TAppCarryPlan(const TAppCarryPlan &obj);
    TAppCarryPlan &operator=(const TAppCarryPlan &obj);

protected:
    TAppCarryPlan(QStringList &params, int n=0, QString nm="");
    ~TAppCarryPlan();

public:
    static TAppCarryPlan *instance(QStringList &params, int n=0, QString nm="");

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

#endif // TAPPCARRYPLAN_H
