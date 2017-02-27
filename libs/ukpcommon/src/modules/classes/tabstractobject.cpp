
#include <TAbstractObject>

TAbstractObject::TAbstractObject(int id, int n, QString nm, TAbstractObject *parent) : QObject(parent), fId(id), fNum(n), fName(nm), fScrName(""), fDtCreation(QDateTime::currentDateTime()), fDtLastChanging(fDtCreation), fIsChecked(false)
{
}
//-----------------------------------------------------------------------------

TAbstractObject::TAbstractObject(const TAbstractObject &obj) : QObject(obj.parent()), fId(obj.fId), fNum(obj.fNum), fName(obj.fName), fScrName(obj.fScrName), fDtCreation(QDateTime::currentDateTime()), fDtLastChanging(fDtCreation), fIsChecked(obj.fIsChecked)
{
}
//-----------------------------------------------------------------------------

TAbstractObject::~TAbstractObject()
{
}
//-----------------------------------------------------------------------------


TAbstractObject &TAbstractObject::operator=(const TAbstractObject &obj)
{
    if (&obj==this) return *this;
    setParent(obj.parent());
    fId = obj.fId;
    fNum = obj.fNum;
    fName = obj.fName;
    fScrName = obj.fScrName;
    fDtCreation = obj.fDtCreation;
    fDtLastChanging = obj.fDtLastChanging;
    fIsChecked = obj.fIsChecked;
    return *this;
}
//-----------------------------------------------------------------------------

void TAbstractObject::reset(bool)
{
    setParent(NULL);
    fId = 0;
    fNum = 0;
    fName = "";
    fScrName = "";
    fDtCreation = QDateTime::currentDateTime();
    fDtLastChanging = fDtCreation;
    fIsChecked = false;
}
//-----------------------------------------------------------------------------

int TAbstractObject::id() const
{
    return fId;
}
//-----------------------------------------------------------------------------

void TAbstractObject::setId(int id)
{
    fId = id;
}
//-----------------------------------------------------------------------------

int TAbstractObject::num() const
{
    return fNum;
}
//-----------------------------------------------------------------------------

void TAbstractObject::setNum(int n)
{
    fNum = n;
}
//-----------------------------------------------------------------------------

QString &TAbstractObject::name() const
{
    return const_cast<QString&>(fName);
}
//-----------------------------------------------------------------------------

void TAbstractObject::setName(QString nm)
{
    fName = nm;
}
//-----------------------------------------------------------------------------

QString &TAbstractObject::scrName() const
{
    return const_cast<QString&>(fScrName);
}
//-----------------------------------------------------------------------------

void TAbstractObject::setScrName(QString nm)
{
    fScrName = nm;
}
//-----------------------------------------------------------------------------

QDateTime &TAbstractObject::dtCreation() const
{
    return const_cast<QDateTime&>(fDtCreation);
}
//-----------------------------------------------------------------------------

QDateTime &TAbstractObject::dtLastChanging() const
{
    return const_cast<QDateTime&>(fDtLastChanging);
}
//-----------------------------------------------------------------------------

int TAbstractObject::iSort() const
{
    return fISort;
}
//-----------------------------------------------------------------------------

void TAbstractObject::setISort(int is)
{
    fISort = is;
}
//-----------------------------------------------------------------------------

QString &TAbstractObject::sSort() const
{
    return const_cast<QString&>(fSSort);
}
//-----------------------------------------------------------------------------

void TAbstractObject::setSSort(QString ss)
{
    fSSort = ss;
}
//-----------------------------------------------------------------------------

bool TAbstractObject::isChecked() const
{
    return fIsChecked;
}
//-----------------------------------------------------------------------------

void TAbstractObject::setChecked(bool ch)
{
    fIsChecked = ch;
}
//-----------------------------------------------------------------------------

QString TAbstractObject::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TAbstractObject::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TAbstractObject::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TAbstractObject::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------


