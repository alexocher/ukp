#include "../Headers/Geom.h"
#include <QJsonDocument>
#include <QDebug>
/*!
 * \brief Набор констант для разбора и формирования JSON
 */
namespace Consts {
    const char* _TYPE               =   "type";
    const char* _FEATURE_COLLECTION =   "FeatureCollection";
    const char* _FEATURES           =   "features";
    const char* _FEATERE            =   "Feature";
    const char* _PROPERTIES         =   "properties";
    const char* _GEOMETRY           =   "geometry";
    const char* _COVERAGE           =   "coverage";

    const char* _POINT              =   "Point";
    const char* _POLYGON            =   "Polygon";
    const char* _COORDINATES        =   "coordinates";
}

void printJsonObject(QJsonObject obj){
    QJsonDocument doc = QJsonDocument(obj);
    qDebug()<<doc.toJson(/*QJsonDocument::Compact*/);
}

// Geometry -----------------------------------------------------------------------------------------------------
Geom::Geometry::Geometry(Geom::Geometry::GEOMETRY_TYPE type){
    _type = type;_size = 0;
}
void Geom::Geometry::size(std::size_t size){
    _size = size;
}
bool Geom::Geometry::isEmpty() const{
    return _size==0;
}

bool Geom::Geometry::equils(const Geom::COORDINATE &pt1, const Geom::COORDINATE &pt2){
    return (pt1.x==pt2.x)&(pt1.y==pt2.y);
}

void Geom::Geometry::copy(Geom::P_COORDINATE src, Geom::P_COORDINATE dest, std::size_t size){
    memcpy(dest,src,sizeof(COORDINATE)*size);
}
std::size_t Geom::Geometry::size() const{
    return _size;
}
Geom::Geometry::GEOMETRY_TYPE Geom::Geometry::type() const{
    return _type;
}
QJsonObject Geom::Geometry::JSONheader(QJsonObject content,bool is_coverrage){
    QJsonObject o1;
    if(is_coverrage){
        o1.insert(Consts::_COVERAGE,content);
    }else{
        QJsonObject o2;
        o2.insert(Consts::_TYPE,Consts::_FEATERE);
        o2.insert(Consts::_PROPERTIES,QJsonObject());
        o2.insert(Consts::_GEOMETRY,content);

        QJsonArray a;
        a.append(o2);

        o1.insert(Consts::_TYPE,Consts::_FEATURE_COLLECTION);
        o1.insert(Consts::_FEATURES, a);
    }
    return o1;
}

// Point -----------------------------------------------------------------------------------------------------
Geom::Point::Point():Geometry(Geometry::GT_POINT){
    _pt = 0;
}
Geom::Point::Point(const COORDINATE &pt):Geometry(Geometry::GT_POINT){
    _pt = new COORDINATE;
    _pt->x = pt.x;
    _pt->y = pt.y;
    size(1);
}
Geom::Point::Point(double x,double y):Geometry(Geometry::GT_POINT){
    _pt = new COORDINATE;
    _pt->x = x;
    _pt->y = y;
    size(1);
}
Geom::Point::Point(const Point& obj):Geometry(Geometry::GT_POINT){
    if(!obj.isEmpty()){
        _pt = new COORDINATE;
        _pt->x = obj._pt->x;
        _pt->y = obj._pt->y;
        size(1);
    }else{
        _pt = 0;
    }
}
Geom::Point::~Point(){
    if(_pt!=0)delete _pt;
}
Geom::COORDINATE& Geom::Point::operator[](std::size_t idx)throw(CommonException::UnsupportedOperation,CommonException::IndexOutOfBound,GeomExceptions::EmptyGeom){
    if(isEmpty()){throw GeomExceptions::EmptyGeom();}
    if(idx>size()-1){throw CommonException::IndexOutOfBound();}
    return *_pt;
}
QJsonObject Geom::Point::toJSON(bool is_coverage)throw(CommonException::UnsupportedOperation){
    if(is_coverage) throw CommonException::UnsupportedOperation();

    QJsonArray a;
    if(!isEmpty()){
        a.append(_pt->x);
        a.append(_pt->y);
    }
    QJsonObject content;
    content.insert(Consts::_TYPE,Consts::_POINT);
    content.insert(Consts::_COORDINATES,a);

    return JSONheader(content);
}
QList< QSharedPointer<Geom::COORDINATE> > Geom::Point::coordinates(){
    QList< QSharedPointer<COORDINATE> > ret;
    if(!isEmpty()){
        QSharedPointer<COORDINATE> pt = QSharedPointer<COORDINATE>(new COORDINATE);
        pt->x = _pt->x;
        pt->y = _pt->y;
        ret<< pt;
    }
    return ret;
}
// Polygon -----------------------------------------------------------------------------------------------------
Geom::Polygon::Polygon():Geometry(GT_POLYGON){
    _pt=0;
}
Geom::Polygon::Polygon(const P_COORDINATE pt,std::size_t lsize):Geometry(GT_POLYGON){
    if(lsize>0 && pt!=0){
        _pt = new COORDINATE[lsize];
        copy(pt,_pt,lsize);
        size(lsize);
    }else{
        _pt=0;
    }
}
Geom::Polygon::Polygon(double pt[],std::size_t lsize):Geometry(GT_POLYGON){
    if(lsize>1 && (lsize&0x1)==0){
        _pt = new COORDINATE[lsize>>1];
        for(std::size_t i=0,j = 0;i<lsize;i+=2,j++){
            _pt[j].x = pt[i];
            _pt[j].y = pt[i+1];
        }
        size(lsize>>1);
    }else{
        _pt=0;
    }
}

