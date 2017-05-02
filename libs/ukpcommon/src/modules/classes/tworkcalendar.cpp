#include <EM_Calendar>
#include <defMacro>
#include <TWorkCalendar>
#include <TAbstractApp>

TCompanyCalendar::TCompanyCalendar(int year, int id, int n, QString nm, TAbstractObject *parent) : TAbstractObject(id,n,nm,parent), fYear(year)
{

}
//-----------------------------------------------------------------------------

TCompanyCalendar::~TCompanyCalendar()
{

}
//-----------------------------------------------------------------------------

void TCompanyCalendar::reset(bool thisonly)
{
    if (!thisonly) TAbstractObject::reset(true);
    fYear = 0;
    fWeekEnds.clear();
}
//-----------------------------------------------------------------------------

int TCompanyCalendar::year() const
{
    return fYear;
}
//-----------------------------------------------------------------------------

QList<int> &TCompanyCalendar::weekEnds() const
{
    return const_cast<QList<int>&>(fWeekEnds);
}
//-----------------------------------------------------------------------------

void TCompanyCalendar::insertWeekEnd(const QDate &dt)
{
  int day(dt.dayOfYear());
    if (fWeekEnds.indexOf(day)==-1)
    {
        fWeekEnds.append(day);
        qStableSort(fWeekEnds.begin(),fWeekEnds.end());
    }
}
//-----------------------------------------------------------------------------

void TCompanyCalendar::insertWeekEnd(int day)
{
    if (fWeekEnds.indexOf(day)==-1)
    {
        fWeekEnds.append(day);
        qStableSort(fWeekEnds.begin(),fWeekEnds.end());
    }
}
//-----------------------------------------------------------------------------

void TCompanyCalendar::clearWeekEnd()
{
    fWeekEnds.clear();
}
//-----------------------------------------------------------------------------

