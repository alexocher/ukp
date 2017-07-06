#include <defMacro>
#include <gen>
#include <TIdent>
#include <TAbstractPlanElement>

TAbstractPlanElement::TAbstractPlanElement(int id, int n, QString nm, TAbstractObject *parent) : TAbstractObject(id, n, nm, parent), fCondition(cocNone), fProblem(copNone), fSourcesTitle(""), fResultsTitle(""), fEmployee(NULL), fTemplatePeriod(0), fPlanPeriod(0), fDtPlanBegin(NULL), fDtPlanEnd(NULL), fDtRealBegin(NULL), fDtRealEnd(NULL), fIsSaved(false), fIsVolatile(true), fDescr(""), fUnitId(0), fCarryOutPercent(0)
{
    fSources.setAutoDelete(true);
    fResults.setAutoDelete(true);
    fPossibleEmployees.setAutoDelete(false);
}
//-----------------------------------------------------------------------------

TAbstractPlanElement::TAbstractPlanElement(const TAbstractPlanElement &ape) : TAbstractObject(ape), fCondition(ape.fCondition), fProblem(ape.fProblem), fSourcesTitle(ape.fSourcesTitle), fResultsTitle(ape.fResultsTitle), fTemplateRoles(ape.fTemplateRoles), fEmployee(ape.fEmployee), fTemplatePeriod(ape.fTemplatePeriod), fPlanPeriod(ape.fPlanPeriod), fDtPlanBegin(ape.fDtPlanBegin ? new QDateTime(*ape.fDtPlanBegin) : NULL), fDtPlanEnd(ape.fDtPlanEnd ? new QDateTime(*ape.fDtPlanEnd) : NULL), fDtRealBegin(ape.fDtRealBegin ? new QDateTime(*ape.fDtRealBegin) : NULL), fDtRealEnd(ape.fDtRealEnd ? new QDateTime(*ape.fDtRealEnd) : NULL), fStatuses(ape.fStatuses), fIsSaved(ape.fIsSaved), fIsVolatile(ape.fIsVolatile), fDescr(ape.fDescr), fUnitId(ape.fUnitId), fCarryOutPercent(ape.fCarryOutPercent)
{
    fSources.setAutoDelete(true);
    foreach (TAbstractAttachment *el, ape.fSources)
        switch (el->type())
        {
            case attDocument:
                if (TDocument *doc = dynamic_cast<TDocument*>(el)) fSources.append(new TDocument(*doc));
                break;
            case attBdfData:
                if (TBdfData *bdt = dynamic_cast<TBdfData*>(el)) fSources.append(new TBdfData(*bdt));
                break;
            case attOobgdData:
                if (TOobgdData *odt = dynamic_cast<TOobgdData*>(el)) fSources.append(new TOobgdData(*odt));
                break;
            default: ; // attNone
        }
    fResults.setAutoDelete(true);
    foreach (TAbstractAttachment *el, ape.fResults)
        switch (el->type())
        {
            case attDocument:
                if (TDocument *doc = dynamic_cast<TDocument*>(el)) fResults.append(new TDocument(*doc));
                break;
            case attBdfData:
                if (TBdfData *bdt = dynamic_cast<TBdfData*>(el)) fResults.append(new TBdfData(*bdt));
                break;
            case attOobgdData:
                if (TOobgdData *odt = dynamic_cast<TOobgdData*>(el)) fResults.append(new TOobgdData(*odt));
                break;
            default: ; // attNone
        }
    fPossibleEmployees.setAutoDelete(false);
    foreach (TEmployee * empl, ape.fPossibleEmployees) fPossibleEmployees.append(empl);
    fLinkedElements = ape.fLinkedElements;
}
//-----------------------------------------------------------------------------

TAbstractPlanElement::~TAbstractPlanElement()
{
    DELETE(fDtPlanBegin);
    DELETE(fDtPlanEnd);
    DELETE(fDtRealBegin);
    DELETE(fDtRealEnd);
}
//-----------------------------------------------------------------------------

