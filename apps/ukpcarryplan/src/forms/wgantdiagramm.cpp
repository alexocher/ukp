
#include <QTreeWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QStack>
#include <QScrollBar>
#include <defMacro>
#include <qtools>
#include <gen>
#include <TModulePlans>
#include <WGantDiagramm>
#include <TAppCarryPlan>

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

    QComboBox *CB_SCALE(NULL);

    const int HEADER_H(60),
          COLUMN_W(30),
          ROW_H(40);

}

WGantDiagramm::WGantDiagramm(QWidget *parent) : QDialog(parent)
{
    ui->setupUi(this);

    QGridLayout *grl(new QGridLayout(this));

    TREE = new QTreeWidget(this);
    EL_MINMAX_WIDTH(TREE, 500);
    if (QTreeWidgetItem *hdrIt = TREE->headerItem())
    {
        hdrIt->setText(0, " Наименование");
        hdrIt->setTextAlignment(0, Qt::AlignLeft | Qt::AlignVCenter);
        QSize sz = hdrIt->sizeHint(0);
        sz.setHeight(HEADER_H);
        hdrIt->setSizeHint(0, sz);
    }

    TREE->setColumnWidth(0, 1000);
    TREE->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    TREE->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    TREE->setIconSize(QSize(36, 36));
    TREE->setUniformRowHeights(true);
    TREE->setSortingEnabled(true);
    TREE->setAlternatingRowColors(true);
    grl->addWidget(TREE, 0, 0, 1, 1);

    DIAGR = new TGantGraphicsView(0, -1, PROJ->workDayBegin(), this);
    DIAGR->set_scrollbarVert(TREE->verticalScrollBar());

    grl->addWidget(DIAGR, 0, 1, 1, 1);

    FR_BUTTONS = new QFrame(this);
    EL_MINMAX_HEIGHT(FR_BUTTONS, 50);

    QHBoxLayout *hbl(new QHBoxLayout(FR_BUTTONS));

    QLabel *LBL_SCALE(new QLabel("Масштаб", FR_BUTTONS));

    CB_SCALE = new QComboBox(FR_BUTTONS);
    EL_RESIZE(CB_SCALE, 120, 40);
    QStringList scales;
    scales << "Часы" << "Дни" << "Недели" << "Месяцы";
    CB_SCALE->addItems(scales);
    CB_SCALE->setCurrentIndex(1);
    connect(CB_SCALE, SIGNAL(currentIndexChanged(int)), this, SLOT(scaleChanged(int)));

    QFrame *LINE1(new QFrame(FR_BUTTONS));
    EL_RESIZE(LINE1, 30, 50);
    LINE1->setFrameShadow(QFrame::Raised);
    LINE1->setFrameShape(QFrame::VLine);

    BTN_ALL = new QPushButton("Все", FR_BUTTONS);
    EL_RESIZE(BTN_ALL, 200, 50);
    BTN_ALL->setCheckable(true);
    connect(BTN_ALL, SIGNAL(clicked()), this, SLOT(resetGantDiagramm()));

    BTN_PLAN = new QPushButton("План", FR_BUTTONS);
    EL_RESIZE(BTN_PLAN, 200, 50);
    BTN_PLAN->setCheckable(true);
    connect(BTN_PLAN, SIGNAL(clicked()), this, SLOT(resetGantDiagramm()));

    BTN_REAL = new QPushButton("Выполнение", FR_BUTTONS);
    EL_RESIZE(BTN_REAL, 200, 50);
    BTN_REAL->setCheckable(true);
    connect(BTN_REAL, SIGNAL(clicked()), this, SLOT(resetGantDiagramm()));

    QFrame *LINE2(new QFrame(FR_BUTTONS));
    EL_RESIZE(LINE2, 30, 50);
    LINE2->setFrameShadow(QFrame::Raised);
    LINE2->setFrameShape(QFrame::VLine);

    hbl->addWidget(LBL_SCALE);
    hbl->addWidget(CB_SCALE);
    hbl->addWidget(LINE1);
    hbl->addWidget(BTN_ALL);
    hbl->addWidget(BTN_ALL);
    hbl->addWidget(BTN_PLAN);
    hbl->addWidget(BTN_REAL);
    hbl->addWidget(LINE2);
    hbl->addStretch();

    hbl->setMargin(0);
    hbl->setSpacing(10);

    grl->addWidget(FR_BUTTONS, 1, 0, 1, 2);

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
    TGantGraphicsView::ScaleView sc((TGantGraphicsView::ScaleView)CB_SCALE->currentIndex());
    if (&btn == BTN_ALL)
    {
        prepare(modPlans->carryTasks(), TGantGraphicsView::cdAll, sc);
    }
    else if (&btn == BTN_PLAN)
    {
        prepare(modPlans->carryTasks(), TGantGraphicsView::cdPlan, sc);
    }
    else if (&btn == BTN_REAL)
    {
        prepare(modPlans->carryTasks(), TGantGraphicsView::cdReal, sc);
    }
}
//-----------------------------------------------------------------------------

