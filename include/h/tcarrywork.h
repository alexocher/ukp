#ifndef TCARRYWORK_H
#define TCARRYWORK_H

#include <TAbstractPlanElement>
#include <defUkpPlan>

class UKPPLANSHARED_EXPORT TCarryProcedure;

class UKPPLANSHARED_EXPORT TCarryWork : public TAbstractPlanElement
{
private:
    TCarryProcedure    *fCarryProcedure; // Указатель на процедуру
    TExternalModuleType fExternalModule; // Тип внещнего модуля
    bool                fIsOptional,   // Признак необязательности (для шаблона)
                        fIsPresent;    // Признак присутствия в плане (для плана)

public:
    explicit TCarryWork(TCarryProcedure *crypr, TExternalModuleType emt=emtNone, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TCarryWork(const TCarryWork &wrk);
    ~TCarryWork();

    TCarryProcedure *carryProcedure() const; // Указатель на процедуру
    TExternalModuleType externalModule() const; // Тип внещнего модуля
    void setExternalModule(TExternalModuleType emt);
    bool isOptional() const;           // Признак необязательности (для шаблона)
    void setOptional(bool opt);
    bool isPresent() const;            // Признак присутствия в плане (для плана)
    void setPresent(bool pr);

    TCarryWork &operator=(const TCarryWork &wrk);

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

typedef UKPPLANSHARED_EXPORT QPtrList<TCarryWork*> TCarryWorkList;

#endif // TCARRYWORK_H
