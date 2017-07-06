#ifndef EM_PLAN_H
#define EM_PLAN_H

#include "commondef.h"
#include "defdbconst.h"
#include "enums.h"
#include "BaseTree.h"
#include "exceptions.h"
#include "EM_AddressBook.h"
#include "EM_Production.h"

#include <QDateTime>
#include <QObject>
#include <QTimer>

// типы узла
enum NODE_TYPE{
    ROOT,       // корень
    PROJECT,    // проект
    STAGE,      // этап
    TASK,       // задача
    CARRY_PLAN, // план
    UNDEF
};



class EM_YearPlan;
/**
 * @brief EM_AbstractParam
 * Класс входных/выходных данных задач
 */
class ENTITYMANAGERSHARED_EXPORT EM_AbstractParam:public AbstractEntity{
    friend class EM_YearPlan;
private:
    TAttachmentType _type;  // тип параметра
protected:
    EM_AbstractParam(TAttachmentType type);
public:
    EM_AbstractParam();
    ~EM_AbstractParam(){;}

    TAttachmentType getAttachmentType() const;
};

/**
 * @brief The EM_BasePlanItem class
 * Базавый узел плана
 */
class ENTITYMANAGERSHARED_EXPORT EM_BasePlanItem:public BaseTreeItem<int,NODE_TYPE>,AbstractEntity,EM_InternalState{
    friend class EM_YearPlan;
private:
    // статусы и типы
    TCarryOutCondition       _state;                // состояние
    TCarryOutProblem         _problem;              // проблема
    // временные характеристики
    QDateTime                _planBegin;            // плановое время начала
    QDateTime                _planEnd;              // плановое время завершения
    QDateTime                _realBegin;            // реальное время начала
    QDateTime                _realEnd;              // реальное время завершения

    QString                  _fullTitle;            // полное название
    QString                  _shortTitle;           // короткое название
    QString                  _descr;                // дополнительная информация в формате JSON



    int                      _typal_duration;       // типовая длительность
    int                      _duration;             // длительность
    QList<TStatus>           _status;               // список статусов узла

    QString                  _srcTitle;             // Общее наименование исходных данных
    QString                  _resTitle;             // Общее наименование результатов

    QList<EM_AbstractParam*> _inParam;              // входные параметры
    QList<EM_AbstractParam*> _outParam;             // выходные параметры

    int                      _custom_number;        // пользовательский номер узла


    int                      _oshs_item_id;         // идентификатор внешнего элемента ошс

    int                      _progress;             // прогресс

    bool                     _is_tester;            // провверяющая задача
    int                      _ext_proc_num;         // код внешней процедуры

    QList<int>               _linkeditem;
protected:
    QList<TEmployeeType>     _template_employee;    // список возможных исполителей (роли)
    TEmployeeType            _templ_employee;       // возможный исполнитель
    EM_User*                 _employee;             // назначенный исполнитель
    bool                     _external_employee;    // признак внешнего исполнителя
    bool                     _visual_hide;          // признак скрытия узла
protected:
    EM_BasePlanItem(NODE_TYPE type,int suid);

//    EM_BasePlanItem(const EM_BasePlanItem& o);
//    EM_BasePlanItem& operator=(const EM_BasePlanItem& o);

    void setInternalState(INTERNAL_STATE state,bool child=false);
    void setModify(bool status=false);    

    void setVisualHideFalse();
    void setVisualHideTrue();
    void setVisualHideReset();
    void setVisualHide(bool value);    
private:
    // типы записей времени
    enum INTERNAL_TIME_TYPE{
        PLAN_BEGIN = 10,
        PLAN_END,
        REAL_BEGIN,
        REAL_END,
    };


    EM_OPERATION_RETURNED_STATUS saveStatus()const throw(CommonException::OpenDBException,CommonException::SQLException);
    QDateTime child_time(INTERNAL_TIME_TYPE time_type)const;
public:
    virtual ~EM_BasePlanItem();

