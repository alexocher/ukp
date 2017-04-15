#include <iostream>
#include <QStack>

#include "../Headers/EM_Plan.h"
#include "../DBConnect/dbconnect.h"

// Запрсы
// ================================================================================
// Запрос на выгребание всех узлов дерева с зависимыми таблицами
#define query_SelectPlanItemWithJoin "SELECT "\
                                    " i.suid, "\
                                    " i.parent_suid, "\
                                    " i.ftitle, "\
                                    " i.stitle, "\
                                    " i.itemtype, "\
                                    " i.production_id, "\
                                    " i.typal_duration, "\
                                    " i.duration, " \
                                    " i.priority, " \
                                    " i.is_optional, " \
                                    " i.is_present, " \
                                    " i.ext_module_type, " \
                                    " i.descr, " \
                                    " i.src_title, "\
                                    " i.res_title, "\
                                    " i.ext_proc, "\
                                    " i.custom_number, "\
                                    " i.template_employee, "\
                                    " i.oshs_item_id, "\
                                    " i.progress, " \
                                    " i.tbegin, " \
                                    " i.tend, "  \
                                    " i.is_tester, " \
                                    " s.main_state, "\
                                    " s.state, " \
                                    " s.problem " \
                                    " FROM "\
                                    "   plan.planitem AS i "\
                                    " JOIN plan.planitem_state AS s ON s.planitem_suid = i.suid "\
                                    " WHERE i.yearplan_suid = :suid "
#define query_SelectTime            "SELECT planitem_suid, time_value, type "\
                                    " FROM plan.time_tbl " \
                                    " WHERE planitem_suid IN (%1) "
#define query_SelectUser            "SELECT m.planitem_suid, s.login, m.external "\
                                    " FROM plan.members AS m" \
                                    " JOIN oshs.secure AS s ON s.suid = m.user_suid " \
                                    " WHERE planitem_suid IN (%1) "
#define query_SelectStatus          "SELECT planitem_suid, status "\
                                    " FROM plan.planitem_status " \
                                    " WHERE planitem_suid IN (%1) "
// добавление/обновление нового плана
#define  query_InsertYearPlan       "INSERT INTO plan.yearplan " \
                                    " (year, title, descr, state) " \
                                    " VALUES " \
                                    " (:year, :title, :descr, :state)" \
                                    " RETURNING suid"
#define  query_UpdateYearPlan       "UPDATE plan.yearplan " \
                                    " SET " \
                                    " year = :year, title = :title, descr = :descr, state = :state" \
                                    " WHERE suid = :suid "
// добавление/обновление узла
#define  query_InsertPlanItem       "INSERT INTO plan.planitem "\
                                    " ("\
                                    " parent_suid, ftitle, stitle, "\
                                    " itemtype, yearplan_suid, production_id, typal_duration, priority, "\
                                    " is_optional, is_present, ext_module_type, descr, src_title, res_title, "\
                                    " ext_proc, custom_number, template_employee, oshs_item_id, duration, progress, " \
                                    " tbegin, tend," \
                                    " is_tester " \
                                    " ) "\
                                    " VALUES "\
                                    " ("\
                                    " :parent_suid, :ftitle, :stitle, "\
                                    " :itemtype, :yearplan_suid, :production_id, :typal_duration, :priority, "\
                                    " :is_optional, :is_present, :ext_module_type, :descr, :src_title, :res_title, "\
                                    " :ext_proc, :custom_number, :template_employee, :oshs_item_id, :duration, :progress," \
                                    " to_timestamp(:tbegin)::timestamp, to_timestamp(:tend)::timestamp, " \
                                    " :is_tester " \
                                    " ) "\
                                    " RETURNING suid"

#define  query_UpdatePlanItem       "UPDATE plan.planitem "\
                                    " SET "\
                                    " parent_suid = :parent_suid, ftitle = :ftitle, stitle = :stitle, "\
                                    " itemtype = :itemtype, production_id = :production_id, "\
                                    " typal_duration = :typal_duration, priority = :priority, "\
                                    " is_optional = :is_optional, is_present=:is_present, "\
                                    " ext_module_type=:ext_module_type, descr=:descr, src_title=:src_title, res_title=:res_title, "\
                                    " ext_proc=:ext_proc, custom_number=:custom_number, template_employee=:template_employee, oshs_item_id=:oshs_item_id, " \
                                    " duration=:duration, progress=:progress, tbegin=to_timestamp(:tbegin)::timestamp, tend = to_timestamp(:tend)::timestamp, " \
                                    " is_tester = :is_tester " \
                                    " WHERE suid = :suid "
// Добавление/Обновление статусов узла
#define query_InsertPlanItemState   "INSERT INTO plan.planitem_state "\
                                    " (planitem_suid, state, problem ) "\
                                    " VALUES "\
                                    " (:planitem_suid, :state, :problem ) "
#define query_UpdatePlanItemState   "UPDATE plan.planitem_state " \
                                    " SET " \
                                    " state = :state, problem = :problem " \
                                    " WHERE planitem_suid = :planitem_suid "
// Добавление/Обновление времени узла
#define query_InsertPlanItemTime    "INSERT INTO plan.time_tbl " \
                                    " (planitem_suid,time_value,type) " \
                                    " VALUES " \
                                    " (?,to_timestamp(?)::timestamp,?) "
#define query_DeletePlanItemTime    "DELETE FROM plan.time_tbl " \
                                    " WHERE planitem_suid IN (%1)"
// Добавление/Обновление исполнителя
#define query_InsertPlanItemUser    "INSERT INTO plan.members " \
                                    " (planitem_suid,user_suid,external) " \
                                    " VALUES " \
                                    " (?,?,?) "
#define query_DeletePlanItemUser    "DELETE FROM plan.members " \
                                    " WHERE planitem_suid IN (%1)"
// Добавление/Обновление статусов
#define query_InsertPlanItemStatus  "INSERT INTO plan.planitem_status " \
                                    " (planitem_suid,status) " \
                                    " VALUES " \
                                    " (?,?) "
#define query_DeletePlanItemStatus  "DELETE FROM plan.planitem_status " \
                                    " WHERE planitem_suid IN (%1)"

// Удаление узла
#define query_DeletePlanItem        "DELETE FROM plan.planitem " \
                                    " WHERE suid IN (%1) "
// Удаление плана
#define query_DeleteYearPlan        "DELETE FROM plan.yearplan "\
                                    " WHERE year = :year "
// Получение всех планов
#define query_SelectAllYearPlans    "SELECT "\
                                    " suid, title, descr, state, year "\
                                    " FROM plan.yearplan "\
                                    " WHERE template = false "
// Получение плана
#define query_SelectYearPlan        "SELECT "\
                                    " suid, title, descr, state "\
                                    " FROM plan.yearplan "\
                                    " WHERE year = :year AND template = false AND state!=:state"
// обновление статуса плана
#define query_UpdateYearPlanState   "UPDATE plan.yearplan "\
                                    " SET "\
                                    " state = :state"\
                                    " WHERE year = :year "
// Получение типовых исполнителей
#define query_SelectPlanItemTemplateEmployee    "SELECT employeetype FROM plan.template_employee " \
                                                " WHERE planitem_suid=:suid"
// Удалить типовых исполнителей для узла
#define query_DeletePlanItemTemplateEmployee    "DELETE FROM plan.template_employee " \
                                                " WHERE planitem_suid=:suid"

// Заполнение элементов
#define query_InsertPlanItemTemplateEmployee    "INSERT INTO plan.template_employee " \
                                                " (planitem_suid,employeetype) " \
                                                " VALUES " \
                                                " (?,?) "
//
#define query_PlanItemState     " SELECT s.planitem_suid, s.state FROM plan.planitem AS p " \
                                " JOIN plan.planitem_state AS s ON s.planitem_suid = p.suid " \
                                " WHERE p.yearplan_suid = :year "



