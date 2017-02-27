
//***************************************************************************//
//                                                                           //
//                    Реализация делегата TTableDelegate                     //
//                                                                           //
//***************************************************************************//

#include <QSpinBox>
#include <QLineEdit>
#include <defMacro>
#include <qtools>
#include <TTableDelegate>
#include <TTableWidgetItemData>

// Отрисовка элемента
void TTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (TTableWidgetItemData *twid = qtools::tableWidgetItemData(index.data()))
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect,option.palette.highlight());
            twid->paint(painter,option.rect,option.palette,true);
        }
        else twid->paint(painter,option.rect,option.palette,false);
        delete twid;
    }
    else QStyledItemDelegate::paint(painter,option,index);
}
//-----------------------------------------------------------------------------

// Создание редактора
QWidget *TTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (TTableWidgetItemData *twid = qtools::tableWidgetItemData(index.data()))
    {
      QWidget *editor(NULL);
        switch (twid->editorType())
        {
            case TTableWidgetItemData::etComboBox:
                if (TTableWidgetItemData::ComboBoxData *d = twid->comboBoxData())
                {
                    editor = new QComboBox(parent);
                    ((QComboBox*)editor)->insertItems(0,d->strings);
                    connect((QComboBox*)editor,SIGNAL(activated(int)),this,SLOT(commitAndCloseEditor()));
                }
                break;
            case TTableWidgetItemData::etSpinBox:
                if (TTableWidgetItemData::SpinBoxData *d = twid->spinBoxData())
                {
                    editor = new QSpinBox(parent);
                    ((QSpinBox*)editor)->setMinimum(d->minValue);
                    ((QSpinBox*)editor)->setMaximum(d->maxValue);
                    ((QSpinBox*)editor)->setSingleStep(d->step);
                    connect((QSpinBox*)editor,SIGNAL(editingFinished()),this,SLOT(commitAndCloseEditor()));
                }
                break;
            case TTableWidgetItemData::etDoubleSpinBox:
                if (TTableWidgetItemData::DoubleSpinBoxData *d = twid->doubleSpinBoxData())
                {
                    editor = new QDoubleSpinBox(parent);
                    ((QDoubleSpinBox*)editor)->setMinimum(d->minValue);
                    ((QDoubleSpinBox*)editor)->setMaximum(d->maxValue);
                    ((QDoubleSpinBox*)editor)->setSingleStep(d->step);
                    connect((QDoubleSpinBox*)editor,SIGNAL(editingFinished()),this,SLOT(commitAndCloseEditor()));
                }
                break;
            case TTableWidgetItemData::etDateTime:
                if (TTableWidgetItemData::DateTimeData *d = twid->dateTimeData())
                {
                    editor = new QDateTimeEdit(parent);
                    ((QDateTimeEdit*)editor)->setDisplayFormat(d->displayFormat());
                    ((QDateTimeEdit*)editor)->setCalendarPopup(d->isCalendar);
                    if (d->isMinDt) ((QDateTimeEdit*)editor)->setMinimumDateTime(d->minDt);
                    if (d->isMaxDt) ((QDateTimeEdit*)editor)->setMaximumDateTime(d->maxDt);
                    connect((QDateTimeEdit*)editor,SIGNAL(editingFinished()),this,SLOT(commitAndCloseEditor()));
                }
                break;
            default: ;
        }
        delete twid;
        if (editor) return editor;
        else return QStyledItemDelegate::createEditor(parent,option,index);
    }
    else return QStyledItemDelegate::createEditor(parent,option,index);
}
//-----------------------------------------------------------------------------

// Отображение данных TTableWidgetItemData в редакторе
void TTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (TTableWidgetItemData *twid = qtools::tableWidgetItemData(index.data()))
    {
      bool customEdit(true);
        switch (twid->editorType())
        {
            case TTableWidgetItemData::etComboBox:
                if (QComboBox *cbEditor = qobject_cast<QComboBox*>(editor)) cbEditor->setCurrentIndex(cbEditor->findText(twid->text()));
                else customEdit = false;
                break;
            case TTableWidgetItemData::etSpinBox:
                if (QSpinBox *cbEditor = qobject_cast<QSpinBox*>(editor)) cbEditor->setValue(twid->text().toInt());
                else customEdit = false;
                break;
            case TTableWidgetItemData::etDoubleSpinBox:
                if (QDoubleSpinBox *cbEditor = qobject_cast<QDoubleSpinBox*>(editor)) cbEditor->setValue(twid->text().toDouble());
                else customEdit = false;
                break;
            case TTableWidgetItemData::etDateTime:
                customEdit = false;
                if (QDateTimeEdit *dtEditor = qobject_cast<QDateTimeEdit*>(editor))
                    if (TTableWidgetItemData::DateTimeData *d = twid->dateTimeData())
                    {
                        dtEditor->setDateTime(d->dateTimeFromText(twid->text()));
                        customEdit = true;
                    }
                break;
            default: customEdit = false;
        }
        if (!customEdit)
        {
            if (QLineEdit *edEditor = qobject_cast<QLineEdit*>(editor)) edEditor->setText(twid->text());
            else QStyledItemDelegate::setEditorData(editor,index);
        }
        delete twid;
    }
    else QStyledItemDelegate::setEditorData(editor,index);
}
//-----------------------------------------------------------------------------

//#include <oz4/defMacro.h>

// Сохранение данных редактора в TTableWidgetItemData
void TTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (TTableWidgetItemData *twid = qtools::tableWidgetItemData(index.data()))
    {
      bool customEdit(true);
        switch (twid->editorType())
        {
            case TTableWidgetItemData::etComboBox:
                if (QComboBox *cbEditor = qobject_cast<QComboBox*>(editor))
                {
                    twid->setText(cbEditor->currentText());
                    model->setData(index,qVariantFromValue(*twid));
                }
                else customEdit = false;
                break;
            case TTableWidgetItemData::etSpinBox:
                if (QSpinBox *sbEditor = qobject_cast<QSpinBox*>(editor))
                {
                    twid->setText(QString("%1").arg(sbEditor->value()));
                    model->setData(index,qVariantFromValue(*twid));
                }
                else customEdit = false;
                break;
            case TTableWidgetItemData::etDoubleSpinBox:
                if (QDoubleSpinBox *sbEditor = qobject_cast<QDoubleSpinBox*>(editor))
                {
                    twid->setText(QString("%1").arg(sbEditor->value()));
                    model->setData(index,qVariantFromValue(*twid));
                }
                else customEdit = false;
                break;
            case TTableWidgetItemData::etDateTime:
                customEdit = false;
                if (QDateTimeEdit *dtEditor = qobject_cast<QDateTimeEdit*>(editor))
                    if (TTableWidgetItemData::DateTimeData *d = twid->dateTimeData())
                    {
                        twid->setText(dtEditor->dateTime().toString(d->displayFormat()));
                        customEdit = true;
                    }
                break;
            default: customEdit = false;
        }
        if (!customEdit)
        {
            if (QLineEdit *edEditor = qobject_cast<QLineEdit*>(editor))
            {
                twid->setText(edEditor->text());
                model->setData(index,qVariantFromValue(*twid));
            }
            else QStyledItemDelegate::setModelData(editor,model,index);
        }
    }
    else QStyledItemDelegate::setModelData(editor,model,index);
}
//-----------------------------------------------------------------------------

// Окончание редактирования
void TTableDelegate::commitAndCloseEditor()
{
  QWidget *editor = qobject_cast<QWidget*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
//-----------------------------------------------------------------------------
