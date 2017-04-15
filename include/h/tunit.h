#ifndef TUNIT_H
#define TUNIT_H

#include <QTreeWidgetItem>
#include <enums>
#include <TEmployee>
#include <defUkpCommon>

class TUnit;

typedef UKPCOMMONSHARED_EXPORT QPtrList<TUnit*> TUnitList;

// Подразделение
class UKPCOMMONSHARED_EXPORT TUnit : public TAbstractObject
{
private:
    TUnit();

    TUnitLevel          fLevel;        // Уровень подразделения
    TEmployee          *fChief;        // Начальник
    TEmployeeList       fPlEmployees;  // Список фактического персонала подразделения (без начальника)
    int                 fShtatEmployeeCount; // Штатное количество персонала
    TUnit              *fChiefUnit;    // Подразделение, которому подчинено данное
    TUnitList           fSubUnits;     // Список подчиненных подразделений

public:
    TUnit(int id, QString nm, QString shrtnm, TUnit *chfun = NULL, int n = 0, TAbstractObject *parent = NULL);
    TUnit(const TUnit &obj);
    ~TUnit();

    TUnitLevel level() const;          // Уровень подразделения
    void setLevel(TUnitLevel lvl);
    TEmployee *chief() const;          // Начальник
    void setChief(TEmployee *chf);
    TEmployeeList &employees() const;  // Список фактического персонала подразделения (без начальника)
    void insertEmployee(TEmployee *empl); // ... вставить
    void removeEmployee(TEmployee *empl); // ... удалить
    void clearEmployees();             // ... очистить
    TEmployee *findEmployee(int id, bool withsubunits=false); // ... поиск по id в текущем подразделени (withsubunits===false) или в текущем и во всех подчиненных (withsubunits===true)
    TEmployeeList *findEmployees(const TEmployeeRoleList &rls); // ... поиск с подходящими ролями
    int shtatEmployeeCount() const;    // Штатное количество персонала
    void setShtatEmployeeCount(int cnt);
    bool isInternalEmployee(int id);   // Проверка, входит ли ДЛ в текущее подразделение или в какое-либо подчиненное
    TUnit *chiefUnit() const;          // Подразделение, которому подчинено данное
    void setChiefUnit(TUnit *un);
    TUnitList &subUnits() const;       // Список подчиненных подразделений
    void insertSubUnit(TUnit *un);
    void removeSubUnit(TUnit *un);
    void clearSubUnits();
    TUnit *findSubUnit(int id);
    TUnit *findSubUnit(QString nm, bool onscrnm = false);

    TUnit &operator=(const TUnit &obj);

    bool isTopUnit(TUnit *un);         // Признак подчиненности подразделению un
    void reflectSubUnitsToTree(QTreeWidgetItem &twiself, bool withempl = false); // Отобразить на дереве

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo = true);
    bool toDB(QString param);
    bool fromDB(QString param);

};

#endif // TUNIT_H
