#ifndef WPLAN_H
#define WPLAN_H

#include <QFrame>
#include "ui_wplan.h"

class WPlan : public QFrame, private Ui::WPlan
{
    Q_OBJECT
private:
    void resetPlan(const QPushButton &btn);

public:
    explicit WPlan(QWidget *parent = 0);
    ~WPlan();

    void fillForm();

public slots:
    void resetPlan();
    void selectPlanElement(QTreeWidgetItem *cur, QTreeWidgetItem *prev);
    void onItemChanged(QTreeWidgetItem *it, int col);

};

#endif // WPLAN_H
