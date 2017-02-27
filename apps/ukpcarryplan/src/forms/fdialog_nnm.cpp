#include <QMessageBox>
#include <TfrmDialog_NNm>

TfrmDialog_NNm::TfrmDialog_NNm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
    setupUi(this);
}
//-----------------------------------------------------------------------------

TfrmDialog_NNm::~TfrmDialog_NNm()
{
}
//-----------------------------------------------------------------------------

void TfrmDialog_NNm::prepare(const QString &title, int n, const QString &nm)
{
    setWindowTitle(title);
    fPropNum = n;
    fPropName = nm;
    sbNum->setValue(fPropNum);
    edName->setText(fPropName);
}
//-----------------------------------------------------------------------------

int TfrmDialog_NNm::propNum() const
{
    return fPropNum;
}
//-----------------------------------------------------------------------------

QString TfrmDialog_NNm::propName() const
{
    return fPropName;
}
//-----------------------------------------------------------------------------

void TfrmDialog_NNm::apply()
{
    fPropNum = sbNum->value();
    fPropName = edName->text();
    if (fPropName.isEmpty())
    {
      QMessageBox mb(QMessageBox::Question,tr("Внимание"),tr("Задано пустое наименование. Продолжить?"),QMessageBox::Yes | QMessageBox::No | QMessageBox::Default,this);
        mb.setButtonText(QMessageBox::Yes,tr("Да"));
        mb.setButtonText(QMessageBox::No,tr("Отмена"));
        if (mb.exec()==QMessageBox::No) return;
    }
    accept();
}
//-----------------------------------------------------------------------------