// ---------------------------------------------------------------------------------------------------
EM_AbstractParam::EM_AbstractParam(TAttachmentType type):AbstractEntity(-1){
    _type = type;
}
EM_AbstractParam::EM_AbstractParam():AbstractEntity(-1){
    _type = attNone;
}
TAttachmentType EM_AbstractParam::getAttachmentType() const{
    return _type;
}
// ---------------------------------------------------------------------------------------------------

EM_BasePlanItem::~EM_BasePlanItem(){
    qDeleteAll(_inParam);
    qDeleteAll(_outParam);    
}

EM_BasePlanItem::EM_BasePlanItem(NODE_TYPE type, int suid):BaseTreeItem(suid,type),AbstractEntity(suid),EM_InternalState(INTERNAL_STATE_NEW_ITEM){
    _state = cocNone;
    _problem = copNone;

    _fullTitle = "UNDEF";
    _shortTitle = "UNDEF";
    _descr = "";

    _employee = 0;
    _external_employee = false;

    _typal_duration = -1;

    _visual_hide = false;

    _custom_number = -1;

    _templ_employee = eltNone;
    _oshs_item_id = -1;
}
void EM_BasePlanItem::setInternalState(INTERNAL_STATE state, bool child){
    EM_InternalState::setInternalState(state);
    if(child){
        for(EM_BasePlanItem::iterator iter = begin();iter!=end();++iter){
            (*iter)->setInternalState(state,true);
        }
    }
}
QDateTime EM_BasePlanItem::child_time(INTERNAL_TIME_TYPE time_type)const{
    QDateTime ret;
    if(isEmpty()){
        switch(time_type){
        case PLAN_BEGIN: ret = _planBegin; break;
        case PLAN_END: ret = _planEnd; break;
        case REAL_BEGIN: ret = _realBegin; break;
        case REAL_END: ret = _realEnd; break;
        }
    }else{
        bool bFirst = true;
        EM_BasePlanItem::const_iterator iter = cbegin();
        QDateTime ctime;
        do{
            EM_BasePlanItem* cur = *iter;

            switch(time_type){
            case PLAN_BEGIN: ctime = cur->planBegin(); break;
            case PLAN_END: ctime = cur->planEnd(); break;
            case REAL_BEGIN: ctime = cur->realBegin(); break;
            case REAL_END: ctime = cur->realEnd(); break;
            }



            if(bFirst){
                bFirst = false;
                ret = ctime;
            }else{
                switch(time_type){
                case REAL_BEGIN:
                case PLAN_BEGIN:
                    if(ret.secsTo(ctime)<0){
                        ret = ctime;
                    }
                    break;
                case PLAN_END:
                case REAL_END:
                    if(ret.secsTo(ctime)>0){
                        ret = ctime;
                    }
                    break;
                }
            }

            ++iter;
        }while(iter!=cend());
    }

    return ret;
}

#define CONN_UPDATE_STATUS "UpdateStatusItem"
EM_OPERATION_RETURNED_STATUS EM_BasePlanItem::saveStatus() const throw(CommonException::OpenDBException,CommonException::SQLException){
    QSqlDatabase db;
    if(QSqlDatabase::contains(CONN_UPDATE_STATUS)){
        db = QSqlDatabase::database(CONN_UPDATE_STATUS);
    }else{
        db = getConnection(CONN_UPDATE_STATUS) ;
    }
    if(!db.open()){        
        throw CommonException::OpenDBException(db.lastError().text());
    }
    QSqlQuery *pqState = new QSqlQuery(db);
    pqState->prepare(query_UpdatePlanItemState);

    pqState->bindValue(":planitem_suid",suid());
    pqState->bindValue(":state",_state);
    pqState->bindValue(":problem",_problem);
    if(!pqState->exec()){
        QString mes = pqState->lastError().text();
        delete pqState;
        db.close();
        throw CommonException::SQLException(mes);
    }
    delete pqState;
    db.close();
    return opOK;
}


void EM_BasePlanItem::setModify(bool status){
    if(getInternalState()!=INTERNAL_STATE_NEW_ITEM){
        if(status){ // статусы сейвим сразу
            saveStatus();
        }else{
            setInternalState(INTERNAL_STATE_MODIFY_ITEM);
        }
    }
}
EM_BasePlanItem::iterator EM_BasePlanItem::begin(){
    return iterator(((BaseTreeItem*)this)->begin());
}
EM_BasePlanItem::iterator EM_BasePlanItem::end(){
    return iterator(((BaseTreeItem*)this)->end());
}
EM_BasePlanItem::const_iterator EM_BasePlanItem::cbegin() const{
    return const_iterator(((BaseTreeItem*)this)->cbegin());
}
EM_BasePlanItem::const_iterator EM_BasePlanItem::cend() const{
    return const_iterator(((BaseTreeItem*)this)->cend());
}
TCarryOutCondition EM_BasePlanItem::getState() const{
    return _state;
}
EM_User* EM_BasePlanItem::getEmployee(){
    return _employee;
}
bool EM_BasePlanItem::isEmployeeExternal() const{
    return _external_employee;
}
QString const& EM_BasePlanItem::getSrcTitle() const{
    return _srcTitle;
}
QString const& EM_BasePlanItem::getResTitle() const{
    return _resTitle;
}
int EM_BasePlanItem::getTypalDuration() const{
    return _typal_duration;
}
int EM_BasePlanItem::getDuration() const{
    return _duration;
}
QList<TStatus>& EM_BasePlanItem::getStatus(){
    return _status;
}
QString EM_BasePlanItem::getStatusAsString() const{
    QString ret = "<";
    if(!_status.isEmpty()){
        QList<TStatus>::const_iterator iter;
        for(iter = _status.begin();iter!=_status.end();++iter){
            ret+=QString::number(*iter)+", ";
        }
    }
    ret+=">";
    return ret;
}
int  EM_BasePlanItem::getNum() const{
    return _custom_number;
}
TEmployeeType  EM_BasePlanItem::getTemplEmployee() const{
    return _templ_employee;
}
int EM_BasePlanItem::getOshsItemID() const{
    return _oshs_item_id;
}
int EM_BasePlanItem::getProgress() const{
    return _progress;
}
bool EM_BasePlanItem::isTester() const{
    return _is_tester;
}
void EM_BasePlanItem::setTester(bool value){
    _is_tester = value;
    setModify();
}
TCarryOutProblem EM_BasePlanItem::getProblem() const{
    return _problem;
}
bool EM_BasePlanItem::isProblem() const{
    return (_problem!=copNone);
}
void EM_BasePlanItem::setProblem(const TCarryOutProblem &value){
    _problem = value;
    setModify();
}
time_t EM_BasePlanItem::getPlanBegin() const{
    return planBegin().toTime_t();
}
time_t EM_BasePlanItem::getPlanEnd()const{
    return planEnd().toTime_t();
}
time_t EM_BasePlanItem::getRealBegin()const{
    return realBegin().toTime_t();
}
time_t EM_BasePlanItem::getRealEnd()const{
    return realEnd().toTime_t();
}
QDateTime EM_BasePlanItem::planBegin()const{
    return child_time(PLAN_BEGIN);
}
QDateTime EM_BasePlanItem::planEnd()const{
    return child_time(PLAN_END);
}
QDateTime EM_BasePlanItem::realBegin()const{
    return child_time(REAL_BEGIN);
}
QDateTime EM_BasePlanItem::realEnd()const{
    return child_time(REAL_END);
}
const QString &EM_BasePlanItem::getFullTitle() const{
    return _fullTitle;
}
const QString &EM_BasePlanItem::getShortTitle() const{
    return _shortTitle;
}
const QString &EM_BasePlanItem::getDescr() const{
    return _descr;
}
const QString &EM_BasePlanItem::getMemberId() const{
    return _employee->getLogin();
}
void EM_BasePlanItem::setState(const TCarryOutCondition &value, bool withoutupdate){
    _state = value;
    if(!withoutupdate){
        setModify(true);
    }
}
void EM_BasePlanItem::setEmployee(EM_User* employee, bool external){
    _employee = employee;
    _external_employee = external;
    setModify();
}
void    EM_BasePlanItem::setTypalDuration(int value){
    _typal_duration = value;
    setModify();
}
void    EM_BasePlanItem::setDuration(int value){
    _duration = value;
    setModify();
}
void     EM_BasePlanItem::setSrcTitle(const QString& value){
    _srcTitle = value;
    setModify();
}
void     EM_BasePlanItem::setResTitle(const QString& value){
    _resTitle = value;
    setModify();
}
void    EM_BasePlanItem::setNum(int value){
    _custom_number = value;
    setModify();
}
void    EM_BasePlanItem::setTemplEmployee(TEmployeeType value){
    _templ_employee = value;
    setModify();
}
void    EM_BasePlanItem::setOshsItemID(int value){
    _oshs_item_id = value;
    setModify();
}
void EM_BasePlanItem::setProgress(int value){
    _progress = value;
    setModify();
}

