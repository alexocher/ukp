/*!
\file
\brief Заголовочный файл с описанием классов и констант по работе с БДФ

Данный файл содержит в себе определения основных
классов, используемых для доступа к БДФ
*/
#ifndef EM_BDF
#define EM_BDF

#include <QMap>
#include "../Headers/commondef.h"
#include "../Headers/exceptions.h"
#include "../DBConnect/BDF/GateBDF.h"
#include "../Headers/Geom.h"
namespace BDF{



namespace Asset{

class EM_AssetDic;
class EM_Asset;

/*!
 * \brief Метаданные объекта БДФ
 * \version     0.1-beta
 * \date        30.09.2016
 * \warning     НЕТ
 */
class ENTITYMANAGERSHARED_EXPORT Content{
friend class EM_Asset;
friend class EM_AssetDic;

public:
    // типы объектов
    enum CONTENT_TYPE{
        ctUNDEF,
        ctSEA_CHART,    // sea-chart
        ctBOOK,         // book
        ctREPORT,       // report
        ctHYDRO_SHEET   // hydrographic-sheet
    };
    // тип даты
    enum DATE_TYPE{
        dtPUBLICATION,  // публикация
    };

protected:
    CONTENT_TYPE                _type;          // тип объекта
    QList<QString>              _docs;          // список идентификаторов документов
    QString                     _title;         // название
    QString                     _ident_code;    // иденификатор
    QMap<DATE_TYPE,QDateTime>   _datemap;       // набор дат, по типам
protected:
    Content();
    Content(const QJsonObject& obj, const QString &id, const QString &schema);
public:
    Content(const Content& obj);
    Content& operator= (Content const& obj);
    ~Content();
    CONTENT_TYPE getType() const;
    const QList<QString>& getDocs() const;
    const QString& getTitle() const;
    const QString& getIdentCode() const;
    const QMap<DATE_TYPE,QDateTime>& getDate() const;
};

/*!
 * \brief   Объект хранения БДФ
 * \details Каждый объект класса содержит нобор отребутов хранящихся в БДФ
 * \version     0.1-beta
 * \date        30.08.2016
 * \warning     НЕТ
 */
class ENTITYMANAGERSHARED_EXPORT EM_Asset{
    friend class EM_AssetDic;
private:
    // состояния записи
    enum INTERNAL_ASSET_STATE{
        NEW_ASSET = 1000,   // новый объект
        MOD_ASSET,          // изменения в объекте
        DEL_ASSET,          // удаленный объект
        SYNC_ASSET,         // синхронизированный объект
        BAD_ASSET           // ошибки в процессе разбора данных
    };

    INTERNAL_ASSET_STATE    _interanl_state;    // внутреннее состояние записи
    QString                 _id;                // идентификатор записи        
    QString                 _schema;            // идентификатор схемы
    Content*                _content;           // набор метаданных (пока так)
    Geom::Geometry*         _coverage;          // геометрия области
protected:
    /*!
     * \brief Создание объекта из JSON формата
     * \param obj - объект JSON
     */
    EM_Asset(const QJsonObject &obj);
    /*!
     * \brief Формирование объекта формата JSON
     * \return объект JSON
     */
    const QJsonObject toJSON() const;
    /*!
     * \brief получить внутреннее состояние
     * \return состояние
     */
    INTERNAL_ASSET_STATE internal_state()const;
public:
    ~EM_Asset();
    /*!
     * \brief получить область объекта
     * \return геометрия
     */
    Geom::Geometry* coverage() const;
    /*!
     * \brief получить идентификатор обекта
     * \return строка идентификатора
     */
    const QString& id() const;
    /*!
     * \brief получить метаданные
     * \return метаданные
     */
    Content* content() const;
};

/*!
 * \brief       Словарь объектов БДФ
 * \details     Предоставляет набор методов для работы с данными БДФ
 * \version     0.3-beta
 * \date        31.08.2016
 * \warning     НЕТ
 */
class ENTITYMANAGERSHARED_EXPORT EM_AssetDic{
private:
    QMap<QString,EM_Asset*> _mAsset;    // карта объектов БДФ
    GateBDF*                _mGateBDF;  // объект доступа RAW к БДФ

    // Блокируем произвольное создание словаря
    EM_AssetDic();
    // удалить объект в ручеую нельзя
    ~EM_AssetDic();
    // Закрываем возможность копирования
    EM_AssetDic(EM_AssetDic const&);
    EM_AssetDic& operator= (EM_AssetDic const&);

    static bool _instflag;
    static EM_AssetDic* _instance;
private:
    // структура хранения параметров доступа к БДФ
    typedef struct BDF_PROP{
        char* login;
        char* pass;
        char* url;
    }* P_BDF_PROP;
    // сервисный метод: загрузка файла конфигурации
    static void loadProp(P_BDF_PROP prop);
    // Загрузка всех данных из БДФ
    QMap<QString,EM_Asset*> loadAssetsFromBDF()throw (BDFException::ParseInputJSONException);
public:
    /*!
     * \brief Instance
     * \return Указатель на статический объект словаря
     */
    static EM_AssetDic* Instance(){
        if(! _instflag){
            _instance = new EM_AssetDic();
            _instflag = true;
        }
        return _instance;
    }
    /*!
     * \brief Создание файла конфигурации в домашней директории пользователя
     * \param login     - логин для доступа к БДФ
     * \param password  - пароль для доступа к БДФ
     * \param url       - адрес сервера БДФ
     */
    static void initConnectProperties(const QString& login,const QString& password,const QString& url);
    /*!
     * \brief Проверка наличия файла конфигурации и валидация структуры
     * Если файл конфигурации отсутствует, создается новй с пустыми атрибутами
     * \return Результат проверки
     */
    static bool isConnectProperties();
    /*!
     * \brief Синхронизация данных с БДФ
     * \param store -  признак сохранеия изменений, по умолчанию данные только подгружаются
     * \throw BDFException::ParseInputJSONException
     */
    void sync(bool store=false) throw (BDFException::ParseInputJSONException);
    /*!
     * \brief сохранение изменений в БДФ
     */
    void store() ;
    /*!
     * \brief тестовый вывод
     */
    void print();
    /*!
     * \brief получение объекта по ключу
     * \param id - ключ
     * \return объект БДФ
     */
    const EM_Asset* by(const QString& id)const;
    /*!
     * \brief поиск объектов БДФ по области
     * \param coverage - область
     * \return список объектов
     */
    QList<const EM_Asset*> find(Geom::Polygon *coverage) const;
};



}

namespace Schema{

}

}


#endif // EM_BDF

