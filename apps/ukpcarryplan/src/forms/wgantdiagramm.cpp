
#include <QTreeWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QStack>
#include <QScrollBar>
#include <defMacro>
#include <qtools>
#include <TModulePlans>
#include <WGantDiagramm>

WGantDiagramm *wGantDiagramm(NULL);

namespace
{

    QTreeWidget *TREE(NULL);

    TGantGraphicsView *DIAGR(NULL);

    QFrame *FR_BUTTONS(NULL);
           //*FR_DIAGR(NULL);

    QPushButton *BTN_ALL(NULL),
                *BTN_PLAN(NULL),
                *BTN_REAL(NULL);

    const int HEADER_H(60),
              COLUMN_W(30),
              ROW_H(40);

}

WGantDiagramm::WGantDiagramm(QWidget *parent) : QDialog(parent)
{
    ui->setupUi(this);

  QGridLayout *grl(new QGridLayout(this));

    TREE = new QTreeWidget(this);
    EL_MINMAX_WIDTH(TREE,500);
    if (QTreeWidgetItem *hdrIt = TREE->headerItem())
    {
        hdrIt->setText(0," Наименование");
        hdrIt->setTextAlignment(0,Qt::AlignLeft | Qt::AlignVCenter);
      QSize sz = hdrIt->sizeHint(0);
        sz.setHeight(HEADER_H);
        hdrIt->setSizeHint(0,sz);
    }

    TREE->setColumnWidth(0,1000);
    TREE->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    TREE->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    TREE->setIconSize(QSize(36,36));
    TREE->setUniformRowHeights(true);
    TREE->setSortingEnabled(true);
    TREE->setAlternatingRowColors(true);
    grl->addWidget(TREE,0,0,1,1);

    DIAGR = new TGantGraphicsView(0,-1,this);
    DIAGR->set_scrollbarVert(TREE->verticalScrollBar());

    grl->addWidget(DIAGR,0,1,1,1);

    FR_BUTTONS = new QFrame(this);
    EL_MINMAX_HEIGHT(FR_BUTTONS,50);

    BTN_ALL = new QPushButton("Все",FR_BUTTONS);
    BTN_ALL->setGeometry(0,0,200,50);
    //FONT_BOLD(BTN_ALL,true);
    connect(BTN_ALL,SIGNAL(clicked()),this,SLOT(resetGantDiagramm()));

    BTN_PLAN = new QPushButton("План",FR_BUTTONS);
    BTN_PLAN->setGeometry(210,0,200,50);
    //FONT_BOLD(BTN_PLAN,true);
    connect(BTN_PLAN,SIGNAL(clicked()),this,SLOT(resetGantDiagramm()));

    BTN_REAL = new QPushButton("Выполнение",FR_BUTTONS);
    BTN_REAL->setGeometry(420,0,200,50);
    //FONT_BOLD(BTN_REAL,true);
    connect(BTN_REAL,SIGNAL(clicked()),this,SLOT(resetGantDiagramm()));

    grl->addWidget(FR_BUTTONS,1,0,1,2);

    grl->setMargin(10);
    grl->setHorizontalSpacing(0);
    grl->setVerticalSpacing(10);

}
//-----------------------------------------------------------------------------

WGantDiagramm::~WGantDiagramm()
{
}
//-----------------------------------------------------------------------------

void WGantDiagramm::resetGantDiagramm()
{
    if (QPushButton *btn = dynamic_cast<QPushButton*>(sender())) resetGantDiagramm(*btn);
}
//-----------------------------------------------------------------------------

void WGantDiagramm::resetGantDiagramm(const QPushButton &btn)
{
  MODULE(Plans);
    if (&btn==BTN_ALL)
    {
        prepare(modPlans->carryTasks(),TGantGraphicsView::cdAll);
    }
    else if (&btn==BTN_PLAN)
    {
        prepare(modPlans->carryTasks(),TGantGraphicsView::cdPlan);
    }
    else if (&btn==BTN_REAL)
    {
        prepare(modPlans->carryTasks(),TGantGraphicsView::cdReal);
    }
}
//-----------------------------------------------------------------------------