void    EM_BasePlanItem::addStatus(TStatus value){
    if(!_status.contains(value)){
        _status<<value;
        setModify();
    }
}
void    EM_BasePlanItem::remStatus(TStatus value){
    if(_status.contains(value)){
        int idx = _status.indexOf(value);
        if(idx>-1){
            _status.removeAt(idx);
            setModify();
        }
    }
}
void    EM_BasePlanItem::setPlanBegin(const time_t& value){
    planBegin( QDateTime::fromTime_t(value) );
}
void    EM_BasePlanItem::setPlanEnd(const time_t& value){
    planEnd( QDateTime::fromTime_t(value) );
}
void    EM_BasePlanItem::setRealBegin(const time_t& value){
    realBegin( QDateTime::fromTime_t(value) );
}
void    EM_BasePlanItem::setRealEnd(const time_t& value){
    realEnd( QDateTime::fromTime_t(value) );
}
void    EM_BasePlanItem::planBegin(const QDateTime& value){
    _planBegin = value;
    setModify();
}
void    EM_BasePlanItem::planEnd(const QDateTime& value){
    _planEnd = value;
    setModify();
}
void    EM_BasePlanItem::realBegin(const QDateTime& value){
    _realBegin = value;
    setModify();
}
void    EM_BasePlanItem::realEnd(const QDateTime& value){
    _realEnd = value;
    setModify();
}
void    EM_BasePlanItem::setFullTitle(const QString& value){
    _fullTitle = value;
    setModify();
}
void    EM_BasePlanItem::setShortTitle(const QString& value){
    _shortTitle = value;
    setModify();
}
void    EM_BasePlanItem::setDescr(const QString& value){
    _descr = value;
    setModify();
}
//DEPRICATED
void    EM_BasePlanItem::setMemberId(const QString& value){
    qDebug()<<"Use depricated function with param: "<<value;
}
int EM_BasePlanItem::getOrder() {
    return order();
}
int EM_BasePlanItem::getID() const{
    return SUID();
}
EM_BasePlanItem* EM_BasePlanItem::operator[](int index){
    return at(index);
}
EM_BasePlanItem* EM_BasePlanItem::at(int index){
    if(index<0 || index>getSize()-1){
        return this;
    }
    return (EM_BasePlanItem*)BaseTreeItem::at(index);
}
int EM_BasePlanItem::getChildCount() const{
    return getSize();
}
bool EM_BasePlanItem::isVisualHide() const{
    return _visual_hide;
}
void EM_BasePlanItem::setVisualHideFalse(){
    _visual_hide = false;
    if(getParent()!=0){
        ((EM_BasePlanItem*)getParent())->setVisualHideFalse();
    }
}
void EM_BasePlanItem::setVisualHideTrue(){
    setVisualHide(true);
}
void EM_BasePlanItem::setVisualHideReset(){
    setVisualHide(false);
}
void EM_BasePlanItem::setVisualHide(bool value){
    EM_BasePlanItem::iterator iter;
    for(iter=begin();iter!=end();++iter){
        (*iter)->setVisualHide(value);
    }
    _visual_hide = value;
}
//----------------------------------------------------
EM_ProjectPlanItem::EM_ProjectPlanItem(int suid):EM_BasePlanItem(PROJECT, suid){
    _production = 0;
    _priority = 0;
}

EM_Production* EM_ProjectPlanItem::getProduction() const{
    return _production;
}
int EM_ProjectPlanItem::getPriority() const{
    return _priority;
}
void EM_ProjectPlanItem::setProduction(EM_Production* value){
    _production = value;
    setModify();
}
void EM_ProjectPlanItem::setPriority(int value){
    _priority = value;
    setModify();
}
const QDateTime& EM_ProjectPlanItem::getTimeBegin() const{
    return _tbegin;
}
const QDateTime& EM_ProjectPlanItem::getTimeEnd() const   {
    return _tend;
}
void EM_ProjectPlanItem::setTimeBegin(const QDateTime& value){
    _tbegin = value;
    setModify();
}
void EM_ProjectPlanItem::setTimeEnd(const QDateTime& value){
    _tend = value;
    setModify();
}
//----------------------------------------------------
EM_PlanItem::EM_PlanItem(int suid):EM_BasePlanItem(CARRY_PLAN, suid){
    ;
}
//----------------------------------------------------
EM_StagePlanItem::EM_StagePlanItem(int suid):EM_BasePlanItem(STAGE, suid){
    ;
}
void EM_StagePlanItem::setExtProcNum(int value){
    _ext_proc_num = value;
    setModify();
}
int EM_StagePlanItem::getExtProcNum() const{
    return _ext_proc_num;
}
//----------------------------------------------------
EM_TaskPlanItem::EM_TaskPlanItem(int suid):EM_BasePlanItem(TASK, suid){
    _ext_module_type = emtNone;
    _optional = false;
    _present = false;
    _util = 0;
}

EM_TaskPlanItem::~EM_TaskPlanItem(){
    if(_util!=0){
        delete _util;
    }
}
const EM_TaskUtil * EM_TaskPlanItem::getUtil() const{
    return _util;
}
bool EM_TaskPlanItem::isOptional() const{
    return _optional;
}
bool EM_TaskPlanItem::isPresent() const{
    return _present;
}
TExternalModuleType EM_TaskPlanItem::getExtModuleType() const{
    return _ext_module_type;
}
void EM_TaskPlanItem::setOptional(bool value){
    _optional = value;
    setModify();
}
void EM_TaskPlanItem::setPresent(bool value){
    _present = value;
    setModify();
}
void EM_TaskPlanItem::setExtModuleType(TExternalModuleType value){
    _ext_module_type = value;
    setModify();
}
EM_User* EM_TaskPlanItem::getEmployee(){
    EM_User* ret = 0;
    if(_employee==0){
        if(getParent()!=0){
            ret = ((EM_BasePlanItem*)getParent())->getEmployee();
        }
    }
    return ret;
}
TEmployeeType  EM_TaskPlanItem::getTemplEmployee() const{
    TEmployeeType ret = eltNone;
    if(_templ_employee==0){
        if(getParent()!=0){
            ret = ((EM_BasePlanItem*)getParent())->getTemplEmployee();
        }
    }
    return ret;
}
//----------------------------------------------------
EM_TaskUtil::EM_TaskUtil(TExternalModuleType module){
    _external = module;
    _descr = "";
}
bool EM_TaskUtil::isInternal() const{
    return (_external==emtNone);
}
//----------------------------------------------------
bool EM_YearPlanDic::_instflag = false;
EM_YearPlanDic* EM_YearPlanDic::_instance = 0;
EM_YearPlanDic::~EM_YearPlanDic(){
    ls.clear();
    QSqlDatabase::removeDatabase(_connect_name);
}
EM_YearPlanDic::EM_YearPlanDic() throw(CommonException::OpenDBException,CommonException::SQLException):_connect_name(generateConnectionCustomName("YearPlanDic")){
    getConnection(_connect_name);
    reinit();
}
QList< QSharedPointer<EM_YearPlan> >& EM_YearPlanDic::getList(){
    return ls;
}
bool EM_YearPlanDic::contains(int year) const{
    return m.contains(year) ;
}
QSharedPointer<EM_YearPlan> EM_YearPlanDic::by(int year) const{
    if(contains(year)){
        return m[year];
    }
    return QSharedPointer<EM_YearPlan>();
}
void EM_YearPlanDic::removeAll()throw(CommonException::OpenDBException,CommonException::SQLException){
    foreach (QSharedPointer<EM_YearPlan> iter, ls) {
        iter->removeFromDB();
    }
    ls.clear();
    m.clear();
}
void EM_YearPlanDic::remove(int year)throw(CommonException::OpenDBException,CommonException::SQLException){
    if(contains(year)){
        QSharedPointer<EM_YearPlan> item = m[year];
        item->removeFromDB();
        ls.removeAt(ls.indexOf(item));
        m.remove(year);
    }
}

