#include <TProduction>

TProduction::TProduction(int id, QString nm, TProductionType prtp, TProductionView prvw, QString scrnm, int n, TAbstractObject *parent) : TAbstractObject(id,n,nm,parent), fProductionType(prtp), fProductionView(prvw), fRubr2(pr2None), fRubr3(pr3None), fRubr4(pr4None), fIsIn(false), fIsOut(false)
{
    setScrName(scrnm);
}
//-----------------------------------------------------------------------------

TProduction::TProduction(const TProduction &obj) : TAbstractObject(obj), fProductionType(obj.fProductionType), fProductionView(obj.fProductionView), fRubr2(obj.fRubr2), fRubr3(obj.fRubr3), fRubr4(obj.fRubr4), fIsIn(obj.fIsIn), fIsOut(obj.fIsOut)
{
}
//-----------------------------------------------------------------------------

TProduction::~TProduction()
{

}
//-----------------------------------------------------------------------------

void TProduction::reset(bool thisonly)
{
    if (!thisonly) TAbstractObject::reset(true);
    fProductionType = prtNone;
    fProductionView = prvNone;
    fRubr2 = pr2None;
    fRubr3 = pr3None;
    fRubr4 = pr4None;
    fIsIn = false;
    fIsOut = false;
}
//-----------------------------------------------------------------------------

TProduction &TProduction::operator=(const TProduction &obj)
{
    if (&obj==this) return *this;
    TAbstractObject::operator=(obj);
    fProductionType = obj.fProductionType;
    fProductionView = obj.fProductionView;
    fRubr2 = obj.fRubr2;
    fRubr3 = obj.fRubr3;
    fRubr4 = obj.fRubr4;
    fIsIn = obj.fIsIn;
    fIsOut = obj.fIsOut;
    return *this;
}
//-----------------------------------------------------------------------------

TProductionType TProduction::productionType() const
{
    return fProductionType;
}
//-----------------------------------------------------------------------------

void TProduction::setProductionType(TProductionType tp)
{
    fProductionType = tp;
}
//-----------------------------------------------------------------------------

TProductionView TProduction::productionView() const
{
    return fProductionView;
}
//-----------------------------------------------------------------------------

void TProduction::setProductionView(TProductionView vw)
{
    fProductionView = vw;
}
//-----------------------------------------------------------------------------

TProductionRubr2 TProduction::rubr2() const
{
    return fRubr2;
}
//-----------------------------------------------------------------------------

void TProduction::setRubr2(TProductionRubr2 r)
{
    fRubr2 = r;
}
//-----------------------------------------------------------------------------

TProductionRubr3 TProduction::rubr3() const
{
    return fRubr3;
}
//-----------------------------------------------------------------------------

void TProduction::setRubr3(TProductionRubr3 r)
{
    fRubr3 = r;
}
//-----------------------------------------------------------------------------

TProductionRubr4 TProduction::rubr4() const
{
    return fRubr4;
}
//-----------------------------------------------------------------------------

void TProduction::setRubr4(TProductionRubr4 r)
{
    fRubr4 = r;
}
//-----------------------------------------------------------------------------

bool TProduction::isIn() const
{
    return fIsIn;
}
//-----------------------------------------------------------------------------

void TProduction::setIn(bool isin)
{
    fIsIn = isin;
}
//-----------------------------------------------------------------------------

bool TProduction::isOut() const
{
    return fIsOut;
}
//-----------------------------------------------------------------------------

void TProduction::setOut(bool isout)
{
    fIsOut = isout;
}
//-----------------------------------------------------------------------------

QString TProduction::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TProduction::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TProduction::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TProduction::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
