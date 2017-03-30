#ifndef TGANTGRAPHICSVIEW_H
#define TGANTGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMap>
#include <QDateTime>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT TGantItem;

typedef UKPCOMMONSHARED_EXPORT QList<TGantItem*> TGantItemList;


//class TGantItem;

//typedef QList<TGantItem*> TGantItemList;

#define GANT_IND_PLAN         0 // индекс плановая работа
#define GANT_IND_REAL         1 // индекс реальная работа
#define GANT_KOL_DAY        366 //

// Элемент диаграммы
class UKPCOMMONSHARED_EXPORT TGantItem
{
private:
    TGantItem(const TGantItem &git);
    TGantItem &operator=(const TGantItem &git);

public:
    enum TGantItemType { gitProject=0, gitPlan=1, gitProcedure=2, gitWork=3 };
    enum TGantItemView { givRoundRect=0, gitRect=1 };

public:
    TGantItem(TGantItemType tp, QString nm, TGantItem *prnt=NULL, int id=0, int n=0);
    ~TGantItem();

    int id() const;                    // идентификатор
    void setId(int id);
    int num() const;                   // номер
    void setNum(int n);
    QString &name() const;             // наименование
    void setName(const QString &nm);
    TGantItemType type() const;        // тип
    void setType(TGantItemType tp);
    TGantItemView view() const;        // вид
    void setView(TGantItemView vw);
    int level() const;                 // уровень
    void setLevel(int lvl);
    bool isOpen() const;               // признак открытого узла дерева
    void setOpen(bool op);
    QDateTime begin(int ind) const;          // время начала
    void setBegin(int ind, const QDateTime &dt);
    QDateTime end(int ind) const;            // время окончания
    void setEnd(int ind, const QDateTime &dt);
    QPen &pen(int ind) const;          // перо
    void setPen(int ind, const QPen &p);
    QBrush &brush(int ind) const;      // кисть
    void setBrush(int ind, const QBrush &b);
    QString &label(int ind) const;     // кисть
    void setLabel(int ind, const QString &lbl);
    TGantItem *parent() const;         // родитель
    void setParent(TGantItem *prnt);
    TGantItemList &childs() const;     // дочерние элементы
    void insertChild(TGantItem *ch);   // ... добавление
    void removeChild(TGantItem *ch);   // ... удаление
    void clearChilds();                // ... очистка
    TGantItem *findChild(int n, bool onid=false); // ... поиск по id или по номеру
    TGantItem *findChild(const QString &nm); // ... поиск по name
    int carryOutPercent() const;       // Процент выполненной работы
    void setCarryOutPercent(int cop);

private:
    // идентификаторы
    int                 m_id,
                        m_num;
    QString             m_name;
    TGantItemType       m_type;
    TGantItemView       m_view;
    int                 m_level;
    // временные параметры (проверка установленного времени - isValid())
    QDateTime           m_begin[2],
                        m_end[2];
    // параметры отрисовки элементов
    QPen                m_pen[2];
    QBrush              m_brush[2];
    // надписи на элементах
    QString             m_label[2];
    // данные для построения иерархии
    TGantItem          *m_parent;
    TGantItemList       m_childs;
    // признак открытого узла
    bool                m_isOpen;
    // Процент выполненной работы
    int                 m_carryOutPercent;
};

// Диаграмма
class UKPCOMMONSHARED_EXPORT TGantGraphicsView : public QGraphicsView

{
    Q_OBJECT
//private:
    public:
    TGantGraphicsView(const TGantGraphicsView &git);
    TGantGraphicsView &operator=(const TGantGraphicsView &git);

public:
    enum ContentDraw { cdPlan=0, cdReal=1, cdAll=2 };
    enum ScaleView { svHour=0, svDay=1, svWeek=2, svMonth=3 };

public:
    explicit TGantGraphicsView( int year=0,  int curday=-1, QWidget *prnt=NULL); // year: если 0, то текущий год; curday: если < 0, то не отображать
    ~TGantGraphicsView();

    int year() const;                  // год графика
    void setYear(int yr);
    int currentDay() const;            // текущий день
    void setCurrentDay(int cd);
    bool isWeekend(int ind) const;     // выходные
    void setWeekend(int ind, bool isw);
    void resetWeekends();              // все false
    int headerHeight() const;          // высота заголовка, пикс.
    void setHeaderHeight(int h);
    int columnWidth() const;           // ширина колонки, пикс.
    void setColumnWidth(int w);
    int rowHeight() const;             // высота строки, пикс.
    void setRowHeight(int h);
    TGantItemList &topItems() const;   // элементы верхнего уровня
    void insertTopItem(TGantItem *ch); // ... добавление
    void removeTopItem(TGantItem *ch); // ... удаление
    void clearTopItems();              // ... очистка
    TGantItem *findTopItem(int n, bool onid=false); // ... поиск по id или по номеру
    TGantItem *findTopItem(const QString &nm); // ... поиск по name
    QPen &gridPen() const;             // перо сетки
    void setGridPen(const QPen &p);
    QBrush &gridBrush() const;         // кисть фона
    void setGridBrush(const QBrush &b);
    QPen &weekendPen() const;          // перо выходных
    void setWeekendPen(const QPen &p);
    QBrush &weekendBrush() const;      // кисть выходных
    void setWeekendBrush(const QBrush &b);
    ScaleView scaleView() const;       // Вид шкалы
    void setScaleView(ScaleView sv);

