#ifndef TMODULEPRODUCTIONS_H
#define TMODULEPRODUCTIONS_H

#include <QTreeWidget>
#include <TAbstractModule>
#include <TProduction>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT TModuleProductions : public TAbstractModule
{
private:
    TModuleProductions(const TModuleProductions &obj);
    TModuleProductions &operator=(const TModuleProductions &obj);

    TProductionList     fProductions;  // Список продукции (входной и выходной)

public :
    explicit TModuleProductions(TProjModuleType t, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    ~TModuleProductions();

    TProductionList &productions() const;  // Список продукции (входной и выходной)
    TProductionList *inProductions() const; //                  только входной
    TProductionList *outProductions() const; //                 только выходной

    void reflectToTree(const TProductionList &prods, QTreeWidget &tw); // Отобразить на дереве

public: // TAbstractModule interface
    bool init();

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);

};

#endif // TMODULEPRODUCTIONS_H
