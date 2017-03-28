
#include <iostream>
#include <QDate>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QTextDocument>
#include <QScrollBar>
#include <QSlider>
#include <TGantGraphicsView>
#include <QGridLayout>
#include <QStack>
#include <QTimer>
//#include "windows.h"

//#define DEBUG_INFO

//*****************************************************************************
// Элемент диаграммы - TGantItem

TGantItem::TGantItem(TGantItem::TGantItemType tp, QString nm, TGantItem *prnt, int id, int n) : m_id(id), m_num(n), m_name(nm), m_type(tp), m_view(gitRect), m_level(0), m_parent(prnt), m_isOpen(false)
{
    m_begin[0] = m_begin[1] = 0;
    m_end[0] = m_end[1] = 0;
    m_label[0] = m_label[1] = "";
}

//-----------------------------------------------------------------------------

TGantItem::~TGantItem()
{
    foreach (TGantItem *it,m_childs)
        if (it) delete it;
}
//-----------------------------------------------------------------------------

int TGantItem::id() const
{
    return m_id;
}
//-----------------------------------------------------------------------------

void TGantItem::setId(int id)
{
    m_id = id;
}
//-----------------------------------------------------------------------------

int TGantItem::num() const
{
    return m_num;
}
//-----------------------------------------------------------------------------

void TGantItem::setNum(int n)
{
    m_num = n;
}
//-----------------------------------------------------------------------------

QString &TGantItem::name() const
{
    return const_cast<QString&>(m_name);
}
//-----------------------------------------------------------------------------

void TGantItem::setName(const QString &nm)
{
    m_name = nm;
}
//-----------------------------------------------------------------------------

TGantItem::TGantItemType TGantItem::type() const
{
    return m_type;
}
//-----------------------------------------------------------------------------

void TGantItem::setType(TGantItem::TGantItemType tp)
{
    m_type = tp;
}
//-----------------------------------------------------------------------------

TGantItem::TGantItemView TGantItem::view() const
{
    return m_view;
}
//-----------------------------------------------------------------------------

void TGantItem::setView(TGantItem::TGantItemView vw)
{
    m_view = vw;
}
//-----------------------------------------------------------------------------

int TGantItem::level() const
{
    return m_level;
}
//-----------------------------------------------------------------------------

void TGantItem::setLevel(int lvl)
{
    m_level = lvl;
}
//-----------------------------------------------------------------------------

bool TGantItem::isOpen() const
{
    return m_isOpen;
}
//-----------------------------------------------------------------------------

void TGantItem::setOpen(bool op)
{
    m_isOpen = op;
}
//-----------------------------------------------------------------------------

int TGantItem::begin(int ind) const
{
  int curInd(GANT_IND_PLAN);
    if (ind==GANT_IND_PLAN || ind==GANT_IND_REAL) curInd = ind;
    return m_begin[curInd];
}
//-----------------------------------------------------------------------------

void TGantItem::setBegin(int ind, int val)
{
  int curInd(GANT_IND_PLAN);
    if (ind==GANT_IND_PLAN || ind==GANT_IND_REAL) curInd = ind;
    m_begin[curInd] = val;
}
//-----------------------------------------------------------------------------

int TGantItem::end(int ind) const
{
  int curInd(GANT_IND_PLAN);
    if (ind==GANT_IND_PLAN || ind==GANT_IND_REAL) curInd = ind;
    return m_end[curInd];
}
//-----------------------------------------------------------------------------

void TGantItem::setEnd(int ind, int val)
{
  int curInd(GANT_IND_PLAN);
    if (ind==GANT_IND_PLAN || ind==GANT_IND_REAL) curInd = ind;
    m_end[curInd] = val;
}
//-----------------------------------------------------------------------------

QPen &TGantItem::pen(int ind) const
{
  int curInd(GANT_IND_PLAN);
    if (ind==GANT_IND_PLAN || ind==GANT_IND_REAL) curInd = ind;
    return const_cast<QPen&>(m_pen[curInd]);
}
//-----------------------------------------------------------------------------

void TGantItem::setPen(int ind, const QPen &p)
{
  int curInd(GANT_IND_PLAN);
    if (ind==GANT_IND_PLAN || ind==GANT_IND_REAL) curInd = ind;
    m_pen[curInd] = p;
}
//-----------------------------------------------------------------------------

QBrush &TGantItem::brush(int ind) const
{
  int curInd(GANT_IND_PLAN);
    if (ind==GANT_IND_PLAN || ind==GANT_IND_REAL) curInd = ind;
    return const_cast<QBrush&>(m_brush[curInd]);
}
//-----------------------------------------------------------------------------

void TGantItem::setBrush(int ind, const QBrush &b)
{
  int curInd(GANT_IND_PLAN);
    if (ind==GANT_IND_PLAN || ind==GANT_IND_REAL) curInd = ind;
    m_brush[curInd] = b;
}
//-----------------------------------------------------------------------------

QString &TGantItem::label(int ind) const
{
  int curInd(GANT_IND_PLAN);
    if (ind==GANT_IND_PLAN || ind==GANT_IND_REAL) curInd = ind;
    return const_cast<QString&>(m_label[curInd]);
}
//-----------------------------------------------------------------------------

void TGantItem::setLabel(int ind, const QString &lbl)
{
  int curInd(GANT_IND_PLAN);
    if (ind==GANT_IND_PLAN || ind==GANT_IND_REAL) curInd = ind;
    m_label[curInd] = lbl;
}
//-----------------------------------------------------------------------------

TGantItem *TGantItem::parent() const
{
    return m_parent;
}
//-----------------------------------------------------------------------------

void TGantItem::setParent(TGantItem *prnt)
{
    m_parent = prnt;
}
//-----------------------------------------------------------------------------

TGantItemList &TGantItem::childs() const
{
    return const_cast<TGantItemList&>(m_childs);
}
//-----------------------------------------------------------------------------

void TGantItem::insertChild(TGantItem *ch)
{
    if (ch && m_childs.indexOf(ch)==-1) m_childs.append(ch);
}
//-----------------------------------------------------------------------------

void TGantItem::removeChild(TGantItem *ch)
{
    if (ch && m_childs.indexOf(ch)>-1)
    {
        m_childs.removeOne(ch);
        delete ch;
    }
}
//-----------------------------------------------------------------------------

void TGantItem::clearChilds()
{
    foreach (TGantItem *it,m_childs)
        if (it) delete it;
    m_childs.clear();
}
//-----------------------------------------------------------------------------

TGantItem *TGantItem::findChild(int n, bool onid)
{
    foreach (TGantItem *it,m_childs)
        if (onid)
        {
            if (it->id()==n) return it;
        }
        else if (it->num()==n) return it;
    return NULL;
}
//-----------------------------------------------------------------------------

TGantItem *TGantItem::findChild(const QString &nm)
{
    foreach (TGantItem *it,m_childs)
        if (it->name()==nm) return it;
    return NULL;
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// Диаграмма - TGantGraphicsView


TGantGraphicsView::TGantGraphicsView(int year, int curday, QWidget *prnt) :
    QGraphicsView(prnt),
    m_year(year ? year : QDate::currentDate().year()), m_currentDay(curday), m_headerHeight(0), m_columnWidth(0), m_rowHeight(0), m_contentDraw(cdAll)
{
//TGantGraphicsView::TGantGraphicsView(int year, int curday, QGraphicsScene *pscene, QWidget *prnt) :
//      QGraphicsView(pscene, prnt),
//      m_year(year ? year : QDate::currentDate().year()), m_currentDay(curday), m_headerHeight(0), m_columnWidth(0), m_rowHeight(0), m_contentDraw(cdAll)
//{
    for (int i=0; i<GANT_KOL_DAY; i++)
    {
        m_dates[i] = 0;
        m_weekends[i] = false;
    }
    m_monthLabels[ 0] = "Январь";
    m_monthLabels[ 1] = "Февраль";
    m_monthLabels[ 2] = "Март";
    m_monthLabels[ 3] = "Апрель";
    m_monthLabels[ 4] = "Май";
    m_monthLabels[ 5] = "Июнь";
    m_monthLabels[ 6] = "Июль";
    m_monthLabels[ 7] = "Август";
    m_monthLabels[ 8] = "Сентябрь";
    m_monthLabels[ 9] = "Октябрь";
    m_monthLabels[10] = "Ноябрь";
    m_monthLabels[11] = "Декабрь";

     prnt_gv = prnt;
     //qscrollbarVertgvPlanTree=NULL;

#ifdef DEBUG_INFO
    std::cerr << " data in end " << std::endl;
#endif

#ifdef DEBUG_INFO
    std::cerr << " QGraphicsView  end  year = " << m_year<< std::endl;
#endif

    qscrollbarHoriz = NULL;
    qscrollbarVert = NULL;

    sceneTable=NULL;
    gvTable=NULL;
    sceneGant=NULL;
    gvPlan=NULL;
    scene=NULL;
    gv=NULL;

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this, SLOT(redraw()));
    timer->start(100);

    counter =0;
    connect(this,SIGNAL(start_newPlan()),this, SLOT(newWindow()));


}
//-----------------------------------------------------------------------------