void WGantDiagramm::scaleChanged(int ind)
{
    TGantGraphicsView &gd = *DIAGR;
    gd.setScaleView((TGantGraphicsView::ScaleView)ind);
}
//-----------------------------------------------------------------------------

void WGantDiagramm::prepare(TCarryTaskList &tasks, TGantGraphicsView::ContentDraw whatdraw, TGantGraphicsView::ScaleView sc)
{
    BTN_ALL->setChecked(whatdraw == TGantGraphicsView::cdAll);
    BTN_PLAN->setChecked(whatdraw == TGantGraphicsView::cdPlan);
    BTN_REAL->setChecked(whatdraw == TGantGraphicsView::cdReal);

    MODULE(Plans);
    modPlans->reflectCarryTasksToTree(tasks, *TREE, false, ROW_H); // отобразить все
    TREE->sortItems(0, Qt::AscendingOrder);
    qtools::expand(*TREE);

    QPen planItemsTaskPen(Qt::darkGray);
    QBrush planItemsTaskBrash(Qt::darkGray);
    QPen planItemsPlanPen(Qt::darkGray);
    QBrush planItemsPlanBrash(Qt::darkGray);
    QPen planItemsProcedurePen(Qt::darkGray);
    QBrush planItemsProcedureBrash(Qt::darkGray);
    QPen planItemsWorkPen(Qt::blue);
    QBrush planItemsWorkBrash(Qt::blue);

    TGantGraphicsView &gd = *DIAGR;
    gd.setScaleView(sc);
    gd.setHeaderHeight(HEADER_H);
    gd.setColumnWidth(COLUMN_W);
    gd.setRowHeight(ROW_H);
    gd.setGridPen(QPen(Qt::darkGray));
    gd.setWeekendPen(QPen(Qt::darkRed));
    gd.setGridBrush( QBrush(Qt::white));
    gd.setWeekendBrush(QBrush(Qt::lightGray));
    gd.clearTopItems();

    // плановые элементы
    if (whatdraw != TGantGraphicsView::cdReal)
    {
        foreach (TCarryTask *tsk, tasks)
        {
            TCarryPlan *plans[2] = { tsk->ordPlan(), tsk->carryPlan() };
            TGantItem *tskGit(new TGantItem(TGantItem::gitProject, tsk->scrName()));
            tskGit->setLabel(GANT_IND_PLAN, gen::intToStr(tsk->num()));
            tskGit->setPen(GANT_IND_PLAN, planItemsTaskPen);
            tskGit->setBrush(GANT_IND_PLAN, planItemsTaskBrash);
            tskGit->setLevel(0);
            tskGit->setOpen(true);
            tskGit->setBegin(GANT_IND_PLAN, tsk->dtPlanBegin() ? *tsk->dtPlanBegin() : QDateTime());
            tskGit->setEnd(GANT_IND_PLAN, tsk->dtPlanEnd() ? *tsk->dtPlanEnd() : QDateTime());
            for (int i = 0; i < 2; i++)
                if (TCarryPlan *plan = plans[i])
                {
                    TGantItem *planGit(new TGantItem(TGantItem::gitPlan, plan->scrName(), tskGit));
                    planGit->setLabel(GANT_IND_PLAN, gen::intToStr(plan->num()));
                    planGit->setPen(GANT_IND_PLAN, planItemsPlanPen);
                    planGit->setBrush(GANT_IND_PLAN, planItemsPlanBrash);
                    planGit->setLevel(1);
                    planGit->setOpen(true);
                    planGit->setBegin(GANT_IND_PLAN, plan->dtPlanBegin() ? *plan->dtPlanBegin() : QDateTime());
                    planGit->setEnd(GANT_IND_PLAN, plan->dtPlanEnd() ? *plan->dtPlanEnd() : QDateTime());
                    foreach (TCarryProcedure *pr, plan->procedures())
                    {
                        TGantItem *prGit(new TGantItem(TGantItem::gitProcedure, pr->scrName(), planGit));
                        prGit->setLabel(GANT_IND_PLAN, gen::intToStr(pr->num()));
                        prGit->setPen(GANT_IND_PLAN, planItemsProcedurePen);
                        prGit->setBrush(GANT_IND_PLAN, planItemsProcedureBrash);
                        prGit->setLevel(2);
                        prGit->setOpen(true);
                        prGit->setBegin(GANT_IND_PLAN, pr->dtPlanBegin() ? *pr->dtPlanBegin() : QDateTime());
                        prGit->setEnd(GANT_IND_PLAN, pr->dtPlanEnd() ? *pr->dtPlanEnd() : QDateTime());
                        foreach (TCarryWork *wrk, pr->works())
                        {
                            TGantItem *wrkGit(new TGantItem(TGantItem::gitWork, wrk->scrName(), prGit));
                            wrkGit->setLabel(GANT_IND_PLAN, gen::intToStr(wrk->num()));
                            wrkGit->setPen(GANT_IND_PLAN, planItemsWorkPen);
                            wrkGit->setBrush(GANT_IND_PLAN, planItemsWorkBrash);
                            wrkGit->setLevel(3);
                            wrkGit->setOpen(true);
                            wrkGit->setBegin(GANT_IND_PLAN, wrk->dtPlanBegin() ? *wrk->dtPlanBegin() : QDateTime());
                            wrkGit->setEnd(GANT_IND_PLAN, wrk->dtPlanEnd() ? *wrk->dtPlanEnd() : QDateTime());
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

    QPen realItemsTaskPen(Qt::darkRed);
    QBrush realItemsTaskBrash(Qt::darkRed);
    QPen realItemsPlanPen(Qt::darkRed);
    QBrush realItemsPlanBrash(Qt::darkRed);
    QPen realItemsProcedurePen(Qt::darkRed);
    QBrush realItemsProcedureBrash(Qt::darkRed);

    QPen realItemsGetWorkPen(Qt::darkGreen);
    QBrush realItemsGetWorkBrash(Qt::darkGreen);
    QPen realItemsBegWorkPen(Qt::green);
    QBrush realItemsBegWorkBrash(Qt::green);
    QPen realItemsEndWorkPen(Qt::blue);
    QBrush realItemsEndWorkBrash(Qt::blue);
    QPen realItemsProblemWorkPen(Qt::red);
    QBrush realItemsProblemWorkBrash(Qt::red);

    // реальные элементы
    if (whatdraw != TGantGraphicsView::cdPlan)
    {
        foreach (TCarryTask *tsk, tasks)
        {
            TCarryPlan *plans[2] = { tsk->ordPlan(), tsk->carryPlan() };
            TGantItem *tskGit(new TGantItem(TGantItem::gitProject, tsk->scrName()));
            tskGit->setPen(GANT_IND_REAL, realItemsTaskPen);
            tskGit->setBrush(GANT_IND_REAL, realItemsTaskBrash);
            tskGit->setLevel(0);
            tskGit->setOpen(true);
            tskGit->setBegin(GANT_IND_REAL, tsk->dtPlanBegin() ? *tsk->dtPlanBegin() : QDateTime());
            tskGit->setEnd(GANT_IND_REAL, tsk->dtPlanEnd() ? *tsk->dtPlanEnd() : QDateTime());
            tskGit->setCarryOutPercent(tsk->carryOutPercent());
            for (int i = 0; i < 2; i++)
                if (TCarryPlan *plan = plans[i])
                {
                    TGantItem *planGit(new TGantItem(TGantItem::gitPlan, plan->scrName(), tskGit));
                    planGit->setPen(GANT_IND_REAL, realItemsPlanPen);
                    planGit->setBrush(GANT_IND_REAL, realItemsPlanBrash);
                    planGit->setLevel(1);
                    planGit->setOpen(true);
                    planGit->setBegin(GANT_IND_REAL, plan->dtPlanBegin() ? *plan->dtPlanBegin() : QDateTime());
                    planGit->setEnd(GANT_IND_REAL, plan->dtPlanEnd() ? *plan->dtPlanEnd() : QDateTime());
                    planGit->setCarryOutPercent(plan->carryOutPercent());
                    foreach (TCarryProcedure *pr, plan->procedures())
                    {
                        TGantItem *prGit(new TGantItem(TGantItem::gitProcedure, pr->scrName(), planGit));
                        prGit->setPen(GANT_IND_REAL, realItemsProcedurePen);
                        prGit->setBrush(GANT_IND_REAL, realItemsProcedureBrash);
                        prGit->setLevel(2);
                        prGit->setOpen(true);
                        prGit->setBegin(GANT_IND_REAL, pr->dtPlanBegin() ? *pr->dtPlanBegin() : QDateTime());
                        prGit->setEnd(GANT_IND_REAL, pr->dtPlanEnd() ? *pr->dtPlanEnd() : QDateTime());
                        prGit->setCarryOutPercent(pr->carryOutPercent());
                        foreach (TCarryWork *wrk, pr->works())
                        {
                            TGantItem *wrkGit(new TGantItem(TGantItem::gitWork, wrk->scrName(), prGit));
                            switch (wrk->condition())
                            {
                                case cocCarryBeg:
                                    wrkGit->setPen(GANT_IND_REAL, realItemsBegWorkPen);
                                    wrkGit->setBrush(GANT_IND_REAL, realItemsBegWorkBrash);
                                    break;
                                case cocCarryEnd:
                                    wrkGit->setPen(GANT_IND_REAL, realItemsEndWorkPen);
                                    wrkGit->setBrush(GANT_IND_REAL, realItemsEndWorkBrash);
                                    break;
                                case cocProblem:
                                    wrkGit->setPen(GANT_IND_REAL, realItemsProblemWorkPen);
                                    wrkGit->setBrush(GANT_IND_REAL, realItemsProblemWorkBrash);
                                    break;
                                default: // cocGet
                                    wrkGit->setPen(GANT_IND_REAL, realItemsGetWorkPen);
                                    wrkGit->setBrush(GANT_IND_REAL, realItemsGetWorkBrash);
                            }
                            wrkGit->setLevel(3);
                            wrkGit->setOpen(true);
                            wrkGit->setBegin(GANT_IND_REAL, wrk->dtPlanBegin() ? *wrk->dtPlanBegin() : QDateTime());
                            wrkGit->setEnd(GANT_IND_REAL, wrk->dtPlanEnd() ? *wrk->dtPlanEnd() : QDateTime());
                            wrkGit->setCarryOutPercent(wrk->carryOutPercent());
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

    gd.moveToDay(QDate::currentDate().dayOfYear() - 1);
    gd.draw(whatdraw);

    printDiagrammTree();

}
//-----------------------------------------------------------------------------

void WGantDiagramm::printDiagrammTree()
{
    TGantGraphicsView &gd = *DIAGR;
    PR(0, "GantGraphicsView:");
    foreach (TGantItem *topIt, gd.topItems())
    {
        PR3((topIt->level() + 1) * 4, "%1 [%2..%3]", topIt->name(), topIt->begin(0).isValid() ? topIt->begin(0).toString("dd.MM hh:00") : "", topIt->end(0).isValid() ? topIt->end(0).toString("dd.MM hh:00") : ""); // task
        foreach (TGantItem *planIt, topIt->childs())
        {
            PR3((planIt->level() + 1) * 4, "%1 [%2..%3]", planIt->name(), planIt->begin(0).isValid() ? planIt->begin(0).toString("dd.MM hh:00") : "", planIt->end(0).isValid() ? planIt->end(0).toString("dd.MM hh:00") : ""); // plan
            foreach (TGantItem *procIt, planIt->childs())
            {
                PR3((procIt->level() + 1) * 4, "%1 [%2..%3]", procIt->name(), procIt->begin(0).isValid() ? procIt->begin(0).toString("dd.MM hh:00") : "", procIt->end(0).isValid() ? procIt->end(0).toString("dd.MM hh:00") : ""); // procedure
                foreach (TGantItem *workIt, procIt->childs())
                {
                    PR3((workIt->level() + 1) * 4, "%1 [%2..%3]", workIt->name(), workIt->begin(0).isValid() ? workIt->begin(0).toString("dd.MM hh:00") : "", workIt->end(0).isValid() ? workIt->end(0).toString("dd.MM hh:00") : ""); // work
                }
            }
        }
    }
}
//-----------------------------------------------------------------------------
