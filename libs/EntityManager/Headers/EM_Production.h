#ifndef EM_PRODUCTION
#define EM_PRODUCTION

#include <QMap>

#include "commondef.h"
#include "enums.h"
#include "exceptions.h"

class EM_ProductionDic;

// Элемент классификатора продукции
class ENTITYMANAGERSHARED_EXPORT EM_Production{
    friend class EM_ProductionDic;

private:
    TProductionType     _production_type;       // тип продукции
    QString             _fname;                 // полное название
    QString             _sname;                 // сокращенное название
    TProductionView     _cl_view;               // классификатор по виду
    TProductionRubr2    _cl_rubr2;              // рубрикатор
    TProductionRubr3    _cl_rubr3;              // рубрикатор
    TProductionRubr4    _cl_rubr4;              // рубрикатор
    bool                _mode_in;               // признак входной продукции
    bool                _mode_out;              // признак выходной продукции
protected:
    EM_Production(TProductionType production_type, const QString& fname, const QString& sname, TProductionView cl_view,TProductionRubr2 cl_rubr2,TProductionRubr3 cl_rubr3, TProductionRubr4 cl_rubr4,bool mode_in, bool mode_out);
public:
    ~EM_Production(){;}
    // функции доступа к полям
    TProductionType getProductionType() const;
    const QString& getFullName() const;
    const QString& getShortName() const;
    TProductionView getClView() const;
    TProductionRubr2 getClRubr2() const;
    TProductionRubr3 getClRubr3() const;
    TProductionRubr4 getClRubr4() const;
    bool isModeIn() const;
    bool isModeOut() const;
};
// словарь типов продукции
class ENTITYMANAGERSHARED_EXPORT EM_ProductionDic{
private:
    EM_ProductionDic() throw(CommonException::OpenDBException,CommonException::SQLException);
    ~EM_ProductionDic();
    void clear();

    const QString _connect_name;

    EM_ProductionDic(EM_ProductionDic const&);
    EM_ProductionDic& operator= (EM_ProductionDic const&);
protected:
    QList<EM_Production*> lsItem;
    QMap<TProductionType,EM_Production*> mType;

    static bool _instflag;
    static EM_ProductionDic* _instance;
public:
    static EM_ProductionDic* getInstance() throw(CommonException::OpenDBException,CommonException::SQLException){
        if(! _instflag){
            _instance = new EM_ProductionDic();
            _instflag = true;
        }
        return _instance;
    }
    static EM_ProductionDic& Instance() throw(CommonException::OpenDBException,CommonException::SQLException){
        return *getInstance();
    }
    // тестовый вывод на консоль
    void printProductionDic()const;
    // получить по типу
    EM_Production* by(TProductionType type) const;
    // перегрузка словаря
    void reload()throw(CommonException::OpenDBException,CommonException::SQLException);
    // итераторы
    QList<EM_Production*>::const_iterator begin();
    QList<EM_Production*>::const_iterator end();
};



#endif // EM_PRODUCTION

