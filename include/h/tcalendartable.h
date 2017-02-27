
//***************************************************************************//
//                                                                           //
//                         Описание TCalendarTable                           //
//                                                                           //
//***************************************************************************//

#ifndef TCALENDARTABLEH
#define TCALENDARTABLEH

#include <QTableWidget>
#include <TTableWidgetItemData>
#include <defUkpPlan>

// Таблица-календарь
class UKPPLANSHARED_EXPORT TCalendarTable : public QTableWidget
{
Q_OBJECT
public:
  struct Period
  {
  private:
      Period();
      Period(const Period &p);
      Period &operator=(const Period &p);
  public:
      Period(QString nm, int mindt, int maxdt);

      QString           name;
      int               minDate,
                        maxDate;
  };
  typedef QPtrList<Period*> PeriodList;

private:
  int                   fVisibleMonthCount, // Количество отображаемых месяцев
                        fLeftVisibleMonth; // Левый (первый) отображаемый месяц
  QList<int>            fWeekEnds;     // Выходные
  PeriodList            fPlRests;      // Нерабочие промежутки

public :
  explicit TCalendarTable(int vismnthcnt, QWidget *parent=0);

  QList<int> &weekEnds() const;        // Выходные
  void setWeekEnds(const QList<int> &we);
  PeriodList &rests() const;           // Нерабочие промежутки
  void insertRest(Period *pd);         // ... добавить
  void removeRest(Period *pd);         // ... удалить
  void clearRests();                   // ... очистить
  Period *findRest(QString nm);        // ... поиск по наименованию

protected :
  void keyReleaseEvent(QKeyEvent *ke); // Обработка события отпускания клавиши
  void mouseDoubleClickEvent(QMouseEvent *me); // Обработка события двойного клика

  void onDoubleClicked(int row, int col, Qt::KeyboardModifiers mdfrs); // Обработка двойного клика мыши или нажатия клавиши ПРОБЕЛ

public slots :
  //void zakrepFormOnTask(const TIdent &idntFrm, const TIdent &idntTsk, int kel); // Закрепить (открепить) формирование c идентификатором idntFrm за задачей ОПП c идентификатором idntTsk (kel - количество закрепленных элементов или 0)

signals :
  //void sigZakrepFormOnTask(const TIdent &idntFrm, const TIdent &idntTsk, int kel); // Закрепить (открепить) формирование c идентификатором idntFrm за задачей ОПП c идентификатором idntTsk (kel - количество закрепленных элементов или 0)

};

#endif // TCALENDARTABLEH
