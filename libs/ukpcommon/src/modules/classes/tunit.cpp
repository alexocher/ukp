#include <defMacro>
#include <defPictures>
#include <convertEnums>
#include <gen>
#include <TIdent>
#include <TUnit>
#include <TModuleUnits>

TUnit::TUnit(int id, QString nm, QString shrtnm, TUnit *chfun, int n, TAbstractObject *parent) : TAbstractObject(id, n, nm, parent), fLevel(ulvNone), fChief(NULL), fShtatEmployeeCount(0), fChiefUnit(chfun)
{
    fPlEmployees.setAutoDelete(false);
    fSubUnits.setAutoDelete(false);
    fScrName = shrtnm;
}
//-----------------------------------------------------------------------------

TUnit::TUnit(const TUnit &obj) : TAbstractObject(obj), fLevel(obj.fLevel), fChief(obj.fChief), fShtatEmployeeCount(obj.fShtatEmployeeCount), fChiefUnit(obj.fChiefUnit)
{
    fPlEmployees.setAutoDelete(false);
    foreach (TEmployee * empl, obj.fPlEmployees) fPlEmployees.append(empl);
    fSubUnits.setAutoDelete(false);
    foreach (TUnit * un, obj.fSubUnits) fSubUnits.append(un);
}
//-----------------------------------------------------------------------------

TUnit::~TUnit()
{
}
//-----------------------------------------------------------------------------

TUnit &TUnit::operator=(const TUnit &obj)
{
    if (&obj == this) return *this;
    TAbstractObject::operator=(obj);
    fPlEmployees.clear();
    foreach (TEmployee * empl, obj.fPlEmployees) fPlEmployees.append(empl);
    fChiefUnit = obj.fChiefUnit;
    fSubUnits.clear();
    foreach (TUnit * un, obj.fSubUnits) fSubUnits.append(un);
    return *this;
}
//-----------------------------------------------------------------------------

TUnitLevel TUnit::level() const
{
    return fLevel;
}
//-----------------------------------------------------------------------------

void TUnit::setLevel(TUnitLevel lvl)
{
    fLevel = lvl;
}
//-----------------------------------------------------------------------------

TEmployee *TUnit::chief() const
{
    return fChief;
}

void TUnit::setChief(TEmployee *chf)
{
    fChief = chf;
}
//-----------------------------------------------------------------------------

TEmployeeList &TUnit::employees() const
{
    return const_cast<TEmployeeList&>(fPlEmployees);
}
//-----------------------------------------------------------------------------

void TUnit::insertEmployee(TEmployee *empl)
{
    if (empl && fPlEmployees.indexOf(empl) == -1)
    {
        fPlEmployees.append(empl);
        empl->setUnit(this);
    }
}
//-----------------------------------------------------------------------------

void TUnit::removeEmployee(TEmployee *empl)
{
    if (empl) fPlEmployees.removeOne(empl);
}
//-----------------------------------------------------------------------------

void TUnit::clearEmployees()
{
    fPlEmployees.clear();
}
//-----------------------------------------------------------------------------

// ... поиск по id в текущем подразделени  (insubunits===false) или в текущем и во всех подчиненных)
TEmployee *TUnit::findEmployee(int id, bool withsubunits)
{
    foreach (TEmployee *empl, fPlEmployees)
        if (empl->id() == id) return empl;
    if (withsubunits)
        foreach (TUnit *un,fSubUnits)
            if (TEmployee *empl = un->findEmployee(id,true)) return empl;
    return NULL;
}
//-----------------------------------------------------------------------------

TEmployeeList *TUnit::findEmployees(const TEmployeeRoleList &rls)
{
    TEmployeeList *res(new TEmployeeList());
    res->setAutoDelete(false);
    foreach (TEmployee *empl, fPlEmployees)
    {
        bool isAppend(false);
        foreach (TEmployeeRole rl, rls)
            if (empl->role().isMatch(rl))
            {
                isAppend = true;
                break;
            }
        if (isAppend) res->append(empl);
    }
    if (!res->count()) DELETE(res);
    return res;
}
//-----------------------------------------------------------------------------

int TUnit::shtatEmployeeCount() const
{
    return fShtatEmployeeCount;
}
//-----------------------------------------------------------------------------

void TUnit::setShtatEmployeeCount(int cnt)
{
    fShtatEmployeeCount = cnt;
}
//-----------------------------------------------------------------------------

// Проверка, входит ли ДЛ в текущее подразделение или в какое-либо подчиненное
bool TUnit::isInternalEmployee(int id)
{
  MODULE(Employees);
    if (modEmployees->selfEmployee()->id()==id) return true;
  TEmployeeRole &rl = modEmployees->selfEmployee()->role();
    if (rl.type()==eltChief || rl.type()==eltSubstituent || rl.type()==eltMajorEngineer)
        if (findEmployee(id,true)) return true;
    return false;
}
//-----------------------------------------------------------------------------

