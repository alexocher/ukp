
#include <QTreeWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QStack>
#include <QScrollBar>
#include <defMacro>
#include <qtools>
#include <gen>
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

// ??? Раскрасить ???

void WGantDiagramm::prepare(TCarryTaskList &tasks, TGantGraphicsView::ContentDraw whatdraw)
{
  MODULE(Plans);
    modPlans->reflectCarryTasksToTree(tasks,*TREE,true,ROW_H); // только отмеченные
    TREE->sortItems(0,Qt::AscendingOrder);
    qtools::expand(*TREE);

  QPen planItemsPen(Qt::darkBlue);
  QBrush planItemsTaskBrash(Qt::darkGray);
  QBrush planItemsPlanBrash(Qt::darkGray);
  QBrush planItemsProcedureBrash(Qt::darkGray);
  QBrush planItemsWorkBrash(Qt::blue);

  QPen realItemsPen(Qt::darkGreen);
  QBrush realItemsTaskBrash(Qt::blue);
  QBrush realItemsPlanBrash(Qt::blue);
  QBrush realItemsProcedureBrash(Qt::blue);
  QBrush realItemsWorkBrash(Qt::blue);

  TGantGraphicsView &gd = *DIAGR;
    gd.setHeaderHeight(HEADER_H);
    gd.setColumnWidth(COLUMN_W);
    gd.setRowHeight(ROW_H);
    gd.setGridPen(QPen(Qt::darkGray));
    gd.setWeekendPen(QPen(Qt::darkRed));
    gd.setGridBrush( QBrush(Qt::white));
    gd.setWeekendBrush(QBrush(Qt::lightGray));
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
            tskGit->setLabel(GANT_IND_PLAN,gen::intToStr(tsk->num()));
            tskGit->setPen(GANT_IND_PLAN,planItemsPen);
            tskGit->setBrush(GANT_IND_PLAN,planItemsTaskBrash);
            tskGit->setLevel(0);
            tskGit->setOpen(true);
            tskGit->setBegin(GANT_IND_PLAN,tsk->dayPlanBegin());
            tskGit->setEnd(GANT_IND_PLAN,tsk->dayPlanEnd());
            for (int i=0; i<2; i++)
                if (TCarryPlan *plan = plans[i])
                    if (plan->isChecked())
                    {
                      TGantItem *planGit(new TGantItem(TGantItem::gitPlan,plan->scrName(),tskGit));
                        planGit->setLabel(GANT_IND_PLAN,gen::intToStr(plan->num()));
                        planGit->setPen(GANT_IND_PLAN,planItemsPen);
                        planGit->setBrush(GANT_IND_PLAN,planItemsPlanBrash);
                        planGit->setLevel(1);
                        planGit->setOpen(true);
                        planGit->setBegin(GANT_IND_PLAN,plan->dayPlanBegin());
                        planGit->setEnd(GANT_IND_PLAN,plan->dayPlanEnd());
                        foreach (TCarryProcedure *pr,plan->procedures())
                        {
                          TGantItem *prGit(new TGantItem(TGantItem::gitProcedure,pr->scrName(),planGit));
                            prGit->setLabel(GANT_IND_PLAN,gen::intToStr(pr->num()));
                            prGit->setPen(GANT_IND_PLAN,planItemsPen);
                            prGit->setBrush(GANT_IND_PLAN,planItemsProcedureBrash);
                            prGit->setLevel(2);
                            prGit->setOpen(true);
                            prGit->setBegin(GANT_IND_PLAN,pr->dayPlanBegin());
                            prGit->setEnd(GANT_IND_PLAN,pr->dayPlanEnd());
                            foreach (TCarryWork *wrk,pr->works())
                            {
                              TGantItem *wrkGit(new TGantItem(TGantItem::gitWork,wrk->scrName(),prGit));
                                wrkGit->setLabel(GANT_IND_PLAN,gen::intToStr(wrk->num()));
                                wrkGit->setPen(GANT_IND_PLAN,planItemsPen);
                                wrkGit->setBrush(GANT_IND_PLAN,planItemsWorkBrash);
                                wrkGit->setLevel(3);
                                wrkGit->setOpen(true);
                                wrkGit->setBegin(GANT_IND_PLAN,wrk->dayPlanBegin());
                                wrkGit->setEnd(GANT_IND_PLAN,wrk->dayPlanEnd());
                            //PR3((wrkGit->level()+1)*4,"%1 [%2..%3]",wrkGit->name(),wrkGit->begin(0),wrkGit->end(0));
                                prGit->insertChild(wrkGit);
                            }
                        //PR3((prGit->level()+1)*4,"%1 [%2..%3]",prGit->name(),prGit->begin(0),prGit->end(0));
                            planGit->insertChild(prGit);
                        }
                    //PR3((planGit->level()+1)*4,"%1 [%2..%3]",planGit->name(),planGit->begin(0),planGit->end(0));
                        tskGit->insertChild(planGit);
                    }
        //PR3((tskGit->level()+1)*4,"%1 [%2..%3]",tskGit->name(),tskGit->begin(0),tskGit->end(0));
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
            tskGit->setBrush(GANT_IND_REAL,realItemsTaskBrash);
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
                        planGit->setBrush(GANT_IND_REAL,realItemsPlanBrash);
                        planGit->setLevel(1);
                        planGit->setOpen(true);
                        planGit->setBegin(GANT_IND_REAL,plan->dayRealBegin());
                        planGit->setEnd(GANT_IND_REAL,plan->dayRealEnd());
                        foreach (TCarryProcedure *pr,plan->procedures())
                        {
                          TGantItem *prGit(new TGantItem(TGantItem::gitProcedure,pr->scrName(),planGit));
                            prGit->setPen(GANT_IND_REAL,realItemsPen);
                            prGit->setBrush(GANT_IND_REAL,realItemsProcedureBrash);
                            prGit->setLevel(2);
                            prGit->setOpen(true);
                            prGit->setBegin(GANT_IND_REAL,pr->dayRealBegin());
                            prGit->setEnd(GANT_IND_REAL,pr->dayRealEnd());
                            foreach (TCarryWork *wrk,pr->works())
                            {
                              TGantItem *wrkGit(new TGantItem(TGantItem::gitWork,wrk->scrName(),prGit));
                                wrkGit->setPen(GANT_IND_REAL,realItemsPen);
                                wrkGit->setBrush(GANT_IND_REAL,realItemsWorkBrash);
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

    printDiagrammTree();

    gd.moveToDay(QDate::currentDate().dayOfYear()-1);
    gd.draw(whatdraw);

    printDiagrammTree();

}
//-----------------------------------------------------------------------------

void WGantDiagramm::printDiagrammTree()
{
  TGantGraphicsView &gd = *DIAGR;
    PR(0,"GantGraphicsView:");
    foreach (TGantItem *topIt,gd.topItems())
    {
    PR3((topIt->level()+1)*4,"%1 [%2..%3]",topIt->name(),topIt->begin(0),topIt->end(0)); // task
        foreach (TGantItem *planIt,topIt->childs())
        {
        PR3((planIt->level()+1)*4,"%1 [%2..%3]",planIt->name(),planIt->begin(0),planIt->end(0)); // plan
            foreach (TGantItem *procIt,planIt->childs())
            {
            PR3((procIt->level()+1)*4,"%1 [%2..%3]",procIt->name(),procIt->begin(0),procIt->end(0)); // procedure
                foreach (TGantItem *workIt,procIt->childs())
                {
                PR3((workIt->level()+1)*4,"%1 [%2..%3]",workIt->name(),workIt->begin(0),workIt->end(0)); // work
                }
            }
        }
    }
}
//-----------------------------------------------------------------------------
