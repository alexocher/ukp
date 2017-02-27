#ifndef TCARRYPROCEDURE_H
#define TCARRYPROCEDURE_H

#include <TCarryWork>
#include <defUkpPlan>

class UKPPLANSHARED_EXPORT TCarryPlan;

// Шаблон внешней процедуры
struct UKPPLANSHARED_EXPORT TExternProcedureTemplate
{
private:
    int                 fNum;          // Номер
    QString             fName;         // Наименование
    TExternalModuleTypeList fExternalModules; // Список типов ВПМ

    TExternProcedureTemplate();
    TExternProcedureTemplate(const TExternProcedureTemplate &epr);
    TExternProcedureTemplate &operator=(const TExternProcedureTemplate &epr);

public:
    TExternProcedureTemplate(int n, const QString &nm, const TExternalModuleTypeList &extmods);

    int num() const;                   // Номер
    QString &name() const;             // Наименование
    TExternalModuleTypeList &externalModules() const; // Список типов ВПМ

    QString scrName();                 // Отображаемое наименование

};

typedef UKPPLANSHARED_EXPORT QPtrList<TExternProcedureTemplate*> TExternProcedureTemplateList;

class UKPPLANSHARED_EXPORT TCarryProcedure : public TAbstractPlanElement
{
private:
    TCarryPlan         *fCarryPlan;    // Указатель на план
    int                 fExternProcedureNum; // Номер внешней процедуры
    TCarryWorkList      fPlWorks;      // Список выполняемых работ

public:
    explicit TCarryProcedure(TCarryPlan *crypl, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TCarryProcedure(const TCarryProcedure &crp);
    ~TCarryProcedure();

    TCarryPlan *carryPlan() const;     // Указатель на план
    bool isExtern() const;             // Признак внешней процедуры (работы с ВПМ)
    int externProcedureNum() const;    // Номер внешней процедуры
    void setExternProcedureNum(int n);
    TCarryWorkList &works() const;     // Список выполняемых работ
    void insertWork(TCarryWork *wrk);  // ... добавить
    void removeWork(TCarryWork *wrk);  // ... удалить
    void clearWorks();                 // ... очистить
    TCarryWork *findWork(int n);       // ... поиск по номеру
    TCarryWork *findWork(QString nm, bool onscrnm=false); // ... поиск по наименованию (по умолчанию) или по отображаемому наименованию
    int nextWorkNum();                 // Получить следующий свободный номер работы

    TCarryProcedure &operator=(const TCarryProcedure &crp);

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

typedef UKPPLANSHARED_EXPORT QPtrList<TCarryProcedure*> TCarryProcedureList;

#endif // TCARRYPROCEDURE_H