    void prepare(int hdrH, int colW, int rowH); // ??? подготовить диаграмму
    void draw(ContentDraw cd);         // отрисовать диаграмму
    //void draw_time(ContentDraw cd);         // отрисовать диаграмму
    void moveToDay(int day);           // прокрутить диаграмму, чтобы первым был day [0..GANT_KOL_DAY)
    TGantItem *findItem(const QStringList &slitnms); // глобальный поиск элементов (в элементах верхнего уровня и во всех вложенных)
    void setOpen(TGantItem *it, bool isop); // раскрытие (сворачивание) элементов
    void show();

public slots:
     void ScrollVert(int value);    // получение сигнала вертикальной прокрутки ( QAbstractSlider::sliderMoved(int value) ) от синхронизируемого элемента
     void ScrollHoriz(int value);
     void ScrollHoriz_T(int value);
     void ScrollHoriz_P(int value);
     void ScrollVert_P(int value);
     void set_scrollbarVert(QScrollBar *qscrollbarVertgvPlanTree_);
     void set_scrollbarHoriz(QScrollBar *qscrollbarHorizPlan_);
     void redraw();         // переотрисовка диаграмму
     void newWindow();

protected:
      void resizeEvent(QResizeEvent *event);//
      //void paintEvent(QPaintEvent *event);
private:

    QTimer *timer;

    QWidget *prnt_gv;
    QGraphicsScene *pscene_gv;

    // параметры календаря
    int                 m_year,
                        m_currentDay;
    bool                m_weekends[ GANT_KOL_DAY ];
    // размеры ячеек и заголовка
    int                 m_headerHeight,
                        m_columnWidth,
                        m_rowHeight;
    // элементы диаграммы верхнего уровня (m_parent==0)
    TGantItemList       m_topItems;
    // для удаления Item
   // TGantItemList       m_items_for_del ; // ???
    // параметры отрисовки сетки
    QPen                m_gridPen,
                        m_weekendPen;
    QBrush              m_gridBrush,
                        m_weekendBrush;
    // данные заполнения заголовка
    int                 m_dates[ GANT_KOL_DAY ];
    int                 m_monthStarts[ 12 ]; // статровый день месяца [ 0..GANT_KOL_DAY )
    QString             m_monthLabels[ 12 ]; // наименования месяцев
    // общие параметры отрисовки
    ContentDraw         m_contentDraw;
    ScaleView           m_scaleView;

    //начальная отрисовка (для появления ползунка)
    bool nachalo;

    void fillHeaderDatas(); // заполнить данные заголовка ( m_dates, m_monthStarts )
    void newPlan();
    void deletePlan();

    //.........................................................................

    int  get_kol_items(TGantItem *topItems);
    void get_kol_curr(TGantItem *topItems);
    void set_open(TGantItem  *items, bool op);
    QGraphicsRectItem *draw_rec(qreal x, qreal y, qreal w, qreal h,  QPen &pen , QBrush &brush);
    QGraphicsRectItem *draw_rec_tbl(qreal x, qreal y, qreal w, qreal h,  QPen &pen , QBrush &brush);
    void  draw_curr_items(TGantItem *topItems);
    void  draw_curr_items_n(TGantItem *topItems);
    void  draw_txt(qreal x, qreal y, qreal w, QString txt);

    QGraphicsView  *gvPlan;
    QGraphicsView  *gvTable;
    QGraphicsScene *sceneGant;
    QGraphicsScene *sceneTable;
    QGraphicsScene *scene;
    QGraphicsView  *gv;

    //QScrollBar *qscrollbarVertgvPlanTree;
    QScrollBar *qscrollbarHorgvTable;
    QScrollBar *qscrollbarVertgvPlan;
    QScrollBar *qscrollbarHorgvPlan;

    QScrollBar *qscrollbarHoriz;
    QScrollBar *qscrollbarVert;

    bool Vert;
    bool Vert_P;

    int kol_curr;
    int rowcount_all;
    int dob ;

    int counter;

    //bool first_resizeEvent;
    // календарь
    QMap<int,QString>  months;
    QMap<int,int>  days;

    QPoint p;

//public:
//    QGraphicsView  *gv;

 signals:

    void start_newPlan();

};

#endif // TGANTGRAPHICSVIEW_H
