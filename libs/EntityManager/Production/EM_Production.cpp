#include "../Headers/EM_Production.h"
#include <QtSql>
#include "../DBConnect/dbconnect.h"


#define query_Select    "SELECT " \
                        " id_production_type, fname, sname, cl_view, cl_rubr2, " \
                        " cl_rubr3, cl_rubr4, mode_in, mode_out" \
                        " FROM plan.production_dic "

EM_Production::EM_Production(TProductionType production_type, const QString& fname, const QString& sname, TProductionView cl_view,TProductionRubr2 cl_rubr2,TProductionRubr3 cl_rubr3, TProductionRubr4 cl_rubr4,bool mode_in, bool mode_out){
    _production_type = production_type;
    _fname = fname;
    _sname = sname;
    _cl_view = cl_view;
    _cl_rubr2 = cl_rubr2;
    _cl_rubr3 = cl_rubr3;
    _cl_rubr4 = cl_rubr4;
    _mode_in = mode_in;
    _mode_out = mode_out;
}

TProductionType EM_Production::getProductionType() const{return _production_type;}
const QString& EM_Production::getFullName() const{return _fname;}
const QString& EM_Production::getShortName() const{return _sname;}
TProductionView EM_Production::getClView() const{return _cl_view;}
TProductionRubr2 EM_Production::getClRubr2() const{return _cl_rubr2;}
TProductionRubr3 EM_Production::getClRubr3() const{return _cl_rubr3;}
TProductionRubr4 EM_Production::getClRubr4() const{return _cl_rubr4;}
bool EM_Production::isModeIn() const{return _mode_in;}
bool EM_Production::isModeOut() const{return _mode_out;}
// -------------------------------------------------------------------------------------------
void EM_ProductionDic::reload()throw(CommonException::OpenDBException,CommonException::SQLException){
    qDebug()<<"Reload ProductionDic";
    clear();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_Select);

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    QSqlRecord rec = q->record();
    const int ind_id_production_type = rec.indexOf("id_production_type");
    const int ind_fname = rec.indexOf("fname");
    const int ind_sname = rec.indexOf("sname");
    const int ind_cl_view = rec.indexOf("cl_view");
    const int ind_cl_rubr2  = rec.indexOf("cl_rubr2");
    const int ind_cl_rubr3  = rec.indexOf("cl_rubr3");
    const int ind_cl_rubr4  = rec.indexOf("cl_rubr4");
    const int ind_mode_in  = rec.indexOf("mode_in");
    const int ind_mode_out  = rec.indexOf("mode_out");


    TProductionType     production_type;
    QString             fname;
    QString             sname;
    TProductionView     cl_view;
    TProductionRubr2    cl_rubr2;
    TProductionRubr3    cl_rubr3;
    TProductionRubr4    cl_rubr4;
    bool                mode_in;
    bool                mode_out;

    while(q->next()){
        try{
            production_type = (TProductionType)q->value(ind_id_production_type).toInt();
        }catch(...){
            production_type = prtNone;
        }

        fname = q->value(ind_fname).toString();
        sname = q->value(ind_sname).toString();
        cl_view  = (TProductionView)q->value(ind_cl_view).toInt();
        cl_rubr2 = (TProductionRubr2)q->value(ind_cl_rubr2).toInt();
        cl_rubr3 = (TProductionRubr3)q->value(ind_cl_rubr3).toInt();
        cl_rubr4 = (TProductionRubr4)q->value(ind_cl_rubr4).toInt();
        mode_in = q->value(ind_mode_in).toBool();
        mode_out = q->value(ind_mode_out).toBool();

        EM_Production *newItem = new EM_Production(production_type, fname, sname, cl_view,cl_rubr2,cl_rubr3, cl_rubr4,mode_in, mode_out);
        if(!mType.contains(production_type)){
            mType[production_type] = newItem;
        }
        lsItem<<newItem;
    }

    delete q;
    db.close();
}


EM_ProductionDic::EM_ProductionDic()throw(CommonException::OpenDBException,CommonException::SQLException):_connect_name(generateConnectionCustomName("ProductionDic")){
   qDebug()<<"Create ProductionDic";
   getConnection(_connect_name);
   reload();
}
EM_ProductionDic::~EM_ProductionDic(){
    clear();
    QSqlDatabase::removeDatabase(_connect_name);
}
void EM_ProductionDic::clear(){
    QList<EM_Production*>::iterator iter;
    for(iter = lsItem.begin();iter!=lsItem.end();++iter){
        delete (*iter);
    }
    lsItem.clear();
    mType.clear();
}

void EM_ProductionDic::printProductionDic()const{
    QMap<TProductionType,EM_Production*>::const_iterator iter;
    for(iter = mType.begin();iter!=mType.end();++iter){
        qDebug().noquote()<< (*iter)->getShortName();
    }
}
QList<EM_Production*>::const_iterator EM_ProductionDic::begin(){return lsItem.begin();}
QList<EM_Production*>::const_iterator EM_ProductionDic::end(){return lsItem.end();}
EM_Production* EM_ProductionDic::by(TProductionType type) const{
    EM_Production* ret = 0;
    if(mType.contains(type)){
        ret = mType[type];
    }
    return ret;
}

bool EM_ProductionDic::_instflag = false;
EM_ProductionDic* EM_ProductionDic::_instance = 0;

