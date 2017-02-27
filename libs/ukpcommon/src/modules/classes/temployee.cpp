#include <defMacro>
#include <TEmployee>

TEmployeeRole::TEmployeeRole(TEmployeeType empt) : fType(empt), fIsExtern(false), fUnitId(0)
{
}
//-----------------------------------------------------------------------------

TEmployeeRole::TEmployeeRole(const TEmployeeRole &rl) : fType(rl.fType), fIsExtern(rl.fIsExtern), fUnitId(rl.fUnitId), fProductionTypes(rl.fProductionTypes)
{
}
//-----------------------------------------------------------------------------

TEmployeeRole::~TEmployeeRole()
{
}
//-----------------------------------------------------------------------------

TEmployeeRole &TEmployeeRole::operator=(const TEmployeeRole &rl)
{
    if (&rl==this) return *this;
    fType = rl.fType;
    fIsExtern = rl.fIsExtern;
    fUnitId = rl.fUnitId;
    fProductionTypes = rl.fProductionTypes;
    return *this;
}
//-----------------------------------------------------------------------------

TEmployeeType TEmployeeRole::type() const
{
    return fType;
}
//-----------------------------------------------------------------------------

void TEmployeeRole::setType(TEmployeeType t)
{
    fType = t;
}
//-----------------------------------------------------------------------------

bool TEmployeeRole::isExtern() const
{
    return fIsExtern;
}
//-----------------------------------------------------------------------------

void TEmployeeRole::setExtern(bool ext)
{
    fIsExtern = ext;
}
//-----------------------------------------------------------------------------

int TEmployeeRole::unitId() const
{
    return fUnitId;
}
//-----------------------------------------------------------------------------

void TEmployeeRole::setUnitId(int id)
{
    fUnitId = id;
}
//-----------------------------------------------------------------------------

TProductionTypeList &TEmployeeRole::productionTypes() const
{
    return const_cast<TProductionTypeList&>(fProductionTypes);
}
//-----------------------------------------------------------------------------

void TEmployeeRole::setProductionTypes(const TProductionTypeList &prtps)
{
    fProductionTypes = prtps;
}
//-----------------------------------------------------------------------------

// Проверка соответствия (полное (true) или только значимых полей (false) - по умолчанию)
// !!! каждое значение из this->fProductionTypes должно присутствовать в other.fProductionTypes
bool TEmployeeRole::isMatch(TEmployeeRole &other, bool isfull)
{
    if (isfull)
    {
        if (fType!=other.fType) return false;
        if (fIsExtern!=other.fIsExtern) return false;
        if (fUnitId!=other.fUnitId) return false;
        foreach (TProductionType prtp,fProductionTypes)
            if (other.fProductionTypes.indexOf(prtp)==-1) return false;
    }
    else
    {
        if (other.fType!=eltNone && fType!=other.fType) return false;
        if (fUnitId)
        {
            if (fUnitId!=other.fUnitId) return false;
            if (fIsExtern!=other.fIsExtern) return false;
        }
        if (other.fProductionTypes.count())
            foreach (TProductionType prtp,fProductionTypes)
                if (other.fProductionTypes.indexOf(prtp)==-1) return false;
    }
    return true;
}
//-----------------------------------------------------------------------------

//*****************************************************************************

TEmployee::TEmployee(TUnit *un, int id, int n, QString nm, TAbstractObject *parent) : TAbstractObject(id,n,nm,parent), fUnit(un), fEstablishedPost(eptNone), fCalendar(NULL)
{
}
//-----------------------------------------------------------------------------

TEmployee::TEmployee(const TEmployee &empl) : TAbstractObject(empl), fUnit(empl.fUnit), fRole(empl.fRole), fEstablishedPost(empl.fEstablishedPost), fCalendar(empl.fCalendar ? new TEmployeeCalendar(*empl.fCalendar) : NULL)
{

}
//-----------------------------------------------------------------------------

TEmployee::~TEmployee()
{
    DELETE(fCalendar);
}
//-----------------------------------------------------------------------------

TEmployee &TEmployee::operator=(const TEmployee &empl)
{
    if (&empl==this) return *this;
    fUnit = empl.fUnit;
    fRole = empl.fRole;
    fEstablishedPost = empl.fEstablishedPost;
    DELETE(fCalendar);
    if (empl.fCalendar) fCalendar = new TEmployeeCalendar(*empl.fCalendar);
    return *this;
}
//-----------------------------------------------------------------------------

void TEmployee::reset(bool thisonly)
{
    if (!thisonly) TAbstractObject::reset(true);
    fUnit = NULL;
    fRole = TEmployeeRole();
    fEstablishedPost = eptNone;
    DELETE(fCalendar);
}
//-----------------------------------------------------------------------------

TUnit *TEmployee::unit() const
{
    return fUnit;
}
//-----------------------------------------------------------------------------

void TEmployee::setUnit(TUnit *un)
{
    fUnit = un;
}
//-----------------------------------------------------------------------------

TEmployeeRole &TEmployee::role() const
{
    return const_cast<TEmployeeRole&>(fRole);
}
//-----------------------------------------------------------------------------

void TEmployee::setRole(const TEmployeeRole &rl)
{
    fRole = rl;
}
//-----------------------------------------------------------------------------

TEstablishedPostType TEmployee::establishedPost() const
{
    return fEstablishedPost;
}
//-----------------------------------------------------------------------------

void TEmployee::setEstablishedPost(TEstablishedPostType ept)
{
    fEstablishedPost = ept;
}
//-----------------------------------------------------------------------------

TEmployeeCalendar *TEmployee::calendar() const
{
    return fCalendar;
}
//-----------------------------------------------------------------------------

void TEmployee::setCalendar(TEmployeeCalendar *clndr)
{
    DELETE(fCalendar);
    fCalendar = clndr;
}
//-----------------------------------------------------------------------------

QString TEmployee::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TEmployee::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TEmployee::toDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------

bool TEmployee::fromDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------
