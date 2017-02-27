
//***************************************************************************//
//                                                                           //
//                         Описание функций qtools                           //
//                                                                           //
//  ( Функции возвращают указатель на тип, начинающийся на T... через new )  //
//                                                                           //
//***************************************************************************//

#ifndef QTOOLSH
#define QTOOLSH

#include <QTreeWidget>
#include <QTableWidget>
#include <QListWidget>
#include <QComboBox>
#include <TIdent>
#include <TTableWidgetItemData>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT qtools
{
private:
    // Развернуть элемент
    static void expand(const QTreeWidgetItem &twi);

public:

    //*****************************************************************************
    // Функции работы с QTreeWidget
    //*****************************************************************************

    // Заполнение списка элементов элемента QTreeWidgetItem
    static int treeToList(const QTreeWidgetItem &item, QList<QTreeWidgetItem*> &items, QList<QTreeWidgetItem::ItemType> &types, bool onlyon, bool onlyvis);

    // Получение списка всех элементов дерева QTreeWidget
    // ВОЗВРАТ - количество элементов
    static int treeToList(const QTreeWidget &tree, QList<QTreeWidgetItem*> &items, bool onlyon, bool onlyvis);

    // Получение списка элементов определенных типов дерева QTreeWidget
    // ВОЗВРАТ - количество элементов
    static int treeToList(const QTreeWidget &tree, QList<QTreeWidgetItem*> &items, QList<QTreeWidgetItem::ItemType> &types, bool onlyon, bool onlyvis);

    // Получение списка элементов определенного состояния (checkState(0): Qt::Unchecked, Qt::PartiallyChecked, Qt::Checked)
    // ВОЗВРАТ - количество элементов
    static int treeToList(const QTreeWidget &tree, QList<QTreeWidgetItem*> &items, Qt::CheckState  st);

    // Получение списка элементов определенного состояния
    // ВОЗВРАТ - количество элементов
    static int treeToList(const QTreeWidget &tree, QList<QTreeWidgetItem*> &items, const QList<Qt::CheckState>  &states);

    // Получение списка элементов
    // ВОЗВРАТ - количество элементов
    static int treeToList(const QTreeWidget &tree, QList<QTreeWidgetItem*> &items);

    // Удалить элемент дерева
    // ( если removeEmtyParent==true, то удалить всех пустых родителей )
    static bool removeItem(QTreeWidgetItem *item, bool removeEmtyParent=false);

    // Удалить элемент дерева
    // ( если removeEmtyParent==true, то удалить всех пустых родителей )
    static bool removeItem(const QTreeWidget &tree, const TIdent &idnt, bool removeEmtyParent=false);

    // Поиск элемента
    static QTreeWidgetItem *findItem(const QTreeWidget &tree, const TIdent &idnt);

    // Поиск элемента
    static QTreeWidgetItem *findItem(const QTreeWidget &tree, const QString &txt, int column=0, bool isfirst=true);

    // Идентификатор
    static TIdent *ident(const QTreeWidgetItem &twi);

    // Уровень, на котором находится элемент
    static int level(const QTreeWidgetItem &twi);

    // Развернуть дерево, чтобы были видны все элементы до depth включительно
    // -1 - видны все (по умолчанию)
    //  0 - скрыты все (виден только 0 уровень)
    static void expand(const QTreeWidget &tree, int depth=-1);

    // Развернуть элемент и всех его детей (если closeother, то свернуть все остальные)
    static void expand(const QTreeWidget &tree, const QTreeWidgetItem &twi, bool collapseother=true);

    // Список наименований (text(0)) twi и всех родительских (twi->text(0) - последний)
    static QStringList hierarchyTexts(const QTreeWidgetItem &twi);

    //*****************************************************************************
    // Функции работы с QTableWidget
    //*****************************************************************************

    // Получение значения из QVariant c учетом наследования
    static TTableWidgetItemData *tableWidgetItemData(const QVariant &var);
    static TTableWidgetItemDataIdent *tableWidgetItemDataIdent(const QVariant &var);

    // Данные ячейки
    static TTableWidgetItemData *itemData(const QTableWidget &tbl, int row, int col);
    static TTableWidgetItemData *verticalHeaderData(const QTableWidget &tbl, int row);
    static TTableWidgetItemData *horizontalHeaderData(const QTableWidget &tbl, int col);

    // Идентификатор ячейки
    static TIdent *ident(const QTableWidget &tbl, int row, int col);
    static TIdent *verticalHeaderIdent(const QTableWidget &tbl, int row);
    static TIdent *horizontalHeaderIdent(const QTableWidget &tbl, int col);

    //*****************************************************************************
    // Функции работы с QListWidget
    //*****************************************************************************

    // Идентификатор
    static TIdent *ident(const QListWidgetItem &lwi);

    //*****************************************************************************
    // Функции работы с QComboBox
    //*****************************************************************************

    // Идентификатор
    static TIdent *ident(const QComboBox &cb, int ind);

};

#endif // QTOOLSH