TGantGraphicsView::~TGantGraphicsView()
{
    foreach (TGantItem *it,m_topItems)
        if (it) delete it;

    deletePlan();
}

//-----------------------------------------------------------------------------
void TGantGraphicsView::newPlan(){

    std::cerr << " ! ==============newPlan()=============== ! " << std::endl;

    int kolDays(GANT_KOL_DAY);
      if (m_dates[GANT_KOL_DAY-1]==0) kolDays = GANT_KOL_DAY-1;

    int headerHeight(m_headerHeight);
    int rowcount(0);
      kol_curr = 0;
    TGantItemList &items = m_topItems;
    TGantItemList::iterator it2;
      for (it2=items.begin(); it2!=items.end(); ++it2)
      {
        TGantItem *curr = *it2;
          rowcount = rowcount+get_kol_items(curr);
          get_kol_curr(curr);
      }

      rowcount =kol_curr;
      rowcount_all =kol_curr;

  #ifdef DEBUG_INFO
      std::cerr << " ==============rowcount=============== " << rowcount<< std::endl;
  #endif
    if (rowcount==0) rowcount=1;

    int colDaysWidth(m_columnWidth);

  #ifdef DEBUG_INFO
      std::cerr << " m_columnWidth new = " << m_columnWidth << std::endl;
      std::cerr << " m_headerHeight new = " << m_headerHeight << std::endl;
      std::cerr << " m_rowHeight new = " << m_rowHeight << std::endl;
      std::cerr << " kolDays new = " << kolDays<< std::endl;
      int totalHeight(m_rowHeight*rowcount+headerHeight);
      std::cerr << " totalHeight new = " << totalHeight << std::endl;
  #endif

      //int dob(0);
      dob = 0;
      if (qscrollbarVert){
         //dob = this ->height() - headerHeight -  qscrollbarVert->pageStep()*m_rowHeight;
          int height(this ->height());
          //int h_v (qscrollbarVertgvPlan->height());
           int h_v (this->horizontalScrollBar()->height());
          //h_v=0;//
          int kol =(int) (((float)(height - headerHeight - h_v)) /m_rowHeight);
          dob = height - headerHeight  -  kol*m_rowHeight;

      }
  #ifdef DEBUG_INFO
       std::cerr << " !!!!!!!    dob new = " << dob << std::endl;
   #endif

      //int slayder(m_rowHeight/2);
      int slayder(dob);
      int slayder_gor(m_columnWidth);

  if (prnt_gv==NULL)
  {
      //sceneTable = new QGraphicsScene(QRectF(0,0,colDaysWidth*kolDays+slayder_gor,headerHeight));
      sceneTable = new QGraphicsScene();//QRectF(0,0,colDaysWidth*kolDays+slayder_gor,headerHeight));
      //sceneTable = new QGraphicsScene(this);
      gvTable = new QGraphicsView(sceneTable);
      //gvTable = new QGraphicsView(prnt);
      //sceneGant = new QGraphicsScene(QRectF(0,0,colDaysWidth*kolDays+slayder_gor,m_rowHeight*rowcount+slayder));
      sceneGant = new QGraphicsScene();
      //sceneGant = new QGraphicsScene(this);
      gvPlan = new QGraphicsView(sceneGant);
      //gvPlan = new QGraphicsView(prnt);
      //scene = new QGraphicsScene(QRectF(0,0,colDaysWidth*kolDays+slayder_gor,headerHeight+m_rowHeight*rowcount+slayder));
      scene = new QGraphicsScene();
      /*
      //sceneTable = new QGraphicsScene(this);
      sceneTable = new QGraphicsScene();
      gvTable = new QGraphicsView(sceneTable);
      //sceneGant = new QGraphicsScene(this);
       sceneGant = new QGraphicsScene();
      gvPlan = new QGraphicsView(sceneGant);
      //scene = new QGraphicsScene(this);
      scene = new QGraphicsScene();
      */
      gv = new QGraphicsView(scene);

  }
  else
  {
      //sceneTable = new QGraphicsScene(QRectF(0,0,colDaysWidth*kolDays+slayder_gor,headerHeight));
      sceneTable = new QGraphicsScene();
      //sceneTable = new QGraphicsScene(this);
      gvTable = new QGraphicsView(sceneTable);
      //gvTable = new QGraphicsView(prnt);
      //sceneGant = new QGraphicsScene(QRectF(0,0,colDaysWidth*kolDays+slayder_gor,m_rowHeight*rowcount+slayder));
      sceneGant = new QGraphicsScene();
      //sceneGant = new QGraphicsScene(this);
      gvPlan = new QGraphicsView(sceneGant);
      //gvPlan = new QGraphicsView(prnt);
      //scene = new QGraphicsScene(QRectF(0,0,colDaysWidth*kolDays+slayder_gor,headerHeight+m_rowHeight*rowcount+slayder));
      scene = new QGraphicsScene();
      //scene = pscene_gv;
      //scene = new QGraphicsScene(this);
      //gv = new QGraphicsView(scene, prnt_gv);
      gv = new QGraphicsView(scene,this);
      //gv = new QGraphicsView(scene);//,this);
      //gv->setGeometry(this->geometry());
      //prnt_gv->layout();
      //this->layout();
      //QBoxLayout *pb = new QBoxLayout(QBoxLayout::LeftToRight);
      //gv->layout();

     //gv->setViewport(prnt_gv);

    /*
    QVBoxLayout *pb = new QVBoxLayout;
    pb->addWidget(gv);
    pb->setGeometry(this->geometry());
    //prnt_gv->setLayout(pb);
    this->setLayout(pb);
    */
  }

  //sceneGant->setSceneRect (QRectF(0,0,colDaysWidth*kolDays+slayder_gor,m_rowHeight*rowcount+slayder));// на слайдер
  //!!!!
  sceneGant->setSceneRect (QRectF(0,0,colDaysWidth*kolDays+slayder_gor,m_rowHeight*rowcount + slayder));//
  //sceneGant->setSceneRect (QRectF(0,0,colDaysWidth*kolDays+slayder_gor,m_rowHeight*rowcount ));//
  //std::cerr << " Вертикаль 1" << std::endl;
  //!!!
  sceneTable->setSceneRect(QRectF(0,0,colDaysWidth*kolDays+slayder_gor,headerHeight));//
  //std::cerr << " Вертикаль 2" << std::endl;
  //scene->setSceneRect(QRectF(0,0,colDaysWidth*kolDays+slayder_gor,headerHeight+m_rowHeight*rowcount+slayder));// на слайдер
  //!!!
  scene->setSceneRect(QRectF(0,0,colDaysWidth*kolDays+slayder_gor,headerHeight+m_rowHeight*rowcount + slayder));//
  //scene->setSceneRect(QRectF(0,0,colDaysWidth*kolDays+slayder_gor,headerHeight+m_rowHeight*rowcount ));//

  //int w=this->width();
  //int h=this->height();//->width();
  //scene->setSceneRect(QRectF(0,0,w,h));// на слайдер

  //gvPlan->setScene(sceneGant);
  //gvTable->setScene(sceneTable);
  //gv->setScene(scene);

  //gv->setViewport(this);

  sceneGant->setBackgroundBrush(m_gridBrush);
  sceneTable->setBackgroundBrush(m_gridBrush);

  gv->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  gvPlan->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  gvTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  gvTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  gvTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  gvPlan->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  gvPlan->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

 // qscrollbarVertgvPlanTree=NULL;
  qscrollbarVertgvPlan = gvPlan->verticalScrollBar();
  qscrollbarHorgvPlan  = gvPlan->horizontalScrollBar();
  qscrollbarHorgvTable = gvTable->horizontalScrollBar();

  connect(qscrollbarHorgvPlan, SIGNAL(valueChanged(int)),
         qscrollbarHorgvTable, SLOT(setValue(int)));
  connect(qscrollbarHorgvTable, SIGNAL(valueChanged(int)),
          qscrollbarHorgvPlan, SLOT(setValue(int)));

  //!!!!!!!! обязательно
  //qscrollbarHoriz = NULL;
  //qscrollbarVert = NULL;

  if (prnt_gv){

    gvPlan->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gvPlan->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

#ifdef DEBUG_INFO
    int w = gv->width();//->sliderPosition();//->SliderPosition();
    std::cerr << " !!!! gv->width() BEFOR =  " << w<<std::endl;
    int h = gv->height();//->width();//->sliderPosition();//->SliderPosition();
    std::cerr << " !!!! gv->height() BEFOR =  " << h<<std::endl;
#endif

   }

  int height (this->height());
  int width (this->width());

  int ww (gv->width());//->sliderPosition();//->SliderPosition();
  int hh (gv->height());//->width();
  int min_w = gv->minimumWidth(); int max_w = gv->maximumWidth();// minimumWidth ()
  int min_h = gv->minimumHeight(); int max_h = gv->maximumHeight();// minimumWidth ()

#ifdef DEBUG_INFO
  std::cerr << " !!!! this->width()   BEFOR =  " <<  this->width() <<std::endl;
  std::cerr << " !!!! this->height()  BEFOR =  " << this->height() <<std::endl;
  std::cerr << " !!!! gv->width()     BEFOR =  " << ww<<std::endl;
  std::cerr << " !!!! gv->height()    BEFOR =  " << hh<<std::endl;
  std::cerr << " !!!! gv->minimumWidth() BEFOR =  " << min_w <<std::endl;
  std::cerr << " !!!! gv->maximumWidth() BEFOR =  " << max_w <<std::endl;
  std::cerr << " !!!! gv->minimumHeight() BEFOR =  " << min_h <<std::endl;
  std::cerr << " !!!! gv->maximumHeight() BEFOR =  " << max_h <<std::endl;
#endif

  //gv->viewport()->setFixedSize(this->width() + 1 ,  this->height() + 1);
  //gvTable->viewport()->setFixedSize(this->width() + 1 ,  this->height() + 1);
  //gvPlan->viewport()->setFixedSize(this->width() + 1 ,  this->height() + 1);

  int hh_new(headerHeight + m_rowHeight*rowcount);
  //int hh_new( height);
  int ww_new(width);

  gv->resize( ww_new,hh_new);
  gvTable->resize( ww_new,headerHeight);
  gvPlan->resize( ww_new,hh_new - headerHeight);

  std::cerr << " !!!! gv->width()     AFT  =  " << gv->width()<<std::endl;
  std::cerr << " !!!! gv->height()    AFT  =  " << gv->height()<<std::endl;

  if (prnt_gv){

    if (ww <= width  )
     ww_new = width + 1;
    if (hh <= height && headerHeight + m_rowHeight*rowcount >  height )
     hh_new = height + 1;

    //gv->setMaximumHeight(hh_new);
    //gv->setMinimumHeight(hh_new);
    //gv->setMaximumWidth(ww_new);
    //gv->setMinimumWidth(ww_new);
    if (headerHeight + m_rowHeight*rowcount >  height ) {
       gv->resize( ww_new,hh_new);
       gvTable->resize( ww_new,headerHeight);
       gvPlan->resize( ww_new,hh_new - headerHeight);
       std::cerr << " !!!! gv->width()     AFT resize =  " << gv->width()<<std::endl;
       std::cerr << " !!!! gv->height()    AFT resize =  " << gv->height()<<std::endl;
    }
     nachalo = true;

  } 

  //!!!! не работает на ползунок
  //this->setMinimumWidth(gv->viewport()->width()+ 1 );
  //this->setMinimumHeight(gv->viewport()->height()+ 1 );
  // (->resize (gv->viewport()->width()+ 1, gv->viewport()->height()+ 1 );
  //this->resize (gv->viewport()->width()+ 1, gv->viewport()->height()+ 1 );

}
//-----------------------------------------------------------------------------
void TGantGraphicsView::deletePlan(){

    std::cerr << " !!!!=================== deletePlan()=================  "<<std::endl;

    if (gvTable) {
        delete gvTable;
        gvTable=NULL;
#ifdef DEBUG_INFO
        std::cerr << " !!!!=================== deletePlan() gvTable============  " <<std::endl;
#endif
    }

    if (sceneTable) {
        delete sceneTable;
        sceneTable=NULL;
#ifdef DEBUG_INFO
        std::cerr << " !!!!=================== deletePlan() sceneTable============  " <<std::endl;
#endif
    }

    if (gvPlan){
        delete gvPlan;
        gvPlan=NULL;
#ifdef DEBUG_INFO
         std::cerr << " !!!!=================== deletePlan() gvPlan=============  " <<std::endl;
#endif
    }

    if (sceneGant){
        delete  sceneGant;
        sceneGant=NULL;
#ifdef DEBUG_INFO
         std::cerr << " !!!!=================== deletePlan() scenePlan=============  " <<std::endl;
#endif
    }

    if (gv){
         delete gv;
         gv=NULL;
#ifdef DEBUG_INFO
         std::cerr << " !!!!=================== deletePlan() GV================  " <<std::endl;
#endif
    }

    if (scene) {
         delete scene;
         scene=NULL;
#ifdef DEBUG_INFO
         std::cerr << " !!!!=================== deletePlan() sceneGV================  " <<std::endl;
#endif
    }

}

