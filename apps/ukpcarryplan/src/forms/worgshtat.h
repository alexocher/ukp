#ifndef WORGSHTAT_H
#define WORGSHTAT_H

#include <QFrame>
#include "ui_worgshtat.h"

class WOrgShtat : public QFrame, private Ui::WOrgShtat
{
    Q_OBJECT

public:
    explicit WOrgShtat(QWidget *parent = 0);
    ~WOrgShtat();

    void fillForm();

};

#endif // WORGSHTAT_H
