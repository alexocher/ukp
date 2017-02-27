#ifndef WCOMPANYCALENDAR_H
#define WCOMPANYCALENDAR_H

#include <QFrame>
#include "ui_wcompanycalendar.h"

class WCompanyCalendar : public QFrame, private Ui::WCompanyCalendar
{
    Q_OBJECT

public:
    explicit WCompanyCalendar(QWidget *parent = 0);
    ~WCompanyCalendar();

private:
    Ui::WCompanyCalendar *ui;
};

#endif // WCOMPANYCALENDAR_H