TAbstractPlanElement &TAbstractPlanElement::operator=(const TAbstractPlanElement &ape)
{
    if (&ape == this) return *this;
    fCondition = ape.fCondition;
    fProblem = ape.fProblem;
    fSourcesTitle = ape.fSourcesTitle;
    fSources.clear();
    foreach (TAbstractAttachment *el, ape.fSources)
        switch (el->type())
        {
            case attDocument:
                if (TDocument *doc = dynamic_cast<TDocument*>(el)) fSources.append(new TDocument(*doc));
                break;
            case attBdfData:
                if (TBdfData *bdt = dynamic_cast<TBdfData*>(el)) fSources.append(new TBdfData(*bdt));
                break;
            case attOobgdData:
                if (TOobgdData *odt = dynamic_cast<TOobgdData*>(el)) fSources.append(new TOobgdData(*odt));
                break;
            default: ; // attNone
        }
    fResultsTitle = ape.fResultsTitle;
    fResults.clear();
    foreach (TAbstractAttachment *el, ape.fResults)
        switch (el->type())
        {
            case attDocument:
                if (TDocument *doc = dynamic_cast<TDocument*>(el)) fResults.append(new TDocument(*doc));
                break;
            case attBdfData:
                if (TBdfData *bdt = dynamic_cast<TBdfData*>(el)) fResults.append(new TBdfData(*bdt));
                break;
            case attOobgdData:
                if (TOobgdData *odt = dynamic_cast<TOobgdData*>(el)) fResults.append(new TOobgdData(*odt));
                break;
            default: ; // attNone
        }
    fTemplateRoles = ape.fTemplateRoles;
    fPossibleEmployees.clear();
    foreach (TEmployee * empl, ape.fPossibleEmployees) fPossibleEmployees.append(empl);
    fEmployee = ape.fEmployee;
    fTemplatePeriod = ape.fTemplatePeriod;
    fPlanPeriod = ape.fPlanPeriod;
    DELETE(fDtPlanBegin);
    if (ape.fDtPlanBegin) fDtPlanBegin = new QDateTime(*ape.fDtPlanBegin);
    DELETE(fDtPlanEnd);
    if (ape.fDtPlanEnd) fDtPlanEnd = new QDateTime(*ape.fDtPlanEnd);
    DELETE(fDtRealBegin);
    if (ape.fDtRealBegin) fDtRealBegin = new QDateTime(*ape.fDtRealBegin);
    DELETE(fDtRealEnd);
    if (ape.fDtRealEnd) fDtRealEnd = new QDateTime(*ape.fDtRealEnd);
    fStatuses = ape.fStatuses;
    fIsSaved = ape.fIsSaved;
    fIsVolatile = ape.fIsVolatile;
    fDescr = ape.fDescr;
    fUnitId = ape.fUnitId;
    fCarryOutPercent = ape.fCarryOutPercent;
    fLinkedElements = ape.fLinkedElements;
    return *this;
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::reset(bool thisonly)
{
    if (!thisonly) TAbstractObject::reset(true);
    fCondition = cocNone;
    fProblem = copNone;
    fSourcesTitle = "";
    fSources.clear();
    fResultsTitle = "";
    fResults.clear();
    fTemplateRoles.clear();
    fPossibleEmployees.clear();
    fEmployee = NULL;
    fTemplatePeriod = 0;
    fPlanPeriod = 0;
    DELETE(fDtPlanBegin);
    DELETE(fDtPlanEnd);
    DELETE(fDtRealBegin);
    DELETE(fDtRealEnd);
    fStatuses.clear();
    fIsSaved = false;
    fIsVolatile = true;
    fDescr = "";
    fUnitId = 0;
    fCarryOutPercent = 0;
    fLinkedElements.clear();
}
//-----------------------------------------------------------------------------

TCarryOutCondition TAbstractPlanElement::condition() const
{
    return fCondition;
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setCondition(TCarryOutCondition cnd)
{
    fCondition = cnd;
}
//-----------------------------------------------------------------------------

TCarryOutProblem TAbstractPlanElement::problem() const
{
    return fProblem;
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setProblem(TCarryOutProblem pr)
{
    fProblem = pr;
}
//-----------------------------------------------------------------------------

QString &TAbstractPlanElement::sourcesTitle() const
{
    return const_cast<QString&>(fSourcesTitle);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setSourcesTitle(QString ttl)
{
    fSourcesTitle = ttl;
}
//-----------------------------------------------------------------------------

TAbstractAttachmentList &TAbstractPlanElement::sources() const
{
    return const_cast<TAbstractAttachmentList&>(fSources);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::insertSource(TAbstractAttachment *att)
{
    if (att && fSources.indexOf(att) == -1) fSources.append(att);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::removeSource(TAbstractAttachment *att)
{
    if (att) fSources.removeOne(att);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::clearSources()
{
    fSources.clear();
}
//-----------------------------------------------------------------------------

TDocumentList *TAbstractPlanElement::sourceDocs() const
{
    TDocumentList *res(new TDocumentList());
    res->setAutoDelete(false);
    foreach (TAbstractAttachment *el, fSources)
        if (el->type() == attDocument)
            if (TDocument *doc = dynamic_cast<TDocument*>(el)) res->append(doc);
    if (!res->count()) DELETE(res);
    return res;
}
//-----------------------------------------------------------------------------

TBdfDataList *TAbstractPlanElement::sourceBdfs() const
{
    TBdfDataList *res(new TBdfDataList());
    res->setAutoDelete(false);
    foreach (TAbstractAttachment *el, fSources)
        if (el->type() == attBdfData)
            if (TBdfData *bdt = dynamic_cast<TBdfData*>(el)) res->append(bdt);
    if (!res->count()) DELETE(res);
    return res;
}
//-----------------------------------------------------------------------------

TOobgdDataList *TAbstractPlanElement::sourceOobgds() const
{
    TOobgdDataList *res(new TOobgdDataList());
    res->setAutoDelete(false);
    foreach (TAbstractAttachment *el, fSources)
        if (el->type() == attOobgdData)
            if (TOobgdData *odt = dynamic_cast<TOobgdData*>(el)) res->append(odt);
    if (!res->count()) DELETE(res);
    return res;
}
//-----------------------------------------------------------------------------

QString &TAbstractPlanElement::resultsTitle() const
{
    return const_cast<QString&>(fResultsTitle);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setResultsTitle(QString ttl)
{
    fResultsTitle = ttl;
}
//-----------------------------------------------------------------------------

TAbstractAttachmentList &TAbstractPlanElement::results() const
{
    return const_cast<TAbstractAttachmentList&>(fResults);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::insertResult(TAbstractAttachment *att)
{
    if (att && fResults.indexOf(att) == -1) fResults.append(att);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::removeResult(TAbstractAttachment *att)
{
    if (att) fResults.removeOne(att);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::clearResults()
{
    fResults.clear();
}
//-----------------------------------------------------------------------------

TDocumentList *TAbstractPlanElement::resultDocs() const
{
    TDocumentList *res(new TDocumentList());
    res->setAutoDelete(false);
    foreach (TAbstractAttachment *el, fResults)
        if (el->type() == attDocument)
            if (TDocument *doc = dynamic_cast<TDocument*>(el)) res->append(doc);
    if (!res->count()) DELETE(res);
    return res;
}
//-----------------------------------------------------------------------------

TBdfDataList *TAbstractPlanElement::resultBdfs() const
{
    TBdfDataList *res(new TBdfDataList());
    res->setAutoDelete(false);
    foreach (TAbstractAttachment *el, fResults)
        if (el->type() == attBdfData)
            if (TBdfData *bdt = dynamic_cast<TBdfData*>(el)) res->append(bdt);
    if (!res->count()) DELETE(res);
    return res;
}
//-----------------------------------------------------------------------------

TOobgdDataList *TAbstractPlanElement::resultOobgds() const
{
    TOobgdDataList *res(new TOobgdDataList());
    res->setAutoDelete(false);
    foreach (TAbstractAttachment *el, fResults)
        if (el->type() == attOobgdData)
            if (TOobgdData *odt = dynamic_cast<TOobgdData*>(el)) res->append(odt);
    if (!res->count()) DELETE(res);
    return res;
}
//-----------------------------------------------------------------------------

QList<TEmployeeRole> &TAbstractPlanElement::templateRoles() const
{
    return const_cast<QList<TEmployeeRole>&>(fTemplateRoles);
}
//-----------------------------------------------------------------------------

TEmployeeRole TAbstractPlanElement::firstTemplateRole() const
{
    return fTemplateRoles.isEmpty() ? TEmployeeRole() : fTemplateRoles.first();
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setSingleTemplateRole(const TEmployeeRole &rl)
{
    fTemplateRoles.clear();
    fTemplateRoles.append(rl);
}
//-----------------------------------------------------------------------------

TEmployeeList &TAbstractPlanElement::possibleEmployees() const
{
    return const_cast<TEmployeeList&>(fPossibleEmployees);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::insertPossibleEmployee(TEmployee *empl)
{
    if (empl && fPossibleEmployees.indexOf(empl) == -1) fPossibleEmployees.append(empl);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::removePossibleEmployee(TEmployee *empl)
{
    if (empl) fPossibleEmployees.removeOne(empl);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::clearPossibleEmployeess()
{
    fPossibleEmployees.clear();
}
//-----------------------------------------------------------------------------

TEmployee *TAbstractPlanElement::employee() const
{
    return fEmployee;
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setEmployee(TEmployee *emp)
{
    fEmployee = emp;
}
//-----------------------------------------------------------------------------

int TAbstractPlanElement::templatePeriod() const
{
    return fTemplatePeriod;
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setTemplatePeriod(int t)
{
    fTemplatePeriod = t;
}
//-----------------------------------------------------------------------------

int TAbstractPlanElement::planPeriod() const
{
    return fPlanPeriod;
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setPlanPeriod(int t)
{
    fPlanPeriod = t;
}
//-----------------------------------------------------------------------------

int TAbstractPlanElement::realPeriod() const
{
    if (!fDtRealBegin || !fDtRealEnd) return 0;
    int diff(gen::round(fDtRealBegin->secsTo(*fDtRealEnd) / 3600.)); // чвс
    return diff > 0 ? diff : 0;
}
//-----------------------------------------------------------------------------

QDateTime *TAbstractPlanElement::dtPlanBegin() const
{
    return fDtPlanBegin;
}

void TAbstractPlanElement::setDtPlanBegin(const QDateTime &dt)
{
    if (dt.isValid()) fDtPlanBegin = new QDateTime(dt);
}
//-----------------------------------------------------------------------------

QDateTime *TAbstractPlanElement::dtPlanEnd() const
{
    return fDtPlanEnd;
}

void TAbstractPlanElement::setDtPlanEnd(const QDateTime &dt)
{
    if (dt.isValid()) fDtPlanEnd = new QDateTime(dt);
}
//-----------------------------------------------------------------------------

QDateTime *TAbstractPlanElement::dtRealBegin() const
{
    return fDtRealBegin;
}

void TAbstractPlanElement::setDtRealBegin(const QDateTime &dt)
{
    if (dt.isValid()) fDtRealBegin = new QDateTime(dt);
}
//-----------------------------------------------------------------------------

QDateTime *TAbstractPlanElement::dtRealEnd() const
{
    return fDtRealEnd;
}

void TAbstractPlanElement::setDtRealEnd(const QDateTime &dt)
{
    if (dt.isValid()) fDtRealEnd = new QDateTime(dt);
}
//-----------------------------------------------------------------------------

QDateTime *TAbstractPlanElement::dtRealEndPrognos() const
{
    if (fDtRealBegin && !fDtRealEnd) return new QDateTime(fDtRealBegin->addSecs(fPlanPeriod * 3600));
    return NULL;
}
//-----------------------------------------------------------------------------

TStatusList &TAbstractPlanElement::statuses() const
{
    return const_cast<TStatusList&>(fStatuses);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setStatuses(const TStatusList &stss)
{
    fStatuses = stss;
}
//-----------------------------------------------------------------------------

bool TAbstractPlanElement::isSaved() const
{
    return fIsSaved;
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setSaved(bool sv)
{
    fIsSaved = sv;
}
//-----------------------------------------------------------------------------

bool TAbstractPlanElement::isVolatile() const
{
    return fIsVolatile;
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setVolatile(bool vt)
{
    fIsVolatile = vt;
}
//-----------------------------------------------------------------------------

QString &TAbstractPlanElement::descr() const
{
    return const_cast<QString&>(fDescr);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setDescr(const QString &dscr)
{
    fDescr = dscr;
}
//-----------------------------------------------------------------------------

int TAbstractPlanElement::unitId() const
{
    return fUnitId;
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setUnitId(int id)
{
    fUnitId = id;
}
//-----------------------------------------------------------------------------

int TAbstractPlanElement::carryOutPercent() const
{
    return fCarryOutPercent;
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setCarryOutPercent(int cop)
{
    fCarryOutPercent = cop;
}
//-----------------------------------------------------------------------------

QList<int> &TAbstractPlanElement::linkedElements() const
{
    return const_cast<QList<int> &>(fLinkedElements);
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::setLinkedElements(const QList<int> &elements)
{
    fLinkedElements = elements;
}
//-----------------------------------------------------------------------------

// Выполняется ли элемент в настоящий момент
bool TAbstractPlanElement::isCarryOutNow()
{
    return fDtRealBegin && !fDtRealEnd;
}
//-----------------------------------------------------------------------------

void TAbstractPlanElement::reflectAttachmentsToLw(const TAbstractAttachmentList &atchmts, QListWidget &lw)
{
    lw.clear();
    foreach (TAbstractAttachment *atchmt, atchmts)
    {
        QListWidgetItem *newItem(new QListWidgetItem);
        newItem->setText(atchmt->scrName());
        newItem->setData(Qt::UserRole, qVariantFromValue(TIdent(atchmt->id(), atchmt->num(), atchmt->name(), (int)atchmt->type())));
        newItem->setFlags(newItem->flags() | Qt::ItemIsTristate);
        newItem->setCheckState(Qt::Unchecked);
        lw.addItem(newItem);
    }
}
//-----------------------------------------------------------------------------

QString TAbstractPlanElement::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TAbstractPlanElement::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TAbstractPlanElement::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TAbstractPlanElement::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
