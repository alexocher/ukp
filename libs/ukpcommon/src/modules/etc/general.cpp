
//***************************************************************************//
//                                                                           //
//                         Реализация общих функций                          //
//                                                                           //
//***************************************************************************//

#include <math.h>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <defConst>
#include <defMacro>
#include <gen>

//*****************************************************************************
// Функции общего назначения

// Нахождение факториала целого числа n!
double gen::fact(int n)
{
  int p = 1;

    if (n<0) {PR2(0,"%1ERROR: Факториал отрицательного числа не существует%2",CLR_RED,CLR_RESET);return 0;}
    if (n) for (int i=0; i<n; i++) p *= i+1;
    return p;
}
//-----------------------------------------------------------------------------

// Округление вещественного до ближайшего целого, кратного step
int gen::round(double d, int step)
{
    if (step<2)
    {
      double d05 = d<0 ? -0.5 : 0.5;
        return (int)(d+d05);
    }
    else
    {
      int iLow = ((int)d/step)*step;
        return fabs(d-iLow)<step/2. ? iLow : (d<0 ? iLow-step : iLow+step);
    }
}
//-----------------------------------------------------------------------------

// Установление точности дробной части
// dVal - величина, которой устанавливается точность (ВХОД и ВЫХОД)
void gen::setPrec(double &dVal, int prec)
{
  int normVal = 1;
  double intPart, fractPart = modf(dVal,&intPart);

    for (int i=0; i<prec; i++) normVal *= 10;
    fractPart = round(fractPart*normVal)/(double)normVal;
    dVal = intPart + fractPart;
}
//-----------------------------------------------------------------------------

// Интерполяция
// d1 --> val1 (ВХОД)
// d  --> x    (ВОЗВРАТ)
// d2 --> val2 (ВХОД)
double gen::interp(double d, double d1, double d2, double val1, double val2)
{
  double k = (d2!=d1) ? (d-d1)/(d2-d1) : 0;

    return val1+k*(val2-val1);
}
//-----------------------------------------------------------------------------

// Найти первое общее ненулевое значение
int gen::findFirstGenValue(QPtrList<QList<int>*> &vals)
{
    if (!vals.count()) return 0;
    if (QList<int> *frstVl = vals.first())
        foreach (int curVal,*frstVl)
        {
          bool isEqv(true);
            foreach (QList<int> *curVl,vals)
                if (curVl->indexOf(curVal)==-1) { isEqv = false; break; }
            if (isEqv) return curVal;
        }
    return 0;
}
//-----------------------------------------------------------------------------

// Событие нахождения в списке непрерывной последовательности чисел
// ( iscircle - признак замкнутости чисел, т.е. после последнего (lastn) должна идти опять единица (1) )
bool gen::isOnlyNearVal(const QList<int> &values, int lastn, bool iscircle)
{
  QList<int> &vals = const_cast<QList<int>&>(values);
  uint kolVals(vals.count());
    if (!kolVals) return false;
    if (kolVals==1) return true;
    qSort(vals);
  bool fromFirst(vals.indexOf(1)>-1), toLast(vals.indexOf(lastn)>-1);
//PR1(4,"vals.count(): %1",vals.count());
//for (uint i=0; i<kolVals; i++) cerr<<"    "<<(*vals.at(i))<<endl;
  int nextVal(vals.at(0)+1);
  QList<int> circleVals;
    for (uint i=1; i<kolVals; i++)
    {
      const int &curVal(vals.at(i));
        if (curVal==nextVal) nextVal = curVal+1;
        else // разрыв
        {
            if (!iscircle || (!fromFirst && !toLast)) return false;
            for (uint j=i; j<kolVals; j++) circleVals<<(vals.at(j)); // оставшиеся числа в начало нового списка
            for (uint j=0; j<i; j++) circleVals<<(lastn+(vals.at(j))); // первые числа в конец списка
            break; // for(i)
        }
    }
    if (circleVals.count()) // проверить новую последовательность с начальными точками в конце
    {
//PR1(4,"circleVals.count(): %1",circleVals.count());
//for (uint i=0; i<circleVals.count(); i++) cerr<<"    "<<(*circleVals.at(i))<<endl;
        nextVal = circleVals.at(0)+1;
        for (int i=1; i<circleVals.count(); i++)
        {
          const int &curVal(circleVals.at(i));
            if (curVal==nextVal) nextVal = curVal+1;
            else return false; // разрыв
        }
    }
    return true;
}
//-----------------------------------------------------------------------------

