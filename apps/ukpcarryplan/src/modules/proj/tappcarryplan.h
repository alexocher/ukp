#ifndef TAPPCARRYPLAN_H
#define TAPPCARRYPLAN_H

#include <TAppPlan>
#include <QTime>

class TAppCarryPlan : public TAppPlan
{
private:
    TAppCarryPlan(const TAppCarryPlan &obj);
    TAppCarryPlan &operator=(const TAppCarryPlan &obj);

    QTime               fWorkDayBegin; // Начало рабочего дня

protected:
    TAppCarryPlan(QStringList &params, QTime workdaybegin = QTime(9, 0, 0), int n = 0, QString nm = "");
    ~TAppCarryPlan();

public:
    static TAppCarryPlan *instance(QStringList &params, QTime workdaybegin = QTime(9, 0, 0), int n = 0, QString nm = "");

    const QTime &workDayBegin() const; // Начало рабочего дня

public: // TAbstractApp interface
    virtual void freeInstance();
    virtual bool init();
    virtual bool run();

public: // TAbstractObject interface
    virtual void reset(bool thisonly);
    virtual QString toStr();
    virtual QString toHtml(bool fullinfo = true);
    virtual bool toDB(QString param);
    virtual bool fromDB(QString param);

};

#endif // TAPPCARRYPLAN_H
