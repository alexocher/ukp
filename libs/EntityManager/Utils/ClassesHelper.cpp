#include "../Headers/commondef.h"
#include <QDebug>
void custom_console(const QString& mes){
#ifdef Q_OS_WIN32
    std::cout << mes.toLocal8Bit().data()<<std::endl;
#else
    qDebug()<<mes;
#endif
}
