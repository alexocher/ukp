#ifndef WEMPLOYEECALENDAR_H
#define WEMPLOYEECALENDAR_H

#include <QFrame>
#include "ui_wemployeecalendar.h"

class WEmployeeCalendar : public QFrame, private Ui::WEmployeeCalendar
{
    Q_OBJECT
public:
    explicit WEmployeeCalendar(QWidget *parent = 0);
    ~WEmployeeCalendar();

};

#endif // WEMPLOYEECALENDAR_H