void EM_YearPlanDic::reinit(){
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    ls.clear();
    m.clear();
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_SelectAllYearPlans);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    QSqlRecord rec = q->record();

    int _suid;
    QString _title;
    QString _descr;
    EM_YearPlan::STATE _state;
    int _year;

    const int idx_suid = rec.indexOf("suid");
    const int idx_title = rec.indexOf("title");
    const int idx_descr = rec.indexOf("descr");
    const int idx_state = rec.indexOf("state");
    const int idx_year = rec.indexOf("year");

    while(q->next()){
        _suid = q->value(idx_suid).toInt();
        _title = q->value(idx_title).toString();
        _descr = q->value(idx_descr).toString();
        _state = (EM_YearPlan::STATE)(q->value(idx_state).toInt());
        _year = q->value(idx_year).toInt();

        QSharedPointer<EM_YearPlan> newPlan = QSharedPointer<EM_YearPlan>(new EM_YearPlan(_year));
        newPlan->setDescr(_descr);
        newPlan->setState(_state);
        newPlan->setTitle(_title);
        newPlan->suid(_suid);

        ls<<newPlan;

        m[_year] = newPlan;
    }

    delete q;
    db.close();
}
//----------------------------------------------------
EM_YearPlan::EM_YearPlan(int year):AbstractEntity(-1),BaseTree<int,NODE_TYPE>(new EM_BasePlanItem(ROOT, -1)),_connect_name(generateConnectionCustomName("YearPlan")){
    _year = year;
    _state = EMPTY;
    _title = "UNDEF";    

    getConnection(_connect_name);

    _tmrStatus = 0;

    const int intreval = 120*1000;

    _tmrStatus = new QTimer(this);
    _tmrStatus->setInterval(intreval);
    connect(_tmrStatus, SIGNAL(timeout()), this, SLOT(updateStatusByTime()));
    _tmrStatus->start(intreval);
}
EM_YearPlan::~EM_YearPlan(){
    if(_tmrStatus!=0){
        _tmrStatus->stop();
        delete _tmrStatus;
    }
    QSqlDatabase::removeDatabase(_connect_name);
}
void EM_YearPlan::updateStatusByTime(){
    QList<int> ls;
    if(_state==EMPTY) return;

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) {qDebug().noquote()<<db.lastError().text();return;}

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_PlanItemState);
    q->bindValue(":year",suid());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        qDebug().noquote()<<mes;
        return;
    }

    QSqlRecord rec = q->record();

    const int ind_suid = rec.indexOf("planitem_suid");
    const int ind_state = rec.indexOf("state");


    int id;
    TCarryOutCondition state;
    while(q->next()){
        id = q->value(ind_suid).toInt();
        state = (TCarryOutCondition)q->value(ind_state).toInt();

        EM_BasePlanItem* pt = find(id);
        if(pt!=0 && pt->getState()!=state){
            ls<<id;
            pt->setState(state,true);
        }
    }

    delete q;
    db.close();




    if(!ls.isEmpty()){
        emit notifyStatusChange(ls);
    }
}
EM_BasePlanItem* EM_YearPlan::getRoot()const {
    return (EM_BasePlanItem*) BaseTree::getRoot();
}
void EM_YearPlan::add(EM_BasePlanItem *parent,EM_BasePlanItem *item){
    // после успеха добавляем в дерево
    BaseTree::add(parent,item);
}
void EM_YearPlan::add(EM_BasePlanItem *item){
    BaseTree::add(getRoot(),item);
}
void EM_YearPlan::add(EM_BasePlanItem *parent,EM_ProjectPlanItem *item){
    add(parent,(EM_BasePlanItem *)item);
}
void EM_YearPlan::add(EM_BasePlanItem *parent,EM_StagePlanItem *item){
    add(parent,(EM_BasePlanItem *)item);
}
void EM_YearPlan::add(EM_BasePlanItem *parent,EM_TaskPlanItem *item){
    add(parent,(EM_BasePlanItem *)item);
}
EM_OPERATION_RETURNED_STATUS EM_YearPlan::rem(EM_BasePlanItem *item) throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException){
    if(item==0) throw CommonException::NullParamException();

    QString cond = "";
    QStack<EM_BasePlanItem*> stack;
    stack.push( item );
    while(!stack.isEmpty()){
        EM_BasePlanItem* cur = stack.pop();

        if(cur->getInternalState()!=EM_InternalState::INTERNAL_STATE_NEW_ITEM){
            if(!cond.isEmpty()){
                cond+=", "+QString::number(cur->suid());
            }else{
                cond+=QString::number(cur->suid());
            }
        }

        for(EM_BasePlanItem::iterator iter = cur->begin();iter!=cur->end();++iter){
            stack.push(*iter);
        }

    }   
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    QString st_query_DeletePlanItem = QString(query_DeletePlanItem).arg(cond);
    q->prepare(st_query_DeletePlanItem);
//    q->prepare(query_DeletePlanItem);
//    q->bindValue(":suid",cond);

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    delete q;
    db.close();


    BaseTree::rem(item);
    return opOK;
}
void EM_YearPlan::move(EM_BasePlanItem *parent,EM_BasePlanItem *item){
    if(item==0) return;
    if(item->getInternalState()!=EM_InternalState::INTERNAL_STATE_NEW_ITEM){
        item->setInternalState(EM_InternalState::INTERNAL_STATE_MODIFY_ITEM);
    }
    BaseTree::move(parent,item);
}
EM_BasePlanItem* EM_YearPlan::find(int id) const{
    return (EM_BasePlanItem*)BaseTree::find(id);
}
void EM_YearPlan::fileterReset(){
    ((EM_BasePlanItem*)getRoot())->setVisualHideReset();
}
void EM_YearPlan::filterByUser(QList<const EM_User*> &ls_employee ){
    if(ls_employee.isEmpty()) return;
    ((EM_BasePlanItem*)getRoot())->setVisualHideTrue();

    EM_BasePlanItem::iterator iter;

    QStack<EM_BasePlanItem*> stack;
    stack.push((EM_BasePlanItem*)getRoot());
    while(!stack.isEmpty()){
        EM_BasePlanItem* cur = stack.pop();

        EM_User* cur_employee = cur->getEmployee() ;
        if(cur_employee!=0){
            foreach (const EM_User* it, ls_employee) {
                if(cur_employee->SUID()==it->SUID()){
                    cur->setVisualHideFalse();
                }
            }
        }

        for(iter=cur->begin();iter!=cur->end();++iter){
            stack.push(*iter);
        }
    }
}

