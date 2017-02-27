#ifndef TABSTRACTPLAN_H
#define TABSTRACTPLAN_H

#include <QTreeWidget>
#include <TAbstractPlanElement>
#include <defUkpPlan>

class UKPPLANSHARED_EXPORT TAbstractPlan : public TAbstractPlanElement
{
protected:

public:
    explicit TAbstractPlan(int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TAbstractPlan(const TAbstractPlan &ap);
    ~TAbstractPlan();

    TAbstractPlan &operator=(const TAbstractPlan &ap);

    virtual void reflectToTree(QTreeWidget &tw, bool astemplate=false)=0; // Отобразить план на дереве

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

typedef UKPPLANSHARED_EXPORT QPtrList<TAbstractPlan*> TAbstractPlanList;

#endif // TABSTRACTPLAN_H
