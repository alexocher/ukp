
#include <QTreeWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QStack>
#include <QScrollBar>
#include <defMacro>
#include <qtools>
#include <gen>
#include <defPictures>
#include <TModulePlans>
#include <WGantDiagramm>
#include <TAppCarryPlan>

WGantDiagramm *wGantDiagramm(NULL);

namespace
{

    const int HEADER_H(60);
    const int COLUMN_W(30);
    const int ROW_H(40);

    int CURRENT_LEVEL(3);

}

WGantDiagramm::WGantDiagramm(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent,f),
    m_tree(nullptr),
    m_diagr(nullptr),
    m_frButtons(nullptr),
    m_pbAll(nullptr),
    m_pbPlan(nullptr),
    m_pbReal(nullptr),
    m_pbExpand(nullptr),
    m_pbCollapse(nullptr),
    m_cbScale(nullptr),
    m_pbTest(nullptr),
    m_dtTest(nullptr)
{
    m_ui->setupUi(this);

    QGridLayout *grl(new QGridLayout(this));

    m_tree = new QTreeWidget(this);
    EL_MINMAX_WIDTH(m_tree, 500);
    if (QTreeWidgetItem *hdrIt = m_tree->headerItem())
    {
        hdrIt->setText(0, " Наименование");
        hdrIt->setTextAlignment(0, Qt::AlignLeft | Qt::AlignVCenter);
        QSize sz = hdrIt->sizeHint(0);
        sz.setHeight(HEADER_H);
        hdrIt->setSizeHint(0, sz);
    }

    m_tree->setColumnWidth(0, 1000);
    m_tree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_tree->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_tree->setIconSize(QSize(36, 36));
    m_tree->setUniformRowHeights(true);
    m_tree->setSortingEnabled(true);
    m_tree->setAlternatingRowColors(true);
    grl->addWidget(m_tree, 0, 0, 1, 1);

    m_diagr = new TGantGraphicsView(0, -1, PROJ->workDayBegin(), this);
    m_diagr->set_scrollbarVert(m_tree->verticalScrollBar());
    m_diagr->set_tree(m_tree);

    grl->addWidget(m_diagr, 0, 1, 1, 1);

    m_frButtons = new QFrame(this);
    EL_MINMAX_HEIGHT(m_frButtons, 50);

    QHBoxLayout *hbl(new QHBoxLayout(m_frButtons));

    QLabel *LBL_SCALE(new QLabel("Масштаб", m_frButtons));

    m_cbScale = new QComboBox(m_frButtons);
    EL_RESIZE(m_cbScale, 120, 40);
    QStringList scales;
    scales << "Часы" << "Дни" << "Недели" << "Месяцы";
    m_cbScale->addItems(scales);
    m_cbScale->setCurrentIndex(1);
    connect(m_cbScale, SIGNAL(currentIndexChanged(int)), this, SLOT(scaleChanged(int)));

    QFrame *LINE1(new QFrame(m_frButtons));
    EL_RESIZE(LINE1, 30, 50);
    LINE1->setFrameShadow(QFrame::Raised);
    LINE1->setFrameShape(QFrame::VLine);

    m_pbAll = new QPushButton("Все", m_frButtons);
    EL_RESIZE(m_pbAll, 200, 50);
    m_pbAll->setCheckable(true);
    connect(m_pbAll, SIGNAL(clicked()), this, SLOT(resetGantDiagramm()));

    m_pbPlan = new QPushButton("План", m_frButtons);
    EL_RESIZE(m_pbPlan, 200, 50);
    m_pbPlan->setCheckable(true);
    connect(m_pbPlan, SIGNAL(clicked()), this, SLOT(resetGantDiagramm()));

    m_pbReal = new QPushButton("Выполнение", m_frButtons);
    EL_RESIZE(m_pbReal, 200, 50);
    m_pbReal->setCheckable(true);
    connect(m_pbReal, SIGNAL(clicked()), this, SLOT(resetGantDiagramm()));

    QFrame *LINE2(new QFrame(m_frButtons));
    EL_RESIZE(LINE2, 30, 50);
    LINE2->setFrameShadow(QFrame::Raised);
    LINE2->setFrameShape(QFrame::VLine);

    m_pbExpand = new QPushButton(m_frButtons);
    //QPushButton &btn = *m_pbExpand;
    m_pbExpand->setIconSize(QSize(32,32));
    m_pbExpand->setIcon(ICONPIX(PIX_DOWN));
    EL_RESIZE(m_pbExpand, 50, 50);
    connect(m_pbExpand, SIGNAL(clicked()), this, SLOT(resetGantDiagramm()));

    m_pbCollapse = new QPushButton(m_frButtons);
    m_pbCollapse->setIcon(ICONPIX(PIX_UP));
    m_pbCollapse->setIconSize(QSize(32,32));
    EL_RESIZE(m_pbCollapse, 50, 50);
    connect(m_pbCollapse, SIGNAL(clicked()), this, SLOT(resetGantDiagramm()));

    m_dtTest = new QDateTimeEdit(QDateTime(QDate::currentDate(),QTime::currentTime()),m_frButtons);
    EL_RESIZE(m_dtTest, 120, 30);
    m_dtTest->setDisplayFormat("hh:mm dd.MM");

    m_pbTest = new QPushButton("Тест", m_frButtons);
    EL_RESIZE(m_pbTest, 200, 50);
    connect(m_pbTest, SIGNAL(clicked()), this, SLOT(resetGantDiagramm()));

    hbl->addWidget(LBL_SCALE);
    hbl->addWidget(m_cbScale);
    hbl->addWidget(LINE1);
    hbl->addWidget(m_pbAll);
    hbl->addWidget(m_pbAll);
    hbl->addWidget(m_pbPlan);
    hbl->addWidget(m_pbReal);
    hbl->addWidget(LINE2);
    hbl->addWidget(m_pbExpand);
    hbl->addWidget(m_pbCollapse);
    hbl->addStretch();
    hbl->addWidget(m_dtTest);
    hbl->addWidget(m_pbTest);

    hbl->setMargin(0);
    hbl->setSpacing(10);

    grl->addWidget(m_frButtons, 1, 0, 1, 2);

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
    TGantGraphicsView::ScaleView sc((TGantGraphicsView::ScaleView)m_cbScale->currentIndex());
    if (&btn == m_pbAll)
    {
        prepare(modPlans->carryTasks(), TGantGraphicsView::cdAll, sc);
    }
    else if (&btn == m_pbPlan)
    {
        prepare(modPlans->carryTasks(), TGantGraphicsView::cdPlan, sc);
    }
    else if (&btn == m_pbReal)
    {
        prepare(modPlans->carryTasks(), TGantGraphicsView::cdReal, sc);
    }
    else if (&btn == m_pbExpand)
    {
        if (CURRENT_LEVEL<3) CURRENT_LEVEL++;
        qtools::expand(*m_tree,CURRENT_LEVEL);
    }
    else if (&btn == m_pbCollapse)
    {
        if (CURRENT_LEVEL>0) CURRENT_LEVEL--;
        qtools::expand(*m_tree,CURRENT_LEVEL);
    }
    else if (&btn == m_pbTest)
    {
      int SECS(172800); // 2 дня
      qsrand(uint(QTime::currentTime().msec()));

        foreach (TCarryTask *tsk, modPlans->carryTasks())
        {
          TCarryPlan *plans[2] = { NULL };
            if (tsk->ordPlan() && tsk->carryPlan())
            {
                if (tsk->ordPlan()->scrName()<tsk->carryPlan()->scrName())
                {
                    plans[0] = tsk->ordPlan(); plans[1] = tsk->carryPlan();
                }
                else
                {
                    plans[1] = tsk->ordPlan(); plans[0] = tsk->carryPlan();
                }
            }
            else if (tsk->ordPlan()) plans[0] = tsk->ordPlan();
            else if (tsk->carryPlan()) plans[0] = tsk->carryPlan();

            tsk->setDtRealBegin(tsk->dtPlanBegin() ? tsk->dtPlanBegin()->addSecs(SECS) : QDateTime());
            tsk->setDtRealEnd(tsk->dtPlanEnd() ? tsk->dtPlanEnd()->addSecs(SECS) : QDateTime());
            tsk->setCarryOutPercent(rand() % (100-0+1)+0);
            for (int i = 0; i < 2; i++)
                if (TCarryPlan *plan = plans[i])
                {
                    plan->setDtRealBegin(plan->dtPlanBegin() ? plan->dtPlanBegin()->addSecs(SECS) : QDateTime());
                    plan->setDtRealEnd(plan->dtPlanEnd() ? plan->dtPlanEnd()->addSecs(SECS) : QDateTime());
                    plan->setCarryOutPercent(rand() % (100-0+1)+0);
                    foreach (TCarryProcedure *pr, plan->procedures())
                    {
                        pr->setDtRealBegin(pr->dtPlanBegin() ? pr->dtPlanBegin()->addSecs(SECS) : QDateTime());
                        pr->setDtRealEnd(pr->dtPlanEnd() ? pr->dtPlanEnd()->addSecs(SECS) : QDateTime());
                        pr->setCarryOutPercent(rand() % (100-0+1)+0);
                        foreach (TCarryWork *wrk, pr->works())
                        {
                            wrk->setDtRealBegin(wrk->dtPlanBegin() ? wrk->dtPlanBegin()->addSecs(SECS) : QDateTime());
                            wrk->setDtRealEnd(wrk->dtPlanEnd() ? wrk->dtPlanEnd()->addSecs(SECS) : QDateTime());
                            wrk->setCarryOutPercent(rand() % (100-0+1)+0);
                        }
                    }
                }
        }
    }
}
//-----------------------------------------------------------------------------