void WGantDiagramm::prepare(TCarryTaskList &tasks, TGantGraphicsView::ContentDraw whatdraw)
{
  MODULE(Plans);
    modPlans->reflectCarryTasksToTree(tasks,*TREE,true,ROW_H); // только отмеченные
    TREE->sortItems(0,Qt::AscendingOrder);
    qtools::expand(*TREE);

  QPen planItemsPen(Qt::darkBlue);
    planItemsPen.setWidth(2);
  QBrush planItemsBrash(Qt::blue);
  QPen realItemsPen(Qt::darkGreen);
    realItemsPen.setWidth(2);
  QBrush realItemsBrash(Qt::green);

  TGantGraphicsView &gd = *DIAGR;
    gd.setHeaderHeight(HEADER_H);
    gd.setColumnWidth(COLUMN_W);
    gd.setRowHeight(ROW_H);
    gd.setGridPen(QPen(Qt::black));
    gd.setWeekendPen(QPen(Qt::darkRed));
    gd.setGridBrush( QBrush(Qt::lightGray));
    gd.setWeekendBrush(QBrush(Qt::darkGray));
    gd.clearTopItems();

    // плановые элементы
    if (whatdraw!=TGantGraphicsView::cdReal)
    {
        foreach (TCarryTask *tsk,tasks)
        {
          bool isReflect(false);
          TCarryPlan *plans[2] = { tsk->ordPlan(),tsk->carryPlan() };
            for (int i=0; i<2; i++) // проверка наличия отображаемых планов
                if (TCarryPlan *plan = plans[i])
                    if (plan->isChecked()) { isReflect = true; break; }
            if (!isReflect) continue;
          TGantItem *tskGit(new TGantItem(TGantItem::gitProject,tsk->scrName()));
            tskGit->setPen(GANT_IND_PLAN,planItemsPen);
            tskGit->setBrush(GANT_IND_PLAN,planItemsBrash);
            tskGit->setLevel(0);
            tskGit->setOpen(true);
            tskGit->setBegin(GANT_IND_PLAN,tsk->dayPlanBegin());
            tskGit->setEnd(GANT_IND_PLAN,tsk->dayPlanEnd());
            for (int i=0; i<2; i++)
                if (TCarryPlan *plan = plans[i])
                    if (plan->isChecked())
                    {
                      TGantItem *planGit(new TGantItem(TGantItem::gitPlan,plan->scrName(),tskGit));
                        planGit->setPen(GANT_IND_PLAN,planItemsPen);
                        planGit->setBrush(GANT_IND_PLAN,planItemsBrash);
                        planGit->setLevel(1);
                        planGit->setOpen(true);
                        planGit->setBegin(GANT_IND_PLAN,plan->dayPlanBegin());
                        planGit->setEnd(GANT_IND_PLAN,plan->dayPlanEnd());
                        foreach (TCarryProcedure *pr,plan->procedures())
                        {
                          TGantItem *prGit(new TGantItem(TGantItem::gitProcedure,pr->scrName(),planGit));
                            prGit->setPen(GANT_IND_PLAN,planItemsPen);
                            prGit->setBrush(GANT_IND_PLAN,planItemsBrash);
                            prGit->setLevel(2);
                            prGit->setOpen(true);
                            prGit->setBegin(GANT_IND_PLAN,pr->dayPlanBegin());
                            prGit->setEnd(GANT_IND_PLAN,pr->dayPlanEnd());
                            foreach (TCarryWork *wrk,pr->works())
                            {
                              TGantItem *wrkGit(new TGantItem(TGantItem::gitWork,wrk->scrName(),prGit));
                                wrkGit->setPen(GANT_IND_PLAN,planItemsPen);
                                wrkGit->setBrush(GANT_IND_PLAN,planItemsBrash);
                                wrkGit->setLevel(3);
                                wrkGit->setOpen(true);
                                wrkGit->setBegin(GANT_IND_PLAN,wrk->dayPlanBegin());
                                wrkGit->setEnd(GANT_IND_PLAN,wrk->dayPlanEnd());
                                prGit->insertChild(wrkGit);
                            }
                            planGit->insertChild(prGit);
                        }
                        tskGit->insertChild(planGit);
                    }
            gd.insertTopItem(tskGit);
        }
    }

    // реальные элементы
    if (whatdraw!=TGantGraphicsView::cdPlan)
    {
        foreach (TCarryTask *tsk,tasks)
        {
          bool isReflect(false);
          TCarryPlan *plans[2] = { tsk->ordPlan(),tsk->carryPlan() };
            for (int i=0; i<2; i++) // проверка наличия отображаемых планов
                if (TCarryPlan *plan = plans[i])
                    if (plan->isChecked()) { isReflect = true; break; }
            if (!isReflect) continue;
          TGantItem *tskGit(new TGantItem(TGantItem::gitProject,tsk->scrName()));
            tskGit->setPen(GANT_IND_REAL,realItemsPen);
            tskGit->setBrush(GANT_IND_REAL,realItemsBrash);
            tskGit->setLevel(0);
            tskGit->setOpen(true);
            tskGit->setBegin(GANT_IND_REAL,tsk->dayRealBegin());
            tskGit->setEnd(GANT_IND_REAL,tsk->dayRealEnd());
            for (int i=0; i<2; i++)
                if (TCarryPlan *plan = plans[i])
                    if (plan->isChecked())
                    {
                      TGantItem *planGit(new TGantItem(TGantItem::gitPlan,plan->scrName(),tskGit));
                        planGit->setPen(GANT_IND_REAL,realItemsPen);
                        planGit->setBrush(GANT_IND_REAL,realItemsBrash);
                        planGit->setLevel(1);
                        planGit->setOpen(true);
                        planGit->setBegin(GANT_IND_REAL,plan->dayRealBegin());
                        planGit->setEnd(GANT_IND_REAL,plan->dayRealEnd());
                        foreach (TCarryProcedure *pr,plan->procedures())
                        {
                          TGantItem *prGit(new TGantItem(TGantItem::gitProcedure,pr->scrName(),planGit));
                            prGit->setPen(GANT_IND_REAL,realItemsPen);
                            prGit->setBrush(GANT_IND_REAL,realItemsBrash);
                            prGit->setLevel(2);
                            prGit->setOpen(true);
                            prGit->setBegin(GANT_IND_REAL,pr->dayRealBegin());
                            prGit->setEnd(GANT_IND_REAL,pr->dayRealEnd());
                            foreach (TCarryWork *wrk,pr->works())
                            {
                              TGantItem *wrkGit(new TGantItem(TGantItem::gitWork,wrk->scrName(),prGit));
                                wrkGit->setPen(GANT_IND_REAL,realItemsPen);
                                wrkGit->setBrush(GANT_IND_REAL,realItemsBrash);
                                wrkGit->setLevel(3);
                                wrkGit->setOpen(true);
                                wrkGit->setBegin(GANT_IND_REAL,wrk->dayRealBegin());
                                wrkGit->setEnd(GANT_IND_REAL,wrk->dayRealEnd());
                                prGit->insertChild(wrkGit);
                            }
                            planGit->insertChild(prGit);
                        }
                        tskGit->insertChild(planGit);
                    }
            gd.insertTopItem(tskGit);
        }
    }

    gd.moveToDay(QDate::currentDate().dayOfYear()-1);
    gd.draw(whatdraw);

    printDiagrammTree();

}
//-----------------------------------------------------------------------------

void WGantDiagramm::printDiagrammTree()
{
  TGantGraphicsView &gd = *DIAGR;
  QStack<TGantItem*> stack;
    PR(0,"GantGraphicsView:");
    foreach (TGantItem *topIt,gd.topItems())
    {
        stack.push(topIt);
        while (!stack.isEmpty())
        {
          TGantItem &curIt = *stack.pop();
            PR3((curIt.level()+1)*4,"%1 [%2..%3]",curIt.name(),curIt.begin(0),curIt.end(0));
            foreach (TGantItem *it,curIt.childs()) stack.push(it);
        }
    }
}
//-----------------------------------------------------------------------------
