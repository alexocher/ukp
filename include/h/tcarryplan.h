#ifndef TCARRYPLAN_H
#define TCARRYPLAN_H

#include <TAbstractPlan>
#include <TCarryProcedure>
#include <defUkpPlan>

class UKPPLANSHARED_EXPORT TCarryTask;

class UKPPLANSHARED_EXPORT TCarryPlan : public TAbstractPlan
{
private:
    TCarryTask         *fCarryTask;    // Указатель на проект
    TCarryProcedureList fPlProcedures; // Список выполняемых процедур

public:
    explicit TCarryPlan(TCarryTask *crytsk, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TCarryPlan(const TCarryPlan &crp);
    ~TCarryPlan();

    TCarryTask *carryTask() const;     // Указатель на проект
    TCarryProcedureList &procedures() const; // Список выполняемых процедур
    void insertProcedure(TCarryProcedure *pr); // ... добавить
    void removeProcedure(TCarryProcedure *pr); // ... удалить
    void clearProcedures();            // ... очистить
    TCarryProcedure *findProcedure(int n); // ... поиск по номеру
    TCarryProcedure *findProcedure(QString nm, bool onscrnm=false); // ... поиск по наименованию (по умолчанию) или по отображаемому наименованию
    int nextProcedureNum();            // Получить следующий свободный номер процедуры

    TCarryPlan &operator=(const TCarryPlan &crp);

    bool fillFromTemplate(const TCarryPlan *tmpl); // Сформировать план на основе шаблона
    void recalcPeriod(bool ontempl);   // Пересчитать продолжительность (шаблонную или плановую) процедур и плана по продолжительности отдельных работ (realPeriod() - рассчитывается в каждом элементе)
    void reflectToTree(QTreeWidget &tw, bool astemplate=false); // Отобразить план на дереве

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param); // template, plan
    bool fromDB(QString param);
};

typedef UKPPLANSHARED_EXPORT QPtrList<TCarryPlan*> TCarryPlanList;

#endif // TCARRYPLAN_H