void WGantDiagramm::scaleChanged(int ind)
{
    TGantGraphicsView &gd = *m_diagr;
    gd.setScaleView(TGantGraphicsView::ScaleView(ind));
}
//-----------------------------------------------------------------------------

void WGantDiagramm::prepare(TCarryTaskList &tasks, TGantGraphicsView::ContentDraw whatdraw, TGantGraphicsView::ScaleView sc)
{
    m_pbAll->setChecked(whatdraw == TGantGraphicsView::cdAll);
    m_pbPlan->setChecked(whatdraw == TGantGraphicsView::cdPlan);
    m_pbReal->setChecked(whatdraw == TGantGraphicsView::cdReal);

    m_cbScale->setCurrentIndex((int)sc);

  MODULE(Plans);
    modPlans->reflectCarryTasksToTree(tasks, *m_tree, false, ROW_H); // отобразить все
    m_tree->sortItems(0, Qt::AscendingOrder);
    m_diagr->disconnect_tree();
    qtools::expand(*m_tree);
    CURRENT_LEVEL = 3;
    m_diagr->set_tree(m_tree);

  // Плановые
  QPen planItemsTaskPen(Qt::darkGray);
  QBrush planItemsTaskBrash(Qt::darkGray);
  QPen planItemsPlanPen(Qt::darkGray);
  QBrush planItemsPlanBrash(Qt::darkGray);
  QPen planItemsProcedurePen(Qt::darkGray);
  QBrush planItemsProcedureBrash(Qt::darkGray);
  QPen planItemsWorkPen(Qt::blue);
  QBrush planItemsWorkBrash(Qt::blue);
  // Реальные
  QPen realItemsTaskPen(Qt::darkRed);
  QBrush realItemsTaskBrash(Qt::darkRed);
  QPen realItemsPlanPen(Qt::darkRed);
  QBrush realItemsPlanBrash(Qt::darkRed);
  QPen realItemsProcedurePen(Qt::darkRed);
  QBrush realItemsProcedureBrash(Qt::darkRed);
  // ... работы в зависимости от статуса
  QPen realItemsGetWorkPen(Qt::darkGreen);
  QBrush realItemsGetWorkBrash(Qt::darkGreen);
  QPen realItemsBegWorkPen(Qt::green);
  QBrush realItemsBegWorkBrash(Qt::green);
  QPen realItemsEndWorkPen(Qt::blue);
  QBrush realItemsEndWorkBrash(Qt::blue);
  QPen realItemsProblemWorkPen(Qt::red);
  QBrush realItemsProblemWorkBrash(Qt::red);

  TGantGraphicsView &gd = *m_diagr;
    gd.setScaleView(sc);
    gd.setHeaderHeight(HEADER_H);
    gd.setColumnWidth(COLUMN_W);
    gd.setRowHeight(ROW_H);
    gd.setGridPen(QPen(Qt::darkGray));
    gd.setWeekendPen(QPen(Qt::darkRed));
    gd.setGridBrush( QBrush(Qt::white));
    gd.setWeekendBrush(QBrush(Qt::lightGray));
    gd.clearTopItems();

  bool showPlan(whatdraw != TGantGraphicsView::cdReal), showReal(whatdraw != TGantGraphicsView::cdPlan);

    foreach (TCarryTask *tsk, tasks)
    {
      TCarryPlan *plans[2] = { NULL };
        if (tsk->ordPlan() && tsk->carryPlan())
        {
            if (tsk->ordPlan()->scrName()<tsk->carryPlan()->scrName())
            {
                plans[0] = tsk->ordPlan(); plans[1] = tsk->carryPlan();
            }
            else
            {
                plans[1] = tsk->ordPlan(); plans[0] = tsk->carryPlan();
            }
        }
        else if (tsk->ordPlan()) plans[0] = tsk->ordPlan();
        else if (tsk->carryPlan()) plans[0] = tsk->carryPlan();
      TGantItem *tskGit(new TGantItem(TGantItem::gitProject, tsk->scrName(),NULL,tsk->id(),tsk->num()));
        tskGit->setLevel(0);
        tskGit->setOpen(true);
        if (showPlan)
        {
            tskGit->setLabel(GANT_IND_PLAN, gen::intToStr(tsk->num()));
            tskGit->setPen(GANT_IND_PLAN, planItemsTaskPen);
            tskGit->setBrush(GANT_IND_PLAN, planItemsTaskBrash);
            tskGit->setBegin(GANT_IND_PLAN, tsk->dtPlanBegin() ? *tsk->dtPlanBegin() : QDateTime());
            tskGit->setEnd(GANT_IND_PLAN, tsk->dtPlanEnd() ? *tsk->dtPlanEnd() : QDateTime());
        }
        if (showReal)
        {
            tskGit->setPen(GANT_IND_REAL, realItemsTaskPen);
            tskGit->setBrush(GANT_IND_REAL, realItemsTaskBrash);
            tskGit->setBegin(GANT_IND_REAL, tsk->dtRealBegin() ? *tsk->dtRealBegin() : QDateTime());
            tskGit->setEnd(GANT_IND_REAL, tsk->dtRealEnd() ? *tsk->dtRealEnd() : QDateTime());
            tskGit->setCarryOutPercent(tsk->carryOutPercent()); // !!! отображать только для GANT_IND_REAL
        }
        for (int i = 0; i < 2; i++)
            if (TCarryPlan *plan = plans[i])
            {
              TGantItem *planGit(new TGantItem(TGantItem::gitPlan, plan->scrName(),tskGit,plan->id(),plan->num()));
                planGit->setLevel(1);
                planGit->setOpen(true);
                if (showPlan)
                {
                    planGit->setLabel(GANT_IND_PLAN, gen::intToStr(plan->num()));
                    planGit->setPen(GANT_IND_PLAN, planItemsPlanPen);
                    planGit->setBrush(GANT_IND_PLAN, planItemsPlanBrash);
                    planGit->setBegin(GANT_IND_PLAN, plan->dtPlanBegin() ? *plan->dtPlanBegin() : QDateTime());
                    planGit->setEnd(GANT_IND_PLAN, plan->dtPlanEnd() ? *plan->dtPlanEnd() : QDateTime());
                }
                if (showReal)
                {
                    planGit->setPen(GANT_IND_REAL, realItemsPlanPen);
                    planGit->setBrush(GANT_IND_REAL, realItemsPlanBrash);
                    planGit->setBegin(GANT_IND_REAL, plan->dtRealBegin() ? *plan->dtRealBegin() : QDateTime());
                    planGit->setEnd(GANT_IND_REAL, plan->dtRealEnd() ? *plan->dtRealEnd() : QDateTime());
                    planGit->setCarryOutPercent(plan->carryOutPercent()); // !!! отображать только для GANT_IND_REAL
                }
                foreach (TCarryProcedure *pr, plan->procedures())
                {
                  TGantItem *prGit(new TGantItem(TGantItem::gitProcedure, pr->scrName(),planGit,pr->id(),pr->num()));
                    prGit->setLevel(2);
                    prGit->setOpen(true);
                    if (showPlan)
                    {
                        prGit->setLabel(GANT_IND_PLAN, gen::intToStr(pr->num()));
                        prGit->setPen(GANT_IND_PLAN, planItemsProcedurePen);
                        prGit->setBrush(GANT_IND_PLAN, planItemsProcedureBrash);
                        prGit->setBegin(GANT_IND_PLAN, pr->dtPlanBegin() ? *pr->dtPlanBegin() : QDateTime());
                        prGit->setEnd(GANT_IND_PLAN, pr->dtPlanEnd() ? *pr->dtPlanEnd() : QDateTime());
                    }
                    if (showReal)
                    {
                        prGit->setPen(GANT_IND_REAL, realItemsProcedurePen);
                        prGit->setBrush(GANT_IND_REAL, realItemsProcedureBrash);
                        prGit->setBegin(GANT_IND_REAL, pr->dtRealBegin() ? *pr->dtRealBegin() : QDateTime());
                        prGit->setEnd(GANT_IND_REAL, pr->dtRealEnd() ? *pr->dtRealEnd() : QDateTime());
                        prGit->setCarryOutPercent(pr->carryOutPercent()); // !!! отображать только для GANT_IND_REAL
                    }
                    foreach (TCarryWork *wrk, pr->works())
                    {
                      TGantItem *wrkGit(new TGantItem(TGantItem::gitWork, wrk->scrName(),prGit,wrk->id(),wrk->num()));
                        wrkGit->setLevel(3);
                        wrkGit->setOpen(true);
                        if (showPlan)
                        {
                            wrkGit->setLabel(GANT_IND_PLAN, gen::intToStr(wrk->num()));
                            wrkGit->setPen(GANT_IND_PLAN, planItemsWorkPen);
                            wrkGit->setBrush(GANT_IND_PLAN, planItemsWorkBrash);
                            wrkGit->setBegin(GANT_IND_PLAN, wrk->dtPlanBegin() ? *wrk->dtPlanBegin() : QDateTime());
                            wrkGit->setEnd(GANT_IND_PLAN, wrk->dtPlanEnd() ? *wrk->dtPlanEnd() : QDateTime());
                            //PR3((wrkGit->level()+1)*4,"%1 [%2..%3]",wrkGit->name(),wrkGit->begin(0),wrkGit->end(0));
                        }
                        if (showReal)
                        {
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
                            wrkGit->setBegin(GANT_IND_REAL, wrk->dtRealBegin() ? *wrk->dtRealBegin() : QDateTime());
                            wrkGit->setEnd(GANT_IND_REAL, wrk->dtRealEnd() ? *wrk->dtRealEnd() : QDateTime());
                            wrkGit->setCarryOutPercent(wrk->carryOutPercent()); // !!! отображать только для GANT_IND_REAL
                        }
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

    PR(0, "111:");
    printDiagrammTree();

    gd.moveToDay(QDate::currentDate().dayOfYear() - 1);
    gd.draw(whatdraw);

    PR(0, "222:");
    //printDiagrammTree();

}
//-----------------------------------------------------------------------------

void WGantDiagramm::printDiagrammTree()
{
    TGantGraphicsView &gd = *m_diagr;
    PR(0, "GantGraphicsView:");
    foreach (TGantItem *topIt, gd.topItems())
    {
        PR6((topIt->level() + 1) * 4, "%1 [план: %2..%3, вып.: %4..%5 (%6)]", topIt->name(), topIt->begin(0).isValid() ? topIt->begin(0).toString("dd.MM hh:00") : "", topIt->end(0).isValid() ? topIt->end(0).toString("dd.MM hh:00") : "", topIt->begin(1).isValid() ? topIt->begin(1).toString("dd.MM hh:00") : "", topIt->end(1).isValid() ? topIt->end(1).toString("dd.MM hh:00") : "",topIt->carryOutPercent()); // task
        foreach (TGantItem *planIt, topIt->childs())
        {
            PR6((planIt->level() + 1) * 4, "%1 [план: %2..%3, вып.: %4..%5 (%6)]", planIt->name(), planIt->begin(0).isValid() ? planIt->begin(0).toString("dd.MM hh:00") : "", planIt->end(0).isValid() ? planIt->end(0).toString("dd.MM hh:00") : "", planIt->begin(1).isValid() ? planIt->begin(1).toString("dd.MM hh:00") : "", planIt->end(1).isValid() ? planIt->end(1).toString("dd.MM hh:00") : "",planIt->carryOutPercent()); // plan
            foreach (TGantItem *procIt, planIt->childs())
            {
                PR6((procIt->level() + 1) * 4, "%1 [план: %2..%3, вып.: %4..%5 (%6)]", procIt->name(), procIt->begin(0).isValid() ? procIt->begin(0).toString("dd.MM hh:00") : "", procIt->end(0).isValid() ? procIt->end(0).toString("dd.MM hh:00") : "", procIt->begin(1).isValid() ? procIt->begin(1).toString("dd.MM hh:00") : "", procIt->end(1).isValid() ? procIt->end(1).toString("dd.MM hh:00") : "",procIt->carryOutPercent()); // procedure
                foreach (TGantItem *workIt, procIt->childs())
                {
                    PR6((workIt->level() + 1) * 4, "%1 [план: %2..%3, вып.: %4..%5 (%6)]", workIt->name(), workIt->begin(0).isValid() ? workIt->begin(0).toString("dd.MM hh:00") : "", workIt->end(0).isValid() ? workIt->end(0).toString("dd.MM hh:00") : "", workIt->begin(1).isValid() ? workIt->begin(1).toString("dd.MM hh:00") : "", workIt->end(1).isValid() ? workIt->end(1).toString("dd.MM hh:00") : "",workIt->carryOutPercent()); // work
                }
            }
        }
    }
}
//-----------------------------------------------------------------------------
