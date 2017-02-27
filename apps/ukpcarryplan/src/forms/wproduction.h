#ifndef WPRODUCTION_H
#define WPRODUCTION_H

#include <QFrame>
#include "ui_wproduction.h"

class WProduction : public QFrame, private Ui::WProduction
{
    Q_OBJECT

public:
    explicit WProduction(QWidget *parent = 0);
    ~WProduction();

    void fillForm();

};

#endif // WPRODUCTION_H
