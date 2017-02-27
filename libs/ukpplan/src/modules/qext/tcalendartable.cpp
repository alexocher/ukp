#include <TCalendarTable>

TCalendarTable::Period::Period(QString nm, int mindt, int maxdt) : name(nm), minDate(mindt), maxDate(maxdt)
{
}
//-----------------------------------------------------------------------------

TCalendarTable::TCalendarTable(int vismnthcnt, QWidget *parent) : QTableWidget(parent), fVisibleMonthCount(vismnthcnt), fLeftVisibleMonth(1)
{
    fPlRests.setAutoDelete(true);
}
//-----------------------------------------------------------------------------

QList<int> &TCalendarTable::weekEnds() const
{
    return const_cast<QList<int>&>(fWeekEnds);
}
//-----------------------------------------------------------------------------

void TCalendarTable::setWeekEnds(const QList<int> &we)
{
    fWeekEnds = we;
}
//-----------------------------------------------------------------------------

TCalendarTable::PeriodList &TCalendarTable::rests() const
{
    return const_cast<TCalendarTable::PeriodList&>(fPlRests);
}
//-----------------------------------------------------------------------------

void TCalendarTable::insertRest(TCalendarTable::Period *pd)
{
    if (pd && fPlRests.indexOf(pd)==-1) fPlRests.append(pd);
}
//-----------------------------------------------------------------------------

void TCalendarTable::removeRest(TCalendarTable::Period *pd)
{
    if (pd) fPlRests.removeOne(pd);
}
//-----------------------------------------------------------------------------

void TCalendarTable::clearRests()
{
    fPlRests.clear();
}
//-----------------------------------------------------------------------------

TCalendarTable::Period *TCalendarTable::findRest(QString nm)
{
    foreach (TCalendarTable::Period *pd,fPlRests)
        if (pd->name==nm) return pd;
    return NULL;
}
//-----------------------------------------------------------------------------

void TCalendarTable::keyReleaseEvent(QKeyEvent *ke)
{

}
//-----------------------------------------------------------------------------

void TCalendarTable::mouseDoubleClickEvent(QMouseEvent *me)
{

}
//-----------------------------------------------------------------------------

void TCalendarTable::onDoubleClicked(int row, int col, Qt::KeyboardModifiers mdfrs)
{

}
//-----------------------------------------------------------------------------
