#include <QStack>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <common>
#include <gen>
#include <defMacro>
#include <defPictures>
#include <convertEnums>
#include <TIdent>
#include <TModuleUnits>
#include <EM_Oshs>

TModuleUnits::TModuleUnits(TProjModuleType t, int id, int n, QString nm, TAbstractObject *parent) : TAbstractModule(t,id,n,nm,parent), fSelfUnit(NULL), fMainUnit(NULL)
{
    fUnits.setAutoDelete(true);
}
//-----------------------------------------------------------------------------

TModuleUnits::~TModuleUnits()
{

}
//-----------------------------------------------------------------------------

void TModuleUnits::reset(bool thisonly)
{
    if (!thisonly) TAbstractModule::reset(true);
    fSelfUnit = NULL;
    fMainUnit = NULL;
    fUnits.clear();
}
//-----------------------------------------------------------------------------

bool TModuleUnits::init()
{
    fSelfUnit = NULL;
    fMainUnit = NULL;
    fUnits.clear();
  EM_OSHSDic *dic(new EM_OSHSDic());
    try
    {
      EM_OPERATION_RETURNED_STATUS resDb(dic->fromDB());
        if (resDb==opOK)
        {
          int selfUnitId(0);
          MODULE(Employees);

            if (EM_OSHSItem *selfOshs = dic->getOshsItemByUser(modEmployees->selfEmployee()->id())) selfUnitId = ((AbstractEntity*)selfOshs)->SUID();// getID().toInt(); // throw(CommonException::OpenDBException,CommonException::SQLException);
            //if (selfUnitId) { INSERT_ERROR(QString("Не найдено подразделение по id ДЛ (%1)").arg(modEmployees->selfEmployee()->id()),true); delete dic; return false; }
            else { INSERT_ERROR(QString("Не найдено подразделение по id ДЛ (%1)").arg(modEmployees->selfEmployee()->id()),true); delete dic; return false; }

          QStack<EM_OSHSItem*> stack;
            if (EM_OSHSItem *rootIt = dic->getRoot())
            {
                stack.push(rootIt);
                while (!stack.isEmpty())
                {
                  EM_OSHSItem &curIt = *stack.pop();
                //PR3(curIt.getLevel()*4,"%1 (%2, level: %3)",curIt.getShortTitle(),curIt.getID(),curIt.getOSHSLevel());
                //update oshs.oshsitem SET level=3 where suid=147;
                  TUnit *newUn(new TUnit(curIt.getID(),curIt.getFullTitle(),curIt.getShortTitle()));
                    newUn->setLevel((TUnitLevel)(curIt.getOSHSLevel()+1));
                    newUn->setShtatEmployeeCount(curIt.getEemployeeCount());
                    if (newUn->id()==selfUnitId)
                    {
                        fSelfUnit = newUn;
                        modEmployees->selfEmployee()->setUnit(fSelfUnit);
                    }
                    fUnits.append(newUn);
                    if (EM_OSHSItem *prntIt = curIt.getParent())
                    {
                        if (TUnit *unChief = findUnit(prntIt->getID()))
                        {
                            newUn->setChiefUnit(unChief);
                            unChief->insertSubUnit(newUn);
                        }
                    }
                    else fMainUnit = newUn; // ЦКП
                    for (EM_OSHSItem::iterator iter=curIt.begin(); iter!=curIt.end(); ++iter) stack.push(*iter);
                }
                if (!fSelfUnit) { INSERT_ERROR("Не найдено собственное подразделение",true); delete dic; return false; }
                else PR1(0,"SELF UNIT: %1",fSelfUnit->scrName());
            }
            else { INSERT_ERROR("Не найдено подразделение уровня предприятия",true); delete dic; return false; }
        }
        else { INSERT_ERROR(QString("Ошибка чтения ОШС из БД №%1").arg(resDb),true); delete dic; return false; }
    }
    catch (CommonException::OpenDBException &e)
    {
        INSERT_ERROR(QString("%1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); delete dic; return false;
    }
    catch (CommonException::SQLException &e)
    {
        INSERT_ERROR(QString("%1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); delete dic; return false;
    }
    catch (CommonException::AllocateMemoryException &e)
    {
        INSERT_ERROR(QString("%1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); delete dic; return false;
    }
    delete dic;
    return true;
}
//-----------------------------------------------------------------------------

TUnit *TModuleUnits::selfUnit() const
{
    return fSelfUnit;
}
//-----------------------------------------------------------------------------

TUnit *TModuleUnits::mainUnit() const
{
    return fMainUnit;
}
//-----------------------------------------------------------------------------

TUnitList &TModuleUnits::units() const
{
    return const_cast<TUnitList&>(fUnits);
}
//-----------------------------------------------------------------------------

TUnit *TModuleUnits::findUnit(int id)
{
    foreach (TUnit *un,fUnits)
        if (un->id()==id) return un;
    return NULL;
}
//-----------------------------------------------------------------------------

TUnit *TModuleUnits::findUnit(QString nm, bool onscrnm)
{
    foreach (TUnit *un,fUnits)
        if (onscrnm)
        {
            if (un->scrName()==nm) return un;
        }
        else if (un->name()==nm) return un;
    return NULL;
}
//-----------------------------------------------------------------------------

void TModuleUnits::reflectToTree(const TUnitList &units, QTreeWidget &tw, bool withempl)
{
    tw.clear();
  TUnit *mainUnit(NULL);
    foreach (TUnit *un,units)
        if (!un->chiefUnit()) { mainUnit = un; break; }
    if (mainUnit)
    {
      QTreeWidgetItem *twiNew(new QTreeWidgetItem(&tw));
        twiNew->setText(0,mainUnit->name());
        if (mainUnit==fSelfUnit) twiNew->setIcon(0,ICONPIX(PIX_CHECKED));
        twiNew->setText(1,mainUnit->scrName());
        twiNew->setText(2,gen::intToStr(mainUnit->id()));
        twiNew->setData(0,Qt::UserRole,qVariantFromValue(TIdent(mainUnit->id(),0,mainUnit->scrName(),0)));
        twiNew->setFlags(twiNew->flags() | Qt::ItemIsTristate);
        twiNew->setCheckState(0,Qt::Unchecked);
        if (withempl)
        {
          MODULE(Employees);
          TEmployeeList localEmployees;
            localEmployees.setAutoDelete(false);
            if (mainUnit->chief()) localEmployees.append(mainUnit->chief());
            foreach (TEmployee *empl,mainUnit->employees())
                if (empl!=mainUnit->chief()) localEmployees.append(empl);
            foreach (TEmployee *empl,localEmployees)
            {
              QTreeWidgetItem *twiNewEmpl(new QTreeWidgetItem(twiNew));
                twiNewEmpl->setText(0,QString("%1, %2").arg(convertEnums::enumToStr(empl->role().type())).arg(empl->name()));
                twiNewEmpl->setIcon(0,ICONPIX(empl==modEmployees->selfEmployee() ? PIX_CHECKED : PIX_ADDUSER));
                twiNewEmpl->setText(1,empl->scrName());
                twiNewEmpl->setText(2,gen::intToStr(empl->id()));
                twiNewEmpl->setData(0,Qt::UserRole,qVariantFromValue(TIdent(empl->id(),0,empl->scrName(),0)));
                twiNewEmpl->setFlags(twiNew->flags() | Qt::ItemIsTristate);
                twiNewEmpl->setCheckState(0,Qt::Unchecked);
            }
        }
        mainUnit->reflectSubUnitsToTree(*twiNew,withempl);
    }
}
//-----------------------------------------------------------------------------

// Отобразить в списке подразделения указанных (lvls) уровней, подчиненные родителю с fromparentid (если withchilds==true, то отобразить вместе со своими подчиненными)
void TModuleUnits::reflectToCb(QComboBox &cb, int fromparentid, TUnitLevelList lvls, bool withempty, bool withparent, bool withchilds)
{
    cb.clear();
    if (withempty) cb.addItem("");
    if (!withchilds)
    {
    //PR1(0,"lvls.count(): %1",lvls.count());
    //foreach (TUnitLevel lvl,lvls) PR1(4,":%1",lvl);
        if (TUnit *prnUn = findUnit(fromparentid))
        {
            if (withparent) cb.addItem(prnUn->scrName());
            foreach (TUnit *subUn,prnUn->subUnits())
            {
            //PR2(0,"%1: %2",subUn->scrName(),(int)subUn->level());
                if (lvls.indexOf(subUn->level())>-1)
                {
                //PR1(4,"ADD %1",subUn->scrName());
                    cb.addItem(subUn->scrName());
                }
            }
        }
    }
    else
    {
        // ???
    }
}
//-----------------------------------------------------------------------------

// Отобразить в списке внешние подразделения указанных уровней (lvls) и шаблонные подразделения по линии иерархии
// PIX_CHECKED - свое подразделение
// PIX_LOW     - подчиненное подразделение
// PIX_HIGH    - подразделение-начальник
// PIX_LEFT    - внешнее подразделение
void TModuleUnits::reflectForTemplateToCb(QComboBox &cb, TUnitLevelList lvls)
{
  QList<TUnitLevel> levels = { ulvCompany, ulvDirection, ulvDepartment, ulvSection };
  int selfIndex(-1);
    foreach (TUnitLevel lvl,levels)
        if (selfUnit()->level()==lvl) // мой уровень
        {
            cb.addItem(ICONPIX(PIX_CHECKED),convertEnums::enumToStr(lvl),QVariant(-(int)lvl));
            selfIndex = cb.count()-1;
        }
        else cb.addItem(ICONPIX(selfIndex>-1 ? PIX_LOW : PIX_HIGH),convertEnums::enumToStr(lvl),QVariant(-(int)lvl));
    foreach (TUnit *subUn,fMainUnit->subUnits())
        if (lvls.indexOf(subUn->level())>-1)
            cb.addItem(ICONPIX(PIX_LEFT),subUn->scrName(),QVariant(subUn->id()));
    cb.setCurrentIndex(selfIndex);
}
//-----------------------------------------------------------------------------

// Найти в списке подразделение (конкретное или шаблонное) в списке по id (-lvl)
// Вернуть индекс и сделать текущим
int TModuleUnits::findUnitForTemplateInCb(QComboBox &cb, int id)
{
    for (int i=0; i<cb.count(); i++)
        if (cb.itemData(i).toInt()==id)
        {
            cb.setCurrentIndex(i);
            return i;
        }
    cb.setCurrentIndex(-1);
    return -1;
}
//-----------------------------------------------------------------------------

QString TModuleUnits::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TModuleUnits::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TModuleUnits::toDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------

bool TModuleUnits::fromDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------