//-----------------------------------------------------------------------------

int TGantGraphicsView::year() const
{
    return m_year;
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::setYear(int yr)
{
    m_year = yr ? yr : QDate::currentDate().year();
    fillHeaderDatas();
}
//-----------------------------------------------------------------------------

int TGantGraphicsView::currentDay() const
{
    return m_currentDay;
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::setCurrentDay(int cd)
{
    m_currentDay = cd;
    draw(m_contentDraw);
}
//-----------------------------------------------------------------------------

bool TGantGraphicsView::isWeekend(int ind) const
{
    if (ind<0 && ind>(GANT_KOL_DAY-1)) return false;
    return m_weekends[ind];
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::setWeekend(int ind, bool isw)
{
    if (ind<0 && ind>(GANT_KOL_DAY-1)) return;
    m_weekends[ind] = isw;
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::resetWeekends()
{
    for (int i=0; i<GANT_KOL_DAY; i++) m_weekends[i] = false;
}
//-----------------------------------------------------------------------------

int TGantGraphicsView::headerHeight() const
{
    return m_headerHeight;
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::setHeaderHeight(int h)
{
    m_headerHeight = h;
}
//-----------------------------------------------------------------------------

int TGantGraphicsView::columnWidth() const
{
    return m_columnWidth;
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::setColumnWidth(int w)
{
    m_columnWidth = w;
}
//-----------------------------------------------------------------------------

int TGantGraphicsView::rowHeight() const
{
    return m_rowHeight;
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::setRowHeight(int h)
{
    m_rowHeight = h;
}
//-----------------------------------------------------------------------------

TGantItemList &TGantGraphicsView::topItems() const
{
    return const_cast<TGantItemList&>(m_topItems);
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::insertTopItem(TGantItem *ch)
{
    if (ch && m_topItems.indexOf(ch)==-1) m_topItems.append(ch);
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::removeTopItem(TGantItem *ch)
{
    if (ch && m_topItems.indexOf(ch)>-1)
    {
        m_topItems.removeOne(ch);
        delete ch;
    }
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::clearTopItems()
{
    foreach (TGantItem *it,m_topItems)
        if (it) delete it;
    m_topItems.clear();
}
//-----------------------------------------------------------------------------

TGantItem *TGantGraphicsView::findTopItem(int n, bool onid)
{
    foreach (TGantItem *it,m_topItems)
        if (onid)
        {
            if (it->id()==n) return it;
        }
        else if (it->num()==n) return it;
    return NULL;
}
//-----------------------------------------------------------------------------

TGantItem *TGantGraphicsView::findTopItem(const QString &nm)
{
    foreach (TGantItem *it,m_topItems)
        if (it->name()==nm) return it;
    return NULL;
}
//-----------------------------------------------------------------------------

QPen &TGantGraphicsView::gridPen() const
{
    return const_cast<QPen&>(m_gridPen);
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::setGridPen(const QPen &p)
{
    m_gridPen = p;
}
//-----------------------------------------------------------------------------

QBrush &TGantGraphicsView::gridBrush() const
{
    return const_cast<QBrush&>(m_gridBrush);
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::setGridBrush(const QBrush &b)
{
    m_gridBrush = b;
}
//-----------------------------------------------------------------------------

QPen &TGantGraphicsView::weekendPen() const
{
    return const_cast<QPen&>(m_weekendPen);
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::setWeekendPen(const QPen &p)
{
    m_weekendPen = p;
}
//-----------------------------------------------------------------------------

QBrush &TGantGraphicsView::weekendBrush() const
{
    return const_cast<QBrush&>(m_weekendBrush);
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::setWeekendBrush(const QBrush &b)
{
    m_weekendBrush = b;
}
//-----------------------------------------------------------------------------

void TGantGraphicsView::prepare(int hdrH, int colW, int rowH)
{
    m_headerHeight = hdrH;
    m_columnWidth = colW;
    m_rowHeight = rowH;
}

//-----------------------------------------------------------------------------
void TGantGraphicsView::moveToDay(int day)
{
    m_currentDay = day;
    // ??? draw(m_contentDraw);
}
//-----------------------------------------------------------------------------
TGantItem *TGantGraphicsView::findItem(const QStringList &slitnms)
{
    if (slitnms.count())
        if (TGantItem *topIt = findTopItem(slitnms.at(0)))
        {
            if (slitnms.count()==1) return topIt;
            for (int i=1; i<slitnms.count(); i++)
            {
              const QString &str = slitnms.at(i);
                if (TGantItem *curIt = topIt->findChild(str))
                {
                    if (i==slitnms.count()-1) return curIt;
                    else topIt = curIt;
                }
            }
        }
    return NULL;
}
//-----------------------------------------------------------------------------
void TGantGraphicsView::setOpen(TGantItem *it, bool isop)
{
    // isop==false - it остается видимым, все вложенные элементы скрываются
    // isop==true  - it его m_childs остаются видимым, m_childs[i]->m_childs - в соответствии с m_childs[i]->m_isOpen, и так в глубину
    if (it) set_open(it,isop);
}
//-----------------------------------------------------------------------------
void TGantGraphicsView::ScrollVert(int value)
{
    if (Vert) return;
    if (qscrollbarVert==NULL) return;
    int min_  = qscrollbarVert->minimum();//
    int max_  = qscrollbarVert->maximum();//
    //int pagestep_ = qscrollbarVert->pageStep();
    // qscrollbarVert->setMaximum(max_ + qscrollbarVert->pageStep());

    int min  = qscrollbarVertgvPlan->minimum();//
    int max  = qscrollbarVertgvPlan->maximum();
    //int pagestep = qscrollbarVertgvPlan->pageStep();

    float k =0;

    if (max - min  >0 && max_ - min_ >0){
        k= (((float)(max-min ))/(max_-min_ ));
        // k= (((max-min ))/(max_-min_ ));
    }

     int rez =(int)(k * (value - min_ ) + min );
     //if (k * (value - min_ ) + min - rez>0.5) rez= rez+1;
#ifdef DEBUG_INFO
    //std::cerr << " !!! value = " << value << std::endl;
    //std::cerr << " !!! qscrollbarVertgvPlan max = " << max << std::endl;
    //std::cerr << " !!! qscrollbarVertgvPlan min = " << min << std::endl;
    //std::cerr << " !!! qscrollbarVert max = " << max_ << std::endl;
    //std::cerr << " !!! qscrollbarVert min = " << min_ << std::endl;
    //std::cerr << " !!! k = " << k << std::endl;
    //std::cerr << " !!! rez = " << rez<< std::endl;
#endif

     //disconnect(qscrollbarVertgvPlan,SIGNAL(valueChanged(int)),this,SLOT(ScrollVert_P(int)));
      Vert_P =true;
     qscrollbarVertgvPlan->setSliderPosition(rez);
      Vert_P =false;
     //connect(qscrollbarVertgvPlan,SIGNAL(valueChanged(int)),this,SLOT(ScrollVert_P(int)));

}

void TGantGraphicsView::ScrollVert_P(int value)
{
    if (Vert_P) return;
    if (qscrollbarVert==NULL) return;

    int min_  = qscrollbarVertgvPlan->minimum();//
    int max_  = qscrollbarVertgvPlan->maximum();//

    //int page_vp_ = qscrollbarVertgvPlan->pageStep();
    //qscrollbarVertgvPlan->setMaximum(m_rowHeight*rowcount   - qscrollbarVertgvPlan->pageStep() );
    //max_ = m_rowHeight*rowcount_all - qscrollbarVertgvPlan->pageStep();

    int min  = qscrollbarVert->minimum();//
    int max  = qscrollbarVert->maximum();

    float k =0;
    //if (max - min + pagestep >0 && max_ - min_ + pagestep_>0){
    //    k= (((float)(max-min + pagestep))/(max_-min_ + pagestep_));
    //}

    if (max - min  >0 && max_ - min_ >0){
        k= (((float)(max-min ))/(max_-min_ ));
        // k= (((max-min ))/(max_-min_ ));
    }

     int rez =(int)(k * (value - min_ ) + min );
     //if (k * (value - min_ ) + min - rez>0.5) rez= rez+1;
#ifdef DEBUG_INFO
    //std::cerr << " !!! value = " << value << std::endl;
    //std::cerr << " !!! qscrollbarVertgvPlan max = " << max << std::endl;
    //std::cerr << " !!! qscrollbarVertgvPlan min = " << min << std::endl;
    //std::cerr << " !!! qscrollbarVert max = " << max_ << std::endl;
    //std::cerr << " !!! qscrollbarVert min = " << min_ << std::endl;
    //std::cerr << " !!! k = " << k << std::endl;
    //std::cerr << " !!! rez = " << rez<< std::endl;
#endif

     //disconnect(qscrollbarVert,SIGNAL(valueChanged(int)),this,SLOT(ScrollVert(int)));
     //Vert =true;
     qscrollbarVert->setSliderPosition(rez);
     Vert =false;
     //connect(qscrollbarVert,SIGNAL(valueChanged(int)),this,SLOT(ScrollVert(int)));

}
//-----------------------------------------------------------------------------
void TGantGraphicsView::ScrollHoriz(int value)
{

    if (qscrollbarHoriz==NULL) return;
    int min_  = qscrollbarHoriz->minimum();//->setMinimum(0);
    int max_  = qscrollbarHoriz->maximum();//->minimax();
    //qscrollbarHoriz->setMaximum(max_ + qscrollbarHoriz->pageStep());

    int min  = qscrollbarHorgvPlan->minimum();//
    int max  = qscrollbarHorgvPlan->maximum();

    float k =0;
    if (max-min>0 && max_ - min_ >0){
      //  k= (max-min)/(max_-min_);
        k= (((float)(max-min))/(max_-min_));
    }

    //qscrollbarHorgvPlan->setMaximum(colDaysWidth*kolDays+slayder_gor);

//#ifdef DEBUG_INFO
//    int position = qscrollbarHorgvPlan->sliderPosition();
//    std::cerr << "position = " <<position << std::endl;
//#endif
    int rez =(int)(k * (value - min_) + min);
    qscrollbarHorgvPlan->setSliderPosition(rez);
//#ifdef DEBUG_INFO
//    position = qscrollbarHorgvPlan->sliderPosition();//->SliderPosition();
//    std::cerr << "position new = " <<position << std::endl;
//#endif
}

//-----------------------------------------------------------------------------
void TGantGraphicsView::ScrollHoriz_P(int value)
{

    if (qscrollbarHoriz==NULL) return;
    int min_  = qscrollbarHorgvPlan->minimum();//
    int max_  = qscrollbarHorgvPlan->maximum();//

    int min  = qscrollbarHoriz->minimum();//
    int max  = qscrollbarHoriz->maximum();

    float k =0;
    if (max-min>0 && max_ - min_ >0){
       // k= (max-min)/(max_-min_);
         k= (((float)(max-min))/(max_-min_));
    }

//#ifdef DEBUG_INFO
//    int position = qscrollbarHoriz->sliderPosition();
//    std::cerr << "position = " <<position << std::endl;
//#endif
    int rez =(int)(k * (value - min_) + min);
    disconnect(qscrollbarHoriz,SIGNAL(valueChanged(int)),this,SLOT(ScrollHoriz(int)));
    qscrollbarHoriz->setSliderPosition(rez);
    connect(qscrollbarHoriz,SIGNAL(valueChanged(int)),this,SLOT(ScrollHoriz(int)));
//#ifdef DEBUG_INFO
//    position = qscrollbarHoriz->sliderPosition();//->SliderPosition();
//    std::cerr << "position new = " <<position << std::endl;
//#endif
}

//-----------------------------------------------------------------------------
void TGantGraphicsView::ScrollHoriz_T(int value)
{
    if (qscrollbarHoriz==NULL) return;
    int min_  = qscrollbarHorgvTable->minimum();//->setMinimum(0);
    int max_  = qscrollbarHorgvTable->maximum();//->minimax();
    //qscrollbarHoriz->setMaximum(max_ + qscrollbarHoriz->pageStep());

    int min  = qscrollbarHoriz->minimum();//
    int max  = qscrollbarHoriz->maximum();

    float k =0;
    if (max-min>0 && max_ - min_ >0){
        // k= (max-min)/(max_-min_);
         k= (((float)(max-min))/(max_-min_));
    }

//#ifdef DEBUG_INFO
//    int position = qscrollbarHoriz->sliderPosition();
//    std::cerr << "position = " <<position << std::endl;
//#endif
    int rez =(int)(k * (value - min_) + min);

    disconnect(qscrollbarHoriz,SIGNAL(valueChanged(int)),this,SLOT(ScrollHoriz(int)));
    qscrollbarHoriz->setSliderPosition(rez);
    connect(qscrollbarHoriz,SIGNAL(valueChanged(int)),this,SLOT(ScrollHoriz(int)));

//#ifdef DEBUG_INFO
//    position = qscrollbarHoriz->sliderPosition();//->SliderPosition();
//    std::cerr << "position new = " <<position << std::endl;
//#endif
}

//-----------------------------------------------------------------------------
void TGantGraphicsView::show()
{
    //gv->show();
}
//-----------------------------------------------------------------------------
void TGantGraphicsView::fillHeaderDatas()
{       
     // календарь
     for (int i=0;i<12;i++){
         months.insert(i+1, m_monthLabels[i]);
     }

   QDate *d(NULL);
     for (int i=0;i<12;i++)
     {
          d = new  QDate(m_year,i+1,1);
          if (d!=NULL)
          {
            days.insert(i+1,d->daysInMonth());
            delete d;
          }
     }

   QMap<int,int>::const_iterator it;
   int ind(0), j(0), nach(0);
     for(it =days.begin(); it != days.end();++it)
     {
        m_monthStarts[ind]=nach;
      int kol(it.value());
        for (int i=1; i<=kol; ++i)
        {
             m_dates[j] = i;
             d = new QDate(m_year,ind+1,i);
             if (d!=NULL)
             {
               int day(d->dayOfWeek());
                 if (day==6 || day==7)
                     m_weekends[j] = true;
                  delete d;
             }
             j++;
        }
        ind++;
        nach = nach+kol+1;
     }
}
//-----------------------------------------------------------------------------

int  TGantGraphicsView::get_kol_items(TGantItem *items)
{
    int kol(0);
    if (items==NULL) return 0;
    if (!items->isOpen()) return 0;
    TGantItemList &m_items = items->childs();
    if (m_items.isEmpty()) return 0;
#ifdef DEBUG_INFO
    std::cerr <<"==============kol=============== "<< kol<< std::endl;
#endif
    TGantItemList::iterator it2;
    for (it2=m_items.begin(); it2!=m_items.end(); ++it2)
    {
       TGantItem *curr = *it2;
       kol = kol+get_kol_items(curr);
    }
    return kol;
}
//---------------------------------------------------------------------------------

void  TGantGraphicsView::get_kol_curr(TGantItem *items)
{
    if (items==NULL) return;
    kol_curr++;
    if (!items->isOpen()) return;
  TGantItemList &m_items = items->childs();
    if (m_items.isEmpty()) return;
#ifdef DEBUG_INFO
   // std::cerr <<"==============kol=============== "<< kol<< std::endl;
#endif
  TGantItemList::iterator it2;
    for (it2=m_items.begin(); it2!=m_items.end(); ++it2)
    {
      TGantItem *curr = *it2;
      get_kol_curr(curr);
    }
}

//---------------------------------------------------------------------------------
void  TGantGraphicsView::set_open(TGantItem *items, bool op)
{
    if (items==NULL) return;
    items->setOpen(op);//
    TGantItemList &m_items = items->childs();
    if (m_items.isEmpty()) return;
#ifdef DEBUG_INFO
    //std::cerr <<"==============kol=============== "<< kol<< std::endl;
#endif
    TGantItemList::iterator it2;
    for (it2=m_items.begin(); it2!=m_items.end(); ++it2)
    {
        TGantItem *curr = *it2;
        set_open(curr,op);
    }
}

//------------------------------------------------------------------------------
void  TGantGraphicsView::draw_curr_items(TGantItem *items)
{
  if (items==NULL) return;
  kol_curr ++;
#ifdef DEBUG_INFO
    std::cerr <<"============== kol_curr in=============== "<<  kol_curr<< std::endl;
#endif
  qreal x(0.), y(0.), w(0.), h(0.);
  QPen pen;
  QBrush brush;
  QString txt;
  int m_begin(0), m_end(0);

    switch (m_contentDraw)
    {
        case cdPlan:

            // временные параметры
            m_begin = items->begin(GANT_IND_PLAN);//
            m_end = items->end(GANT_IND_PLAN);

            if (m_begin<0 || m_end<0) break;

            // параметры отрисовки элементов
            pen = items->pen(GANT_IND_PLAN);
            brush = items->brush(GANT_IND_PLAN);

            // надписи на элементах
            txt = items->label(GANT_IND_PLAN);

            x = m_begin*m_columnWidth;
            y = (kol_curr-1)*m_rowHeight;
            h = m_rowHeight;

            if (m_end-m_begin>0)
                w = (m_columnWidth)*(m_end-m_begin+1);
            else
                w = (m_columnWidth)*(m_begin-m_end+1);

            draw_rec(x,y,w,h,pen,brush);

            draw_txt(x,y+m_rowHeight*0.2,w,txt);

            break;

        case  cdReal:

            // временные параметры
            m_begin = items->begin(GANT_IND_REAL);//
            m_end = items->end(GANT_IND_REAL);

            if (m_begin<0 || m_end<0) break;

            // параметры отрисовки элементов
            pen = items->pen(GANT_IND_REAL);
            brush = items->brush(GANT_IND_REAL);

            // надписи на элементах
            txt = items->label(GANT_IND_REAL);

            x = m_begin*m_columnWidth;

            y = (kol_curr-1)*m_rowHeight;
            h = m_rowHeight;

            if (m_end-m_begin>0) w = (m_columnWidth)*(m_end-m_begin+1);
            else w = (m_columnWidth)*(m_begin-m_end+1);

            draw_rec(x,y,w,h,pen,brush);

            draw_txt(x,y+m_rowHeight*0.2,w,txt);

            break;

        case cdAll:

            // временные параметры
            m_begin = items->begin(GANT_IND_PLAN);//
            m_end = items->end(GANT_IND_PLAN);

            if (m_begin<0 || m_end<0) break;

            // параметры отрисовки элементов
            pen = items->pen(GANT_IND_PLAN);
            brush = items->brush(GANT_IND_PLAN);

            // надписи на элементах
            txt = items->label(GANT_IND_PLAN);

            x = m_begin*m_columnWidth;
            y = (kol_curr-1)*m_rowHeight;
            h = (m_rowHeight*0.5);

            if (m_end-m_begin>0) w = m_columnWidth*(m_end-m_begin+1);
            else w = m_columnWidth*(m_begin-m_end+1);

            draw_rec(x,y,w,h,pen,brush);

            draw_txt(x,y,w,txt);

            // временные параметры
            m_begin = items->begin(GANT_IND_REAL);
            m_end = items->end(GANT_IND_REAL);

             if (m_begin<0 || m_end<0) break;

             // параметры отрисовки элементов
            pen = items->pen(GANT_IND_REAL);
            brush = items->brush(GANT_IND_REAL);

            // надписи на элементах
            txt = items->label(GANT_IND_REAL);

            x = m_begin*m_columnWidth;

            y = (kol_curr-1)*m_rowHeight+m_rowHeight*0.5;
            h = m_rowHeight*0.5;

            if (m_end-m_begin>0) w = m_columnWidth*(m_end-m_begin+1);
            else w = m_columnWidth*(m_begin-m_end+1);

            draw_rec(x,y,w,h,pen,brush);

            draw_txt(x,y,w,txt);

            break;

        default: return;
    }

    if (!items->isOpen()) return;
    TGantItemList &m_items = items->childs();
    if (m_items.isEmpty()) return;

    TGantItemList::iterator it2;
    for (it2=m_items.begin(); it2!=m_items.end(); ++it2)
    {
      TGantItem *curr = *it2;
        draw_curr_items(curr);
    }
}

//------------------------------------------------------------------------------
void  TGantGraphicsView::draw_curr_items_n(TGantItem *items)
{
  if (items==NULL) return;
  kol_curr ++;
#ifdef DEBUG_INFO
    std::cerr <<"============== kol_curr in=============== "<<  kol_curr<< std::endl;
#endif
  qreal x(0.), y(0.), w(0.), h(0.);
  QPen pen;
  QBrush brush;
  QString txt;
  int m_begin(0), m_end(0);

    switch (m_contentDraw)
    {
        case cdPlan:

            // временные параметры
            m_begin = items->begin(GANT_IND_PLAN);//
            m_end = items->end(GANT_IND_PLAN);

            if (m_begin<0 || m_end<0) break;

            // параметры отрисовки элементов
            pen = items->pen(GANT_IND_PLAN);
            brush = items->brush(GANT_IND_PLAN);

            // надписи на элементах
            txt = items->label(GANT_IND_PLAN);

            x = m_begin*m_columnWidth;
            y = (kol_curr-1)*m_rowHeight;
            h = m_rowHeight;

            if (m_end-m_begin>0)
                w = (m_columnWidth)*(m_end-m_begin+1);
            else
                w = (m_columnWidth)*(m_begin-m_end+1);

            draw_rec(x,y,w,h,pen,brush);

            draw_txt(x,y+m_rowHeight*0.2,w,txt);

            break;

        case  cdReal:

            // временные параметры
            m_begin = items->begin(GANT_IND_REAL);//
            m_end = items->end(GANT_IND_REAL);

            if (m_begin<0 || m_end<0) break;

            // параметры отрисовки элементов
            pen = items->pen(GANT_IND_REAL);
            brush = items->brush(GANT_IND_REAL);

            // надписи на элементах
            txt = items->label(GANT_IND_REAL);

            x = m_begin*m_columnWidth;

            y = (kol_curr-1)*m_rowHeight;
            h = m_rowHeight;

            if (m_end-m_begin>0) w = (m_columnWidth)*(m_end-m_begin+1);
            else w = (m_columnWidth)*(m_begin-m_end+1);

            draw_rec(x,y,w,h,pen,brush);

            draw_txt(x,y+m_rowHeight*0.2,w,txt);

            break;

        case cdAll:

            // временные параметры
            m_begin = items->begin(GANT_IND_PLAN);//
            m_end = items->end(GANT_IND_PLAN);

            if (m_begin<0 || m_end<0) break;

            // параметры отрисовки элементов
            pen = items->pen(GANT_IND_PLAN);
            brush = items->brush(GANT_IND_PLAN);

            // надписи на элементах
            txt = items->label(GANT_IND_PLAN);

            x = m_begin*m_columnWidth;
            y = (kol_curr-1)*m_rowHeight;
            h = (m_rowHeight*0.5);

            if (m_end-m_begin>0) w = m_columnWidth*(m_end-m_begin+1);
            else w = m_columnWidth*(m_begin-m_end+1);

            draw_rec(x,y,w,h,pen,brush);

            draw_txt(x,y,w,txt);

            // временные параметры
            m_begin = items->begin(GANT_IND_REAL);
            m_end = items->end(GANT_IND_REAL);

             if (m_begin<0 || m_end<0) break;

             // параметры отрисовки элементов
            pen = items->pen(GANT_IND_REAL);
            brush = items->brush(GANT_IND_REAL);

            // надписи на элементах
            txt = items->label(GANT_IND_REAL);

            x = m_begin*m_columnWidth;

            y = (kol_curr-1)*m_rowHeight+m_rowHeight*0.5;
            h = m_rowHeight*0.5;

            if (m_end-m_begin>0) w = m_columnWidth*(m_end-m_begin+1);
            else w = m_columnWidth*(m_begin-m_end+1);

            draw_rec(x,y,w,h,pen,brush);

            draw_txt(x,y,w,txt);

            break;

        default: return;
    }
/*
    if (!items->isOpen()) return;
    TGantItemList &m_items = items->childs();
    if (m_items.isEmpty()) return;

    TGantItemList::iterator it2;
    for (it2=m_items.begin(); it2!=m_items.end(); ++it2)
    {
      TGantItem *curr = *it2;
        draw_curr_items(curr);
    }
    */
}


//----------------------------------------------------------------------

QGraphicsRectItem *TGantGraphicsView::draw_rec_tbl(qreal x, qreal y, qreal w, qreal h, QPen &pen, QBrush &brush)
{
#ifdef DEBUG_INFO
    std::cerr <<" ==============cdPlan tbl======x========= "<<x<< std::endl;
    std::cerr <<" ==============cdPlan tbl======y========= "<<y<< std::endl;
#endif
    return sceneTable->addRect(x,y,w,h,pen,brush);
}
//------------------------------------------------------------------------

QGraphicsRectItem*TGantGraphicsView::draw_rec(qreal x, qreal y, qreal w, qreal h, QPen &pen, QBrush &brush)
{
#ifdef DEBUG_INFO
    std::cerr <<" ==============cdPlan rec======x========= "<<x<< std::endl;
    std::cerr <<" ==============cdPlan rec======y========= "<<y<< std::endl;
#endif
    return sceneGant->addRect(x,y,w,h,pen,brush);
}
//------------------------------------------------------------------------

void TGantGraphicsView::draw_txt(qreal x, qreal y, qreal w, QString txt)
{
  qreal x_txt(0.), y_txt(0.);
  QGraphicsTextItem *textDate(new QGraphicsTextItem(txt));
    textDate->setPlainText(txt);
    textDate->setDefaultTextColor(Qt::white);
    x_txt = x+w/2-textDate->document()->idealWidth()/2; // To center
    y_txt = y;
    textDate->setPos(x_txt,y_txt);
    sceneGant->addItem(textDate);
}
//----------------------------------------------------------------------------
void  TGantGraphicsView::set_scrollbarVert(QScrollBar *qscrollbarVertgvPlanTree_)
{
    //qscrollbarVertgvPlanTree = qscrollbarVertgvPlanTree_;
    qscrollbarVert = qscrollbarVertgvPlanTree_;
}
//-----------------------------------------------------------------------------
 void TGantGraphicsView:: set_scrollbarHoriz(QScrollBar *qscrollbarHorizPlan_)
{
    qscrollbarHoriz = qscrollbarHorizPlan_;
}

 /*
 void TGantGraphicsView::draw_time(TGantGraphicsView::ContentDraw cd)
 {
      std::cerr << " !!!============================  new  draw time ======================= !!! "  << std::endl;

      m_contentDraw = cd;
      //first_resizeEvent=true;
      timer ->start(100);

 }
*/

// !!! толщина (высота) элемента зависит от TGantGraphicsView::ContentDraw:
//     cdPlan или cdReal - полная
//     cdAll             - половинная
void TGantGraphicsView::draw(TGantGraphicsView::ContentDraw cd)
{
    std::cerr << " !!!============================  new  draw ======================= !!! "  << std::endl;

    //emit start_newPlan();

    m_contentDraw = cd;

    Vert =false;
    Vert_P =false;

    deletePlan();
    newPlan();

    fillHeaderDatas();

#ifdef DEBUG_INFO
    std::cerr << " draw year = " << m_year << " draw cd = " << cd << std::endl;
#endif

  int kolDays(GANT_KOL_DAY);
    if (m_dates[GANT_KOL_DAY-1]==0) kolDays = GANT_KOL_DAY-1;

  int headerHeight(m_headerHeight);
  int rowcount(0);
  kol_curr = 0;
  TGantItemList &items = m_topItems;
  TGantItemList::iterator it2;
    for (it2=items.begin(); it2!=items.end(); ++it2)
    {
      TGantItem *curr = *it2;
        rowcount = rowcount+get_kol_items(curr);
        get_kol_curr(curr);
    }

    rowcount =kol_curr;
#ifdef DEBUG_INFO
    std::cerr << " ==============rowcount all=============== " << rowcount<< std::endl;
#endif
  if (rowcount==0) rowcount=1;

  int colDaysWidth(m_columnWidth);

#ifdef DEBUG_INFO
    std::cerr << " m_columnWidth = " << m_columnWidth << std::endl;
    std::cerr << " m_headerHeight = " << m_headerHeight << std::endl;
    std::cerr << " m_rowHeight = " << m_rowHeight << std::endl;
    std::cerr << " kolDays = " << kolDays<< std::endl;
    int totalHeight(m_rowHeight*rowcount+headerHeight);
    std::cerr << " totalHeight = " << totalHeight << std::endl;
#endif

    //std::cerr << " !!!!!!!!  m_headerHeight = " << m_headerHeight << std::endl;
    //std::cerr << " !!!!!!!! m_rowHeight = " << m_rowHeight << std::endl;

    //int slayder(m_rowHeight/2);
    //int slayder_gor(m_columnWidth);

    // Сетка
    // Вертикальная
#ifdef DEBUG_INFO
    std::cerr << " Вертикаль " << std::endl;
#endif
    for (int i=0; i<kolDays; i++)
    {
        if (m_weekends[i])
        {
            //sceneGant->addRect(i*colDaysWidth,0,colDaysWidth,m_rowHeight*rowcount+slayder,m_weekendPen,m_weekendBrush);
            sceneGant->addRect(i*colDaysWidth,0,colDaysWidth,m_rowHeight*rowcount,m_weekendPen,m_weekendBrush);
            sceneTable->addRect(i*colDaysWidth,0,colDaysWidth,headerHeight,m_weekendPen,m_weekendBrush);
        }
        QGraphicsLineItem *line(NULL);
        //line = new QGraphicsLineItem(i*colDaysWidth,0,i*colDaysWidth,m_rowHeight*rowcount+slayder);
        line = new QGraphicsLineItem(i*colDaysWidth,0,i*colDaysWidth,m_rowHeight*rowcount);
        line->setPen(QPen(Qt::black));
        if (m_weekends[i]) line->setPen(m_weekendPen);
        else line->setPen(m_gridPen);
        sceneGant->addItem(line);

        line = new QGraphicsLineItem(i*colDaysWidth,0,i*colDaysWidth,headerHeight);
        line->setPen(QPen(Qt::black));
        if (m_weekends[i]) line->setPen(m_weekendPen);
        else line->setPen(m_gridPen);

        sceneTable->addItem(line);
    }
    // Горизонтальная
#ifdef DEBUG_INFO
    std::cerr << " Горизонталь " << std::endl;
#endif

    // первая горизонталь
  QGraphicsLineItem *line(new QGraphicsLineItem(0,headerHeight/2,colDaysWidth*kolDays,headerHeight/2));
    line->setPen(QPen(Qt::black));
    sceneTable->addItem(line);

    for (int i=0; i<rowcount; i++)
    {
      QGraphicsLineItem *line(new QGraphicsLineItem(0,i*m_rowHeight,colDaysWidth*kolDays,i*m_rowHeight));
        line->setPen(QPen(Qt::black));
        sceneGant->addItem(line);
    }

    // месяцы
  QMap<int,QString>::const_iterator it;
  qreal x(0.);
  int w(0), h(headerHeight/2);
    for (it=months.begin(); it!=months.end(); ++it)
    {
      int kod(it.key());
      QString value(it.value());
      int kolDays(days.take(kod));

        x = x+w;
        w = colDaysWidth*kolDays;
        draw_rec_tbl(x,0,w,h,m_gridPen,m_gridBrush);

      QString txt(value);
      qreal x_txt(0.), y_txt(0.);
      QGraphicsTextItem *textDate(new QGraphicsTextItem(txt));
        textDate->setPlainText(txt);
        x_txt = x+w/2-textDate->document()->idealWidth()/2; // To center
        y_txt = 0;

        textDate->setPos(x_txt,y_txt);
        sceneTable->addItem(textDate);
    }

#ifdef DEBUG_INFO
    std::cerr << " Нумерация " << std::endl;
#endif
    // Нумерация дней в шапке
    for (int i=0; i<kolDays; i++)
    {
#ifdef DEBUG_INFO
       // std::cerr << " m_dates[i] = " << m_dates[i] << std::endl;
#endif
      QString num(QString::number(m_dates[i]));
      QGraphicsTextItem *textDate(new QGraphicsTextItem(num));
        textDate->setPlainText(num);

      qreal x(i*colDaysWidth+colDaysWidth/2-textDate->document()->idealWidth()/2); // To center
      qreal y(headerHeight/2);
        textDate->setPos(x,y);
        sceneTable->addItem(textDate);
    }

    // выполняемые работы
    kol_curr=0;
    TGantItemList & m_items = m_topItems;
    //if (m_items.isEmpty()) return;

    /*
#ifdef DEBUG_INFO
    std::cerr << " ==============kol_curr=============== " << kol_curr << std::endl;
#endif
    TGantItemList::iterator it2_;
    for (it2_ = m_items.begin(); it2_ != m_items.end(); ++it2_)
    //for (it2_ = m_topItems.begin(); it2_ != m_topItems.end(); ++it2_)
    {
         TGantItem *curr = *it2_;
#ifdef DEBUG_INFO
    std::cerr << " ==============kol_curr bef =============== " << kol_curr << std::endl;
#endif
         draw_curr_items(curr);
#ifdef DEBUG_INFO
    std::cerr << " ==============kol_curr after =============== " << kol_curr << std::endl;
#endif
    }
*/

      QStack<TGantItem*> stack;
      foreach (TGantItem *topIt,m_items)
      {
          stack.push(topIt);
          while (!stack.isEmpty())
          {
              TGantItem *curIt = stack.pop();
              //PR3((curIt.level()+1)*4,"%1 [%2..%3]",curIt.name(),curIt.begin(0),curIt.end(0));
              draw_curr_items_n(curIt);
#ifdef DEBUG_INFO
    std::cerr << " ============== текущая kol_curr  =============== " << kol_curr <<  "  name "<<curIt->name().toStdString().c_str()<< std::endl;
#endif
              /*
              foreach (TGantItem *it,curIt->childs())
              {
                 stack.push(it);
              }
              */

              for (int i=curIt->childs().size()-1; i>=0;--i)
              {
                  TGantItem *it =curIt->childs().at(i);
                  if (it==NULL) continue;
                  stack.push(it);
              }

          }
      }

     int height (this->height());

    QGraphicsProxyWidget *wid(scene->addWidget(gvTable));
    QTransform tr(wid->transform());

    tr.translate(0,0);
    wid->setTransform(tr);

    wid = scene->addWidget(gvPlan);
    tr = wid->transform();

    tr.translate(0,headerHeight);
    wid->setTransform(tr);    

    // устанавливаем в положение слайдер
    qscrollbarHorgvPlan->setMinimum(0);
    //qscrollbarHorgvPlan->setMaximum(colDaysWidth*kolDays+slayder_gor);
    //qscrollbarHorgvPlan->setMaximum(colDaysWidth*kolDays+slayder_gor - qscrollbarHorgvPlan->pageStep());
    //qscrollbarHorgvPlan->setMaximum(colDaysWidth*kolDays );
    qscrollbarHorgvPlan->setMaximum(colDaysWidth*kolDays  - qscrollbarHorgvPlan->pageStep());

    qscrollbarHorgvTable->setMinimum(0);
    //qscrollbarHorgvTable->setMaximum(colDaysWidth*kolDays+slayder_gor - qscrollbarHorgvTable->pageStep());
    qscrollbarHorgvTable->setMaximum(colDaysWidth*kolDays - qscrollbarHorgvTable->pageStep());
    //qscrollbarHorgvTable->setMaximum(colDaysWidth*kolDays );

    //2017
    //if (prnt_gv)
    //   gv->setViewport(prnt_gv);
    //gv->layout();

    qscrollbarVertgvPlan->setMinimum(0);    
    qscrollbarVertgvPlan->setMaximum(m_rowHeight*rowcount );
    qscrollbarVertgvPlan->setSliderPosition(0);

    int dob (0);

    if (qscrollbarVert!=NULL)
    {

        if (prnt_gv)
        {
            int h_v (this->horizontalScrollBar()->height());
            // h_v =0;
            int kol =(int) (((float)(height - headerHeight - h_v )) /m_rowHeight);
            dob = height - headerHeight  -  kol*m_rowHeight;

            if (headerHeight + m_rowHeight*rowcount >  height )
            {
             connect(qscrollbarVert,SIGNAL(valueChanged(int)),this,SLOT(ScrollVert(int)));//
             connect(qscrollbarVertgvPlan, SIGNAL(valueChanged(int)),
                    this, SLOT(ScrollVert_P(int)));
             int page_v = qscrollbarVert->pageStep();//
             int min = qscrollbarVert->minimum();
             int max = qscrollbarVert->maximum();
             int min_ = qscrollbarVertgvPlan->minimum();
             int page_vp = qscrollbarVertgvPlan->pageStep();//
             qscrollbarVertgvPlan->setMaximum(m_rowHeight*rowcount + dob  - page_vp );
             int max_ = qscrollbarVertgvPlan->maximum();

//#ifdef DEBUG_INFO
             std::cerr << " !!! qscrollbarVertgvPlan max = " << max_ << std::endl;
             std::cerr << " !!! qscrollbarVertgvPlan min = " << min_ << std::endl;
             std::cerr << " !!! qscrollbarVertgvPlan pageStep = " << page_vp  << std::endl;
             std::cerr << " !!! qscrollbarVert max = " << max << std::endl;
             std::cerr << " !!! qscrollbarVert min = " << min << std::endl;
             std::cerr << " !!! qscrollbarVert pageStep = " << page_v  << std::endl;
             std::cerr << " !!! qscrollbarVert->width() = " << qscrollbarVert->width() << std::endl;
             std::cerr << " !!! qscrollbarVert->height() = " << qscrollbarVert->height() << std::endl;
             std::cerr << " !!! dob = " << dob << std::endl;
             std::cerr << " !!! this->width()  = " << this->width() << std::endl;
             std::cerr << " !!! this->height() = " << this->height() << std::endl;
//#endif

//#ifdef DEBUG_INFO
             std::cerr << "  !!! qscrollbarVert DETERMINANT  !!! " << std::endl;
//#endif
            }

        }

        qscrollbarVert->setSliderPosition(0);
    }

    if (prnt_gv){

        int width = this->width();//
        //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        //qscrollbarHoriz = (QScrollBar * ) GetDlgItem(IDC_SCROLLBAR1);
        qscrollbarHoriz = horizontalScrollBar();

        //qscrollbarHoriz =new QScrollBar(Qt::Horizontal,this);
        //setHorizontalScrollBar(qscrollbarHoriz );

        if (qscrollbarHoriz!=NULL){
            qscrollbarHoriz->setEnabled (true );
            qscrollbarHoriz->setVisible (true );
            qscrollbarHoriz->setMaximum((int)((float)width*9/10));
            qscrollbarHoriz->setMinimum(0);
            //qscrollbarHoriz->triggerAction(QAbstractSlider::SliderMove );
            qscrollbarHoriz->setPageStep((int)((float)width/10));
            qscrollbarHoriz->setValue(qscrollbarHoriz->minimum());
            //qscrollbarHoriz->setTracking(true);
            //qscrollbarHoriz->setSliderDown(false);

//#ifdef DEBUG_INFO
            std::cerr << " !!!! qscrollbarHoriz DETERMINANT !!!! "  << std::endl;
            int min = this->minimumWidth(); int max = this->maximumWidth();//
            int pagestep = qscrollbarHoriz->pageStep();//->PageStep
            std::cerr << " !!!! min width = !!!! "  << min <<std::endl;
            std::cerr << " !!!! max width = !!!! "  << max <<std::endl;
            std::cerr << " !!!! width = !!!! "  << width <<std::endl;
            std::cerr << " !!!! pagestep = !!!! "  << pagestep <<std::endl;
            std::cerr << " !!!! qscrollbarHoriz->maximum() = !!!! "  <<  qscrollbarHoriz->maximum() <<std::endl;
            std::cerr << " !!!! qscrollbarHoriz->minimum() = !!!! "  <<  qscrollbarHoriz->minimum() <<std::endl;
            std::cerr << " !!!! qscrollbarHoriz->value() = !!!! "  <<  qscrollbarHoriz->value() <<std::endl;
//#endif

        }      
    }

    if (qscrollbarHoriz!=NULL)
    {
        connect(qscrollbarHoriz,SIGNAL(valueChanged(int)),this,SLOT(ScrollHoriz(int)));
        qscrollbarHorgvPlan->setSliderPosition(0);
        int min = qscrollbarHoriz->minimum();
        qscrollbarHoriz->setSliderPosition(min);

        //
        connect(qscrollbarHorgvPlan, SIGNAL(valueChanged(int)),
               this, SLOT(ScrollHoriz_P(int)));
        connect(qscrollbarHorgvTable, SIGNAL(valueChanged(int)),
               this, SLOT(ScrollHoriz_T(int)));
    }

    if (m_currentDay>=0)
    {

#ifdef DEBUG_INFO
        int position(qscrollbarHorgvPlan->sliderPosition());
        std::cerr << " position = " <<position << std::endl;
#endif
        //qscrollbarHorgvPlan->setSliderPosition(colDaysWidth*m_currentDay);
#ifdef DEBUG_INFO
        position = qscrollbarHorgvPlan->sliderPosition();
        std::cerr << " position new = " <<position << std::endl;
#endif
       // qscrollbarHorgvTable->setMinimum(0);
       // qscrollbarHorgvTable->setMaximum(colDaysWidth*kolDays+slayder_gor);

#ifdef DEBUG_INFO
        position = qscrollbarHorgvTable->sliderPosition();
        std::cerr << " position = " <<position << std::endl;
#endif
        //qscrollbarHorgvTable->setSliderPosition(colDaysWidth*m_currentDay);
#ifdef DEBUG_INFO
        position =qscrollbarHorgvTable->sliderPosition();
        std::cerr << " position new = " <<position << std::endl;
#endif
        if (qscrollbarHoriz!=NULL) {
            int min_  = qscrollbarHoriz->minimum();//->setMinimum(0);
            int max_  = qscrollbarHoriz->maximum();//->minimax();
            int min   = qscrollbarHorgvPlan->minimum();
            int max   = qscrollbarHorgvPlan->maximum();
            std::cerr << " qscrollbarHoriz min = " <<min_ << std::endl;
            std::cerr << " qscrollbarHoriz max = " <<max_ << std::endl;
            std::cerr << " qscrollbarHorizPlan min = " << min << std::endl;
            std::cerr << " qscrollbarHorizPlan max = " << max << std::endl;
             std::cerr << " m_currentDay = " << m_currentDay << std::endl;
            float k =0;
            if (max-min>0 && max_ - min_ >0){
               k= ((float)(max-min)/(max_-min_));
            }
            if (k>0){
                int value =(int) ((float)(colDaysWidth*m_currentDay - min)/k + min_);
                if (value>=0)
                   qscrollbarHoriz->setSliderPosition(value);
                 std::cerr << " qscrollbarHoriz value new = " <<value << std::endl;
            }
        }
        qscrollbarHorgvPlan->setSliderPosition(colDaysWidth*m_currentDay);
    }

//#ifdef DEBUG_INFO
    std::cerr << " !!!! gv->width()   AFTER  =  " << gv->width() <<std::endl;
    std::cerr << " !!!! gv->height()  AFTER  =  " << gv->height() <<std::endl;
    //std::cerr << " !!!! gv->minimumWidth() AFTER =  " << gv->minimumWidth() <<std::endl;
    //std::cerr << " !!!! gv->maximumWidth() AFTER =  " << gv->maximumWidth() <<std::endl;
    //std::cerr << " !!!! gv->minimumHeight() AFTER =  " << gv->minimumHeight() <<std::endl;
    //std::cerr << " !!!! gv->maximumHeight() AFTER =  " << gv->maximumHeight() <<std::endl;
//#endif

    gv->show();

}
//-----------------------------------------------------------------------------
 void TGantGraphicsView:: resizeEvent(QResizeEvent *event) {// Q_DECL_OVERRIDE{

     //timer->start(50);
     // Sleep(2000);

     //#ifdef DEBUG_INFO
          std::cerr << " !!! resizeEvent this->width()  = " << this->width() << std::endl;
          std::cerr << " !!! resizeEvent this->height() = " << this->height() << std::endl;
          //-----std::cerr << " !!! resizeEvent gv->width()  = " << gv->width() << std::endl;
          //-----std::cerr << " !!! resizeEvent gv->height() = " << gv->height() << std::endl;
     //#endif
     // if (first_resizeEvent) return;

      timer->start(100);
      QGraphicsView::resizeEvent(event);

    //fitInView(gvTable->rect(), Qt::KeepAspectRatio);
    //fitInView(gvPlan->rect(), Qt::KeepAspectRatio);
    //fitInView(gv->rect(), Qt::KeepAspectRatio);
    //------gv->resize(this->width() ,  this->height() );
    //

    // draw(m_contentDraw );
    //gv->viewport()->setFixedSize(this->width() ,  this->height());

    /*
    draw(m_contentDraw );
    Sleep(2000);
    this->horizontalScrollBar()->setVisible(false);
    //this->horizontalScrollBar()->setValue(100);
    Sleep(2000);
    this->horizontalScrollBar()->setVisible(true);
    //this->horizontalScrollBar()->setValue(0);
    //this->horizontalScrollBar()->setMinimum(0);
    //this->horizontalScrollBar()->setMaximum(10000);
    //this->show();
    //this->resize(this->width() + 1, this->height()+1);
    */

     //repaint();

 }

//---------------------------------------------------------------------------------
 void TGantGraphicsView:: redraw() {// Q_DECL_OVERRIDE{

     timer->stop();
     draw(m_contentDraw );

 }

 void TGantGraphicsView::  newWindow() {//

     counter++;

 }
