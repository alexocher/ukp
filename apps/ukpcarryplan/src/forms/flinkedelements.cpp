#include <QMessageBox>
#include <TfrmLinkedElements>

TfrmLinkedElements::TfrmLinkedElements(const TCarryProcedureList &procedures, QList<int> &linkedProcedures, QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent,f),
    fProcedures(procedures),
    fLinkedProcedures(linkedProcedures)
{
    setupUi(this);

    twProcedures->setColumnWidth(0, 580);
    twProcedures->setIconSize(QSize(36, 36));
    twProcedures->setUniformRowHeights(true);
    twProcedures->setSortingEnabled(false);
    twProcedures->setAlternatingRowColors(true);
    twProcedures->header()->hide();

    foreach (const TCarryProcedure *proc, procedures)
    {
        QTreeWidgetItem *twi(new QTreeWidgetItem(twProcedures));
        twi->setText(0, proc->scrName());
        twi->setData(0, Qt::UserRole, proc->num());
        twi->setCheckState(0, linkedProcedures.indexOf(proc->num())==-1 ? Qt::Unchecked : Qt::Checked);
    }
}
//-----------------------------------------------------------------------------

TfrmLinkedElements::~TfrmLinkedElements()
{
}
//-----------------------------------------------------------------------------

void TfrmLinkedElements::apply()
{
    fLinkedProcedures.clear();
    for (int i=0; i<twProcedures->topLevelItemCount(); i++)
        if (QTreeWidgetItem *twi = twProcedures->topLevelItem(i))
            if (twi->checkState(0) == Qt::Checked)
                fLinkedProcedures << twi->data(0,Qt::UserRole).toInt();
    accept();
}
//-----------------------------------------------------------------------------
