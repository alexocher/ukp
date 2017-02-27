#ifndef GEOM_H
#define GEOM_H

#include <QJsonObject>
#include <QSharedPointer>
#include <QJsonArray>
#include "commondef.h"
#include "exceptions.h"

/*!
 * \brief Пакет классов геометрий
 */
namespace Geom{


/*!
 * \brief Пакет генерируемых исключений
 */

/*!
 * \brief элементарная точка
 */
typedef struct COORDINATE{
    double x;
    double y;
}* P_COORDINATE;

class GeometryFactory;

/*!
 * \brief Объект работы с GeoJSON, базовый класс для обобщения методов и свойств геометрий
 * \version 0.1 - alpha
 * \date 16.09.2016
 */
class ENTITYMANAGERSHARED_EXPORT Geometry{
public:
    /*!
     * \brief типы геметрий
     */
    enum GEOMETRY_TYPE{
        GT_UNDEF,           // не определено
        GT_POINT,           // +точка
        GT_MULTYPOINT,      // множество точек
        GT_LINESTRING,      // линия
        GT_POLYGON,         // +полигон
        GT_MULTYPOLYGON,    // мультиполигон
    };
private:
    GEOMETRY_TYPE   _type;    // тип геометрии
    std::size_t     _size;    // кол-во точек
private:
    Geometry();
    Geometry(const Geometry& obj);
    Geometry& operator= (const Geometry& obj);
protected:
    Geometry(GEOMETRY_TYPE type);
    void size(std::size_t size);
    QJsonObject JSONheader(QJsonObject content, bool is_coverrage=false);    
public:
    virtual ~Geometry(){;}
    /*!
     * \brief получения точки по индексу
     * \param idx - индекс точки
     * \return ссылка на точку
     * \throw Exceptions::UnsupportedOperation,Exceptions::IndexOutOfBound,Exceptions::EmptyGeom
     */
    virtual COORDINATE& operator[](std::size_t idx)throw(CommonException::UnsupportedOperation,CommonException::IndexOutOfBound,GeomExceptions::EmptyGeom)=0;
    /*!
     * \brief Сформировать объект GeoJSON
     * \param is_coverage - условие формирования специального JSON
     * \return объект JSON
     * \throw Exceptions::UnsupportedOperation
     */
    virtual QJsonObject toJSON(bool is_coverage=false)throw(CommonException::UnsupportedOperation)=0;
    /*!
     * \brief получить список точек
     * \return список точек
     */
    virtual QList< QSharedPointer<COORDINATE> > coordinates()=0;
    /*!
     * \brief кол-во точек геометрии
     * \return размер геометрии
     */
    std::size_t size() const;
    /*!
     * \brief тип геометрии
     * \return тип
     */
    GEOMETRY_TYPE type() const;
    /*!
     * \brief проверка на пустую геометрию (пустая - кол-во точек равно нулю)
     * \return результат проверки
     */
    bool isEmpty() const;
    /*!
     * \brief компаратор для точек
     * \param pt1 - точка
     * \param pt2 - точка
     * \return результат сравнения
     */
    static bool equils(const COORDINATE& pt1, const COORDINATE& pt2);
    /*!
     * \brief копирование точек
     * \param src - исходный массив
     * \param dest - целевой массив
     * \param size - кол-во
     * \warning внутри функции проверки входных параметров не производятся
     */
    static void copy(P_COORDINATE src,P_COORDINATE dest,std::size_t size);
};

/*!
 * \brief Реализауия геометрии: Точка
 */
class ENTITYMANAGERSHARED_EXPORT Point:public Geometry{
    friend class GeometryFactory;
private:
    P_COORDINATE _pt;
private:
    Point& operator= (const Point& obj);
protected:
    Point();
    Point(const COORDINATE& pt);
    Point(double x,double y);        
public:
    Point(const Point& obj);
    ~Point();

    COORDINATE& operator[](std::size_t idx)throw(CommonException::UnsupportedOperation,CommonException::IndexOutOfBound,GeomExceptions::EmptyGeom);
    QJsonObject toJSON(bool is_coverage=false)throw(CommonException::UnsupportedOperation);
    QList< QSharedPointer<COORDINATE> > coordinates();

};

/*!
 * \brief Реализауия геометрии: Полигон
 */
class ENTITYMANAGERSHARED_EXPORT Polygon:public Geometry{
    friend class GeometryFactory;
private:
    P_COORDINATE _pt;
private:
    Polygon& operator= (const Polygon& obj);
protected:
    Polygon();
    Polygon(const P_COORDINATE pt, std::size_t lsize);
    Polygon(double pt[], std::size_t lsize);
    Polygon(const QList<P_COORDINATE> &ls);

    bool contains(COORDINATE &test);
public:
    Polygon(const Polygon& obj);
    ~Polygon();

    COORDINATE& operator[](std::size_t idx)throw(CommonException::UnsupportedOperation,CommonException::IndexOutOfBound,GeomExceptions::EmptyGeom);
    QJsonObject toJSON(bool is_coverage=false)throw(CommonException::UnsupportedOperation);
    QList< QSharedPointer<COORDINATE> > coordinates();

    // проверка вхождения в область
    bool contains(Geometry* geom)throw (CommonException::NPE,GeomExceptions::EmptyGeom,CommonException::UnsupportedOperation);
    bool contains(Polygon* polygon)throw (CommonException::NPE,GeomExceptions::EmptyGeom);
    bool contains(Point* pt )throw (CommonException::NPE,GeomExceptions::EmptyGeom);
};

/*!
 * \brief Фабрика для работы с геометриями
 */
class ENTITYMANAGERSHARED_EXPORT GeometryFactory{
private:
    QJsonArray coordinates(const QJsonObject &json,const char* type);
public:
    Point createPoint(const COORDINATE& pt);
    Point createPoint(double x,double y);
    Point createPoint(const COORDINATE pt[],std::size_t size)throw (CommonException::IndexOutOfBound);
    Point createPoint(const QJsonObject& json)throw (JSONFormatException::FormatException);

    Polygon* createPolygon(const P_COORDINATE pt, std::size_t size)throw (CommonException::IndexOutOfBound,CommonException::NPE);
    Polygon* createPolygon(const QJsonObject& json, bool is_coverage=false)throw (JSONFormatException::FormatException);
    Polygon* createPolygon(const QList<P_COORDINATE> &ls);        
};

}



#endif // GEOM_H