// Получение остатка от деления (аналог % для целых)
void gen::mod(double &d, double dd)
{
    if (d>0) while (d>=dd) d -= dd;
    else if (d<0) while (d<=dd) d += dd;
}
//-----------------------------------------------------------------------------

// Преобразование срединного отклонения в СКО
double gen::errToSigma(double e)
{
    return e/ROSQRT2;
}
//-----------------------------------------------------------------------------

// Генерация СВ, распределенной по НЗР
// ( Marsaglia-Bray algorithm, Delphi )
double gen::randG(double mog, double sigma)
{
  double u1(0.), u2(0.), s(0.);
    do
    {
        u1 = 2*(0.0001*(rand()%10001))-1.;
        u2 = 2*(0.0001*(rand()%10001))-1.;
        s = u1*u1+u2*u2;
    }
    while (s>=1.);
    return sqrt(-2*log(s)/s)*u1*sigma+mog;
/*
    function RandG(Mean, StdDev: Extended): Extended;
    var
      U1, S2: Extended;
    begin
      repeat
        U1 := 2*Random - 1;
        S2 := Sqr(U1) + Sqr(2*Random-1);
      until S2 < 1;
      Result := Sqrt(-2*Ln(S2)/S2) * U1 * StdDev + Mean;
    end;
*/
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// Функции преобразования углов

// Из делений угломера в радианы
double gen::duToRad(int ug)
{
    return degToRad(ug/100.*6.);
}
//-----------------------------------------------------------------------------

// Из радиан в деления угломера
int gen::radToDu(double ug)
{
    return round(radToDeg(ug)/6.*100.);
}
//-----------------------------------------------------------------------------

// Из делений угломера в градусы
double gen::duToDeg(int ug)
{
    return ug/100.*6.;
}
//-----------------------------------------------------------------------------

// Из градусов в деления угломера
int gen::degToDu(double ug)
{
    return round(ug/6.*100.);
}
//-----------------------------------------------------------------------------

// Из градусов в радианы
double gen::degToRad(double ug)
{
    return ug/GR_IN_RAD;
}
//-----------------------------------------------------------------------------

double gen::degToRad(double gr, double min, double sec)
{
    return degToDeg(gr,min,sec)/GR_IN_RAD;
}
//-----------------------------------------------------------------------------

// Из радиан в градусы
double gen::radToDeg(double ug)
{
    return ug*GR_IN_RAD;
}
//-----------------------------------------------------------------------------

// Из градусов в градусы
double gen::degToDeg(double gr, double min, double sec)
{
  int sign(gr<0. ? -1 : 1);
    return sign*(fabs(gr)*3600+60*min+sec)/3600;
}
//-----------------------------------------------------------------------------

void gen::degToDeg(double dgr, int &gr, uint &min, uint &sec)
{
  double dsecs(fabs(dgr)*3600.);
    gr = (uint)(fabs(dgr)); dsecs -= gr*3600;
    min = ((uint)dsecs)/60; dsecs -= min*60;
    sec = round(dsecs);
    gr *= (dgr<0. ? -1 : 1);
}
//-----------------------------------------------------------------------------

// Преобразование ДУ в корректное значение (0...5999)
int gen::toValidDu(int du)
{
    if(du >= 0 && du < 6000) return du;
    while(du < 0) du += 6000;
    while(du > 5999) du -= 6000;
    return du;
}
//-----------------------------------------------------------------------------

//***************************************************************************
// Функции работы со строками QString

// Преобразование строки (ЧЧ:ММ:СС ДД.ММ.ГГ || ЧЧ:ММ ДД.ММ) в дату и время
// sdt - строка
// full - полный формат строки (true) (ЧЧ:ММ:СС ДД.ММ.ГГ) или сокр. (false) (ЧЧ:ММ ДД.ММ)
QDateTime gen::strToDateTime(QString sdt, bool full)
{
  QString sDel, sTime, sDate, sYear, sMonth, sDay, sHour, sMin, sSec;
  int posDel, iYear, iMonth, iDay, iHour, iMin, iSec;

    posDel = sdt.indexOf(" ");
    sTime = sdt.left(posDel);
    sdt.remove(0,posDel+1);
    sDate = sdt;
    // Время
    posDel = sTime.indexOf(":");
    sHour = sTime.left(posDel);
    sTime.remove(0,posDel+1);
    posDel = sTime.indexOf(":");
    if (posDel>-1)
    {
        sMin = sTime.left(posDel);
        sTime.remove(0,posDel+1);
    }
    else
    {
        sMin = sTime;
        sTime = "00";
    }
    // Дата
    posDel = sDate.indexOf(".");
    sDay = sDate.left(posDel);
    sDate.remove(0,posDel+1);
    posDel = sDate.indexOf(".");
    if (posDel>-1)
    {
        sMonth = sDate.left(posDel);
        sDate.remove(0,posDel+1);
    }
    else
    {
        sMonth = sDate;
        iYear = QDateTime::currentDateTime().date().year();
        sDate = intToStr(iYear);
    }
    if (full)
    {
        sSec = sTime;
        sYear = sDate;
    }
    else
    {
        sSec = "00";
        iYear = QDateTime::currentDateTime().date().year();
        sYear = intToStr(iYear);
    }
    try { iHour = sHour.toInt(); } // Часы
    catch (...) { PR3(0,"%1ERROR: Часы заданы неправильно: %2%3",CLR_RED,sHour,CLR_RESET); iHour = 0; }
    try { iMin = sMin.toInt(); } // Минуты
    catch (...) { PR3(0,"%1ERROR: Минуты заданы неправильно: %2%3",CLR_RED,sMin,CLR_RESET); iMin = 0; }
    try { iSec = sSec.toInt(); } // Секунды
    catch (...) { PR3(0,"%1ERROR: Секунды заданы неправильно: %2%3",CLR_RED,sSec,CLR_RESET); iSec = 0; }
    try { iDay = sDay.toInt(); } // День
    catch (...) { PR3(0,"%1ERROR: День задан неправильно: %2%3",CLR_RED,sDay,CLR_RESET); iDay = 0; }
    try { iMonth = sMonth.toInt(); } // Месяц
    catch (...) { PR3(0,"%1ERROR: Месяц задан неправильно: %2%3",CLR_RED,sMonth,CLR_RESET); iMonth = 0; }
    try { iYear = sYear.toInt(); } // Год
    catch (...) { PR3(0,"%1ERROR: Год задан неправильно: %2%3",CLR_RED,sYear,CLR_RESET); iYear = 0; }
    return QDateTime(QDate(iYear,iMonth,iDay),QTime(iHour,iMin,iSec,0));
}
//-----------------------------------------------------------------------------

// Преобразование целого в строку
QString gen::intToStr(int i, bool withsign)
{
  QString res(QString::number(i));
    if (withsign && i>0) res = "+"+res;
    return res;
}
//-----------------------------------------------------------------------------

// Преобразование вещественного в строку
// prec - количество знаков после запятой
QString gen::floatToStr(double d, int prec)
{
    return QString("%1").arg(d,prec+2,'f',prec);
}
//-----------------------------------------------------------------------------

// Преобразование целого угла в д.у. в привычную строку
QString gen::formatDu(int ug)
{
  QString sDu(intToStr(ug));
    if(ug<0) sDu = intToStr(abs(ug));
    if (sDu.length()<3) sDu = sDu.rightJustified(3,'0');
    sDu.insert(sDu.length()-2,'-');
    if(ug<0) sDu.insert(0,'-');
    return sDu;
}
//-----------------------------------------------------------------------------

// Преобразование д.у. в целое число
int gen::fromFormatDu(QString ug)
{
    ug.remove('-');
    bool ok;
    int res = ug.toInt(&ok);
    if(ok) return res;
    return 0;
}
//-----------------------------------------------------------------------------

// Преобразование строки в весовой знак
int gen::strToQ(QString txt)
{
    if (txt==QString("ТЖ")) return 4;
    else if (txt==QString("3+")) return 3;
    else if (txt==QString("2+")) return 2;
    else if (txt==QString("+")) return 1;
    else if (txt==QString("Н")) return 0;
    else if (txt==QString("-")) return -1;
    else if (txt==QString("2-")) return -2;
    else if (txt==QString("3-")) return -3;
    else return -4; // "ЛГ"
}
//-----------------------------------------------------------------------------

// Преобразование весового знака в строку
QString gen::qToStr(int q)
{
    if (q>=4) return "ТЖ";
    else if (q==3) return "3+";
    else if (q==2) return "2+";
    else if (q==1) return "+";
    else if (q==0) return "Н";
    else if (q==-1) return "-";
    else if (q==-2) return "2-";
    else if (q==-3) return "3-";
    else return "ЛГ"; // <=-4
}
//-----------------------------------------------------------------------------

// Преобразование логического в строку
QString gen::boolToStr(bool b)
{
    return b ? "'t'" : "'f'";
}
//-----------------------------------------------------------------------------

// Преобразование минут в строку от времени "Ч"
QString gen::minuteToTimeCh(int m)
{
  int iH, iM;
  QString sgn = m<0 ? "- " : (m==0 ? "" : "+ "), s0;

    iH = (int)fabs(m)/60;
    iM = (int)fabs(m)%60;
    s0 = iM<10 ? "0" : "";
    return sgn+intToStr(iH)+"."+s0+intToStr(iM);
}
//-----------------------------------------------------------------------------

// Преобразовать строку в набор строк, разделенных "\n", длиной 80 символов
QString gen::dispOnLine(QString s, int lln)
{
    if (!lln) return "Нулевая длина строки";

  QString res="";
  int lines = s.length()/lln+1, pos=0;

    for (int i=0; i<lines; i++)
    {
        res += s.mid(pos,lln);
        if (i<(lines-1)) res += "\n";
        pos += lln;
    }
    return res;
}
//-----------------------------------------------------------------------------

// Дополнить строку символами sh до длины l, дописывая символы справа (по умолчанию) или слева
QString gen::fillStr(QString str, QChar sh, int l, bool fromleft)
{
  int curL = str.length();
  QString addStr;

    if (l<=curL) return str;
    return fromleft ? addStr.fill(sh,l-curL)+str : str+addStr.fill(sh,l-curL);
}
//-----------------------------------------------------------------------------

// Преобразование строки формата чч:мм:сс дд.ММ.гг в QDateTime
QDateTime gen::parseDateTime(QString sdt)
{
  QString stime = sdt.section(" ",0,0), sdate = sdt.section(" ",1,1), syear = sdate.section(".",2,2);
  int ihour = stime.section(":",0,0).toInt(), imin = stime.section(":",1,1).toInt(), isec = stime.section(":",2,2).toInt(), iday = sdate.section(".",0,0).toInt(), imonth = sdate.section(".",1,1).toInt();

    return QDateTime(QDate(syear.isEmpty() ? QDate::currentDate().year() : 2000 + syear.toInt(),imonth,iday),QTime(ihour,imin,isec));
}
//-----------------------------------------------------------------------------

// Преобразование строки формата чч:мм:сс в QTime
QTime gen::parseTime(QString st)
{
  QString stime = st.section(" ",0,0);
  int ihour = stime.section(":",0,0).toInt(), imin = stime.section(":",1,1).toInt(), isec = stime.section(":",2,2).toInt();

    return QTime(ihour,imin,isec);
}
//-----------------------------------------------------------------------------

// Преобразование строк в зависимости от числительных
// str - строчными буквами единственное число именительный падеж
QString gen::strToStrOfNum(QString str, uint n)
{
//PR_FN_LN
//PR1(4,"str: [%1]",str);
//PR1(4,"n: [%1]",n);
//PR1(4,"n10: [%1]",(n%10));
    if (str==QString("секунда"))
        switch (n%10)
        {
            case 1: return "секунда";
            case 2: case 3: case 4: return "секунды";
            default : return "секунд";
        }
    else if (str==QString("минута"))
        switch (n%10)
        {
            case 1: return "минута";
            case 2: case 3: case 4: return "минуты";
            default : return "минут";
        }
    else if (str==QString("час"))
        switch (n%10)
        {
            case 1: return "час";
            case 2: case 3: case 4: return "часа";
            default : return "часов";
        }
    else if (str==QString("снаряд"))
        switch (n%10)
        {
            case 1: return "снаряд";
            case 2: case 3: case 4: return "снаряда";
            default : return "снарядов";
        }
    else if (str==QString("орудие"))
       switch (n%10)
        {
            case 1: return "орудие";
            case 2: case 3: case 4: return "орудия";
            default : return "орудий";
        }
    return "";
}
//-----------------------------------------------------------------------------

// Определение количества дней, часов, минут, секунд от времени dtfrom до времени dtto
QString gen::timeFromTime(QDateTime &dtfrom, QDateTime &dtto)
{
  QString resStr("");
  int daysto(dtfrom.daysTo(dtto)), secsto(dtfrom.secsTo(dtto)-daysto*86400);
    if (dtfrom>dtto) // отрицательная разница
    {
        if (secsto>0) { daysto++; secsto -= 86400; }
        daysto *= -1; secsto *= -1;
      QTime tm(secsto/3600,(secsto/60)%60,secsto%60);
        resStr = QString("-%1%2").arg((daysto ? QString("%1 дн. ").arg(daysto) : "")).arg(tm.toString("hh:mm:ss"));
    }
    else // положительная разница
    {
        if (secsto<0) { daysto--; secsto += 86400; }
      QTime tm(secsto/3600,(secsto/60)%60,secsto%60);
        resStr = QString("%1%2").arg((daysto ? QString("%1 дн. ").arg(daysto) : "")).arg(tm.toString("hh:mm:ss"));
    }
    return resStr;
}
//-----------------------------------------------------------------------------

// Определение количества минут, секунд от времени dtfrom до времени dtto с ограничением, в минутах (не более 60)
QString gen::timeFromTime(QDateTime &dtfrom, QDateTime &dtto, unsigned char ogrmin, int &dsec)
{
  QString sOgr(ogrmin<60 ? QString(">%1 мин.").arg(ogrmin) : QString(">1 часа"));
  int daysto(dtfrom.daysTo(dtto)), secsto(dtfrom.secsTo(dtto)-daysto*86400), ogrsecs(ogrmin*60);
  bool isNegative(dtfrom>dtto); // отрицательная разница
    dsec = secsto;
    if (isNegative) secsto *= -1;
    if (daysto || secsto>=ogrsecs)
    {
        dsec = isNegative ? -ogrsecs : ogrsecs;
        return isNegative ? QString("Просроч.") : (daysto ? QString("> суток") : sOgr);
    }
  QTime tm(secsto/3600,(secsto/60)%60,secsto%60);
    return QString("%1%2").arg(isNegative ? "-" : "").arg(tm.toString("mm:ss"));
}
//-----------------------------------------------------------------------------

// Привести строку к виду 000.000.000.000
QString gen::formatIPv4(QString sip)
{
  QStringList sl = sip.split(".");
    if (sl.count()!=4) return "";
    return QString("%1.%2.%3.%4").arg(sl.at(0).toInt(),3,10,QChar('0')).arg(sl.at(1).toInt(),3,10,QChar('0')).arg(sl.at(2).toInt(),3,10,QChar('0')).arg(sl.at(3).toInt(),3,10,QChar('0'));
}
//-----------------------------------------------------------------------------

// Размер данных в байтах или килобайтах, или мегабайтах
QString gen::dataSize(qint64 sz)
{
    if (sz<1024) return intToStr(sz);
    if (sz<(1024*1024)) return floatToStr(sz/1024.,1)+"K";
    return floatToStr(sz/1024./1024.,1)+"M";
}
//-----------------------------------------------------------------------------
