#ifndef TMODULEEMPLOYEES_H
#define TMODULEEMPLOYEES_H

#include <QComboBox>
#include <QListWidget>
#include <TAbstractModule>
#include <TEmployee>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT TModuleEmployees : public TAbstractModule
{
private:
    TModuleEmployees(const TModuleEmployees &obj);
    TModuleEmployees &operator=(const TModuleEmployees &obj);

    TEmployee          *fSelfEmployee; // Собственное ДЛ
    TEmployeeList       fPlEmployees;  // Полный список персонала

    void insertEmployee(TEmployee *emp); // Добавить ДЛ

public :
    explicit TModuleEmployees(TProjModuleType t, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    ~TModuleEmployees();

    TEmployee *selfEmployee() const;   // Собственное ДЛ
    TEmployeeList &emploees() const;   // Полный список персонала
    TEmployee *findEmployee(int id);   // ... поиск по id
    TEmployee *findEmployee(const QString &nm, bool onscrnm=false);   // ... поиск по наименованию (false) или по отображаемому наименованию (true)
    TEmployeeList *findEmployees(const TEmployeeRoleList &rls); // ... поиск с подходящими ролями
    TEmployeeList *findEmployees(int unitid, const TEmployeeRoleList &rls); // ... поиск с подходящими ролями в заданном формировании

    bool createSelfEmployee();         // Создать собственное ДЛ
    static void reflectEmployeeTypesToCb(QComboBox &cb); // Отобразить в списке типы должностей
    void reflectEmployeesToLw(const TEmployeeList &empls, QListWidget &lw); // Отобразить ДЛ в списке

public: // TAbstractModule interface
    bool init();

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);

};

#endif // TMODULEEMPLOYEES_H
