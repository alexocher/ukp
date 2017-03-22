#include <defMacro>
#include <qtools>
#include <TModuleUnits>
#include <WOrgShtat>

WOrgShtat *wOrgShtat(NULL);

WOrgShtat::WOrgShtat(QWidget *parent) : QFrame(parent)
{
    setupUi(this);

    twData->setColumnWidth(0,750); twData->setColumnWidth(1,750); twData->setColumnWidth(2,100);
    if (QTreeWidgetItem *hdrIt = twData->headerItem())
    {
        hdrIt->setText(0,"Наименование"); hdrIt->setText(1,"Сокр. наим."); hdrIt->setText(2,"id");
        hdrIt->setTextAlignment(0,Qt::AlignCenter); hdrIt->setTextAlignment(1,Qt::AlignCenter);
        //for (int i=0; i<2; i++) hdrIt->setTextAlignment(i,Qt::AlignCenter);
    }
    twData->setIconSize(QSize(32,32));
    twData->setUniformRowHeights(true);
}
//-----------------------------------------------------------------------------

WOrgShtat::~WOrgShtat()
{
}
//-----------------------------------------------------------------------------

void WOrgShtat::fillForm()
{
  MODULE(Units);
    modUnits->reflectToTree(modUnits->units(),*twData,true);
    qtools::expand(*twData);
}
//-----------------------------------------------------------------------------
