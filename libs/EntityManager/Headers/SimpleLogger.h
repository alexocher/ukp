#ifndef SIMPLELOGGER
#define SIMPLELOGGER

#include "commondef.h"
#include <QObject>
#include <QFile>
#include <QTextStream>

/*!
 * \brief Класс для ведения лога
 */
class ENTITYMANAGERSHARED_EXPORT SimpleLogger:public QObject{
    Q_OBJECT
    // уровни
    enum SEVERITY {
        _INFO = 1000,
        _WARNING,
        _ERROR
    };

private:
    explicit SimpleLogger();

    void read_config();
    void write_log(SEVERITY type,const QString& mes);

private:
    static SimpleLogger* _instance;
    static bool _isInst;

    QFile       *_flog;
    int         _maxlogsize;
public:
    ~SimpleLogger();

    static SimpleLogger* Instance();
public slots:
    void warn(QString);
    void err(QString);
    void log(QString);
};



class ENTITYMANAGERSHARED_EXPORT LogObserv:public QObject{
    Q_OBJECT

public:
    explicit LogObserv(){
        SimpleLogger* _log = SimpleLogger::Instance();
        connect(this,SIGNAL( slog(QString) ),_log,SLOT( log(QString) )  );
        connect(this,SIGNAL( serr(QString) ),_log,SLOT( err(QString) )  );
        connect(this,SIGNAL( swarn(QString) ),_log,SLOT( warn(QString) )  );
    }

    ~LogObserv(){}
    void log(const QString& mes){
        emit slog(mes);
    }
    void warn(const QString& mes){
        emit swarn(mes);
    }
    void err(const QString& mes){
        emit serr(mes);
    }
signals:
    void slog(QString);
    void serr(QString);
    void swarn(QString);

};




#endif // SIMPLELOGGER




