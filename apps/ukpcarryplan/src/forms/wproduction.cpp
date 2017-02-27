#include <defMacro>
#include <qtools>
#include <TModuleProductions>
#include <WProduction>

WProduction *wProduction(NULL);

WProduction::WProduction(QWidget *parent) : QFrame(parent)
{
    setupUi(this);

    twData->setColumnWidth(0,800); twData->setColumnWidth(1,50); twData->setColumnWidth(2,50); twData->setColumnWidth(2,200);
    if (QTreeWidgetItem *hdrIt = twData->headerItem())
    {
        hdrIt->setText(0,"Наименование"); hdrIt->setText(1,"Вх."); hdrIt->setText(2,"Вых."); hdrIt->setText(3,"Примечания");
        for (int i=0; i<4; i++) hdrIt->setTextAlignment(i,Qt::AlignCenter);
    }
    twData->setIconSize(QSize(48,48));
    twData->setUniformRowHeights(true);
}
//-----------------------------------------------------------------------------

WProduction::~WProduction()
{
}
//-----------------------------------------------------------------------------

void WProduction::fillForm()
{
  MODULE(Productions);
    modProductions->reflectToTree(modProductions->productions(),*twData);
    qtools::expand(*twData);
}
//-----------------------------------------------------------------------------
