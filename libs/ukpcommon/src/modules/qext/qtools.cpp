
//***************************************************************************//
//                                                                           //
//                         Реализация фугкций qtools                         //
//                                                                           //
//***************************************************************************//

#include <QStack>
#include <qtools>
#include <defMacro>

//*****************************************************************************
// Функции работы с QTreeWidget
//*****************************************************************************

// Заполнение списка элементов элемента QTreeWidgetItem
int qtools::treeToList(const QTreeWidgetItem &item, QList<QTreeWidgetItem*> &items, QList<QTreeWidgetItem::ItemType> &types, bool onlyon, bool onlyvis)
{
  bool appendAll(!types.count());
    for (int i=0; i<item.childCount(); i++)
    {
      QTreeWidgetItem *curIt = item.child(i);
      bool appendIt(true);
        if (!appendAll && types.indexOf((QTreeWidgetItem::ItemType)curIt->type())==-1) appendIt = false;
        if (onlyon && curIt->checkState(0)==Qt::Unchecked) appendIt = false;
        if (onlyvis && curIt->isHidden()) appendIt = false;
        if (appendIt) items.append(curIt);
        treeToList(*curIt,items,types,onlyon,onlyvis);
    }
    return items.count();
}
//-----------------------------------------------------------------------------

// Получение списка всех элементов дерева QTreeWidget
int qtools::treeToList(const QTreeWidget &tree, QList<QTreeWidgetItem*> &items, bool onlyon, bool onlyvis)
{
    items.clear();
  QList<QTreeWidgetItem::ItemType> types;
    for (int i=0; i<tree.topLevelItemCount(); i++)
    {
      QTreeWidgetItem *curIt = tree.topLevelItem(i);
      bool appendIt(true);
        if (onlyon && curIt->checkState(0)==Qt::Unchecked) appendIt = false;
        if (onlyvis && curIt->isHidden()) appendIt = false;
        if (appendIt) items.append(curIt);
        treeToList(*curIt,items,types,onlyon,onlyvis);
    }
    return items.count();
}
//-----------------------------------------------------------------------------

// Получение списка элементов определенных типов дерева QTreeWidget
int qtools::treeToList(const QTreeWidget &tree, QList<QTreeWidgetItem*> &items, QList<QTreeWidgetItem::ItemType> &types, bool onlyon, bool onlyvis)
{
    items.clear();
    for (int i=0; i<tree.topLevelItemCount(); i++)
    {
      QTreeWidgetItem *curIt = tree.topLevelItem(i);
      bool appendIt(true);
        if (types.indexOf((QTreeWidgetItem::ItemType)curIt->type())==-1) appendIt = false;
        if (onlyon && curIt->checkState(0)==Qt::Unchecked) appendIt = false;
        if (onlyvis && curIt->isHidden()) appendIt = false;
        if (appendIt) items.append(curIt);
        treeToList(*curIt,items,types,onlyon,onlyvis);
    }
    return items.count();
}
//-----------------------------------------------------------------------------

int qtools::treeToList(const QTreeWidget &tree, QList<QTreeWidgetItem *> &items, Qt::CheckState st)
{
  QList<Qt::CheckState> states; states<<st;
    return treeToList(tree,items,states);
}
//-----------------------------------------------------------------------------

int qtools::treeToList(const QTreeWidget &tree, QList<QTreeWidgetItem *> &items, const QList<Qt::CheckState> &states)
{
    items.clear();
  QStack<QTreeWidgetItem*> itStack;
    for (int i=0; i<tree.topLevelItemCount(); i++)
    {
        itStack.push(tree.topLevelItem(i));
        while (!itStack.isEmpty())
        {
          QTreeWidgetItem *curIt = itStack.pop();
            for (int j=0; j<curIt->childCount(); j++) itStack.push(curIt->child(j));
            if (states.indexOf(curIt->checkState(0))>-1) items.append(curIt);
        }
    }
    return items.count();
}
//-----------------------------------------------------------------------------

int qtools::treeToList(const QTreeWidget &tree, QList<QTreeWidgetItem *> &items)
{
    items.clear();
  QStack<QTreeWidgetItem*> itStack;
    for (int i=0; i<tree.topLevelItemCount(); i++)
    {
        itStack.push(tree.topLevelItem(i));
        while (!itStack.isEmpty())
        {
          QTreeWidgetItem *curIt = itStack.pop();
            for (int j=0; j<curIt->childCount(); j++) itStack.push(curIt->child(j));
            items.append(curIt);
        }
    }
    return items.count();
}
//-----------------------------------------------------------------------------

