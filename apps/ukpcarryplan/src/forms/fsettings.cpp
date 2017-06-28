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

void TfrmSettings::resetSettings()
{
    if (QPushButton *btn = dynamic_cast<QPushButton *>(sender())) resetSettings(*btn);
}
//-----------------------------------------------------------------------------

void TfrmSettings::resetSettings(const QPushButton &btn)
{
    if (&btn==pbPen)
    {
    }
    else if (&btn==pbBrush)
    {
    }
}
//-----------------------------------------------------------------------------

void TfrmSettings::apply()
{
    accept();
}
//-----------------------------------------------------------------------------
