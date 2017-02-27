#ifndef TABSTRACTPLANELEMENT_H
#define TABSTRACTPLANELEMENT_H

#include <QListWidget>
#include <TDocument>
#include <TBdfData>
#include <TOobgdData>
#include <TEmployee>
#include <defUkpPlan>

class UKPPLANSHARED_EXPORT TAbstractPlanElement : public TAbstractObject
{
protected:
    TCarryOutCondition  fCondition;    // Сосотояние выполнения элемента плана
    TCarryOutProblem    fProblem;      // Причина невозможности выполнения элемента плана
    QString             fSourcesTitle; // Общее наименование исходных данных
    TAbstractAttachmentList fSources;  // Список исходных данных
    QString             fResultsTitle; // Общее наименование результатов
    TAbstractAttachmentList fResults;  // Список результатов
    QList<TEmployeeRole> fTemplateRoles; // Список ролей ДЛ, которые способны выполнить элемент
    TEmployeeList       fPossibleEmployees; // ДЛ, которые могут быть закреплены за элементом
    TEmployee          *fEmployee;     // ДЛ, закрепленное за элементом
    int                 fTemplatePeriod, // Продолжительность выполнения операции (час): типовая
                        fPlanPeriod;   //                                                планируемая
                                       //                                                фактическая (вычисляется)
    QDateTime          *fDtPlanBegin,  // Моменты времени: начала по плану
                       *fDtPlanEnd,    //                  окончания по плану
                       *fDtRealBegin,  //                  начала фактического
                       *fDtRealEnd;    //                  окончания фактического
    TStatusList         fStatuses;     // Список статусов
    bool                fIsSaved,      // Признак сохраненного элемента плана
                        fIsVolatile;   // Признак изменчивости (можно перепланировать)
    QString             fDescr;        // Дескриптор
    int                 fUnitId;       // id формирования, кому принадлежит элемент

public:
    explicit TAbstractPlanElement(int id=0, int n=0, QString nm="", TAbstractObject *parent=NULL);
    TAbstractPlanElement(const TAbstractPlanElement &att);
    ~TAbstractPlanElement();

    TCarryOutCondition condition() const; // Сосотояние выполнения элемента плана
    void setCondition(TCarryOutCondition cnd);
    TCarryOutProblem problem() const;  // Причина невозможности выполнения элемента плана
    void setProblem(TCarryOutProblem pr);
    QString &sourcesTitle() const;     // Общее наименование исходных данных
    void setSourcesTitle(QString ttl);
    TAbstractAttachmentList &sources() const; // Список исходных данных
    void insertSource(TAbstractAttachment *att); // ... добавить
    void removeSource(TAbstractAttachment *att); // ... удалить
    void clearSources();               // ... очистить
    TDocumentList *sourceDocs() const; // Список документов в исходных данных
    TBdfDataList *sourceBdfs() const;  // Список данных БДФ в исходных данных
    TOobgdDataList *sourceOobgds() const; // Список данных ООБГД в исходных данных
    QString &resultsTitle() const;     // Общее наименование результатов
    void setResultsTitle(QString ttl);
    TAbstractAttachmentList &results() const; // Список результатов
    void insertResult(TAbstractAttachment *att); // ... добавить
    void removeResult(TAbstractAttachment *att); // ... удалить
    void clearResults();               // ... очистить
    TDocumentList *resultDocs() const; // Список документов в исходных данных
    TBdfDataList *resultBdfs() const;  // Список данных БДФ в исходных данных
    TOobgdDataList *resultOobgds() const; // Список данных ООБГД в исходных данных
    QList<TEmployeeRole> &templateRoles() const; // Список ролей ДЛ, которые способны выполнить элемент
    TEmployeeRole firstTemplateRole() const; // ... получить первую роль
    void setSingleTemplateRole(const TEmployeeRole &rl); // ... установить единственную роль
    TEmployeeList &possibleEmployees() const; // ДЛ, которые могут быть закреплены за элементом
    void insertPossibleEmployee(TEmployee *empl); // ... добавить
    void removePossibleEmployee(TEmployee *empl); // ... удалить
    void clearPossibleEmployeess();    // ... очистить
    TEmployee *employee() const;       // ДЛ, закрепленное за элементом
    void setEmployee(TEmployee *emp);
    int templatePeriod() const;        // Продолжительность выполнения операции (час): типовая
    void setTemplatePeriod(int t);
    int planPeriod() const;            //                                              планируемая
    void setPlanPeriod(int t);
    int realPeriod() const;            //                                              фактическая (вычисляется)
    QDateTime *dtPlanBegin() const;    // Моменты времени: начала по плану
    int dayPlanBegin() const;
    void setDtPlanBegin(const QDateTime &dt);
    void setDtPlanBegin(int dt, int year=0);
    QDateTime *dtPlanEnd() const;      //                  окончания по плану
    int dayPlanEnd() const;
    void setDtPlanEnd(const QDateTime &dt);
    void setDtPlanEnd(int dt, int year=0);
    QDateTime *dtRealBegin() const;    //                  начала фактического
    int dayRealBegin() const;
    void setDtRealBegin(const QDateTime &dt);
    void setDtRealBegin(int dt, int year=0);
    QDateTime *dtRealEnd() const;      //                  окончания фактического
    int dayRealEnd() const;
    void setDtRealEnd(const QDateTime &dt);
    void setDtRealEnd(int dt, int year=0);
    QDateTime *dtRealEndPrognos() const; //                окончания прогнозируемого (рассчитывается)
    int dayRealEndPrognos() const;
    TStatusList &statuses() const;     // Список статусов
    void setStatuses(const TStatusList &stss);
    bool isSaved() const;              // Признак сохраненного плана
    void setSaved(bool sv);
    bool isVolatile() const;           // Признак изменчивости (можно перепланировать)
    void setVolatile(bool vt);
    QString &descr() const;            // Дескриптор
    void setDescr(const QString &dscr);
    int unitId() const;                // id формирования, кому принадлежит элемент
    void setUnitId(int id);

    TAbstractPlanElement &operator=(const TAbstractPlanElement &att);

    void reflectAttachmentsToLw(const TAbstractAttachmentList &atchmts, QListWidget &lw);

public: // TAbstractObject interface
    void reset(bool thisonly);
    QString toStr();
    QString toHtml(bool fullinfo=true);
    bool toDB(QString param);
    bool fromDB(QString param);
};

typedef UKPPLANSHARED_EXPORT QPtrList<TAbstractPlanElement*> TAbstractPlanElementList;

#endif // TABSTRACTPLANELEMENT_H
