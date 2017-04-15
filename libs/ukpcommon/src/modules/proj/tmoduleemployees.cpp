#include <QStack>
#include <EM_AddressBook>
#include <EM_Oshs>
#include <defMacro>
#include <defPictures>
#include <common>
#include <convertEnums>
#include <TModuleEmployees>
#include <TModuleUnits>

TModuleEmployees::TModuleEmployees(TProjModuleType t, int id, int n, QString nm, TAbstractObject *parent) : TAbstractModule(t,id,n,nm,parent), fSelfEmployee(NULL)
{
    fPlEmployees.setAutoDelete(true);
}
//-----------------------------------------------------------------------------

TModuleEmployees::~TModuleEmployees()
{
    DELETE(fSelfEmployee);
}
//-----------------------------------------------------------------------------

bool TModuleEmployees::createSelfEmployee()
{
  int selfid(0);
#if defined(WORK_WITH_LOCAL_DATA)
  std::string filedata(common::readFile(PROJ->dataPath()+"init.json"));
    if (!filedata.empty())
    {
        selfid = common::value(filedata,"selfEmployee").toInt();
        if (!selfid) return false;
    }
#else
    foreach (QString str,PROJ->params())
        if (str.indexOf("selfUser")>-1) { selfid = str.section("=",1).toInt(); break; }
#endif
    try
    {
      EM_AddressBook &abook = EM_AddressBook::Instance();
      //QList<EM_User*> users = abook.getUsers();
      //foreach (EM_User *usr,users) PR2(4,"USER: %1, %2",usr->SUID(),common::value(usr->getDescr().toStdString(),"name").toString());
        if (EM_User *slfUser = abook.getUser(selfid))
        {
            DELETE(fSelfEmployee);
            fSelfEmployee = new TEmployee(NULL,slfUser->SUID(),0,common::value(slfUser->getDescr().toStdString(),"name").toString());
            fSelfEmployee->setScrName(fSelfEmployee->name());
        PR1(0,"SELF DL: %1",fSelfEmployee->scrName());
        }
        else { INSERT_ERROR(QString("Не найдено ДЛ с кодом %1").arg(selfid),false); return false; }
    }
    catch (CommonException::OpenDBException &e)
    {
        INSERT_ERROR(QString("EM_AddressBook::Instance(): %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
    }
    catch (CommonException::SQLException &e)
    {
        INSERT_ERROR(QString("EM_AddressBook::Instance(): %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
    }
    return true;
}
//-----------------------------------------------------------------------------

bool TModuleEmployees::init()
{
    fPlEmployees.clear();
  EM_OSHSDic *dic(new EM_OSHSDic());
  TEmployeeTypeList qryTypes;
    try
    {
      EM_OPERATION_RETURNED_STATUS resDb(dic->fromDB());
        if (resDb==opOK)
        {
          MODULE(Units);
            // 1. Создать начальников для своего подразделения, всех подразделений-начальников и подразделений уровня предприятия
            foreach (TUnit *un,modUnits->units())
            {
                if (un!=modUnits->selfUnit() && un->chiefUnit()!=modUnits->mainUnit() && !modUnits->selfUnit()->isTopUnit(un)) continue;
            PR1(0,"Add Unit: %1",un->name());
                try
                {
                    un->setChief(NULL);
                  TEmployeeTypeList actualTypes;
                    actualTypes<<eltChief<<eltSubstituent<<eltMajorEngineer; // в такой последовательности
                    foreach (TEmployeeType curTp,actualTypes)
                    {
                        qryTypes.clear();
                        qryTypes<<curTp;
                      QList<QSharedPointer<EM_UserContainer> > users = EM_OSHSDic::getListUserWithEsteblishedPost(un->id(),qryTypes,true);
                        if (users.count())
                        {
                          EM_UserContainer &usrContainer = *users.first();
                            if (const EM_User *us = usrContainer.getUser())
                            {
                              QList<EM_EsteblishedPost*> estblshs = usrContainer.getEsteblishedPost();
                                if (estblshs.count())
                                {
                                  EM_EsteblishedPost &estblsh = *estblshs.first();
                                  TEmployee *curEmpl(us->SUID()==fSelfEmployee->id() ? fSelfEmployee : NULL);
                                    if (!curEmpl) curEmpl = new TEmployee(un,us->SUID(),0,common::value(us->getDescr().toStdString(),"name").toString());
                                    curEmpl->setScrName(QString("%1 - %2").arg(curEmpl->name()).arg(estblsh.getName()));
                                    curEmpl->setRole(TEmployeeRole(estblsh.getGroup()));
                                    curEmpl->setEstablishedPost(estblsh.getType());
                                    // !!! TEmployeeCalendar *fCalendar - при создании общего календаря TModulePlans::fromDB("CompanyCalendar");
                                    un->setChief(curEmpl);
                                    un->insertEmployee(curEmpl);
                                    insertEmployee(curEmpl);
                                PR2(4,"CHEEF (%1): %2",convertEnums::enumToStr(curTp),curEmpl->scrName());
                                    break;
                                }
                            }
                        }
                    }
                }
                catch (CommonException::OpenDBException &e)
                {
                    INSERT_ERROR(QString("EM_OSHSDic::getListUserWithEsteblishedPost(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); delete dic; return false;
                }
                catch (CommonException::SQLException &e)
                {
                    INSERT_ERROR(QString("EM_OSHSDic::getListUserWithEsteblishedPost(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); delete dic; return false;
                }
            }
            // 2. Для своего подразделения и всех подчиненных создать всех ДЛ, кроме созданных на первом этапе
          QStack<TUnit*> unStack;
            qryTypes.clear();
            unStack.push(modUnits->selfUnit());
            while (!unStack.isEmpty())
            {
              TUnit &curUn = *unStack.pop();
            PR1(0,"Cur Unit: %1",curUn.name());
              QList<QSharedPointer<EM_UserContainer> > users = EM_OSHSDic::getListUserWithEsteblishedPost(curUn.id(),qryTypes,true);
                foreach (const QSharedPointer<EM_UserContainer> &usrContainer,users)
                    if (const EM_User *us = usrContainer.data()->getUser())
                    {
                        if (curUn.findEmployee(us->SUID())) continue;
                      QList<EM_EsteblishedPost*> estblshs = usrContainer.data()->getEsteblishedPost();
                        if (estblshs.count())
                        {
                          EM_EsteblishedPost &estblsh = *estblshs.first();
                          TEmployee *curEmpl(us->SUID()==fSelfEmployee->id() ? fSelfEmployee : NULL);
                            if (!curEmpl) curEmpl = new TEmployee(&curUn,us->SUID(),0,common::value(us->getDescr().toStdString(),"name").toString());
                            curEmpl->setScrName(QString("%1 - %2").arg(curEmpl->name()).arg(estblsh.getName()));
                            curEmpl->setRole(TEmployeeRole(estblsh.getGroup()));
                            if (curEmpl->role().type()==eltChief) curUn.setChief(curEmpl);
                            curEmpl->setEstablishedPost(estblsh.getType());
                            // !!! TEmployeeCalendar *fCalendar - при создании общего календаря TModulePlans::fromDB("CompanyCalendar");
                            curUn.insertEmployee(curEmpl);
                            insertEmployee(curEmpl);
                        PR3(4,"EMPL: %1 (%2 *%3*)",curEmpl->scrName(),us->getLogin(),us->getPassword());
                        }
                    }
                foreach (TUnit *subUn,curUn.subUnits()) unStack.push(subUn);
            }
        }
    }
    catch (CommonException::OpenDBException &e)
    {
        INSERT_ERROR(QString("EM_OSHSDic::fromDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); delete dic; return false;
    }
    catch (CommonException::SQLException &e)
    {
        INSERT_ERROR(QString("EM_OSHSDic::fromDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); delete dic; return false;
    }
    catch (CommonException::AllocateMemoryException &e)
    {
        INSERT_ERROR(QString("EM_OSHSDic::fromDB(). %1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); delete dic; return false;
    }
    delete dic;
    return true;
}
//-----------------------------------------------------------------------------

TEmployee *TModuleEmployees::selfEmployee() const
{
    return fSelfEmployee;
}
//-----------------------------------------------------------------------------

TEmployeeList &TModuleEmployees::emploees() const
{
    return const_cast<TEmployeeList&>(fPlEmployees);
}
//-----------------------------------------------------------------------------

void TModuleEmployees::insertEmployee(TEmployee *emp)
{
    if (emp==fSelfEmployee) return;
    if (emp && fPlEmployees.indexOf(emp)==-1) fPlEmployees.append(emp);
}
//-----------------------------------------------------------------------------

TEmployee *TModuleEmployees::findEmployee(int id)
{
    if (fSelfEmployee->id()==id) return fSelfEmployee;
    foreach (TEmployee *empl,fPlEmployees)
        if (empl->id()==id) return empl;
    return NULL;
}
//-----------------------------------------------------------------------------

TEmployee *TModuleEmployees::findEmployee(const QString &nm, bool onscrnm)
{
    if (onscrnm)
    {
        if (fSelfEmployee->scrName()==nm) return fSelfEmployee;
    }
    else if (fSelfEmployee->name()==nm) return fSelfEmployee;
    foreach (TEmployee *empl,fPlEmployees)
        if (onscrnm)
        {
            if (empl->scrName()==nm) return empl;
        }
        else if (empl->name()==nm) return empl;
    return NULL;
}
//-----------------------------------------------------------------------------

TEmployeeList *TModuleEmployees::findEmployees(const TEmployeeRoleList &rls)
{
  TEmployeeList *res(new TEmployeeList());
    res->setAutoDelete(false);
  bool isAppend(false);
    if (fSelfEmployee)
        foreach (TEmployeeRole rl,rls)
            if (fSelfEmployee->role().isMatch(rl)) { isAppend = true; break; }
        if (isAppend) res->append(fSelfEmployee);
    foreach (TEmployee *empl,fPlEmployees)
    {
        isAppend = false;
        foreach (TEmployeeRole rl,rls)
            if (empl->role().isMatch(rl)) { isAppend = true; break; }
        if (isAppend) res->append(empl);
    }
    if (!res->count()) DELETE(res);
    return res;
}
//-----------------------------------------------------------------------------

// Отобрать по TEmployeeRole.type() подходящих должностных лиц для подразделения с unitid
// !!! Если unitid<0 (задано не конкретное подраделение, а типовое),
//     то предварительно найти ВСЕ подразделения, подходящие под данный уровень
TEmployeeList *TModuleEmployees::findEmployees(int unitid, const TEmployeeRoleList &rls)
{
  TEmployeeList *res(new TEmployeeList());
    res->setAutoDelete(false);
  TUnitList givenUnits;
    givenUnits.setAutoDelete(false);
  MODULE(Units);
PR(0,"==============================================================================");
PR1(4,"unitid: %1",unitid);
    if (unitid<0) // типовое подразделение
    {
      TUnitLevel givenLevel((TUnitLevel)abs(unitid));
    PR1(4,"givenLevel: %1",(int)givenLevel);
        if (givenLevel==modUnits->selfUnit()->level()) // свое подразделение
        {
        PR1(8,"appendSelfUnit: %1",modUnits->selfUnit()->name());
            givenUnits.append(modUnits->selfUnit());
        }
        else if (givenLevel>modUnits->selfUnit()->level()) // подчиненные (n) подразделения
        {
            foreach (TUnit *subUnit,modUnits->selfUnit()->subUnits())
                if (subUnit->level()==givenLevel)
                {
                PR1(8,"appendSubUnit: %1",subUnit->name());
                    givenUnits.append(subUnit);
                }
        }
        else if (givenLevel<modUnits->selfUnit()->level()) // подразделение (1) начальников
        {
          TUnit *parentUnit(modUnits->selfUnit());
            while (parentUnit)
            {
                if (parentUnit->level()==givenLevel)
                {
                PR1(8,"appendComUnit: %1",parentUnit->name());
                    givenUnits.append(parentUnit);
                    break;
                }
                parentUnit = parentUnit->chiefUnit();
            }
        }
    }
    else if (TUnit *un = modUnits->findUnit(unitid)) // конкретное подраделение
    {
    PR1(8,"appendUnit: %1",un->scrName());
        givenUnits.append(un);
    }
    foreach (TUnit *givenUnit,givenUnits)
    {
      bool isAppend(false);
        if (fSelfEmployee->unit()==givenUnit)
        {
            if (fSelfEmployee)
                foreach (TEmployeeRole rl,rls)
                    if (fSelfEmployee->role().isMatch(rl)) { isAppend = true; break; }
                if (isAppend) res->append(fSelfEmployee);
        }
        foreach (TEmployee *empl,givenUnit->employees())
        {
            isAppend = false;
            foreach (TEmployeeRole rl,rls)
                if (empl->role().isMatch(rl)) { isAppend = true; break; }
            if (isAppend)
                if (res->indexOf(empl)==-1) res->append(empl);
        }
    }
    if (!res->count()) DELETE(res);
    return res;
}
//-----------------------------------------------------------------------------

// Отобрать ВСЕХ должностных лиц для подразделения с unitid
// !!! Если unitid<0 (задано не конкретное подраделение, а типовое),
//     то предварительно найти ВСЕ подразделения, подходящие под данный уровень
void TModuleEmployees::reflectEmployeesToCb(int unitid, QComboBox &cb)
{
    cb.clear();
  TUnitList givenUnits;
    givenUnits.setAutoDelete(false);
  MODULE(Units);
PR(0,"******************************************************************************");
PR1(4,"unitid: %1",unitid);
    if (unitid<0) // типовое подразделение
    {
      TUnitLevel givenLevel((TUnitLevel)abs(unitid));
    PR1(4,"givenLevel: %1",(int)givenLevel);
        if (givenLevel==modUnits->selfUnit()->level()) // свое подразделение
        {
        PR1(8,"appendSelfUnit: %1",modUnits->selfUnit()->name());
            givenUnits.append(modUnits->selfUnit());
        }
        else if (givenLevel>modUnits->selfUnit()->level()) // подчиненные (n) подразделения
        {
            foreach (TUnit *subUnit,modUnits->selfUnit()->subUnits())
                if (subUnit->level()==givenLevel)
                {
                PR1(8,"appendSubUnit: %1",subUnit->name());
                    givenUnits.append(subUnit);
                }
        }
        else if (givenLevel<modUnits->selfUnit()->level()) // подразделение (1) начальников
        {
          TUnit *parentUnit(modUnits->selfUnit());
            while (parentUnit)
            {
                if (parentUnit->level()==givenLevel)
                {
                PR1(8,"appendComUnit: %1",parentUnit->name());
                    givenUnits.append(parentUnit);
                    break;
                }
                parentUnit = parentUnit->chiefUnit();
            }
        }
    }
    else if (TUnit *un = modUnits->findUnit(unitid)) // конкретное подраделение
    {
    PR1(8,"appendUnit: %1",un->scrName());
        givenUnits.append(un);
    }
  TEmployeeList empls;
    empls.setAutoDelete(false);
    foreach (TUnit *givenUnit,givenUnits)
    {
        if (TEmployee *chf = givenUnit->chief()) empls.append(chf);
        foreach (TEmployee *empl,givenUnit->employees())
            if (empls.indexOf(empl)==-1) empls.append(empl);
    }
    foreach (TEmployee *empl,empls)
        cb.addItem(ICONPIX(empl->role().type()==eltChief ? PIX_ADDUSER : ""),empl->scrName(),QVariant(empl->id()));
}
//-----------------------------------------------------------------------------

void TModuleEmployees::reflectEmployeeTypesToCb(QComboBox &cb)
{
    cb.clear();
  QList<TEmployeeType> lst;
    lst<<eltNone<<eltChief<<eltSubstituent<<eltMajorEngineer<<eltLeadingEditor<<eltMajorEditor<<eltSeniorEditor<<eltTechEditor<<eltResearchEditor<<eltExpertEditor<<eltEditor<<eltOobgdOperator<<eltOperator;
    foreach (TEmployeeType el,lst) cb.addItem(convertEnums::enumToStr(el));
}
//-----------------------------------------------------------------------------

void TModuleEmployees::reflectEmployeesToLw(const TEmployeeList &empls, QListWidget &lw)
{
    lw.clear();
    foreach (TEmployee *empl,empls)
    {
      QListWidgetItem *newIt(new QListWidgetItem(empl->scrName()));
        newIt->setCheckState(Qt::Unchecked);
        lw.addItem(newIt);
    }
}
//-----------------------------------------------------------------------------

void TModuleEmployees::reset(bool thisonly)
{
    if (!thisonly) TAbstractModule::reset(true);
    fPlEmployees.clear();
}
//-----------------------------------------------------------------------------

QString TModuleEmployees::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TModuleEmployees::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TModuleEmployees::toDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------

bool TModuleEmployees::fromDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------
