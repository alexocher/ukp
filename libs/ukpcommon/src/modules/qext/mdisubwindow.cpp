#include <TMdiSubwindow>

TMdiSubWindow::TMdiSubWindow(QWidget *parent, Qt::WindowFlags flags) : QMdiSubWindow(parent,flags)
{

}
//-----------------------------------------------------------------------------

TMdiSubWindow::~TMdiSubWindow()
{

}
//-----------------------------------------------------------------------------

void TMdiSubWindow::hideEvent(QHideEvent *ev)
{
    emit sigHideSubWindow();
    QMdiSubWindow::hideEvent(ev);
}
//-----------------------------------------------------------------------------
