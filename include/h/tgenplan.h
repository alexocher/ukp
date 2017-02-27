#ifndef TGENPLAN_H
#define TGENPLAN_H

#include <TAbstractPlan>
#include <defUkpPlan>

class UKPPLANSHARED_EXPORT TGenPlan : public TAbstractPlan
{
private:
    int                 fYear;         // Год

    TGenPlan(const TGenPlan &crp);
    TGenPlan &operator=(const TGenPlan &crp);

public:
    explicit TGenPlan(int year, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    ~TGenPlan();

    int year() const;                  // Год
    void setYear(int yr);

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

#endif // TGENPLAN_H
