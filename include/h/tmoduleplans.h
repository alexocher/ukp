#ifndef TMODULEPLANS_H
#define TMODULEPLANS_H

#include <QTreeWidget>
#include <QComboBox>
#include <TAbstractModule>
#include <TAbstractPlan>
#include <TWorkCalendar>
#include <TCarryTask>
#include <TGenPlan>
#include <defUkpPlan>

class UKPPLANSHARED_EXPORT TModulePlans : public TAbstractModule
{
private:
    TModulePlans(const TModulePlans &obj);
    TModulePlans &operator=(const TModulePlans &obj);

    TExternProcedureTemplateList fPlExternProcedureTemplates; // Список шаблонов внешних процедур
    TAbstractPlanList   fPlPlanTemplates; // Список шаблонов планов (циклограмм)
    TCompanyCalendar   *fCompanyCalendar; // Календарь предприятия
    TCarryTaskList      fPlCarryTasks; // Список проектов
    TCarryTaskPackage  *fCurrentCarryTaskPackage; // Текущий пакет проектов
    TGenPlan           *fCurrentGenPlan; // Текущий годовой план

public :
    explicit TModulePlans(TProjModuleType t, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    ~TModulePlans();

    TExternProcedureTemplateList &externProcedureTemplates() const; // Список шаблонов внешних процедур
    TExternProcedureTemplate *findExternProcedureTemplate(int n); // ... найти по по номеру
    TExternProcedureTemplate *findExternProcedureTemplate(QString nm, bool onnm=false); // ... найти по отображаемому наименованию (по умолчанию) или по наименованию
    TAbstractPlanList &planTemplates() const; // Список шаблонов планов (циклограмм)
    void insertPlanTemplate(TAbstractPlan *pl); // ... вставить
    void removePlanTemplate(TAbstractPlan *pl); // ... удалить
    void clearPlanTemplates();         // ... очистить
    TAbstractPlan *findPlanTemplate(int id, bool onnum=false); // ... найти по id (по умолчанию) или по номеру
    TAbstractPlan *findPlanTemplate(QString nm, bool onscrnm=false); // ... найти по наименованию (по умолчанию) или по отображаемому наименованию
    TCompanyCalendar *companyCalendar() const; // Календарь предприятия
    void reloadCompanyCalendar(int year); // перечитать для указанного года
    TCarryTaskList &carryTasks() const; // Список проектов
    void insertTask(TCarryTask *tsk);  // ... вставить
    void removeTask(TCarryTask *tsk);  // ... удалить
    void clearTasks();                 // ... очистить
    TCarryTask *findTask(int id, bool onnum=false); // ... найти по id (по умолчанию) или по номеру
    TCarryTask *findTask(QString nm, bool onscrnm=false); // ... найти по наименованию (по умолчанию) или по отображаемому наименованию
    TCarryTask *findTask(TProductionType tp, QString nm); // ... найти по типу продукции и наименованию
    TCarryTaskPackage *currentCarryTaskPackage() const; // Текущий пакет проектов
    void setCurrentCarryTaskPackage(TCarryTaskPackage *pkg);
    TGenPlan *currentGenPlan() const;  // Текущий годовой план
    void setCurrentGenPlan(TGenPlan *pl);

    void testPlanData();
    void setElementIdsForPlan();       // Назначить id для элементов c id==0 (для выполнения createCarryPlans). После записи в БД id изменятся
    bool createCarryPlans(TCarryTaskList &tasks, QString &errs); // Основная функция планирования

    TCarryTask *findTask(const TCarryTaskList &tsks, QString nm, bool onscrnm=false); // ... найти по наименованию (по умолчанию) или по отображаемому наименованию
    TAbstractPlanElement *findCarryElement(const TCarryTaskList &tasks, const QStringList &scrnms); // Найти элемент (последний в scrnms) в иерархии по отображаемому наименованию
    void fillTasks(QList<int> tskids); // Заполнить список проектов с id из списка
    void reflectTemplatesToCb(const TAbstractPlanList &plans, QComboBox &cb); // Отобразить список шаблонов в списке
    void reflectCarryTasksToTree(const TCarryTaskList &tsks, QTreeWidget &tw, bool onlycheckedplans=false, int rowheight=0); // Отобразить на дереве список проектов
    void reflectExternProceduresToCb(const TExternProcedureTemplateList &extprocs, QComboBox &cb, bool withempty=false); // Отобразить в списке список внешних процедур

public: // TAbstractModule interface
    bool init();

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param); // ExternProcedures, Temlates, CompanyCalendar, CarryTasks
    bool fromDB(QString param); // ExternProcedures, Temlates, CompanyCalendar, CarryTasks
};

#endif // TMODULEPLANS_H