QString TCompanyCalendar::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TCompanyCalendar::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TCompanyCalendar::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TCompanyCalendar::fromDB(QString param) // год
{
    fYear = param.isEmpty() ? QDate::currentDate().year() : param.toInt();
    fWeekEnds.clear();
PR(0,"COMPANY CALENDAR");
    try
    {
      EM_CalendarDic &dic = EM_CalendarDic::Instance();
        try
        {
            PROJ->setWorkDayBegin(dic.getBeginWork());
          QList<QSharedPointer<EM_CalendarItem> > intervals = dic.get(QDateTime(QDate(fYear,1,1),QTime(0,0)),QDateTime(QDate(fYear+1,1,1),QTime(0,0)));
            foreach (QSharedPointer<EM_CalendarItem> shIt,intervals)
                if (EM_CalendarItem *cdrIt = shIt.data())
                {
                  const QDateTime &beginDate = cdrIt->getBeginDate(), &endDate = cdrIt->getEndDate();
PR2(4,"[begin]: %1, [end]: %2",beginDate.toString("dd.MM,yy"),endDate.toString("dd.MM,yy"));
                  int frstDay(beginDate.date().dayOfYear()), diffDays(beginDate.date().daysTo(endDate.date()));
                    fWeekEnds<<(frstDay-1);
                    for (int i=0; i<diffDays; i++) fWeekEnds<<(frstDay+i);
                }
        }
        catch (CommonException::OpenDBException &e)
        {
            INSERT_ERROR(QString("EM_CalendarDic::get(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
        }
        catch (CommonException::SQLException &e)
        {
            INSERT_ERROR(QString("EM_CalendarDic::get(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
        }
    }
    catch (CommonException::OpenDBException &e)
    {
        INSERT_ERROR(QString("EM_CalendarDic::Instance(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
    }
    catch (CommonException::SQLException &e)
    {
        INSERT_ERROR(QString("EM_CalendarDic::Instance(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
    }
    return true;
}
//-----------------------------------------------------------------------------

//*****************************************************************************

TWorkPeriod::TWorkPeriod(TWorkPeriodType tp, const QDate &dtbeg, const QDate &dtend, int id, int n, QString nm, TAbstractObject *parent) : TAbstractObject(id,n,nm,parent), fType(tp), fBegin(dtbeg.dayOfYear()), fEnd(dtend.dayOfYear())
{

}
//-----------------------------------------------------------------------------

TWorkPeriod::TWorkPeriod(TWorkPeriodType tp, int dtbeg, int dtend, int id, int n, QString nm, TAbstractObject *parent) : TAbstractObject(id,n,nm,parent), fType(tp), fBegin(dtbeg), fEnd(dtend)
{

}
//-----------------------------------------------------------------------------

TWorkPeriod::TWorkPeriod(const TWorkPeriod &rp) : TAbstractObject(rp), fType(rp.fType), fBegin(rp.fBegin), fEnd(rp.fEnd)
{

}
//-----------------------------------------------------------------------------

TWorkPeriod::~TWorkPeriod()
{

}
//-----------------------------------------------------------------------------

TWorkPeriod &TWorkPeriod::operator=(const TWorkPeriod &rp)
{
    if (&rp==this) return *this;
    TAbstractObject::operator=(rp);
    fType = rp.fType;
    fBegin = rp.fBegin;
    fEnd = rp.fEnd;
    return *this;
}
//-----------------------------------------------------------------------------

void TWorkPeriod::reset(bool thisonly)
{
    if (!thisonly) TAbstractObject::reset(true);
    fType = wptNone;
    fBegin = 0;
    fEnd = 0;
}
//-----------------------------------------------------------------------------

TWorkPeriodType TWorkPeriod::type() const
{
    return fType;
}
//-----------------------------------------------------------------------------

void TWorkPeriod::setType(TWorkPeriodType tp)
{
    fType = tp;
}
//-----------------------------------------------------------------------------

int TWorkPeriod::begin() const
{
    return fBegin;
}
//-----------------------------------------------------------------------------

void TWorkPeriod::setBegin(int dt)
{
    fBegin = dt;
}
//-----------------------------------------------------------------------------

void TWorkPeriod::setBegin(const QDate &dt)
{
    fBegin = dt.dayOfYear();
}
//-----------------------------------------------------------------------------

int TWorkPeriod::end() const
{
    return fEnd;
}
//-----------------------------------------------------------------------------

void TWorkPeriod::setEnd(int dt)
{
    fEnd = dt;
}
//-----------------------------------------------------------------------------

void TWorkPeriod::setEnd(const QDate &dt)
{
    fEnd = dt.dayOfYear();
}
//-----------------------------------------------------------------------------

QString TWorkPeriod::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TWorkPeriod::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TWorkPeriod::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TWorkPeriod::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

//*****************************************************************************

TEmployeeCalendar::TEmployeeCalendar(TCompanyCalendar *cmpclnd, int id, int n, QString nm, TAbstractObject *parent) : TAbstractObject(id,n,nm,parent), fCompanyCalendar(cmpclnd)
{
    fWorkPeriods.setAutoDelete(true);
}
//-----------------------------------------------------------------------------

TEmployeeCalendar::TEmployeeCalendar(const TEmployeeCalendar &emc) : TAbstractObject(emc), fCompanyCalendar(emc.fCompanyCalendar)
{
    fWorkPeriods.setAutoDelete(true);
    foreach (TWorkPeriod *rp,emc.fWorkPeriods) fWorkPeriods.append(new TWorkPeriod(*rp));
}
//-----------------------------------------------------------------------------

TEmployeeCalendar::~TEmployeeCalendar()
{

}
//-----------------------------------------------------------------------------

TEmployeeCalendar &TEmployeeCalendar::operator=(const TEmployeeCalendar &emc)
{
    if (&emc==this) return *this;
    TAbstractObject::operator=(emc);
    fCompanyCalendar = emc.fCompanyCalendar;
    fWorkPeriods.clear();
    foreach (TWorkPeriod *rp,emc.fWorkPeriods) fWorkPeriods.append(new TWorkPeriod(*rp));
    return *this;
}
//-----------------------------------------------------------------------------

void TEmployeeCalendar::reset(bool thisonly)
{
    if (!thisonly) TAbstractObject::reset(true);
    //fCompanyCalendar = NULL;
    fWorkPeriods.clear();
}
//-----------------------------------------------------------------------------

TWorkPeriodList &TEmployeeCalendar::workPeriods() const
{
    return const_cast<TWorkPeriodList&>(fWorkPeriods);
}
//-----------------------------------------------------------------------------

void TEmployeeCalendar::insertWorkPeriod(TWorkPeriod *rp)
{
    if (rp && fWorkPeriods.indexOf(rp)==-1) fWorkPeriods.append(rp);
}
//-----------------------------------------------------------------------------

void TEmployeeCalendar::removeWorkPeriod(TWorkPeriod *rp)
{
    if (rp) fWorkPeriods.removeOne(rp);
}
//-----------------------------------------------------------------------------

void TEmployeeCalendar::clearWorkPeriods()
{
    fWorkPeriods.clear();
}
//-----------------------------------------------------------------------------

QList<int> *TEmployeeCalendar::fullRestDates()
{
  QList<int> *res(new QList<int>);
    if (fCompanyCalendar) res->append(fCompanyCalendar->weekEnds());
    foreach (TWorkPeriod *rp,fWorkPeriods)
        for (int d=rp->begin(); d<=rp->begin(); d++)
            if (d && res->indexOf(d)==-1) res->append(d);
    if (!res->count()) DELETE(res)
    else qStableSort(res->begin(),res->end());
    return res;
}
//-----------------------------------------------------------------------------

QString TEmployeeCalendar::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TEmployeeCalendar::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TEmployeeCalendar::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TEmployeeCalendar::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
