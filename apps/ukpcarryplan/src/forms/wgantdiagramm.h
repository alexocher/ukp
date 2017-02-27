#ifndef WGANTDIAGRAMM_H
#define WGANTDIAGRAMM_H

#include <QDialog>
#include <TCarryTask>
#include <TGantGraphicsView>
#include "ui_wgantdiagramm.h"

class WGantDiagramm : public QDialog, private Ui::WGantDiagramm
{
    Q_OBJECT
private:
    void resetGantDiagramm(const QPushButton &btn);
    void printDiagrammTree();

public:
    explicit WGantDiagramm(QWidget *parent = 0);
    ~WGantDiagramm();

    void prepare(TCarryTaskList &tasks, TGantGraphicsView::ContentDraw whatdraw);

public slots:
    void resetGantDiagramm();

private:
    Ui::WGantDiagramm *ui;

};

#endif // WGANTDIAGRAMM_H