    class iterator{
            friend class EM_BasePlanItem;
            iterator(QList<BaseTreeItem*>::iterator el){iter = el;}
        public:
            // конструкторы
            iterator(){;}
            iterator(const iterator &it){iter=it.iter;}
            // присваивание итераторов – генерируется по умочанию
            // сравнение итераторов
            bool operator==(const iterator &it) const{return (iter==it.iter);}
            bool operator!=(const iterator &it) const{return !(iter==it.iter);}
            // продвижение к следующему элементу – только префиксная форма
            iterator& operator++(){
                ++iter;
                return *this;
            }
            // продвижение к предыдущему элементу – только префиксная форма
            iterator& operator--(){
                --iter;
                return *this;
            }
            // получить ссылку на информационную часть
            // работает справа и слева от знака присваивания
            EM_BasePlanItem* operator*() const{return (EM_BasePlanItem*)*iter;}
        private:
            QList<BaseTreeItem*>::iterator iter;
    };
    class const_iterator{
            friend class EM_BasePlanItem;
            const_iterator(QList<BaseTreeItem*>::const_iterator el){iter = el;}
        public:
            // конструкторы
            const_iterator(){;}
            const_iterator(const const_iterator &it){iter=it.iter;}
            // присваивание итераторов – генерируется по умочанию
            // сравнение итераторов
            bool operator==(const const_iterator &it) const{return (iter==it.iter);}
            bool operator!=(const const_iterator &it) const{return !(iter==it.iter);}
            // продвижение к следующему элементу – только префиксная форма
            const_iterator& operator++(){
                ++iter;
                return *this;
            }
            // продвижение к предыдущему элементу – только префиксная форма
            const_iterator& operator--(){
                --iter;
                return *this;
            }
            // получить ссылку на информационную часть
            // работает справа и слева от знака присваивания
            EM_BasePlanItem* operator*() const{return (EM_BasePlanItem*)*iter;}
        private:
            QList<BaseTreeItem*>::const_iterator iter;
    };
    // итераторы
    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;
    // методы доступа
    TCarryOutCondition      getState() const;
    TCarryOutProblem        getProblem() const;
    bool                    isProblem() const;

    time_t                  getPlanBegin() const;
    time_t                  getPlanEnd() const;
    time_t                  getRealBegin() const;
    time_t                  getRealEnd() const;

    QDateTime               planBegin() const;
    QDateTime               planEnd() const;
    QDateTime               realBegin() const;
    QDateTime               realEnd() const;

    QString     const&      getFullTitle() const;
    QString     const&      getShortTitle() const;
    QString     const&      getDescr() const;
    QString     const&      getMemberId() const;
    virtual     EM_User*    getEmployee();
    bool                    isEmployeeExternal() const;

    QString     const&      getSrcTitle() const;
    QString     const&      getResTitle() const;

    int                     getTypalDuration() const;
    int                     getDuration() const;
    QList<TStatus> &        getStatus();
    QString                 getStatusAsString() const;

    int                     getNum() const;

    virtual TEmployeeType   getTemplEmployee() const;
    int                     getOshsItemID() const;
    int                     getProgress() const;
    bool                    isTester() const;    
    int                     getExtProcNum() const;

    const QList<int>        &getLinkedItem() const; //список для идентификаторов связанных узлов (надо будет доработать на указатели объектов)

    void    addLinkedItem(int item);        // добавить элемент
    void    remLinkedItemByIdx(int idx);    // удалить элемент по индексу
    void    remLinkedItem(int item);        // удалить элемент
    void    remLinkedItem();                // удалить все

    bool    isVisualHide() const; // признак видимости

    void    setState(const TCarryOutCondition& value,bool withoutupdate=false);
    void    setProblem(const TCarryOutProblem& value);

    void    setPlanBegin(const time_t& value);
    void    setPlanEnd(const time_t& value);
    void    setRealBegin(const time_t& value);
    void    setRealEnd(const time_t& value);

    void    planBegin(const QDateTime& value);
    void    planEnd(const QDateTime& value);
    void    realBegin(const QDateTime& value);
    void    realEnd(const QDateTime& value);

    void    setFullTitle(const QString& value);
    void    setShortTitle(const QString& value);
    void    setDescr(const QString& value);
    void    setMemberId(const QString& value);
    void    setEmployee(EM_User* employee,bool external=false);

    void    setTypalDuration(int value);
    void    setDuration(int value);

    void    setSrcTitle(const QString& value);
    void    setResTitle(const QString& value);

    void    addStatus(TStatus value);
    void    remStatus(TStatus value);

    void    setNum(int value);

    void    setTemplEmployee(TEmployeeType value);
    void    setOshsItemID(int value);
    void    setProgress(int value);
    void    setTester(bool value);
    void    setExtProcNum(int value);
    // Получить подэлемент по индексу
    EM_BasePlanItem* operator[](int index);
    EM_BasePlanItem* at(int index);
    // Кол-во дочерних элементов
    int getChildCount() const;
    // Получить порядковый номер (для корня -1)
    int getOrder();
    // suid узла
    int getID() const;

};
/**
 * @brief The EM_ProjectPlanItem class
 * Элемент: типа проект (TCarryTask)
 */
