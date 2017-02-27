#ifndef TPRODUCTION_H
#define TPRODUCTION_H

#include <enums>
#include <TAbstractObject>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT TProduction : public TAbstractObject
{
private:
    TProductionType     fProductionType; // Тип продукции
    TProductionView     fProductionView; // Вид продукции (первый уровень иерархии)
    TProductionRubr2    fRubr2;        // Второй уровень иерархии
    TProductionRubr3    fRubr3;        // Третий уровень иерархии
    TProductionRubr4    fRubr4;        // Четвертый уровень иерархии
    bool                fIsIn,         // Признак входной продукции
                        fIsOut;        // Признак выходной продукции

public:
    TProduction(int id, QString nm, TProductionType prtp, TProductionView prvw, QString scrnm="", int n=0, TAbstractObject *parent=NULL);
    TProduction(const TProduction &obj);
    ~TProduction();

    TProductionType productionType() const; // Тип продукции
    void setProductionType(TProductionType tp);
    TProductionView productionView() const; // Вид продукции (первый уровень иерархии)
    void setProductionView(TProductionView vw);
    TProductionRubr2 rubr2() const;    // Второй уровень иерархии
    void setRubr2(TProductionRubr2 r);
    TProductionRubr3 rubr3() const;    // Третий уровень иерархии
    void setRubr3(TProductionRubr3 r);
    TProductionRubr4 rubr4() const;    // Четвертый уровень иерархии
    void setRubr4(TProductionRubr4 r);
    bool isIn() const;                 // Признак входной продукции
    void setIn(bool isin);
    bool isOut() const;                // Признак выходной продукции
    void setOut(bool isout);

    TProduction &operator=(const TProduction &obj);

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);

};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TProduction*> TProductionList;

#endif // TPRODUCTION_H
