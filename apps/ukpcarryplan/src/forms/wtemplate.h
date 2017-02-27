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

public:
    explicit WTemplate(QWidget *parent = 0);
    ~WTemplate();

public slots:
    void resetTemplates();
    void selectPattern(const QString &nm);
    void selectPlanElement(QTreeWidgetItem *cur, QTreeWidgetItem *prev);

};

#endif // WTEMPLATE_H