class ENTITYMANAGERSHARED_EXPORT EM_ProjectPlanItem:public EM_BasePlanItem{
    friend class EM_YearPlan;
protected:
    EM_ProjectPlanItem(int suid);
    EM_ProjectPlanItem(const EM_ProjectPlanItem& obj);

    EM_Production* _production;
    int            _priority;
    QDateTime      _tbegin;
    QDateTime      _tend;
public:
    ~EM_ProjectPlanItem(){;}

    EM_Production* getProduction() const;
    int getPriority() const;
    void setProduction(EM_Production* value);
    void setPriority(int value);

    const QDateTime& getTimeBegin() const;
    const QDateTime& getTimeEnd() const;
    void setTimeBegin(const QDateTime& value);
    void setTimeEnd(const QDateTime& value);
};
/**
 * @brief The EM_PlanItem class
 * Элемент план (TCarryPlan)
 */
class ENTITYMANAGERSHARED_EXPORT EM_PlanItem:public EM_BasePlanItem{
    friend class EM_YearPlan;
protected:
    EM_PlanItem(int suid);
    EM_PlanItem(const EM_PlanItem& obj);
public:
    ~EM_PlanItem(){;}

};
/**
 * @brief The EM_StagePlanItem class
 * Элемент: этап проекта (TCarryProcedure)
 */
class ENTITYMANAGERSHARED_EXPORT EM_StagePlanItem:public EM_BasePlanItem{
    friend class EM_YearPlan;
/*private:
    int _ext_proc_num;  // код внешней процедуры*/
protected:
    EM_StagePlanItem(int suid);
public:
    ~EM_StagePlanItem(){;}

};
/**
 * @brief The EM_TaskUtil class
 * Класс опиисывает модуль для выполнения задачи
 */
class ENTITYMANAGERSHARED_EXPORT EM_TaskUtil{
private:
    TExternalModuleType _external;  // тип внешнего модуля
    QString             _descr;     // атрибуты в формате JSON
public:
    // module - тип модуля, при None - считается внутренним
    EM_TaskUtil(TExternalModuleType module);
    // проверка внтренний ли модуль
    bool isInternal() const;
};

/**
 * @brief The EM_StagePlanItem class
 * Элемент: задача этапа (TCarryWork)
 */
class ENTITYMANAGERSHARED_EXPORT EM_TaskPlanItem:public EM_BasePlanItem{
    friend class EM_YearPlan;
private:
    TExternalModuleType _ext_module_type;   // тип внешнего модуля
    bool                _optional;          // признак опциональности для шаблонов
    bool                _present;           // признак наличия для плана?

    EM_TaskUtil* _util;                     // используемая утилита
protected:
    EM_TaskPlanItem(int suid);
public:
    ~EM_TaskPlanItem();
    const EM_TaskUtil *getUtil() const;

    bool isOptional() const;
    bool isPresent() const;
    TExternalModuleType getExtModuleType() const;

    void setOptional(bool value);
    void setPresent(bool value);
    void setExtModuleType(TExternalModuleType value);

    EM_User*        getEmployee();
    TEmployeeType   getTemplEmployee() const;
};

class EM_YearPlanDic;
/**
 * @brief The EM_YearPlan class
 * Модель годового плана
 */
class ENTITYMANAGERSHARED_EXPORT EM_YearPlan:public QObject, AbstractEntity,BaseTree<int,NODE_TYPE>{
    Q_OBJECT
    friend class EM_YearPlanDic;
public:
    // состояния плана
    enum STATE{
        EMPTY =     0,
        NEW =       1,  // новый
        PLAN =      2,  // спланированный
        ACCEPT =    3,  // утвержденный
        ENDED =     4,  // завершенный
        DLETED =    5,  // удален
    };
private:
    // типы записей времени
    enum INTERNAL_TIME_TYPE{
        PLAN_BEGIN = 0,
        PLAN_END,
        REAL_BEGIN,
        REAL_END,
        TIPICAL_INTERVAL,
    };

private:
    QTimer*                         _tmrStatus; // таймер обновления статуса
    int                             _year;      // год планирования
    QString                         _title;     // название
    QString                         _descr;     // дополнительные атрибуты в формате JSON
    STATE                           _state;     // состояние

