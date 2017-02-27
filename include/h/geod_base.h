
//***************************************************************************//
//                                                                           //
//                     Прототипы геодезических функций                       //
//                         ( простые типы данных )                           //
//                                                                           //
//***************************************************************************//

#ifndef GEOD_BASEH
#define GEOD_BASEH

#include <QPtrList>
#include <defUkpCommon>

class QPoint;

// Координаты (полярные) в радианах
struct UKPCOMMONSHARED_EXPORT TPositDblAD
{
  TPositDblAD(double aa=0., double dd=0., double ee=0.);
  TPositDblAD(const TPositDblAD &ad);

  double                a, d, e;       // Дирекционный угол, дальность и угол места
  void setPosit(double aa=0., double dd=0., double ee=0.); // Задание координат
  void setPosit(const TPositDblAD &ad);
  bool thisIsRight(double athat);      // Определение события "this::а правее [athat]"
  static void divide(const TPositDblAD &left, const TPositDblAD &right, uint kol, QList<TPositDblAD> &vlSubAD); // пропорциональное деление
  QString toStr();                     // Преобразование в строку

};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TPositDblAD*> TPositDblADList;

class UKPCOMMONSHARED_EXPORT geod_base
{
public:

    // Прямая геодезическая задача QPoint в д.у.
    static QPoint pgz(const QPoint srcPnt, const TPositDblAD ad);

    // Обратная геодезическая задача QPoint в д.у.
    static TPositDblAD ogz(const QPoint srcPnt, const QPoint trgPnt);

    // Обратная геодезическая задача QPoint (только дальность)
    static double ogzd(const QPoint srcPnt, const QPoint trgPnt);

    // Вычисление гипотенузы по двум катетам
    static double hypot(const double dx, const double dy);

    // Вычисление гипотенузы по двум катетам
    static double hypot(const QPoint pnt1, const QPoint pnt2);

    // Вычисление котангенса угла (ang - в радианах)
    static double ctan(double ang);

}; // namespace geod_base

#endif // GEOD_BASEH
