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

namespace BDF{



namespace Asset{

class EM_AssetDic;
class EM_Asset;
/*!
 * \brief Объект хранения области
 * \version     0.1-beta
 * \date        01.09.2016
 */
class ENTITYMANAGERSHARED_EXPORT Coverage{
    friend class EM_AssetDic;
    friend class EM_Asset;
public:
    // внутренне представление точки
    typedef struct INTERNAL_POINT{
        double x;
        double y;
    }* P_INTERNAL_POINT;
private:

    P_INTERNAL_POINT _polygon;  // полигон
    QString          _type;     // тип массива
    std::size_t      _size;     // размер
protected:
    /*!
     * \brief Создание объекта из GeoJSON
     * \param obj - объект GeoJSON
     * \warning При ошибках формта или синтаксиса входных данных будет создана пустая геометрия
     */
    Coverage(const QJsonObject &obj);
    /*!
     * \brief Создание объекта из массива пар точек
     * \param ptr   - указатель на массив;
     * \param size  - размер массива вида 2*N, где N кол-во точек
     * \warning При ошибках формта или синтаксиса входных данных будет создана пустая геометрия
     */
    Coverage(double* ptr, std::size_t size);
    /*!
     * \brief Создание объекта из массива точек
     * \param ptr   - указатель на массив;
     * \param size  - кол-во точек
     * \warning При ошибках формта или синтаксиса входных данных будет создана пустая геометрия
     */
    Coverage(P_INTERNAL_POINT, std::size_t size);
    /*!
     * \brief Формирование объекта формата JSON
     * \return объект JSON
     */
    const QJsonObject toJSON() const;
public:
    ~Coverage();

    INTERNAL_POINT& operator[](std::size_t idx);
    const INTERNAL_POINT& operator[](std::size_t idx) const;
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
    Coverage*               _coverage;          // геометрия области
    void *                  _content;           // набор метаданных (пока так)
    QString                 _schema;            // идентификатор схемы
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
};



}

namespace Schema{

}

}


#endif // EM_BDF