    const QString                   _connect_name;
private slots:
    void updateStatusByTime();
signals:
    void notifyStatusChange(QList<int> ls);
private:
    EM_BasePlanItem* Instance(NODE_TYPE type,int suid);
    QString conv(const QList<int> &ls );
    void conv(EM_BasePlanItem *item, QString str);
public:
    ~EM_YearPlan();
    // создать пустой годовой план
    EM_YearPlan(int year);
    // удалить весь план из БД
    EM_OPERATION_RETURNED_STATUS removeFromDB()throw(CommonException::OpenDBException,CommonException::SQLException);
    // сохранить в БД
    EM_OPERATION_RETURNED_STATUS toDB()throw(CommonException::OpenDBException,CommonException::SQLException);
    // загрузить план за указанный год из БД
    EM_OPERATION_RETURNED_STATUS fromDB(int year)throw(CommonException::OpenDBException,CommonException::SQLException);
    EM_OPERATION_RETURNED_STATUS fromDB()throw(CommonException::OpenDBException,CommonException::SQLException);
    // год
    int getYear() const;
    // название
    QString const& getTitle() const;
    void setTitle(const QString& title);
    // атрибуты
    QString const& getDescr() const;
    void setDescr(const QString& descr);
    // состояние
    STATE getState() const;
    void setState(STATE state);
    // корневой элемент
    EM_BasePlanItem* getRoot()const;
    // добавление нового элемента
    void add(EM_BasePlanItem *parent,EM_BasePlanItem *item);
    void add(EM_BasePlanItem *item);
    void add(EM_BasePlanItem *parent,EM_ProjectPlanItem *item);
    void add(EM_BasePlanItem *parent,EM_StagePlanItem *item);
    void add(EM_BasePlanItem *parent,EM_TaskPlanItem *item);
    // удаление элемента
    EM_OPERATION_RETURNED_STATUS rem(EM_BasePlanItem *item) throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException);
    // перемещение элемента
    void move(EM_BasePlanItem *parent,EM_BasePlanItem *item);
    // создание дефолтного узла, заданного типа
    EM_BasePlanItem* Instance(NODE_TYPE type);
    // получить список шаблонных ролей
    QList<TEmployeeType> getTemplateEmployee(EM_BasePlanItem *item) throw(CommonException::SQLException,CommonException::OpenDBException,CommonException::NullParamException);
    // задать список шаблонных ролей
    void setTemplateEmployee(EM_BasePlanItem *item,const QList<TEmployeeType> &ls)throw(CommonException::SQLException,CommonException::OpenDBException,CommonException::NullParamException);
    // поиск по идентификатору
    EM_BasePlanItem* find(int id) const;
    // сброс фильтров
    void fileterReset();
    // Фильтрация по пользователю
    void filterByUser(QList<const EM_User *> &ls_employee );
    // вывод на консоль дерева (название узлов и подчиненность)
    void printPlanTree() const;
    /*!
     * \brief Сериалтзация
     * \return массив байт
     */
    QByteArray serialize() const;
    /*!
     * \brief Десериализация
     * \param data - массив байт
     * \return код статуса
     */
    int deserialize(const QByteArray& data);
};

/**
 * @brief The EM_YearPlanDic class
 * Класс для управления планами
 */
class ENTITYMANAGERSHARED_EXPORT EM_YearPlanDic{
private:
    QList< QSharedPointer<EM_YearPlan> > ls;
    QMap< int,QSharedPointer<EM_YearPlan> > m;

    const QString                   _connect_name;

    EM_YearPlanDic()throw(CommonException::OpenDBException,CommonException::SQLException); // прячим конструктор
    ~EM_YearPlanDic();

    EM_YearPlanDic(EM_YearPlanDic const&);
    EM_YearPlanDic& operator= (EM_YearPlanDic const&);

    // переменные управления синглтоном
    static bool _instflag;
    static EM_YearPlanDic* _instance;
public:
    static EM_YearPlanDic* getInstance()throw(CommonException::OpenDBException,CommonException::SQLException){
        if(! _instflag){
            _instance = new EM_YearPlanDic();
            _instflag = true;
        }
        return _instance;
    }
    // получение словаря
    static EM_YearPlanDic& Instance()throw(CommonException::OpenDBException,CommonException::SQLException){
        return *getInstance();
    }


public:
    // обновление списка планов
    void reinit();
    // Получить список годовых планов
    QList<QSharedPointer<EM_YearPlan> > &getList();
    // Проверить наличие плана
    bool contains(int year) const;
    // получить план по году
    QSharedPointer<EM_YearPlan> by(int year) const;
    // удалить все планы из БД
    void removeAll()throw(CommonException::OpenDBException,CommonException::SQLException);
    // удалить план из БД
    void remove(int year)throw(CommonException::OpenDBException,CommonException::SQLException);
};



#endif // EM_PLAN_H
