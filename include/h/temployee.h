#ifndef TEMPLOYEE_H
#define TEMPLOYEE_H

#include <enums>
#include <TAbstractObject>
#include <TWorkCalendar>
#include <defUkpCommon>

// Роль должностного лица
class UKPCOMMONSHARED_EXPORT TEmployeeRole
{
private:
    TEmployeeType       fType;         // Тип должностного лица
    bool                fIsExtern;     // Признак внешнего ДЛ (для текущей инстанции)
    int                 fUnitId;       // id подразделения для внешнего ДЛ
    TProductionTypeList fProductionTypes; // Список типов продукции, к работе с которой допущено ДЛ

public:
    explicit TEmployeeRole(TEmployeeType empt=eltNone);
    TEmployeeRole(const TEmployeeRole &rl);
    ~TEmployeeRole();

    TEmployeeType type() const;        // Тип должностного лица
    void setType(TEmployeeType t);
    bool isExtern() const;             // Признак внешнего ДЛ (для текущей инстанции)
    void setExtern(bool ext);
    int unitId() const;                // id подразделения для внешнего ДЛ
    void setUnitId(int id);
    TProductionTypeList &productionTypes() const; // Список типов продукции, к работе с которой допущено ДЛ
    void setProductionTypes(const TProductionTypeList &prtps);

    TEmployeeRole &operator=(const TEmployeeRole &rl);
    bool isMatch(TEmployeeRole &other, bool isfull=false); // Проверка соответствия (полное (true) или только значимых полей (false) - по умолчанию)

};

typedef UKPCOMMONSHARED_EXPORT QList<TEmployeeRole> TEmployeeRoleList;

class TUnit;

// Должностное лицо
class UKPCOMMONSHARED_EXPORT TEmployee : public TAbstractObject
{
private:
    TUnit              *fUnit;         // Подразделение, в котором состоит ДЛ
    TEmployeeRole       fRole;         // Роль должностного лица
    TEstablishedPostType fEstablishedPost; // Тип штатной должности
    TEmployeeCalendar  *fCalendar;     // Календарь

public:
    explicit TEmployee(TUnit *un, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TEmployee(const TEmployee &empl);
    ~TEmployee();

    TUnit *unit() const;               // Подразделение, в котором состоит ДЛ
    void setUnit(TUnit *un);
    TEmployeeRole &role() const;       // Роль должностного лица
    void setRole(const TEmployeeRole &rl);
    TEstablishedPostType establishedPost() const; // Тип штатной должности
    void setEstablishedPost(TEstablishedPostType ept);
    TEmployeeCalendar *calendar() const; // Календарь
    void setCalendar(TEmployeeCalendar  *clndr);

    TEmployee &operator=(const TEmployee &empl);

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TEmployee*> TEmployeeList;

#endif // TEMPLOYEE_H
