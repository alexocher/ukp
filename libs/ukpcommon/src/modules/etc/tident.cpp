
//***************************************************************************//
//                                                                           //
//                            Реализация TIdent                              //
//                                                                           //
//***************************************************************************//

#include <TIdent>

// КОНСТРУКТОР
TIdent::TIdent(int i, int n, QString nm, int tg) : id(i), num(n), name(nm), tag(tg)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TIdent::TIdent(const TIdent &idnt) : id(idnt.id), num(idnt.num), name(idnt.name), tag(idnt.tag)
{
}
//-----------------------------------------------------------------------------

TIdent &TIdent::operator=(const TIdent &idnt)
{
    if (this==&idnt) return *this;
    id = idnt.id;
    num = idnt.num;
    name = idnt.name;
    tag = idnt.tag;
    return *this;
}
//-----------------------------------------------------------------------------

bool TIdent::operator==(const TIdent &idnt) const
{
    if (this==&idnt) return true;
    return id==idnt.id && num==idnt.num && name==idnt.name && tag==idnt.tag;
}
//-----------------------------------------------------------------------------

// Задание значений
void TIdent::setIdent(int i, int n, QString nm, int tg)
{
    id = i;
    num = n;
    name = nm;
    tag = tg;
}
//-----------------------------------------------------------------------------

void TIdent::setIdent(const TIdent &idnt)
{
    id = idnt.id;
    num = idnt.num;
    name = idnt.name;
    tag = idnt.tag;
}
//-----------------------------------------------------------------------------

// Преобразование в строку
QString TIdent::toStr()
{
    return QString("%1;%2;%3;%4").arg(id).arg(num).arg(name).arg(tag);
}
//-----------------------------------------------------------------------------

// Заполнение полей из строки
void TIdent::fromStr(QString s)
{
    id = s.section(";",0,0).toInt();
    num = s.section(";",1,1).toInt();
    name = s.section(";",2,2);
    tag = s.section(";",3,3).toInt();
}
//-----------------------------------------------------------------------------
