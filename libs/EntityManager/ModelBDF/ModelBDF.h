#ifndef MODELBDF_H
#define MODELBDF_H

#include "../Headers/commondef.h"
#include "../Headers/exceptions.h"
#include "../Headers/BaseTree.h"
#include "../Headers/Geom.h"

#include <QtGlobal>
#include <QJsonObject>
/*!
 * \brief Модель данных БДФ
 */
namespace ModelBDF{

/*!
 * \brief Класс хранеия структуры l10n (Localization)
 */
class ENTITYMANAGERSHARED_EXPORT l10n{
private:
    explicit l10n();

    QMap<QString,QMap<QString,QString>* > _content; // содержимое
public:
    ~l10n();
    /*!
     * \brief создание объекта
     * \param value - объект разбора
     * \return экземпляр класса
     */
    static l10n* Instance(QJsonValue &value);
    /*!
     * \brief получить локализацию по умолчанию
     * \return набор полей с локализацией
     */
    const QMap<QString,QString>& getDefaultLocalization() const throw(CommonException::ObjNotFoundException);
    /*!
     * \brief получить докализацию по ключу
     * \param key - ключ
     * \return набор полей с локализацией
     */
    const QMap<QString,QString>& getLocalization(const QString& key) const throw(CommonException::ObjNotFoundException);
    /*!
     * \brief получить список ключей
     * \return список ключей
     */
    const QList<QString> getKeys() const;
};


/*!
 * \brief Модель схем
 */
namespace Schema{


/*!
 * \brief Типы узлов схемы
 */
enum ITEM_TYPE{
    ROOT_ITEM,
    OBJECT_ITEM,    // объект
    STRING_ITEM,    // поле
    REF_ITEM        // ссылка
};

typedef QList<BaseTreeItem<QString,ITEM_TYPE>*> TREE_LIST;

class SchemaDic;
/*!
 * \brief Объект схемы
 */
class ENTITYMANAGERSHARED_EXPORT BaseSchemaObject:public BaseTreeItem<QString,ITEM_TYPE>{
    friend class SchemaDic;
protected:
    explicit BaseSchemaObject(const QString& id,ITEM_TYPE type);
public:
    ~BaseSchemaObject(){;}
    QString getFullID();
};
/*!
 * \brief элемент с полями
 */
class ENTITYMANAGERSHARED_EXPORT SchemaObject:public BaseSchemaObject{
    friend class SchemaDic;
protected:
    explicit SchemaObject(const QString& id);


    l10n*                       _localization;      // локализованое описание
    QMap<QString,QString>       _fields;            // поля
    QStringList                 _required;          // обязательные свойства
    QMap<QString,QStringList>   _enums;             // перечисления
public:
    ~SchemaObject();
    /*!
     * \brief получение свойства Type
     * \param local - локализация
     * \return мап, ключ - значение enum, значение - локализация
     */
    QMap<QString,QString> getPropType(const QString& local);

};

/*!
 * \brief Словарь схем
 */
class ENTITYMANAGERSHARED_EXPORT SchemaDic:public BaseTree<QString,ITEM_TYPE>{
private:
    QMap<QString,BaseSchemaObject*> _cache;         // кеш схем по id
    QMap<QString,BaseSchemaObject*> _schema_cache;  // кеш схем по type
private:
    BaseSchemaObject* parse(QJsonObject& obj,const QString key_obj);

public:
    SchemaDic();
    ~SchemaDic();
    /*!
     * \brief Поиск по идентификатору схемы
     * \param schema_id - идентификатор
     * \param short_id - признак короткого идентификатора
     * \return схема
     */
    const BaseSchemaObject* by(const QString& schema_id,bool short_id = false);
    /*!
     * \brief пытаемся угадать схему по атрибутам
     * \param value - ключ
     * \return схема
     */
    SchemaObject* findEvristicSchema(const QString& value);
    /*!
     * \brief поиск схемы по типу
     * \param value - ключ
     * \return схема
     */
    SchemaObject* findEvristicSchema_v2(const QString& value);
    /*!
     * \brief поиск схем верхнего уровня
     * \param schema_id - идентификатор
     * \return схема
     */
    const BaseSchemaObject* findTopLevel(const QString& schema_id) const;
    /*!
     * \brief Загрузка из БДФ
     */
    void load();
    /*!
     * \brief Загрузка из файла
     * \param file_path - имя файла
     */
    void load(const QString& file_path)throw (CommonException::FileNotFoundException,CommonException::FileOpenException);
    /*!
     * \brief разбор исходных данных (строим дерево схем)
     * \param data - исходные данные в формате GSON
     */
    void parse(const QByteArray& data)throw (JSONFormatException::FormatException);
    /*!
     * \brief Тестовая печать
     */
    void printSchemaTree(bool idonly = false) const;
    /*!
     * \brief Тестовый вывод ключей
     */
    void showKeys() const;
    /*!
     * \brief тестовый вывод верхнего уровня
     */
    void showTopLevel() const;