// Удалить элемент дерева
// ( если removeEmtyParent==true, то удалить всех пустых родителей )
bool qtools::removeItem(QTreeWidgetItem *item, bool removeEmtyParent)
{
    if (!item) return false;
    if (QTreeWidgetItem *twiPrnt = item->parent())
    {
        twiPrnt->removeChild(item);
        DELETE(item);
        if (removeEmtyParent && twiPrnt->childCount()) removeItem(twiPrnt,true);
    }
    else DELETE(item); // элемент верхнего уровня удаляется оператором delete
    return true;
}
//-----------------------------------------------------------------------------

// Удалить элемент дерева
// ( если removeEmtyParent==true, то удалить всех пустых родителей )
bool qtools::removeItem(const QTreeWidget &tree, const TIdent &idnt, bool removeEmtyParent)
{
  QList<QTreeWidgetItem*> items;
    treeToList(tree,items);
    foreach (QTreeWidgetItem *curIt,items)
        if (curIt->data(0,Qt::UserRole).canConvert<TIdent>())
        {
          TIdent curIdent = curIt->data(0,Qt::UserRole).value<TIdent>();
            if (curIdent==idnt)
            {
                qtools::removeItem(curIt,removeEmtyParent);
                return true;
            }
        }
    return false;
}
//-----------------------------------------------------------------------------

// Поиск элемента
QTreeWidgetItem *qtools::findItem(const QTreeWidget &tree, const TIdent &idnt)
{
  QList<QTreeWidgetItem*> items;
    treeToList(tree,items);
    foreach (QTreeWidgetItem *curIt,items)
        if (curIt->data(0,Qt::UserRole).canConvert<TIdent>())
        {
          TIdent curIdent = curIt->data(0,Qt::UserRole).value<TIdent>();
            if (curIdent==idnt) return curIt;
        }
    return NULL;
}
//-----------------------------------------------------------------------------

// Поиск элемента
QTreeWidgetItem *qtools::findItem(const QTreeWidget &tree, const QString &txt, int column, bool isfirst)
{
  QList<QTreeWidgetItem*> items = tree.findItems(txt,Qt::MatchFixedString,column);
    if (items.count()) return isfirst ? items.first() : items.last();
    return NULL;
}
//-----------------------------------------------------------------------------

// Идентификатор
TIdent *qtools::ident(const QTreeWidgetItem &twi)
{
    if (twi.data(0,Qt::UserRole).canConvert<TIdent>()) return new TIdent(twi.data(0,Qt::UserRole).value<TIdent>());
    return NULL;
}
//-----------------------------------------------------------------------------

// Уровень, на котором находится элемент
int qtools::level(const QTreeWidgetItem &twi)
{
  int resLevel(0);
    if (QTreeWidgetItem *prntIt = twi.parent())
    {
        do
        {
            ++resLevel;
            prntIt = dynamic_cast<QTreeWidgetItem*>(prntIt->parent());
        }
        while (prntIt);
    }
    return resLevel;
}
//-----------------------------------------------------------------------------

// Развернуть дерево, чтобы были видны все элементы до depth включительно
// -1 - видны все (по умолчанию)
//  0 - скрыты все (виден только 0 уровень)
void qtools::expand(const QTreeWidget &tree, int depth)
{
  QList<QTreeWidgetItem*> items;
    treeToList(tree,items);
    if (depth<1)
        foreach (QTreeWidgetItem *curIt,items) curIt->setExpanded(depth);
    else
        foreach (QTreeWidgetItem *curIt,items) curIt->setExpanded(level(*curIt)<depth);
}
//-----------------------------------------------------------------------------

// Развернуть элемент
void qtools::expand(const QTreeWidgetItem &twi)
{
    for (int i=0; i<twi.childCount(); i++)
    {
      QTreeWidgetItem *curIt = twi.child(i);
        curIt->setExpanded(true);
        expand(*curIt);
    }
}
//-----------------------------------------------------------------------------

// Развернуть элемент и всех его детей (если closeother, то свернуть все остальные)
void qtools::expand(const QTreeWidget &tree, const QTreeWidgetItem &twi, bool collapseother)
{
  QList<QTreeWidgetItem*> items;
    treeToList(tree,items);
    if (collapseother) // свернуть все при необходимости
        foreach (QTreeWidgetItem *curIt,items) curIt->setExpanded(false);
    foreach (QTreeWidgetItem *curIt,items)
        if (&twi==curIt) { expand(twi); break; }
}
//-----------------------------------------------------------------------------

