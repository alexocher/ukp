#include <QMessageBox>
#include <TfrmSettings>

TfrmSettings::TfrmSettings(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
    setupUi(this);
}
//-----------------------------------------------------------------------------

TfrmSettings::~TfrmSettings()
{
}
//-----------------------------------------------------------------------------

void TfrmSettings::apply()
{
    accept();
}
//-----------------------------------------------------------------------------