void EM_YearPlan::printPlanTree() const{
    qDebug().noquote()<<_title;
    QStack<EM_BasePlanItem*> stack;
    stack.push( getRoot() );
    while(!stack.isEmpty()){
        EM_BasePlanItem* cur = stack.pop();
        QString ret = "";
        for(int i=1;i<cur->getLevel();i++){
            ret+="----";
        }
        QDateTime tBegin = cur->planBegin();
        ret+="("+QString::number(cur->getNum())+") "+cur->getFullTitle()+" ("+(cur->getEmployee()?cur->getEmployee()->getLogin():"***")+") ["+(!tBegin.isNull()?tBegin.time().toString():"---")+"] " + (cur->isModify()?"true":"false");
        qDebug().noquote()<<ret;

        for(EM_BasePlanItem::iterator iter = cur->begin();iter!=cur->end();++iter){
            stack.push(*iter);
        }

    }
}
QByteArray EM_YearPlan::serialize() const{
    qWarning()<<"SchemaDic: UNSUPPORTED OPERATION";
    return QByteArray();
}
int EM_YearPlan::deserialize(const QByteArray& data){
    if(data.isEmpty()){;}
    qWarning()<<"SchemaDic: UNSUPPORTED OPERATION";
    return -1;
}

QList<TEmployeeType> EM_YearPlan::getTemplateEmployee(EM_BasePlanItem *item)throw(CommonException::SQLException,CommonException::OpenDBException,CommonException::NullParamException){
    if(item==0) throw CommonException::NullParamException();
    if(item->_template_employee.isEmpty()){
        QSqlDatabase db = QSqlDatabase::database(_connect_name);
        if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());

        QSqlQuery *q = new QSqlQuery(db);
        q->prepare(query_SelectPlanItemTemplateEmployee);
        q->bindValue(":suid",suid());

        if(!q->exec()){
            QString mes = q->lastError().text();
            delete q;
            db.close();
            throw CommonException::SQLException(mes);
        }

        QSqlRecord rec = q->record();

        const int ind_type = rec.indexOf("employeetype");
        while(q->next()){
            item->_template_employee << (TEmployeeType)q->value(ind_type).toInt();
        }

        delete q;
        db.close();
    }
    return item->_template_employee;
}
void EM_YearPlan::setTemplateEmployee(EM_BasePlanItem *item,const QList<TEmployeeType> &ls)throw(CommonException::SQLException,CommonException::OpenDBException,CommonException::NullParamException){
    if(item==0) throw CommonException::NullParamException();
    if(item->getInternalState()==EM_BasePlanItem::INTERNAL_STATE_NEW_ITEM){

    }else{
        QSqlDatabase db = QSqlDatabase::database(_connect_name);
        if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());

        QSqlQuery *q = new QSqlQuery(db);


        q->prepare(query_DeletePlanItemTemplateEmployee);
        q->bindValue(":suid",item->suid());

        if(!q->exec()){
            QString mes = q->lastError().text();
            delete q;
            db.close();
            throw CommonException::SQLException(mes);
        }
        item->_template_employee.clear();


        if(!ls.isEmpty()){
            QVariantList suid_list;
            QVariantList type_list;
            foreach(TEmployeeType it,ls){
                suid_list<<item->suid();
                type_list<<it;
            }
            q->prepare(query_InsertPlanItemTemplateEmployee);
            q->addBindValue(suid_list);
            q->addBindValue(type_list);
            if(!q->execBatch()){
                QString mes = q->lastError().text();
                delete q;
                db.close();
                throw CommonException::SQLException(mes);
            }
        }


        delete q;
        db.close();
    }
    item->_template_employee<<ls;
}


