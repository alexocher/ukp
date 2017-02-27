#ifndef GATEBDF_H
#define GATEBDF_H
#include "dbconnect.h"
#include "../BaseGate/BaseGate.h"
/*!
 * \brief Класс для подключения и выполнения запросов к БДФ. Все результаты получаются в сыром виде (JSON).
 * \warning Проверки входных параметров не производятся
 *
 * \version 0.5
 */

class SHARED_EXPORT GateBDF:public BaseGate{
public:
    enum BDF_REQUEST_STATUS_CODE{
        BDF_REQUEST_STATUS_CODE_OK = 1000,
        BDF_REQUEST_STATUS_CODE_EMPTY_DATA,
        BDF_REQUEST_STATUS_CODE_ERROR,
    };
private:   
    // убрали конструктор копирования и оператор присваивания
    GateBDF(const GateBDF &o);
    GateBDF &operator=(const GateBDF &o);

    BDF_REQUEST_STATUS_CODE parse_ok(const QByteArray &a);
public:


    ~GateBDF();
    /*!
     * \brief GateBDF     
     */
    GateBDF();
    /*!
     * \brief getNewID получить новый идентификатор для объекта
     * \return строка идентификатора
     */
    QString getNewID();
    /*!
     * \brief getAssets получить все элементы БДФ
     * \return $ref: '#definitions/Assets'
     */
    QByteArray getAssets();
    /*!
     * \brief putAssets сохранить объект Asset в БДФ
     * \param asset - $ref: '#definitions/Asset'
     * \return $ref: '#definitions/OK','#definitions/errorModel'
     */
    QByteArray putAssets(const QByteArray& asset);
    /*!
     * \brief gutAssetsBySpatialCoverage получить объекты Asset из БДФ в зоне покрытия
     * \param coverage - область в формате GeoJson
     * \return $ref: '#definitions/Assets'
     */
    QByteArray getAssetsBySpatialCoverage(const QByteArray& coverage);
    /*!
     * \brief getAssetsByMetaData получить объекты Asset из БДФ по значению методанных
     * \param key - атребут метаданного
     * \param value - значение
     * \return $ref: '#definitions/Assets'
     */
    QByteArray getAssetsByMetaData(const QString& key, const QString& value);
    /*!
     * \brief getAssetByID получить объект по идентификатору
     * \param id - идентификатор объекта
     * \return $ref: '#definitions/Asset'
     */
    QByteArray getAssetByID(const QString& id);    
    /*!
     * \brief putAssetByID загрузит новый объект с идентификатором
     * \param id - идентификатор объекта
     * \param a - $ref: '#definitions/Asset'
     * \return $ref: '#definitions/OK','#definitions/errorModel'
     */
    QByteArray putAssetByID(const QString& id,const QByteArray& a);
    /*!
     * \brief delAssetByID удалить объект по идентификатору
     * \param id - идентификатор объекта
     * \return $ref: '#definitions/OK','#definitions/errorModel'
     */
    QByteArray delAssetByID(const QString& id);
    /*!
     * \brief getFilesForAsset получить список продукции для объекта Asset
     * \param asset_id - идентификатор Asset
     * \return $ref: '#definitions/File'
     */
    QByteArray getFilesForAsset(const QString& asset_id);
    /*!
     * \brief getDataByID получит ресурс по идентификатору
     * \param asset_id - идентификатор asset
     * \param data_id - идентификатор продукции
     * \return binary
     */
    QByteArray getDataByID(const QString& asset_id,const QString& data_id);
    /*!
     * \brief postDataByID загрузить новый ресурс по идентификатору
     * \param asset_id - идентификатор asset
     * \param data_id - идентификатор продукции
     * \param data - данные
     * \return $ref: '#definitions/OK','#definitions/errorModel'
     */
    QByteArray postDataByID(const QString &asset_id, const QString &data_id, const QByteArray &data);
    /*!
     * \brief delDataByID удалить ресурс по идентификатору
     * \param asset_id - идентификатор asset
     * \param data_id - идентификатор продукции
     * \return $ref: '#definitions/OK','#definitions/errorModel'
     */
    QByteArray delDataByID(const QString& asset_id,const QString& data_id);
    /*!
     * \brief getSchemas получить все схемы
     * \return $ref: '#definitions/Schema'
     */
    QByteArray getSchemas();    
    /*!
     * \brief putSchema загрузить схему
     * \param schema - загржуемые данные
     * \return $ref: '#definitions/Schema'
     */
    QByteArray putSchema(const QByteArray &schema);
    /*!
     * \brief getSchemaByID получить схему по идентификатору
     * \param schema_id - идентификатор схемы
     * \return $ref: '#definitions/Schema'
     */
    QByteArray getSchemaByID(const QString &schema_id);
    /*!
     * \brief putSchemaByID сохранить схему по идентификатору
     * \param schema_id - идентификатор схемы
     * \param schema $ref: '#definitions/Schema'
     * \return $ref: '#definitions/OK','#definitions/errorModel'
     */
    QByteArray putSchemaByID(const QString &schema_id, const QByteArray &schema);
    /*!
     * \brief delSchemaByID удалить схему по идентификатору
     * \param schema_id - идентификатор схемы
     * \return $ref: '#definitions/OK','#definitions/errorModel'
     */
    QByteArray delSchemaByID(const QString &schema_id);

    BDF_REQUEST_STATUS_CODE _putAssetByID(const QString& id,const QByteArray& a);
    BDF_REQUEST_STATUS_CODE _delAssetByID(const QString &id);
    BDF_REQUEST_STATUS_CODE _putDataByID(const QString& asset_id,const QString& data_id,const QByteArray& a);
    BDF_REQUEST_STATUS_CODE _delDataByID(const QString& asset_id,const QString& data_id);
};

#endif // GATEBDF_H
