
//***************************************************************************//
//                                                                           //
//                              Общие функции                                //
//                                                                           //
//***************************************************************************//

#ifndef GENERALH
#define GENERALH

#include <stdlib.h>
#include <QList>
#include <QPtrList>
#include <defUkpCommon>

class QDateTime;
class QTime;
class QChar;
class QString;

class UKPCOMMONSHARED_EXPORT gen
{
public:

    //*****************************************************************************
    // Функции общего назначения

    // Нахождение факториала целого числа n!
    double fact(int n);

    // Округление вещественного до ближайшего целого, кратного step
    static int round(double d, int step=1);

    // Установление точности дробной части
    // dVal - величина, которой устанавливается точность (ВХОД и ВЫХОД)
    static void setPrec(double &dVal, int prec=4);

    // Интерполяция
    // d1 --> val1 (ВХОД)
    // d  --> x    (ВОЗВРАТ)
    // d2 --> val2 (ВХОД)
    static double interp(double d, double d1, double d2, double val1, double val2);

    // Найти первое общее ненулевое значение
    static int findFirstGenValue(QPtrList<QList<int>*> &vals);

    // Событие нахождения в списке непрерывной последовательности чисел
    static bool isOnlyNearVal(const QList<int> &values, int lastn, bool iscircle);

    // Получение остатка от деления d/dd (аналог % для целых)
    static void mod(double &d, double dd);

    // Преобразование срединного отклонения в СКО
    static double errToSigma(double e);

    // Генерация СВ, распределенной по НЗР
    static double randG(double mog=0., double sigma=1.);

    //*****************************************************************************
    // Функции преобразования углов

    // Из делений угломера в радианы
    static double duToRad(int ug);

    // Из радиан в деления угломера
    static int radToDu(double ug);

    // Из делений угломера в градусы
    static double duToDeg(int ug);

    // Из градусов в деления угломера
    static int degToDu(double ug);

    // Из градусов в радианы
    static double degToRad(double ug);
    static double degToRad(double gr, double min, double sec=0.);

    // Из радиан в градусы
    static double radToDeg(double ug);

    // Из градусов в градусы
    static double degToDeg(double gr, double min, double sec);
    static void degToDeg(double dgr, int &gr, uint &min, uint &sec);

    // Преобразование ДУ в корректное значение (0...5999)
    static int toValidDu(int du);

    //***************************************************************************
    // Функции работы со строками QString

    // Преобразование строки (ЧЧ:ММ:СС ДД.ММ.ГГ || ЧЧ:ММ ДД.ММ) в дату и время
    // sdt - строка
    // full - полный формат строки (true) (ЧЧ:ММ:СС ДД.ММ.ГГ) или сокр. (false) (ЧЧ:ММ ДД.ММ)
    static QDateTime strToDateTime(QString sdt, bool full=false);

    // Преобразование целого в строку
    static QString intToStr(int i, bool withsign=false);

    // Преобразование вещественного в строку
    // prec - количество знаков после запятой
    static QString floatToStr(double d, int prec=2);

    // Преобразование логического в строку
    static QString boolToStr(bool b);

    // Преобразование целого угла в д.у. в привычную строку
    static QString formatDu(int ug);

    // Преобразование д.у. в целое число
    static int fromFormatDu(QString ug);

    // Преобразование строки в весовой знак
    static int strToQ(QString txt);

    // Преобразование весового знака в строку
    static QString qToStr(int q);

    // Преобразование минут в строку от времени "Ч"
    static QString minuteToTimeCh(int m);

    // Преобразовать строку в набор строк, разделенных "\n", длиной 80 символов
    static QString dispOnLine(QString s, int lln=80);

    // Дополнить строку символами sh до длины l, дописывая символы справа (по умолчанию) или слева
    static QString fillStr(QString str, QChar sh, int l, bool fromleft=false);

    // Преобразование строки формата чч:мм:сс дд.ММ.гг в QDateTime
    static QDateTime parseDateTime(QString sdt);

    // Преобразование строки формата чч:мм:сс в QTime
    static QTime parseTime(QString st);

    // Преобразование строк в зависимости от числительных
    // str - строчные буквы, единственное число, именительный падеж
    static QString strToStrOfNum(QString str, uint n);

    // Определение количества дней, часов, минут, секунд от времени dtfrom до времени dtto
    static QString timeFromTime(QDateTime &dtfrom, QDateTime &dtto);

    // Определение количества минут, секунд от времени dtfrom до времени dtto с ограничением, в минутах (не более 60)
    // dsec - разность в секундах
    static QString timeFromTime(QDateTime &dtfrom, QDateTime &dtto, unsigned char ogrmin, int &dsec);

    // Привести строку к виду 000.000.000.000
    static QString formatIPv4(QString sip);

    // Размер данных в байтах или килобайтах, или мегабайтах
    static QString dataSize(qint64 sz);

}; // namespace gen

#endif // GENERALH