    /*!
     * \brief Сериалтзация
     * \return массив байт
     * \warning не поддерживается
     */
    QByteArray serialize() const;
    /*!
     * \brief Десериализация
     * \param data - массив байт
     * \return код статуса
     * \warning не поддерживается
     */
    int deserialize(const QByteArray& data);
};

} // Schema
/*!
 * \brief Модель контента
 */
namespace Asset {
class EM_AssetDic;
class EM_Asset;
class Content;
/*!
 * \brief "$ref": "http://94.72.62.55:8085/api/schemas/core#/definitions/citation
 * \version     0.1
 * \date        21.12.2016
 * \warning     НЕТ
 */
class ENTITYMANAGERSHARED_EXPORT Citation{
    friend class EM_Asset;
    friend class EM_AssetDic;
    friend class Content;
protected:
    QString                 _title;
    QMap<QString,QString>   _dates;
    QStringList             _code;

protected:
    explicit Citation();
    static Citation* create(const QJsonObject& data,Schema::SchemaObject* schema  );
public:
    ~Citation();
    const QString& getTitle() const;
    const QStringList& getCode() const;
    const QMap<QString,QString>& getDates() const;
};

/*!
 * \brief Метаданные объекта БДФ
 * \version     0.3
 * \date        10.12.2016
 * \warning     НЕТ
 */
class ENTITYMANAGERSHARED_EXPORT Content{
    friend class EM_Asset;
    friend class EM_AssetDic;
public:
    /*!
     * \brief состояние контента
     */
    enum INTERNAL_CONTENT_STATE{
        icsOK,
    };    
protected:
    INTERNAL_CONTENT_STATE _state;  // состояние контента

    QString _type;
    QString _title_type;

    QStringList _files;

    Citation *_citation;
private:
    Content(const Content& obj);
    Content& operator= (Content const& obj);    
protected:
    explicit Content();

    static Content* create(const QJsonObject& data,Schema::SchemaObject* schema);
public:
    ~Content();

    const QStringList& getFiles() const;
    const QString& getType() const;
    const QString& getTitleType() const;

    const Citation* getCitation() const;
};


/*!
 * \brief       Объект хранения БДФ
 * \details     Каждый объект класса содержит нобор отребутов хранящихся в БДФ
 * \version     0.3
 * \date        10.12.2016
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
    EM_Asset(const QJsonObject &obj,QSharedPointer<Schema::SchemaDic> dic);
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
    // Загрузка всех данных из БДФ
    QMap<QString,EM_Asset*> loadAssetsFromBDF(QSharedPointer<Schema::SchemaDic> dic)throw (BDFException::ParseInputJSONException);

    QMap<QString,EM_Asset*> parseAssets(QByteArray data,QSharedPointer<Schema::SchemaDic> dic)throw (BDFException::ParseInputJSONException);
public:
    /*!
     * \brief Instance
     * \return Указатель на статический объект словаря
     */
    static EM_AssetDic* Instance();
    /*!
     * \brief Синхронизация данных с БДФ
     * \param dic - словарь схем
     * \param store -  признак сохранеия изменений, по умолчанию данные только подгружаются
     * \throw BDFException::ParseInputJSONException
     */
    void sync(QSharedPointer<Schema::SchemaDic> dic, bool store=false) throw (BDFException::ParseInputJSONException);
    /*!
     * \brief Загрузка данных БДФ из файла
     * \param filename - имя файла
     * \param dic - словарь схем
     * \throw BDFException::ParseInputJSONException
     */
    void loadAssetsFromFile(const QString& filename, QSharedPointer<Schema::SchemaDic> dic) throw (BDFException::ParseInputJSONException);
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


} // Asset

namespace Region{
class EM_Regions;

class ENTITYMANAGERSHARED_EXPORT EM_RegionItem: public BaseTreeItem<QString,int>{
    friend class EM_Regions;
private:
    explicit EM_RegionItem();
protected:
    QString _type; // группирование
    QString _name;
    QString _url;
protected:
    EM_RegionItem(const QString id, const QString& type, const QString& name, const QString& url);
    EM_RegionItem(const QString id);
    const QString& getType() const;
    const QString& getName() const;
    const QString& getURL() const;
public:
    ~EM_RegionItem();
};
/*!
 * \brief Иерархическая структура региона
 */
class ENTITYMANAGERSHARED_EXPORT EM_Regions: public BaseTree<QString,int>{
private:
    QMap<QString,QList<EM_RegionItem*> > _mapType;  // хэш по типам регионов
    QList<QString> _types; // список типов;
public:
    explicit EM_Regions();
    ~EM_Regions();
    /*!
     * \brief разбор исходных данных (строим дерево схем)
     * \param data - исходные данные в формате GSON
     */
    void parse(const QByteArray& data)throw (JSONFormatException::FormatException);
    /*!
     * \brief получение по типу
     * \param type - тип
     * \return список регионов
     */
    QList<EM_RegionItem*>& byType(const QString& type) throw (CommonException::ObjNotFoundException);
    /*!
     * \brief список типов регионов
     * \return список типов
     */
    QList<QString> types();

    QByteArray serialize()const{return QByteArray();}
    int deserialize(const QByteArray& data){if(data.isEmpty()){;}return -1;}
};
} // Region


} // ModelBDF

#endif // MODELBDF_H
