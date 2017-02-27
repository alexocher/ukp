#ifndef TABSTRACTOBJECT_H
#define TABSTRACTOBJECT_H

#include <QObject>
#include <QDateTime>
#include <QPtrList>
#include <defUkpCommon>

/******************************************************************************

    Иерархия типов проекта

    QObject                        ( lib  )
        TAbstractObject            (common) - абстрактный класс объекта
            TAbstractApp           (common) - абстрактный класс приложения
                TAppPlan           ( plan ) - класс абстрактного приложения планирования
                    TAppGenPlan             - класс приложения годового планирования
                    TAppCarryPlan           - класс приложения планирования выполнения задач управления (выпуска продукции)
            TAbstractModule        (common) - абстрактный класс модуля приложения
                TModuleUnits       (common) - модуль подразделений
                TModuleEmployees   (common) - модуль сотрудников
                TModuleProductions (common) - модуль продукции
                TModulePlans       ( plan ) - модуль планов
            TUnit                  (common) - подразделение
            TEmployee              (common) - сотрудник
            TProduction            (common) - продукция
            TAbstractAttachment    (common) - абстрактный класс вложения
                TDocument          (common) - документ
                TBdfData           (common) - данные БДФ
                TOobgdData         (common) - данные ООБГД
            TAbstractPlanElement   ( plan ) - абстрактный элемент планирования
                TAbstractPlan      ( plan ) - абстрактный план
                    TGenPlan       ( plan ) - главный план (план предприятия)
                    TCarryPlan     ( plan ) - план выполнения задачи управления (составления ОРД)
                TCarryTask         ( plan ) - проект (задача) управления (выпуска продукции)
                TCarryProcedure    ( plan ) - процедура плана выполнения задачи управления
                TCarryWork         ( plan ) - работа (действие) плана выполнения задачи управления
            TCarryTaskPackage      ( plan ) - пакет проектов (задач) управления (выпуска продукции)

******************************************************************************/

// Абстрактный класс объекта
class UKPCOMMONSHARED_EXPORT TAbstractObject : public QObject
{
    Q_OBJECT
protected:
    int                 fId,           // Идентификатор
                        fNum;          // Номер
    QString             fName,         // Наименование
                        fScrName;      // Отображаемое наименование
    QDateTime           fDtCreation,   // Дата и время создания объекта
                        fDtLastChanging; // Дата и время последнего изменения объекта
    int                 fISort;        // Поля для сортировки: целое
    QString             fSSort;        //                      строковое
    bool                fIsChecked;    // Отметка

public:
    explicit TAbstractObject(int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TAbstractObject(const TAbstractObject &obj);
    ~TAbstractObject();

    int id() const;                    // Идентификатор
    void setId(int id);
    int num() const;                   // Номер
    void setNum(int n);
    QString &name() const;             // Наименование
    void setName(QString nm);
    QString &scrName() const;          // Отображаемое наименование
    void setScrName(QString nm);
    QDateTime &dtCreation() const;     // Дата и время создания объекта
    QDateTime &dtLastChanging() const; // Дата и время последнего изменения объекта
    int iSort() const;                 // Поля для сортировки: целое
    void setISort(int is);             //
    QString &sSort() const;            //                      строковое
    void setSSort(QString ss);         //
    bool isChecked() const;            // Отметка
    void setChecked(bool ch);

    TAbstractObject &operator=(const TAbstractObject &obj);
    virtual void reset(bool thisonly=false)=0; // Сброс
    virtual QString toStr()=0;         // Преобразование в строку
    virtual QString toHtml(bool fullinfo=true)=0; // Преобразование в формат HTML
    virtual bool toDB(QString param)=0; // Сохранение в БД
    virtual bool fromDB(QString param)=0; // Чтение из БД

};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TAbstractObject*> TAbstractObjectList;

namespace obj
{

    // Функциональные классы сортировки
    template <typename T>
    class UKPCOMMONSHARED_EXPORT FSortMinToMaxInt
    {
    public:
        inline bool operator()(const T &t1, const T &t2) const
        {
            return (((TAbstractObject*)t1)->iSort()<((TAbstractObject*)t2)->iSort());
        }
    };
    //qStableSort(list.begin(),list.end(),obj::FSortMinToMaxInt<T*>());

    template <typename T>
    class UKPCOMMONSHARED_EXPORT FSortMaxToMinInt
    {
    public:
        inline bool operator()(const T &t1, const T &t2) const
        {
            return (((TAbstractObject*)t2)->iSort()<((TAbstractObject*)t1)->iSort());
        }
    };
    //qStableSort(list.begin(),list.end(),obj::FSortMaxToMinInt<T*>());

    template <typename T>
    class UKPCOMMONSHARED_EXPORT FSortMinToMaxStr
    {
    public:
        inline bool operator()(const T &t1, const T &t2) const
        {
            return (((TAbstractObject*)t1)->sSort()<((TAbstractObject*)t2)->sSort());
        }
    };
    //qStableSort(list.begin(),list.end(),obj::FSortMinToMaxStr<T*>());

    template <typename T>
    class UKPCOMMONSHARED_EXPORT FSortMaxToMinStr
    {
    public:
        inline bool operator()(const T &t1, const T &t2) const
        {
            return (((TAbstractObject*)t2)->sSort()<((TAbstractObject*)t1)->sSort());
        }
    };
    //qStableSort(list.begin(),list.end(),obj::FSortMaxToMinStr<T*>());

}

#endif // TABSTRACTOBJECT_H
