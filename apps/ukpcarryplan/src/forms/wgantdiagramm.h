#ifndef WGANTDIAGRAMM_H
#define WGANTDIAGRAMM_H

#include <QDialog>
#include <TCarryTask>
#include <TGantGraphicsView>
#include <QComboBox>
#include <QDateTimeEdit>
#include "ui_wgantdiagramm.h"

class WGantDiagramm : public QDialog, private Ui::WGantDiagramm
{
    Q_OBJECT
private:
    void resetGantDiagramm(const QPushButton &btn);
    void printDiagrammTree();

public:
    explicit WGantDiagramm(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
    ~WGantDiagramm();

    void prepare(TCarryTaskList &tasks, TGantGraphicsView::ContentDraw whatdraw, TGantGraphicsView::ScaleView sc);

public slots:
    void resetGantDiagramm();
    void scaleChanged(int ind);

private:
    Ui::WGantDiagramm *m_ui;

    QTreeWidget *m_tree;

    TGantGraphicsView *m_diagr;

    QFrame *m_frButtons;

    QPushButton *m_pbAll;

    QPushButton *m_pbPlan;

    QPushButton *m_pbReal;

    QPushButton *m_pbExpand;

    QPushButton *m_pbCollapse;

    QComboBox *m_cbScale;

    QPushButton *m_pbTest;

    QDateTimeEdit *m_dtTest;

};

#endif // WGANTDIAGRAMM_H
