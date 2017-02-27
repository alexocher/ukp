
//***************************************************************************//
//                                                                           //
//             Реализация артиллерийских геодезических функций               //
//                         ( простые типы данных )                           //
//                                                                           //
//***************************************************************************//

#include <math.h>
#include <QPoint>
#include <defConst>
#include <defMacro>
#include <geod_base>
#include <gen>


//*****************************************************************************
// Структура TPositDblAD - Координаты (полярные) в радианах

TPositDblAD::TPositDblAD(double aa, double dd, double ee) : a(aa), d(dd), e(ee)
{
}
//-----------------------------------------------------------------------------

TPositDblAD::TPositDblAD(const TPositDblAD &ad) : a(ad.a), d(ad.d), e(ad.e)
{
}
//-----------------------------------------------------------------------------

// Задание координат
void TPositDblAD::setPosit(double aa, double dd, double ee)
{
    a = aa; d = dd; e = ee;
}
//-----------------------------------------------------------------------------

// Задание координат
void TPositDblAD::setPosit(const TPositDblAD &ad)
{
    a = ad.a; d = ad.d; e = ad.e;
}
//-----------------------------------------------------------------------------

// Определение события "this::а правее [athat]"
bool TPositDblAD::thisIsRight(double athat)
{
    gen::mod(a,2*PI); gen::mod(athat,2*PI);
    return fabs(a-athat)>PI ? a<athat : a>athat;
}
//-----------------------------------------------------------------------------

// Пропорциональное деление
void TPositDblAD::divide(const TPositDblAD &left, const TPositDblAD &right, uint kol, QList<TPositDblAD> &vlSubAD)
{
    vlSubAD.clear();
    if (kol<2) return;
  TPositDblAD &lft = const_cast<TPositDblAD&>(left), &rgt = const_cast<TPositDblAD&>(right);
    if (lft.a>rgt.a) lft.a += 2*PI;
  double dug((rgt.a-lft.a)/kol), dd((rgt.d-lft.d)/kol), de((rgt.e-lft.e)/kol);
    for (uint i=0; i<kol-1; i++)
    {
      TPositDblAD curAD(lft.a+dug*(i+1),lft.d+dd*(i+1),lft.e+de*(i+1));
        gen::mod(curAD.a,2*PI);
        vlSubAD.append(curAD);
    }
}
//-----------------------------------------------------------------------------

// Преобразование в строку
QString TPositDblAD::toStr()
{
    return QString("a: %1, d: %2, e: %3").arg(a).arg(d).arg(e);
}
//-----------------------------------------------------------------------------

// Прямая геодезическая задача QPoint в д.у.
QPoint geod_base::pgz(const QPoint srcPnt, const TPositDblAD ad)
{
  QPoint resPnt;
    resPnt.setX(srcPnt.x()+gen::round(ad.d*cos(ad.a)));
    resPnt.setY(srcPnt.y()+gen::round(ad.d*sin(ad.a)));
    return resPnt;
}
//-----------------------------------------------------------------------------

// Обратная геодезическая задача QPoint в д.у.
TPositDblAD geod_base::ogz(const QPoint srcPnt, const QPoint trgPnt)
{
  TPositDblAD resAD;
    resAD.d = hypot(trgPnt.x()-srcPnt.x(),trgPnt.y()-srcPnt.y());
    if (resAD.d>0.)
    {
        if (trgPnt.x()!=srcPnt.x())
        {
            resAD.a = atan2((trgPnt.y()-srcPnt.y()),(trgPnt.x()-srcPnt.x()));
            if (resAD.a<0) resAD.a+=2*PI;
        }
        else resAD.a = (trgPnt.y()<srcPnt.y()) ? 3*PI/2. : PI/2.;
    }
    return resAD;
}
//-----------------------------------------------------------------------------

// Обратная геодезическая задача QPoint (только дальность)
double geod_base::ogzd(const QPoint srcPnt, const QPoint trgPnt)
{
    return hypot(trgPnt.x()-srcPnt.x(),trgPnt.y()-srcPnt.y());
}
//-----------------------------------------------------------------------------

// Вычисление гипотенузы по двум катетам
double geod_base::hypot(const double dx, const double dy)
{
    return sqrt(dx*dx+dy*dy);
}
//-----------------------------------------------------------------------------

// Вычисление гипотенузы по двум катетам
double geod_base::hypot(const QPoint pnt1, const QPoint pnt2)
{
    return hypot(pnt2.x()-pnt1.x(),pnt2.y()-pnt1.y());
}
//-----------------------------------------------------------------------------

// Вычисление котангенса угла (ang - в радианах)
double geod_base::ctan(double ang)
{
    if (!ang) ang = 2*PI;
    return 1/tan(ang);
}
//-----------------------------------------------------------------------------