Geom::Polygon::Polygon(const Polygon& obj):Geometry(GT_POLYGON){
    if(!obj.isEmpty()){
        _pt = new COORDINATE[obj.size()];
        copy(obj._pt,_pt,sizeof(COORDINATE)*obj.size());
    }else{
        _pt = 0;
    }
}
Geom::Polygon::Polygon(const QList<Geom::P_COORDINATE> &ls):Geometry(GT_POLYGON){
    if(!ls.isEmpty()){
        _pt = new COORDINATE[ls.size()];
        int i = 0;
        foreach (P_COORDINATE iter, ls) {
            _pt[i].x = iter->x;
            _pt[i].y = iter->y;
            ++i;
        }
        size(ls.size());
    }else{
        _pt = 0;
    }
}
Geom::Polygon::~Polygon(){
    if(_pt!=0)delete[] _pt;
}
Geom::COORDINATE& Geom::Polygon::operator[](std::size_t idx)throw(CommonException::UnsupportedOperation,CommonException::IndexOutOfBound,GeomExceptions::EmptyGeom){
    if(isEmpty())throw GeomExceptions::EmptyGeom();
    if(idx>size()-1) throw CommonException::IndexOutOfBound();
    return _pt[idx];
}
QJsonObject Geom::Polygon::toJSON(bool is_coverage)throw(CommonException::UnsupportedOperation){
    QJsonObject ret;


    QJsonArray a;
    if(!isEmpty()){
        QJsonArray a1;
        for(std::size_t i=0;i<size();i++){
            QJsonArray a2;
            a2.append(_pt[i].x);
            a2.append(_pt[i].y);

            a1.append(a2);
        }
        // добиваем последнюю точку
        if(!equils(_pt[0],_pt[size()-1])){
            QJsonArray a2;
            a2.append(_pt[0].x);
            a2.append(_pt[0].y);

            a1.append(a2);
        }
        a.append(a1);
    }
    QJsonObject content;
    content.insert(Consts::_TYPE,Consts::_POLYGON);
    content.insert(Consts::_COORDINATES,a);


    ret = JSONheader(content,is_coverage);

    return ret;
}
QList< QSharedPointer<Geom::COORDINATE> > Geom::Polygon::coordinates(){
    QList< QSharedPointer<Geom::COORDINATE> > ret;
    if(!isEmpty()){
        for(std::size_t i=0;i<size();i++){
            QSharedPointer<COORDINATE> pt = QSharedPointer<COORDINATE>(new COORDINATE);
            pt->x = _pt[i].x;
            pt->y = _pt[i].y;
            ret<< pt;
        }
    }
    return ret;
}
bool Geom::Polygon::contains(Geometry* geom)throw (CommonException::NPE,GeomExceptions::EmptyGeom,CommonException::UnsupportedOperation){
    if(geom==0) throw CommonException::NPE();
    if(geom->type()==GT_POINT) return contains( (Point*)geom );
    if(geom->type()==GT_POLYGON) return contains( (Polygon*)geom );
    throw CommonException::UnsupportedOperation();
}
bool Geom::Polygon::contains(Polygon* polygon)throw (CommonException::NPE,GeomExceptions::EmptyGeom){
    if(polygon==0) throw CommonException::NPE();
    if(polygon->isEmpty()) throw GeomExceptions::EmptyGeom();

    bool ret = false;
    for(std::size_t i=0;i<polygon->size();i++){
        ret = contains( (*polygon)[i] );
        if(ret) break;
    }
    return ret;
}
bool Geom::Polygon::contains(Point* pt )throw (CommonException::NPE,GeomExceptions::EmptyGeom){
    if(pt==0) throw CommonException::NPE();
    if(pt->isEmpty()) throw GeomExceptions::EmptyGeom();

    return contains( (*pt)[0]);
}
/*!
 * \brief проверка попадания точки в полигон
 * \param test - точка контроля
 * \link https://habrahabr.ru/post/125356/
 * \return признак
 */
bool Geom::Polygon::contains(COORDINATE& test){
    static const int q_patt[2][2]= { {0,1}, {3,2} };

    if (size()<3) return false;

    COORDINATE pred_pt = _pt[size()-1];
    pred_pt.x-=test.x;
    pred_pt.y-=test.y;

    int pred_q=q_patt[pred_pt.y<0][pred_pt.x<0];

    int w=0;

    for(std::size_t i = 0;i<size();++i){
        COORDINATE cur_pt = _pt[i];

        cur_pt.x-=test.x;
        cur_pt.y-=test.y;

        int q=q_patt[cur_pt.y<0][cur_pt.x<0];

        switch (q-pred_q){
        case -3:++w;break;
        case 3:--w;break;
        case -2:if(pred_pt.x*cur_pt.y>=pred_pt.y*cur_pt.x) ++w;break;
        case 2:if(!(pred_pt.x*cur_pt.y>=pred_pt.y*cur_pt.x)) --w;break;
        }

        pred_pt = cur_pt;
        pred_q = q;
    }

    return w!=0;
}