EM_BasePlanItem* EM_YearPlan::Instance(NODE_TYPE type){
    return Instance(type,-1);
}
EM_BasePlanItem* EM_YearPlan::Instance(NODE_TYPE type,int suid){
    EM_BasePlanItem * ret = 0;
    switch(type){
    case PROJECT:
        ret = new EM_ProjectPlanItem(suid);
        break;
    case STAGE:
        ret = new EM_StagePlanItem(suid);
        break;
    case TASK:
        ret = new EM_TaskPlanItem(suid);
        break;
    case ROOT:
        ret = new EM_BasePlanItem(ROOT,suid);
        break;
    case CARRY_PLAN:
        ret = new EM_PlanItem(suid);
        break;
    case UNDEF:
    default:
        ret = new EM_BasePlanItem(UNDEF,suid);
    };
    return ret;
}
QString const& EM_YearPlan::getTitle() const{
    return _title;
}
void EM_YearPlan::setTitle(const QString& title){
    _title = title;
}
QString const& EM_YearPlan::getDescr() const{
    return _descr;
}
void EM_YearPlan::setDescr(const QString& descr){
    _descr = descr;
}
EM_YearPlan::STATE EM_YearPlan::getState() const{
    return _state;
}
void EM_YearPlan::setState(STATE state){
    _state = state;
}
EM_OPERATION_RETURNED_STATUS EM_YearPlan::removeFromDB()throw(CommonException::OpenDBException,CommonException::SQLException){
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_DeleteYearPlan);
    q->bindValue(":year",_year);

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    // выставляем признак всем узлам
    _state = EM_YearPlan::EMPTY;
    getRoot()->setInternalState(EM_InternalState::INTERNAL_STATE_NEW_ITEM,true);

    delete q;
    db.close();
    return opOK;
}
EM_OPERATION_RETURNED_STATUS EM_YearPlan::fromDB()throw(CommonException::OpenDBException,CommonException::SQLException){
    return fromDB(_year);
}
EM_OPERATION_RETURNED_STATUS EM_YearPlan::fromDB(int year)throw(CommonException::OpenDBException,CommonException::SQLException){
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    // Подняли основной объект
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_SelectYearPlan);
    q->bindValue(":year",year);
    q->bindValue(":state",EM_YearPlan::DLETED);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    QSqlRecord rec = q->record();
    int ct = 0;
    while(q->next()){
        suid(q->value(rec.indexOf("suid")).toInt());
        _title = q->value(rec.indexOf("title")).toString();
        _descr = q->value(rec.indexOf("descr")).toString();
        _state = (STATE)(q->value(rec.indexOf("state")).toInt());
        _year = year;
        ct++;
    }

    if(ct==0){
        return opYEARPLAN_NOT_EXIST;
    }
    // --------------------------------------------
    q->prepare(query_SelectPlanItemWithJoin);
    q->bindValue(":suid",suid());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    int l_suid;
    int l_parent_suid;
    QString l_ftitle;
    QString l_stitle;
    NODE_TYPE l_itemtype;
    //int l_main_state;
    TCarryOutCondition l_state;
    TCarryOutProblem l_problem;
    TProductionType l_production_type;
    int l_typal_duration;
    int l_duration;
    int l_priority;
    bool l_optional;
    bool l_present;
    TExternalModuleType l_ext_module_type;
    QString l_descr;
    QString l_src_title;
    QString l_res_title;
    int l_ext_proc;
    int l_custom_number;
    TEmployeeType l_templ_employee;
    int l_oshs_item_id;
    int l_progress;
    QDateTime l_tbegin;
    QDateTime l_tend;
    bool l_tester;

    // индексы столбцов
    rec = q->record();
    const int ind_suid = rec.indexOf("suid");
    const int ind_parent_suid = rec.indexOf("parent_suid");
    const int ind_ftitle = rec.indexOf("ftitle");
    const int ind_stitle = rec.indexOf("stitle");
    const int ind_itemtype = rec.indexOf("itemtype");
    //int ind_main_state = rec.indexOf("main_state");
    const int ind_state  = rec.indexOf("state");
    const int ind_problem  = rec.indexOf("problem");
    const int ind_production  = rec.indexOf("production_id");
    const int ind_typal_duration = rec.indexOf("typal_duration");
    const int ind_duration = rec.indexOf("duration");
    const int ind_priority = rec.indexOf("priority");
    const int ind_optional = rec.indexOf("is_optional");
    const int ind_present = rec.indexOf("is_present");
    const int ind_ext_module_type = rec.indexOf("ext_module_type");
    const int ind_descr = rec.indexOf("descr");
    const int ind_src_title = rec.indexOf("src_title");
    const int ind_res_title = rec.indexOf("res_title");
    const int ind_ext_proc = rec.indexOf("ext_proc");
    const int ind_custom_number = rec.indexOf("custom_number");
    const int ind_templ_employee = rec.indexOf("template_employee");
    const int ind_oshs_item_id = rec.indexOf("oshs_item_id");
    const int ind_progress = rec.indexOf("progress");
    const int ind_tbegin = rec.indexOf("tbegin");
    const int ind_tend = rec.indexOf("tend");
    const int ind_tester = rec.indexOf("is_tester");

    QMap<int,EM_BasePlanItem*> mTree;
    EM_BasePlanItem *root = 0;

    delete m_RootItem;
    m_Map.clear();

    EM_ProductionDic& produc_dic = EM_ProductionDic::Instance();

    QString cond = "";
    while(q->next()){

        l_suid = q->value(ind_suid).toInt();
        l_parent_suid = q->value(ind_parent_suid).toInt();
        l_ftitle = q->value(ind_ftitle).toString();
        l_stitle = q->value(ind_stitle).toString();
        l_itemtype  = (NODE_TYPE)(q->value(ind_itemtype).toInt());
        //l_main_state = q->value(ind_main_state).toInt();
        l_state = (TCarryOutCondition)(q->value(ind_state).toInt());
        l_problem = (TCarryOutProblem)(q->value(ind_problem).toInt());
        l_production_type = (TProductionType)(q->value(ind_production).toInt());
        l_typal_duration = q->value(ind_typal_duration).toInt();
        l_duration = q->value(ind_duration).toInt();
        l_priority = q->value(ind_priority).toInt();
        l_optional = q->value(ind_optional).toBool();
        l_present = q->value(ind_present).toBool();
        l_ext_module_type = (TExternalModuleType)(q->value(ind_ext_module_type).toInt());
        l_descr = q->value(ind_descr).toString();
        l_src_title = q->value(ind_src_title).toString();
        l_res_title = q->value(ind_res_title).toString();
        l_ext_proc = q->value(ind_ext_proc).toInt();
        l_custom_number = q->value(ind_custom_number).toInt();
        l_templ_employee = (TEmployeeType)q->value(ind_templ_employee).toInt();
        l_oshs_item_id = q->value(ind_oshs_item_id).toInt();
        l_progress = q->value(ind_progress).toInt();
        l_tbegin = q->value(ind_tbegin).toDateTime();
        l_tend = q->value(ind_tend).toDateTime();
        l_tester = q->value(ind_tester).toBool();

        int pkey = l_parent_suid;
        int key = l_suid;

        EM_BasePlanItem *newItem = 0;
        EM_BasePlanItem *par = 0;
        if(pkey!=-1){
            if(mTree.contains(pkey)){
                par = mTree[pkey];
            }else{
                par = Instance(ROOT,pkey); // создаем временного родителя
                mTree[pkey] = par;
            }

            if(mTree.contains(key)){
                // Проблема здесь!!!
//                newItem = mTree[key];
//                newItem->setNodeType(l_itemtype);
                //
                EM_BasePlanItem* tmpItem = mTree[key];
                if(l_itemtype==tmpItem->getType()){
                    newItem = tmpItem;
                }else{
                    mTree.remove(key);
                    // создали узел корректного типа
                    newItem = Instance(l_itemtype,l_suid);
                    // удалили временный
                    BaseTree::rem(tmpItem,false);
                    // перекладываем детей
                    QList<EM_BasePlanItem*> tmp_ls;
                    for(EM_BasePlanItem::iterator iter = tmpItem->begin();iter!=tmpItem->end();++iter){
                        tmp_ls<<(*iter);
                    }

                    foreach (EM_BasePlanItem* iter, tmp_ls) {
                        BaseTree::rem(iter,false);
                        add(newItem,iter);
                    }
                    // очищаем память временного узла
                    delete tmpItem;
                    // новый узел в память
                    mTree[key] = newItem;
                }
            }else{
                newItem = Instance(l_itemtype,l_suid);
                mTree[key] = newItem;
            }

            newItem->setFullTitle(l_ftitle);
            newItem->setShortTitle(l_stitle);
            newItem->setState(l_state);
            newItem->setProblem(l_problem);
            newItem->setTypalDuration(l_typal_duration);
            newItem->setDuration(l_duration);
            newItem->setDescr(l_descr);
            newItem->setSrcTitle(l_src_title);
            newItem->setResTitle(l_res_title);
            newItem->setNum(l_custom_number);
            newItem->setTemplEmployee(l_templ_employee);
            newItem->setOshsItemID(l_oshs_item_id);
            newItem->setProgress(l_progress);
            newItem->setTester(l_tester);
            if(newItem->getType()==PROJECT){
                EM_Production* prud = produc_dic.by(l_production_type);
                ((EM_ProjectPlanItem*)newItem)->setProduction(prud);
                ((EM_ProjectPlanItem*)newItem)->setPriority(l_priority);
                ((EM_ProjectPlanItem*)newItem)->setTimeBegin(l_tbegin);
                ((EM_ProjectPlanItem*)newItem)->setTimeEnd(l_tend);
            }
            if(newItem->getType()==TASK){
                ((EM_TaskPlanItem*)newItem)->setOptional(l_optional);
                ((EM_TaskPlanItem*)newItem)->setPresent(l_present);
                ((EM_TaskPlanItem*)newItem)->setExtModuleType(l_ext_module_type);
            }
            if(newItem->getType()==STAGE){
                ((EM_StagePlanItem*)newItem)->setExtProcNum(l_ext_proc);
            }
            newItem->setInternalState(EM_InternalState::INTERNAL_STATE_COMMIT_ITEM);

            add(par,newItem);
//            par->addChild( newItem );
//            newItem->setParent( par );
        }else{
            if(!mTree.contains(key)){
                root = Instance(ROOT,key);
                mTree[key] = root;
            }else{
                root = mTree[key];
            }
            setRootItem(root);
            root->setInternalState(EM_InternalState::INTERNAL_STATE_COMMIT_ITEM);
        }

        if(newItem!=0){
            if(!cond.isEmpty()){
                cond+=", "+QString::number(newItem->suid());
            }else{
                cond+=QString::number(newItem->suid());
            }
        }
    }
    if(!cond.isEmpty()){
        // Подъем времен
        q->prepare(QString(query_SelectTime).arg(cond));
        if(!q->exec()){
            QString mes = q->lastError().text();
            delete q;
            db.close();
            throw CommonException::SQLException(mes);
        }

        rec = q->record();
        int idx_suid = rec.indexOf("planitem_suid");
        int idx_value = rec.indexOf("time_value");
        int idx_type = rec.indexOf("type");

        while(q->next()){
            int suid = q->value(idx_suid).toInt();
            if(!mTree.contains(suid)){
                qDebug()<<"!! Node with suid "<<suid<<" not found in map";
                continue;
            }
            EM_BasePlanItem* item = mTree[suid];

            QDateTime value = q->value(idx_value).toDateTime();
            EM_YearPlan::INTERNAL_TIME_TYPE type = (EM_YearPlan::INTERNAL_TIME_TYPE)q->value(idx_type).toInt();
            switch(type){
            case EM_YearPlan::PLAN_BEGIN:
                item->planBegin(value);
                break;
            case EM_YearPlan::PLAN_END:
                item->planEnd(value);
                break;
            case EM_YearPlan::REAL_BEGIN:
                item->realBegin(value);
                break;
            case EM_YearPlan::REAL_END:
                item->realEnd(value);
                break;
            case EM_YearPlan::TIPICAL_INTERVAL:
            default:
                break;
            }
        }

        // подъем статусов
        q->prepare(QString(query_SelectStatus).arg(cond));
        if(!q->exec()){
            QString mes = q->lastError().text();
            delete q;
            db.close();
            throw CommonException::SQLException(mes);}

        rec = q->record();
        idx_suid = rec.indexOf("planitem_suid");
        int idx_status = rec.indexOf("status");

        while(q->next()){
            int suid = q->value(idx_suid).toInt();
            if(!mTree.contains(suid)){
                qDebug()<<"!! Node with suid "<<suid<<" not found in map";
                continue;
            }
            EM_BasePlanItem* item = mTree[suid];

            item->addStatus((TStatus)q->value(idx_status).toInt());
        }

        // подъем исполнителей
        q->prepare(QString(query_SelectUser).arg(cond));
        if(!q->exec()){
            QString mes = q->lastError().text();
            delete q;
            db.close();
            throw CommonException::SQLException(mes);
        }

        rec = q->record();
        idx_suid = rec.indexOf("planitem_suid");
        int idx_user = rec.indexOf("login");
        int idx_external = rec.indexOf("external");

        EM_AddressBook& book = EM_AddressBook::Instance();
        while(q->next()){
            int suid = q->value(idx_suid).toInt();
            if(!mTree.contains(suid)){
                qDebug()<<"!! Node with suid "<<suid<<" not found in map";
                continue;
            }
            EM_BasePlanItem* item = mTree[suid];


            EM_User *user = book.getUser(q->value(idx_user).toString());
            item->setEmployee(user,q->value(idx_external).toBool());
        }
    }
    root->setInternalState(EM_InternalState::INTERNAL_STATE_COMMIT_ITEM,true);

    delete q;
    db.close();
    return opOK;
}

