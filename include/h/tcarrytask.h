#ifndef TCARRYTASK_H
#define TCARRYTASK_H

#include <TCarryPlan>
#include <defUkpPlan>

class UKPPLANSHARED_EXPORT TCarryPlan;

// Проект (задача) управления (выпуска продукции)
class UKPPLANSHARED_EXPORT TCarryTask : public TAbstractPlanElement
{
private:
    int                 fYear,         // Год годового плана
                        fPriority;     // Приоритет
    TProductionType     fProductionType; // Тип продукции
    TCarryPlan         *fOrdPlan,      // План разработки ОРД
                       *fCarryPlan;    // План проекта

    TCarryTask(const TCarryTask &crt);
    TCarryTask &operator=(const TCarryTask &crt);

public:
    TCarryTask(int year, TProductionType prtp, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    ~TCarryTask();

    int year() const;                  // Год годового плана
    //void setYear(int year);
    int priority() const;              // Приоритет
    void setPriority(int pr);
    TProductionType productionType() const; // Тип продукции
    void setProductionType(TProductionType prtp);
    TCarryPlan *ordPlan() const;       // План разработки ОРД
    void setOrdPlan(TCarryPlan *pl);
    TCarryPlan *carryPlan() const;     // План проекта
    void setCarryPlan(TCarryPlan *pl);

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

typedef UKPPLANSHARED_EXPORT QPtrList<TCarryTask*> TCarryTaskList;

// Пакет проектов
class UKPPLANSHARED_EXPORT TCarryTaskPackage : public TAbstractObject
{
private:
    TCarryTaskList   fPlTasks;         // Список проектов

    TCarryTaskPackage(const TCarryTaskPackage &cwp);
    TCarryTaskPackage &operator=(const TCarryTaskPackage &cwp);

public:
    explicit TCarryTaskPackage(int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    ~TCarryTaskPackage();

    TCarryTaskList &tasks() const;     // Список проектов
    void insertTask(TCarryTask *tsk);  // ... добавить
    void removeTask(TCarryTask *tsk);  // ... удалить
    void clearTasks();                 // ... очистить
    TCarryTask *findTask(int id, bool onnum=false); // ... поиск по id (по умолчанию) или по номеру
    TCarryTask *findTask(QString nm, bool onscrnm=false); // ... поиск по наименованию (по умолчанию) или по отображаемому наименованию

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

#endif // TCARRYTASK_H
