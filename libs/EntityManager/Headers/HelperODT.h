#ifndef HELPERODT
#define HELPERODT

#include <QTemporaryDir>
#include <QTextStream>
#include <QMap>
#include "commondef.h"
#include "exceptions.h"




/*!
 * \brief Объект для работы с odt документом, базовый функционал
 * \version 0.2 - alpha
 * \date 03.10.2016
 */
class ENTITYMANAGERSHARED_EXPORT HelperODT{
    class TAG{
        friend class HelperODT;
    protected:
        qint64      _start_pos;
        qint64      _end_pos;
        QString     _tag;
        QString     _replace;
        TAG(){
            _start_pos = 0;
            _end_pos = 0;
        }
    };
private:
    qint64                      _pos;
    QString                     _filename;    // имя обрабатываемого файла
    QTemporaryDir *             _work_dir;    // временная рабочая директория
    QMap<QString,TAG*>          _keywordpos;  // карта позиций ключей
    QList<TAG*>                 _lskeyword;
private:
    HelperODT();
    void init()throw (CommonException::FileNotFoundException);
    TAG* token(QTextStream &file, bool &endfile);
public:
    ~HelperODT();
    /*!
     * \brief получение объекта для работы с выбранным шаблоном
     * \param file - имя файла шаблона
     * \return объект работы с шаблоном
     */
    static HelperODT* open(const QString& file) throw(CommonException::FileNotFoundException,CommonException::FileFormatException,CommonException::ZipError);
    /*!
     * \brief сохранение документа из шаблона
     * \param file_name - имя документа
     * \return результат операции
     */
    bool write(const QString& file_name );
    /*!
     * \brief заменить тег
     * \param tag - тег
     * \param text - значение
     */
    void set(const QString& tag, const QString &text);
};




#endif // HELPERODT

