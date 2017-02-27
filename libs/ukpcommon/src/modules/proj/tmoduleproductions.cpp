#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <TIdent>
#include <common>
#include <convertEnums>
#include <defMacro>
#include <defPictures>
#include <gen>
#include <TModuleProductions>
#include <EM_Production>
#include <EM_exceptions>

TModuleProductions::TModuleProductions(TProjModuleType t, int id, int n, QString nm, TAbstractObject *parent) : TAbstractModule(t,id,n,nm,parent)
{
    fProductions.setAutoDelete(true);
}
//-----------------------------------------------------------------------------

TModuleProductions::~TModuleProductions()
{

}
//-----------------------------------------------------------------------------

TProductionList &TModuleProductions::productions() const
{
    return const_cast<TProductionList&>(fProductions);
}
//-----------------------------------------------------------------------------

TProductionList *TModuleProductions::inProductions() const
{
  TProductionList *res(new TProductionList());
    res->setAutoDelete(false);
    foreach (TProduction *pr,fProductions)
        if (pr->isIn()) res->append(pr);
    if (!res->count()) DELETE(res);
    return res;
}
//-----------------------------------------------------------------------------

TProductionList *TModuleProductions::outProductions() const
{
  TProductionList *res(new TProductionList());
    res->setAutoDelete(false);
    foreach (TProduction *pr,fProductions)
        if (pr->isOut()) res->append(pr);
    if (!res->count()) DELETE(res);
    return res;
}
//-----------------------------------------------------------------------------

void TModuleProductions::reflectToTree(const TProductionList &prods, QTreeWidget &tw)
{
    tw.clear();
    foreach (TProduction *prod,prods)
    {
      QString arRubrs[4] = { convertEnums::enumToStr(prod->productionView()),convertEnums::enumToStr(prod->rubr2()),convertEnums::enumToStr(prod->rubr3()),convertEnums::enumToStr(prod->rubr4()) };
      QTreeWidgetItem *twiRubrs[4] = { NULL,NULL,NULL,NULL }, *twiLast(NULL), *twiWork(NULL);
        for (int j=0; j<4; j++)
            if (!arRubrs[j].isEmpty())
            {
              QList<QTreeWidgetItem*> its = tw.findItems(arRubrs[j],Qt::MatchCaseSensitive | Qt::MatchRecursive);
                if (its.count()) twiRubrs[j] = its.first();
            }
        for (int j=0; j<4; j++)
            if (!arRubrs[j].isEmpty())
            {
                if (!twiRubrs[j])
                {
                    twiRubrs[j] = j==0 ? new QTreeWidgetItem(&tw) : new QTreeWidgetItem(twiRubrs[j-1]);
                    twiRubrs[j]->setFont(0,tw.font());
                    twiRubrs[j]->setText(0,arRubrs[j]);
                    twiRubrs[j]->setFlags(twiRubrs[j]->flags() | Qt::ItemIsTristate); // Qt::ItemIsTristate || ItemIsUserCheckable
                    twiRubrs[j]->setCheckState(0,Qt::Unchecked);
                }
                twiLast = twiRubrs[j];
            }
        if (twiLast)
        {
            twiWork = new QTreeWidgetItem(twiLast);
            twiWork->setFont(0,tw.font());
            twiWork->setText(0,QString("%1. %2").arg(prod->id()).arg(prod->name()));
            if (prod->isIn()) twiWork->setIcon(1,ICONPIX(PIX_IN));
            if (prod->isOut()) twiWork->setIcon(2,ICONPIX(PIX_OUT));
            twiWork->setData(0,Qt::UserRole,qVariantFromValue(TIdent(prod->id(),0,prod->scrName(),0)));
            twiWork->setFlags(twiWork->flags() | Qt::ItemIsUserCheckable);
            twiWork->setCheckState(0,Qt::Unchecked);
        }
/*
      QTreeWidgetItem *twiNew(new QTreeWidgetItem(&tw));
        twiNew->setText(0,prod->name());
        if (prod->isIn()) twiNew->setIcon(1,QIcon(":/images/05.png"));
        if (prod->isOut()) twiNew->setIcon(2,QIcon(":/images/06.png"));
        twiNew->setData(0,Qt::UserRole,qVariantFromValue(TIdent(prod->id(),0,prod->scrName(),0)));
        twiNew->setFlags(twiNew->flags() | Qt::ItemIsUserCheckable);
        twiNew->setCheckState(0,Qt::Unchecked);
*/
    }
/*
      QTreeWidgetItem *twiRubrs[4] = { NULL,NULL,NULL,NULL }, *twiLast(NULL), *twiWork(NULL);
        for (int j=0; j<4; j++)
            if (!arRubrs[j].isEmpty())
            {
              QList<QTreeWidgetItem*> its = twData->findItems(arRubrs[j],Qt::MatchCaseSensitive | Qt::MatchRecursive);
                if (its.count()) twiRubrs[j] = its.first();
            }
        for (int j=0; j<4; j++)
            if (!arRubrs[j].isEmpty())
            {
                if (!twiRubrs[j])
                {
                    twiRubrs[j] = j==0 ? new QTreeWidgetItem(twData) : new QTreeWidgetItem(twiRubrs[j-1]);
                    twiRubrs[j]->setFont(0,font());
                    twiRubrs[j]->setText(0,arRubrs[j]);
                }
                twiLast = twiRubrs[j];
            }
        if (twiLast)
        {
            twiWork = new QTreeWidgetItem(twiLast);
            twiWork->setIcon(0,QIcon(isOut ? ":/images/docsReady.png" : ":/images/docsPattern.png"));
            twiWork->setFont(0,font());
            twiWork->setText(0,sType);
        }
*/
}
//-----------------------------------------------------------------------------

void TModuleProductions::reset(bool thisonly)
{
    if (!thisonly) TAbstractModule::reset(true);
    fProductions.clear();
}
//-----------------------------------------------------------------------------

bool TModuleProductions::init()
{
    fProductions.clear();
    try
    {
      EM_ProductionDic &prodDict = EM_ProductionDic::Instance();
        for (QList<EM_Production*>::const_iterator iter=prodDict.begin(); iter!=prodDict.end(); ++iter)
        {
          EM_Production &curProd = **iter;
          TProduction *newPrdct(new TProduction((int)curProd.getProductionType(),curProd.getFullName(),(TProductionType)curProd.getProductionType(),(TProductionView)curProd.getClView(),curProd.getShortName()));
            newPrdct->setRubr2((TProductionRubr2)curProd.getClRubr2());
            newPrdct->setRubr3((TProductionRubr3)curProd.getClRubr3());
            newPrdct->setRubr4((TProductionRubr4)curProd.getClRubr4());
        //PR3(4,"%1: in: %2, out: %3",curProd.getFullName(),curProd.isModeIn(),curProd.isModeOut());
            newPrdct->setIn(curProd.isModeIn());
            newPrdct->setOut(curProd.isModeOut());
            fProductions.append(newPrdct);
        }
    }
    catch (CommonException::OpenDBException &e)
    {
        INSERT_ERROR(QString("%1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
    }
    catch (CommonException::SQLException &e)
    {
        INSERT_ERROR(QString("%1 (code: %2)").arg(e.getMessage()).arg((int)e.getCode()),false); return false;
    }
    return fProductions.count();
}
//-----------------------------------------------------------------------------

QString TModuleProductions::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TModuleProductions::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TModuleProductions::toDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------

bool TModuleProductions::fromDB(QString )
{
    return true;
}
//-----------------------------------------------------------------------------
