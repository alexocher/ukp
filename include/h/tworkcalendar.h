#ifndef TWORKCALENDAR_H
#define TWORKCALENDAR_H

#include <enums>
#include <TAbstractObject>
#include <defUkpCommon>

// Общий годовой календарь предприятия
class UKPCOMMONSHARED_EXPORT TCompanyCalendar : public TAbstractObject
{
private:
    int                 fYear;         // Год
    QList<int>          fWeekEnds;     // Список выходных [ 1 января -> 0, 31 декабря -> 364 (365) ]

    TCompanyCalendar(const TCompanyCalendar &att);
    TCompanyCalendar &operator=(const TCompanyCalendar &att);

public:
    explicit TCompanyCalendar(int year, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    ~TCompanyCalendar();

    int year() const;                  // Год
    QList<int> &weekEnds() const;      // Список выходных
    void insertWeekEnd(const QDate &dt); // ... добавление
    void insertWeekEnd(int dt);        // ... добавление
    void clearWeekEnd();               // ... очистка

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);        // год
};

// Промежуток рабочего времени
class UKPCOMMONSHARED_EXPORT TWorkPeriod : public TAbstractObject
{
private:
    TWorkPeriodType     fType;         // Тип промежутка рабочего времени
    int                 fBegin,        // Начало периода [ 1 января -> 0, 31 декабря -> 364 (365) ]
                        fEnd;          // Окончание периода

public:
    TWorkPeriod(TWorkPeriodType tp, const QDate &dtbeg, const QDate &dtend, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TWorkPeriod(TWorkPeriodType tp, int dtbeg, int dtend, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TWorkPeriod(const TWorkPeriod &rp);
    ~TWorkPeriod();

    TWorkPeriodType type() const;      // Тип промежутка рабочего времени
    void setType(TWorkPeriodType tp);
    int begin() const;                 // Начало периода
    void setBegin(int dt);
    void setBegin(const QDate &dt);
    int end() const;                   // Окончание периода
    void setEnd(int dt);
    void setEnd(const QDate &dt);

    TWorkPeriod &operator=(const TWorkPeriod &rp);

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TWorkPeriod*> TWorkPeriodList;

typedef UKPCOMMONSHARED_EXPORT QList<QDate> TDateList;

// Персональный календарь сотрудника
class UKPCOMMONSHARED_EXPORT TEmployeeCalendar : public TAbstractObject
{
private:
    TCompanyCalendar   *fCompanyCalendar; // Общий годовой календарь предприятия
    TWorkPeriodList     fWorkPeriods;     // Промежутки рабочего времени

public:
    explicit TEmployeeCalendar(TCompanyCalendar *cmpclnd, int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TEmployeeCalendar(const TEmployeeCalendar &emc);
    ~TEmployeeCalendar();

    TWorkPeriodList &workPeriods() const; // Промежутки рабочего времени
    void insertWorkPeriod(TWorkPeriod *rp); // ... добавление
    void removeWorkPeriod(TWorkPeriod *rp); // ... удаление
    void clearWorkPeriods();           // ... очистка

    TEmployeeCalendar &operator=(const TEmployeeCalendar &emc);
    QList<int> *fullRestDates();       // Полный список выходных дней

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

#endif // TWORKCALENDAR_H