// Список наименований (text(0)) twi и всех родительских (twi->text(0) - последний)
QStringList qtools::hierarchyTexts(const QTreeWidgetItem &twi)
{
  QStringList res;
    res.append(twi.text(0));
  QTreeWidgetItem *prnt(twi.parent());
    while (prnt)
    {
        res.prepend(prnt->text(0));
        prnt = prnt->parent();
    }
    return res;
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// Функции работы с QTableWidget
//*****************************************************************************

// Получение значения из QVariant c учетом наследования
TTableWidgetItemData *qtools::tableWidgetItemData(const QVariant &var)
{
    if (var.canConvert<TTableWidgetItemData>()) return new TTableWidgetItemData(var.value<TTableWidgetItemData>());
    if (var.canConvert<TTableWidgetItemDataIdent>()) return new TTableWidgetItemDataIdent(var.value<TTableWidgetItemDataIdent>());
    return NULL;
}
//-----------------------------------------------------------------------------

TTableWidgetItemDataIdent *qtools::tableWidgetItemDataIdent(const QVariant &var)
{
    if (var.canConvert<TTableWidgetItemDataIdent>()) return new TTableWidgetItemDataIdent(var.value<TTableWidgetItemDataIdent>());
    return NULL;
}
//-----------------------------------------------------------------------------

// Данные ячейки
TTableWidgetItemData *qtools::itemData(const QTableWidget &tbl, int row, int col)
{

    if (QTableWidgetItem *twi = tbl.item(row,col)) return tableWidgetItemData(twi->data(Qt::UserRole));
    return NULL;
}
//-----------------------------------------------------------------------------

TTableWidgetItemData *qtools::verticalHeaderData(const QTableWidget &tbl, int row)
{
    if (QTableWidgetItem *twi = tbl.verticalHeaderItem(row))
        if (twi->data(Qt::UserRole).canConvert<TTableWidgetItemData>()) return new TTableWidgetItemData(twi->data(Qt::UserRole).value<TTableWidgetItemData>());
    return NULL;
}
//-----------------------------------------------------------------------------

TTableWidgetItemData *qtools::horizontalHeaderData(const QTableWidget &tbl, int col)
{
    if (QTableWidgetItem *twi = tbl.horizontalHeaderItem(col)) return tableWidgetItemData(twi->data(Qt::UserRole));
    return NULL;
}
//-----------------------------------------------------------------------------

// Идентификатор ячейки
TIdent *qtools::ident(const QTableWidget &tbl, int row, int col)
{
  TIdent *res(NULL);
    if (QTableWidgetItem *twi = tbl.item(row,col))
        if (TTableWidgetItemDataIdent *twid = tableWidgetItemDataIdent(twi->data(Qt::UserRole)))
        {
            res = new TIdent(twid->ident());
            delete twid;
        }
    return res;
}
//-----------------------------------------------------------------------------

TIdent *qtools::verticalHeaderIdent(const QTableWidget &tbl, int row)
{
  TIdent *res(NULL);
    if (QTableWidgetItem *twi = tbl.verticalHeaderItem(row))
        if (TTableWidgetItemDataIdent *twid = tableWidgetItemDataIdent(twi->data(Qt::UserRole)))
        {
            res = new TIdent(twid->ident());
            delete twid;
        }
    return res;
}
//-----------------------------------------------------------------------------

TIdent *qtools::horizontalHeaderIdent(const QTableWidget &tbl, int col)
{
  TIdent *res(NULL);
    if (QTableWidgetItem *twi = tbl.horizontalHeaderItem(col))
        if (TTableWidgetItemDataIdent *twid = tableWidgetItemDataIdent(twi->data(Qt::UserRole)))
        {
            res = new TIdent(twid->ident());
            delete twid;
        }
    return res;
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// Функции работы с QListWidget
//*****************************************************************************

// Идентификатор
TIdent *qtools::ident(const QListWidgetItem &lwi)
{
    if (lwi.data(Qt::UserRole).canConvert<TIdent>()) return new TIdent(lwi.data(Qt::UserRole).value<TIdent>());
    return NULL;
}
//-----------------------------------------------------------------------------
