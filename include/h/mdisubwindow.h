#ifndef TMDISUBWINDOW_H
#define TMDISUBWINDOW_H

#include <QMdiSubWindow>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT TMdiSubWindow: public QMdiSubWindow
{
    Q_OBJECT
public:
    TMdiSubWindow(QWidget *parent=0, Qt::WindowFlags flags=0);
    ~TMdiSubWindow();

signals:
    void sigHideSubWindow();

protected:
    virtual void hideEvent(QHideEvent *ev);

};

#endif // TMDISUBWINDOW_H
