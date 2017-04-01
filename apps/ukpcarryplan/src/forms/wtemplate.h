#ifndef WTEMPLATE_H
#define WTEMPLATE_H

#include <QFrame>
#include "ui_wtemplate.h"

class WTemplate : public QFrame, private Ui::WTemplate
{
    Q_OBJECT
private:
    void setEnabledControls(bool isenbl);
    void resetTemplates(const QPushButton &btn);
    void periodChanged(const QSpinBox &sb, int val=0);

public:
    explicit WTemplate(QWidget *parent = 0);
    ~WTemplate();

public slots:
    void resetTemplates();
    void selectPattern(const QString &nm);
    void selectPlanElement(QTreeWidgetItem *cur, QTreeWidgetItem *prev);
    void periodChanged(int);
    void toggledTimeVariant(bool);

};

#endif // WTEMPLATE_H