EM_OPERATION_RETURNED_STATUS EM_YearPlan::toDB()throw(CommonException::OpenDBException,CommonException::SQLException){
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }
    QSqlQuery *q = new QSqlQuery(db);
    // помечаем планы того же года как удаленные
    if(_state==EM_YearPlan::EMPTY){
        q->prepare(query_UpdateYearPlanState);
        q->bindValue(":year",_year);
        q->bindValue(":state",EM_YearPlan::DLETED);
        if(!q->exec()){
            QString mes = q->lastError().text();
            delete q;
            db.close();
            throw CommonException::SQLException(mes);
        }
    }

    // сейвим сам план   
    EM_YearPlan::STATE planState;
    if(_state==EM_YearPlan::EMPTY){
        q->prepare(query_InsertYearPlan);
        planState = EM_YearPlan::NEW;
    }else{
        q->prepare(query_UpdateYearPlan);
        planState = _state;
    }

    q->bindValue(":year",_year);
    q->bindValue(":title",_title);
    q->bindValue(":descr",_descr);
    q->bindValue(":state",planState);
    if(_state!=EM_YearPlan::EMPTY){
        q->bindValue(":suid",suid());
    }
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    if(_state==EM_YearPlan::EMPTY){
        int newId = -1;
        while(q->next()){
            newId = q->value(0).toInt();
        }
        suid(newId);
        _state = EM_YearPlan::NEW;
    }

    // -----------------------------------------------------------------------------------------------------------

    // подготавливаем запросы
    QSqlQuery *qPlanItemInsert = new QSqlQuery(db);
    QSqlQuery *qPlanItemUpdate = new QSqlQuery(db);
    QSqlQuery *qPlanItemStateInsert = new QSqlQuery(db);
    QSqlQuery *qPlanItemStateUpdate = new QSqlQuery(db);

    QSqlQuery *qPlanItemTimeInsert = new QSqlQuery(db);
    QSqlQuery *qPlanItemTimeDelete = new QSqlQuery(db);

    QSqlQuery *qPlanItemUserInsert = new QSqlQuery(db);
    QSqlQuery *qPlanItemUserDelete = new QSqlQuery(db);

    qPlanItemInsert->prepare(query_InsertPlanItem);
    qPlanItemUpdate->prepare(query_UpdatePlanItem);

    qPlanItemStateInsert->prepare(query_InsertPlanItemState);
    qPlanItemStateUpdate->prepare(query_UpdatePlanItemState);

    qPlanItemTimeInsert->prepare(query_InsertPlanItemTime);
    qPlanItemTimeDelete->prepare(query_DeletePlanItemTime);

    qPlanItemUserInsert->prepare(query_InsertPlanItemUser);
    qPlanItemUserDelete->prepare(query_DeletePlanItemUser);


    QStringList lsCond;



    QVariantList item_suid_list;
    QVariantList time_value_list;
    QVariantList time_type_list;

    QVariantList emloyee_item_suid_list;
    QVariantList emloyee_user_suid_list;
    QVariantList emloyee_user_type_list;

    QVariantList status_suid_list;
    QVariantList status_value_list;

    QStack<EM_BasePlanItem*> stack;
    QSqlQuery *pq = 0;
    QSqlQuery *pqState = 0;

    stack.push( getRoot() );
    while(!stack.isEmpty()){
        EM_BasePlanItem* cur = stack.pop();

        // выполняем обновления базы только для модифицированных и новых узлов
        if(cur->getInternalState()!=EM_InternalState::INTERNAL_STATE_COMMIT_ITEM){
            lsCond<<QString::number(cur->suid());

            bool isNewItem = (cur->getInternalState()==EM_InternalState::INTERNAL_STATE_NEW_ITEM);


            if(isNewItem){
                pq = qPlanItemInsert;
                pqState = qPlanItemStateInsert;
            }else{
                pq = qPlanItemUpdate;
                pqState = qPlanItemStateUpdate;
            }
            // ================================================================================
            // сейвим узел и ссылку на родителя
            if(cur->getParent()!=0){
                pq->bindValue(":parent_suid", ((EM_BasePlanItem*)cur->getParent())->suid());
            }else{
                pq->bindValue(":parent_suid", -1);
            }

            pq->bindValue(":ftitle", cur->getFullTitle());
            pq->bindValue(":stitle", cur->getShortTitle());
            pq->bindValue(":itemtype", cur->getNodeType());
            pq->bindValue(":typal_duration", cur->getTypalDuration());
            pq->bindValue(":duration", cur->getDuration() );
            pq->bindValue(":descr",cur->getDescr());
            pq->bindValue(":src_title",cur->getSrcTitle());
            pq->bindValue(":res_title",cur->getResTitle());
            pq->bindValue(":custom_number",cur->getNum());
            pq->bindValue(":template_employee",cur->getTemplEmployee());
            pq->bindValue(":oshs_item_id",cur->getOshsItemID());
            pq->bindValue(":progress",cur->getProgress());
            pq->bindValue(":is_tester",cur->isTester());
            if(isNewItem){
                pq->bindValue(":yearplan_suid", suid());
            }else{
                pq->bindValue(":suid",cur->suid());
            }
            TProductionType production_type = prtNone;
            int priority = 0;
            QVariant tbegin = 0;
            QVariant tend = 0;
            if(cur->getType()==PROJECT){
                EM_Production* prod = ((EM_ProjectPlanItem*)cur)->getProduction();
                if(prod!=0){
                    production_type = prod->getProductionType();
                }
                priority = ((EM_ProjectPlanItem*)cur)->getPriority();
                tbegin = ((EM_ProjectPlanItem*)cur)->getTimeBegin().toTime_t();
                tend = ((EM_ProjectPlanItem*)cur)->getTimeEnd().toTime_t();
            }
            pq->bindValue(":production_id",production_type);
            pq->bindValue(":priority",priority);
            pq->bindValue(":tbegin",tbegin);
            pq->bindValue(":tend",tend);

            bool present = false;
            bool optional = false;
            TExternalModuleType ext_module_type = emtNone;
            if(cur->getType()==TASK){
                present = ((EM_TaskPlanItem*)cur)->isPresent();
                optional = ((EM_TaskPlanItem*)cur)->isOptional();
                ext_module_type = ((EM_TaskPlanItem*)cur)->getExtModuleType();
            }
            pq->bindValue(":is_optional",optional);
            pq->bindValue(":is_present",present);
            pq->bindValue(":ext_module_type",ext_module_type);


            int ext_proc = 0;
            if(cur->getType()==STAGE){
                ext_proc = ((EM_StagePlanItem*)cur)->getExtProcNum();
            }
            pq->bindValue(":ext_proc",ext_proc);


            if(!pq->exec()){
                QString mes = pq->lastError().text();
                delete q;
                delete qPlanItemInsert;
                delete qPlanItemUpdate;
                delete qPlanItemStateInsert;
                delete qPlanItemStateUpdate;
                delete qPlanItemTimeInsert;
                delete qPlanItemTimeDelete;
                delete qPlanItemUserInsert;
                delete qPlanItemUserDelete;
                db.close();
                throw CommonException::SQLException(mes);
            }

            if(isNewItem){
                int newId = -1;
                while(pq->next()){
                    newId = pq->value(0).toInt();
                }
                cur->suid(newId);
            }
            // ================================================================================
            // Сейви статусы
            pqState->bindValue(":planitem_suid",cur->suid());
            pqState->bindValue(":state",cur->getState());
            pqState->bindValue(":problem",cur->getProblem());
            if(!pqState->exec()){
                QString mes = pqState->lastError().text();
                delete q;
                delete qPlanItemInsert;
                delete qPlanItemUpdate;
                delete qPlanItemStateInsert;
                delete qPlanItemStateUpdate;
                delete qPlanItemTimeInsert;
                delete qPlanItemTimeDelete;
                delete qPlanItemUserInsert;
                delete qPlanItemUserDelete;
                db.close();
                throw CommonException::SQLException(mes);
            }
            // ================================================================================
            // Сейви время
            if( !cur->planBegin().isNull() ){
                item_suid_list<<cur->suid();
                time_value_list<<cur->planBegin().toTime_t();
                time_type_list<<EM_YearPlan::PLAN_BEGIN;
            }
            if( !cur->planEnd().isNull() ){
                item_suid_list<<cur->suid();
                time_value_list<<cur->planEnd().toTime_t();
                time_type_list<<EM_YearPlan::PLAN_END;
            }
            if( !cur->realBegin().isNull() ){
                item_suid_list<<cur->suid();
                time_value_list<<cur->realBegin().toTime_t();
                time_type_list<<EM_YearPlan::REAL_BEGIN;
            }
            if( !cur->realEnd().isNull() ){
                item_suid_list<<cur->suid();
                time_value_list<<cur->realEnd().toTime_t();
                time_type_list<<EM_YearPlan::REAL_END;
            }
            // ================================================================================
            // Сеив исполнителя
            if(cur->getEmployee()!=0){
                emloyee_item_suid_list<<cur->suid();
                emloyee_user_suid_list<<cur->getEmployee()->SUID();
                emloyee_user_type_list<<cur->isEmployeeExternal();
            }
            // ================================================================================
            // Сейв статусов
            QList<TStatus>::iterator iter_status;
            QList<TStatus>& lsStatus = cur->getStatus();
            for(iter_status = lsStatus.begin();iter_status!=lsStatus.end();++iter_status){
                status_suid_list<<cur->suid();
                status_value_list<<(*iter_status);
            }
            // ================================================================================
            cur->setInternalState(EM_InternalState::INTERNAL_STATE_COMMIT_ITEM); // признак сейва
        }
        for(EM_BasePlanItem::iterator iter = cur->begin();iter!=cur->end();++iter){
            stack.push(*iter);
        }

    }

    QString cond = lsCond.join(", ");
    // Обработка исполнителей
    q->prepare(QString(query_DeletePlanItemUser).arg(cond));
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        delete qPlanItemInsert;
        delete qPlanItemUpdate;
        delete qPlanItemStateInsert;
        delete qPlanItemStateUpdate;
        delete qPlanItemTimeInsert;
        delete qPlanItemTimeDelete;
        delete qPlanItemUserInsert;
        delete qPlanItemUserDelete;
        db.close();
        throw CommonException::SQLException(mes);
    }
    if(!emloyee_item_suid_list.isEmpty()){
        q->prepare(query_InsertPlanItemUser);
        q->addBindValue(emloyee_item_suid_list);
        q->addBindValue(emloyee_user_suid_list);
        q->addBindValue(emloyee_user_type_list);
        if(!q->execBatch()){
            throw CommonException::SQLException(q->lastError().text());
        }
    }
    // Обработка статусов
    q->prepare(QString(query_DeletePlanItemStatus).arg(cond));
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        delete qPlanItemInsert;
        delete qPlanItemUpdate;
        delete qPlanItemStateInsert;
        delete qPlanItemStateUpdate;
        delete qPlanItemTimeInsert;
        delete qPlanItemTimeDelete;
        delete qPlanItemUserInsert;
        delete qPlanItemUserDelete;
        db.close();
        throw CommonException::SQLException(mes);
    }
    if(!status_suid_list.isEmpty()){
        q->prepare(query_InsertPlanItemStatus);
        q->addBindValue(status_suid_list);
        q->addBindValue(status_value_list);
        if(!q->execBatch()){
            QString mes = q->lastError().text();
            delete q;
            delete qPlanItemInsert;
            delete qPlanItemUpdate;
            delete qPlanItemStateInsert;
            delete qPlanItemStateUpdate;
            delete qPlanItemTimeInsert;
            delete qPlanItemTimeDelete;
            delete qPlanItemUserInsert;
            delete qPlanItemUserDelete;
            db.close();
            throw CommonException::SQLException(mes);
        }
    }
    // Обработка времен
    q->prepare(QString(query_DeletePlanItemTime).arg(cond));
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        delete qPlanItemInsert;
        delete qPlanItemUpdate;
        delete qPlanItemStateInsert;
        delete qPlanItemStateUpdate;
        delete qPlanItemTimeInsert;
        delete qPlanItemTimeDelete;
        delete qPlanItemUserInsert;
        delete qPlanItemUserDelete;
        db.close();
        throw CommonException::SQLException(mes);
    }
    if(!item_suid_list.isEmpty()){
        q->prepare(query_InsertPlanItemTime);
        q->addBindValue(item_suid_list);
        q->addBindValue(time_value_list);
        q->addBindValue(time_type_list);
        if(!q->execBatch()){
            QString mes = q->lastError().text();
            delete q;
            delete qPlanItemInsert;
            delete qPlanItemUpdate;
            delete qPlanItemStateInsert;
            delete qPlanItemStateUpdate;
            delete qPlanItemTimeInsert;
            delete qPlanItemTimeDelete;
            delete qPlanItemUserInsert;
            delete qPlanItemUserDelete;
            db.close();
            throw CommonException::SQLException(mes);
        }
    }


    delete qPlanItemInsert;
    delete qPlanItemUpdate;
    delete qPlanItemStateInsert;
    delete qPlanItemStateUpdate;
    delete qPlanItemTimeInsert;
    delete qPlanItemTimeDelete;
    delete qPlanItemUserInsert;
    delete qPlanItemUserDelete;
    delete q;
    db.close();
    return opOK;
}

int EM_YearPlan::getYear() const{
    return _year;
}
