#ifndef TMODULEUNITS_H
#define TMODULEUNITS_H

#include <QTreeWidget>
#include <QComboBox>
#include <TAbstractModule>
#include <TUnit>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT TModuleUnits : public TAbstractModule
{
private:
    TModuleUnits(const TModuleUnits &obj);
    TModuleUnits &operator=(const TModuleUnits &obj);

    TUnit              *fSelfUnit,     // Собственное подразделение
                       *fMainUnit;     // Подразделение уровня предприятия
    TUnitList           fUnits;        // Список подразделений

public :
    explicit TModuleUnits(TProjModuleType t, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    ~TModuleUnits();

    TUnit *selfUnit() const;           // Собственное подразделение
    TUnit *mainUnit() const;           // Подразделение уровня предприятия
    TUnitList &units() const;          // Список подразделений
    TUnit *findUnit(int id);
    TUnit *findUnit(QString nm, bool onscrnm=false);

    void reflectToTree(const TUnitList &units, QTreeWidget &tw, bool withempl=false); // Отобразить на дереве
    void reflectToCb(QComboBox &cb, int fromparentid, TUnitLevelList lvls, bool withempty=false, bool withparent=false, bool withchilds=false); // Отобразить в списке подразделения, подчиненные родителю с fromparentid
    void reflectForTemplateToCb(QComboBox &cb, TUnitLevelList lvls); // Отобразить в списке внешние подразделения указанных уровней (lvls) и шаблонные подразделения по линии иерархии
    int findUnitForTemplateInCb(QComboBox &cb, int id); // Найти в списке подразделение (конкретное или шаблонное) в списке по id (-lvl)

public: // TAbstractModule interface
    bool init();

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);

};

#endif // TMODULEUNITS_H
