
//***************************************************************************//
//                                                                           //
//                     Описание делегата TTableDelegate                      //
//                                                                           //
//***************************************************************************//

#ifndef TTABLEDELEGATEH
#define TTABLEDELEGATEH

#include <QStyledItemDelegate>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT TTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TTableDelegate(QWidget *parent=NULL) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const; - для собственного edit'a
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor();
};

#endif // TTABLEDELEGATEH
