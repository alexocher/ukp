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
    explicit WGantDiagramm(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
    ~WGantDiagramm();

    void prepare(TCarryTaskList &tasks, TGantGraphicsView::ContentDraw whatdraw, TGantGraphicsView::ScaleView sc);

public slots:
    void resetGantDiagramm();
    void scaleChanged(int ind);

private:
    Ui::WGantDiagramm *ui;

};

#endif // WGANTDIAGRAMM_H