TUnit *TUnit::chiefUnit() const
{
    return fChiefUnit;
}
//-----------------------------------------------------------------------------

void TUnit::setChiefUnit(TUnit *un)
{
    fChiefUnit = un;
}
//-----------------------------------------------------------------------------

TUnitList &TUnit::subUnits() const
{
    return const_cast<TUnitList&>(fSubUnits);
}
//-----------------------------------------------------------------------------

void TUnit::insertSubUnit(TUnit *un)
{
    if (fSubUnits.indexOf(un) == -1) fSubUnits.append(un);
}
//-----------------------------------------------------------------------------

void TUnit::removeSubUnit(TUnit *un)
{
    fSubUnits.removeOne(un);
}
//-----------------------------------------------------------------------------

void TUnit::clearSubUnits()
{
    fSubUnits.clear();
}
//-----------------------------------------------------------------------------

TUnit *TUnit::findSubUnit(int id)
{
    foreach (TUnit *un, fSubUnits)
        if (un->id() == id) return un;
    return NULL;
}
//-----------------------------------------------------------------------------

TUnit *TUnit::findSubUnit(QString nm, bool onscrnm)
{
    foreach (TUnit *un, fSubUnits)
        if (onscrnm)
        {
            if (un->scrName() == nm) return un;
        }
        else if (un->name() == nm) return un;
    return NULL;
}
//-----------------------------------------------------------------------------

bool TUnit::isTopUnit(TUnit *un)
{
    if (un == this) return false;
    TUnit *curUnit(this);
    while (curUnit)
    {
        if (curUnit->chiefUnit() == un) return true;
        curUnit = curUnit->chiefUnit();
    }
    return false;
}
//-----------------------------------------------------------------------------

void TUnit::reflectSubUnitsToTree(QTreeWidgetItem &twiself, bool withempl)
{
    //for (int i=twiself.childCount()-1; i>=0; i--) twiself.removeChild(twiself.child(i));
    MODULE(Units);
    foreach (TUnit *un, fSubUnits)
    {
        QTreeWidgetItem *twiNew(new QTreeWidgetItem(&twiself));
        twiNew->setText(0, un->name());
        if (un == modUnits->selfUnit()) twiNew->setIcon(0, ICONPIX(PIX_CHECKED));
        twiNew->setText(1, un->scrName());
        twiNew->setText(2, gen::intToStr(un->id()));
        twiNew->setData(0, Qt::UserRole, qVariantFromValue(TIdent(un->id(), 0, un->scrName(), 0)));
        twiNew->setFlags(twiNew->flags() | Qt::ItemIsTristate);
        twiNew->setCheckState(0, Qt::Unchecked);
        if (withempl)
        {
            MODULE(Employees);
            TEmployeeList localEmployees;
            localEmployees.setAutoDelete(false);
            foreach (TEmployee *empl, un->employees())
                if (empl != un->chief())
                {
                    localEmployees.append(empl);
                    empl->setSSort(QString("%1, %2").arg(convertEnums::enumToStr(empl->role().type())).arg(empl->name()));
                }
            qStableSort(localEmployees.begin(), localEmployees.end(), obj::FSortMinToMaxStr<TEmployee*>());
            if (un->chief())
            {
                un->chief()->setSSort(QString("%1, %2").arg(convertEnums::enumToStr(un->chief()->role().type())).arg(un->chief()->name()));
                localEmployees.prepend(un->chief());
            }
            foreach (TEmployee *empl, localEmployees)
            {
                QTreeWidgetItem *twiNewEmpl(new QTreeWidgetItem(twiNew));
                twiNewEmpl->setText(0, empl->sSort());
                twiNewEmpl->setIcon(0, ICONPIX(empl == modEmployees->selfEmployee() ? PIX_CHECKED : PIX_ADDUSER));
                twiNewEmpl->setText(1, empl->scrName());
                twiNewEmpl->setText(2, gen::intToStr(empl->id()));
                twiNewEmpl->setData(0, Qt::UserRole, qVariantFromValue(TIdent(empl->id(), 0, empl->scrName(), 0)));
                twiNewEmpl->setFlags(twiNew->flags() | Qt::ItemIsTristate);
                twiNewEmpl->setCheckState(0, Qt::Unchecked);
            }
        }
        un->reflectSubUnitsToTree(*twiNew, withempl);
    }
}
//-----------------------------------------------------------------------------

void TUnit::reset(bool thisonly)
{
    if (!thisonly) TAbstractObject::reset(true);
    fChiefUnit = NULL;
    fSubUnits.clear();
}
//-----------------------------------------------------------------------------

QString TUnit::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TUnit::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TUnit::toDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------

bool TUnit::fromDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------