// GeometryFactory -----------------------------------------------------------------------------------------------------
QJsonArray Geom::GeometryFactory::coordinates(const QJsonObject &json, const char* type){
    if(!json.contains(Consts::_FEATURES)) throw JSONFormatException::FormatException("Features not found");

    QJsonValue _features = json[Consts::_FEATURES];
    if(!_features.isArray()) throw JSONFormatException::FormatException("Features not array");

    QJsonArray v = _features.toArray();
    if(!v.isEmpty()){
        QJsonValue _feature = v.first();
        if(!_feature.isObject()) throw JSONFormatException::FormatException("Feature not object");
        QJsonObject o = _feature.toObject();
        if(!o.contains(Consts::_GEOMETRY)) throw JSONFormatException::FormatException("geometry not found");
        QJsonValue _geometry = o[Consts::_GEOMETRY];
        if(!_geometry.isObject()) throw JSONFormatException::FormatException("Geometry not object");
        QJsonObject g = _geometry.toObject();
        if(!g.contains(Consts::_TYPE)) throw JSONFormatException::FormatException("Type not found");
        QJsonValue _type = g[Consts::_TYPE];
        if(!_type.isString()|| _type.toString().compare(type)!=0) throw JSONFormatException::FormatException("Wrong type");

        if(!g.contains(Consts::_COORDINATES)) throw JSONFormatException::FormatException("Coordinates not found");

        QJsonValue _a = g[Consts::_COORDINATES];
        if(!_a.isArray()) throw JSONFormatException::FormatException("Coordinates not array");



        return _a.toArray();
    }

    return QJsonArray();
}

Geom::Point Geom::GeometryFactory::createPoint(const Geom::COORDINATE& pt){
    return Point(pt);
}
Geom::Point Geom::GeometryFactory::createPoint(double x,double y){
    return Point(x,y);
}
Geom::Point Geom::GeometryFactory::createPoint(const Geom::COORDINATE pt[],std::size_t size)throw (CommonException::IndexOutOfBound){
    if(size>0){
        return Point(pt[0]);
    }
    return Point();
}
Geom::Point Geom::GeometryFactory::createPoint(const QJsonObject& json)throw (JSONFormatException::FormatException){
    QJsonArray v = coordinates(json,Consts::_POINT);
    if(!v.isEmpty()){
        if(v.size()==2){
            return Point(v.first().toDouble(),v.last().toDouble());
        }else{
            throw JSONFormatException::FormatException("Coordinates wrong format");
        }
    }
    return Point();
}
Geom::Polygon* Geom::GeometryFactory::createPolygon(const P_COORDINATE pt, std::size_t size)throw (CommonException::IndexOutOfBound,CommonException::NPE){
    if(pt==0) throw CommonException::NPE();
    return new Polygon(pt,size);
}
Geom::Polygon* Geom::GeometryFactory::createPolygon(const QJsonObject& json,bool is_coverage)throw (JSONFormatException::FormatException){
    QJsonArray coord;
    if(is_coverage){
        if(!json.contains(Consts::_COVERAGE)) throw JSONFormatException::FormatException("Coverage not found");
        QJsonValue v = json[Consts::_COVERAGE];
        if(!v.isObject()) throw JSONFormatException::FormatException("Coverage not object");
        QJsonObject o = v.toObject();
        if(!o.contains(Consts::_TYPE)|| !o.contains(Consts::_COORDINATES)) throw JSONFormatException::FormatException("Wrong format coverage");
        v = o[Consts::_TYPE];
        if(!v.isString()|| v.toString().compare(Consts::_POLYGON)!=0)  throw JSONFormatException::FormatException("Coverage not polygon");
        coord = o[Consts::_COORDINATES].toArray();
    }else{
        coord = coordinates(json,Consts::_POLYGON);
    }

    QList<P_COORDINATE> ls;
    // разбираем массив
    coord = coord.first().toArray();
    foreach (QJsonValue it, coord) {
        QJsonArray a = it.toArray();
        P_COORDINATE pt = new COORDINATE;
        pt->x = a.first().toDouble();
        pt->y = a.last().toDouble();
        ls<<pt;

    }
    if(!Geometry::equils(*ls.first(),*ls.last())){
        ls<<ls.first();
    }
    Polygon* ret = new Polygon(ls);
    foreach (P_COORDINATE it, ls) {
        delete it;
    }
    ls.clear();
    return ret;
}
Geom::Polygon *Geom::GeometryFactory::createPolygon(const QList<Geom::P_COORDINATE> &ls){
    return new Polygon(ls);
}
