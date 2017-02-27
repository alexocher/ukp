#ifndef TFRMMAINUKPCARRYPLAN_H
#define TFRMMAINUKPCARRYPLAN_H

#include <QMainWindow>
#include "ui_fmainukpcarryplan.h"

class TfrmMainUkpCarryPlan : public QMainWindow, private Ui::TfrmMainUkpCarryPlan
{
    Q_OBJECT
private:
    void createControls();
    void clickAction(const QAction &act);
    void getSigHideSubWindow(const QWidget &w);

public:
    explicit TfrmMainUkpCarryPlan(QWidget *parent=NULL);
    ~TfrmMainUkpCarryPlan();

    const QRect centralWidgetGlobalRect();
    const QRect formRect();

private slots:
    void clickAction();
    void getSigHideSubWindow();

};

#endif // TFRMMAINUKPCARRYPLAN_H
