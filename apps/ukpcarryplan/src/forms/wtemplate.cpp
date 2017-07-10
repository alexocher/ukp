#include <math.h>
#include <QMessageBox>
#include <defMacro>
#include <defPictures>
#include <qtools>
#include <gen>
#include <convertEnums>
#include <TModulePlans>
#include <TModuleUnits>
#include <WTemplate>
#include <TfrmDialog_NNm>
#include <TfrmLinkedElements>

WTemplate *wTemplate(NULL);

namespace
{

}

WTemplate::WTemplate(QWidget *parent) : QFrame(parent)
{
    setupUi(this);

  MODULE(Plans);
    modPlans->reflectExternProceduresToCb(modPlans->externProcedureTemplates(),*cbExtProcedure,true);

    TModuleEmployees::reflectEmployeeTypesToCb(*cbEmployee);

  MODULE(Units);
  TUnitLevelList lvls;
    lvls<<ulvDepartment<<ulvSection;
    modUnits->reflectForTemplateToCb(*cbExtUnit,lvls);

    twCurrentTemplate->setColumnWidth(0,766); twCurrentTemplate->setColumnWidth(1,50); twCurrentTemplate->setColumnWidth(2,50); twCurrentTemplate->setColumnWidth(3,50);
    if (QTreeWidgetItem *hdrIt = twCurrentTemplate->headerItem())
    {
        hdrIt->setText(0,"Наименование"); hdrIt->setText(1,"ИД"); hdrIt->setText(2,"Рез"); hdrIt->setText(3,"ВПМ");
        for (int i=0; i<4; i++) hdrIt->setTextAlignment(i,Qt::AlignCenter);
    }
    twCurrentTemplate->setIconSize(QSize(36,36));
    twCurrentTemplate->setUniformRowHeights(true);
    twCurrentTemplate->setSortingEnabled(true);
    connect(twCurrentTemplate,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(selectPlanElement(QTreeWidgetItem*,QTreeWidgetItem*)));

    connect(pbAddTemplate,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbRenameTemplate,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbDeleteTemplate,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbRefreshTemplate,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbAddProcedure,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbAddWork,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbDeleteItem,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbExpand,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbCollapse,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbAddSources,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbClearSources,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbAddResults,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbClearResults,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbOptional,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbControl,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbLinkedElements,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbApplyCurrentItem,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbClearCurrentItem,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbDiagramma,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbSaveCurrentTemplate,SIGNAL(clicked()),this,SLOT(resetTemplates()));
    connect(pbSaveAllTemplates,SIGNAL(clicked()),this,SLOT(resetTemplates()));

    connect(cbTemplates,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(selectPattern(const QString&)));

    connect(sbTime_hours,SIGNAL(valueChanged(int)),this,SLOT(periodChanged(int)));
    connect(sbTime_days,SIGNAL(valueChanged(int)),this,SLOT(periodChanged(int)));

    connect(rbTime_hours,SIGNAL(toggled(bool)),this,SLOT(toggledTimeVariant(bool)));
    connect(rbTime_days,SIGNAL(toggled(bool)),this,SLOT(toggledTimeVariant(bool)));

    lblLinkedElements_val->setBackgroundRole(QPalette::Base);

    resetTemplates(*pbRefreshTemplate);

}
//-----------------------------------------------------------------------------

WTemplate::~WTemplate()
{
}
//-----------------------------------------------------------------------------

void WTemplate::resetTemplates()
{
    if (QPushButton *btn = dynamic_cast<QPushButton*>(sender())) resetTemplates(*btn);
}
//-----------------------------------------------------------------------------

void WTemplate::resetTemplates(const QPushButton &btn)
{
    if (&btn==pbAddTemplate)
    {
      TfrmDialog_NNm *dlg(new TfrmDialog_NNm());
        dlg->prepare("Добавить новый шаблон производственного плана",11,"Новый шаблон");
        if (dlg->exec()==QDialog::Accepted)
        {
          MODULE(Plans);
            if (modPlans->findPlanTemplate(dlg->propNum(),true))
            {
              QMessageBox mb(QMessageBox::Critical,tr("Ошибка"),tr("Шаблон с такими номером уже существует"),QMessageBox::Yes,this);
                mb.setButtonText(QMessageBox::Yes,tr("Принять"));
                mb.exec();
                delete dlg;
                return;
            }
          TCarryPlan *newPlan = new TCarryPlan(NULL,0,dlg->propNum(),dlg->propName());
            newPlan->setScrName(QString("%1.%2. %3").arg(newPlan->num()/10).arg(newPlan->num()%10).arg(newPlan->name()));
            modPlans->insertPlanTemplate(newPlan);
            modPlans->reflectTemplatesToCb(modPlans->planTemplates(),*cbTemplates);
            cbTemplates->setCurrentText(newPlan->scrName());
        }
        delete dlg;
    }
    else if (&btn==pbRenameTemplate)
    {
      MODULE(Plans);
        if (TCarryPlan *curTemplate = dynamic_cast<TCarryPlan*>(modPlans->findPlanTemplate(cbTemplates->currentText(),true)))
        {
          TfrmDialog_NNm *dlg(new TfrmDialog_NNm());
            dlg->prepare("Изменить шаблон производственного плана",curTemplate->num(),curTemplate->name());
            if (dlg->exec()==QDialog::Accepted)
            {
              MODULE(Plans);
                if (modPlans->findPlanTemplate(dlg->propNum(),true))
                {
                  QMessageBox mb(QMessageBox::Critical,tr("Ошибка"),tr("Шаблон с такими номером уже существует"),QMessageBox::Yes,this);
                    mb.setButtonText(QMessageBox::Yes,tr("Принять"));
                    mb.exec();
                    return;
                }
                curTemplate->setNum(dlg->propNum());
                curTemplate->setName(dlg->propName());
                curTemplate->setScrName(QString("%1.%2. %3").arg(dlg->propNum()/10).arg(dlg->propNum()%10).arg(dlg->propName()));
              QString scrnm(curTemplate->scrName());
                modPlans->reflectTemplatesToCb(modPlans->planTemplates(),*cbTemplates);
                cbTemplates->setCurrentText(scrnm);
            }
            delete dlg;
        }
    }
    else if (&btn==pbDeleteTemplate)
    {
      MODULE(Plans);
        if (TCarryPlan *curTemplate = dynamic_cast<TCarryPlan*>(modPlans->findPlanTemplate(cbTemplates->currentText(),true)))
        {
          QMessageBox mb(QMessageBox::Question,tr("Внимание"),tr("Текущий шаблон будет удален. Продолжить?"),QMessageBox::Yes | QMessageBox::No | QMessageBox::Default,this);
            mb.setButtonText(QMessageBox::Yes,tr("Да"));
            mb.setButtonText(QMessageBox::No,tr("Отмена"));
            if (mb.exec()==QMessageBox::No) return;
          MODULE(Plans);
            if (modPlans->findPlanTemplate(curTemplate->scrName(),true))
            {
                modPlans->removePlanTemplate(curTemplate);
                modPlans->reflectTemplatesToCb(modPlans->planTemplates(),*cbTemplates);
            }
        }
    }
    else if (&btn==pbRefreshTemplate)
    {
      bool wasNotSaved(false);
      MODULE(Plans);
        foreach (TAbstractPlan *pl,modPlans->planTemplates())
            if (!pl->isSaved()) { wasNotSaved = true; break; }
        if (wasNotSaved)
        {
          QMessageBox mb(QMessageBox::Question,tr("Внимание"),tr("Все несохраненные изменения будут удалены. Продолжить?"),QMessageBox::Yes | QMessageBox::No | QMessageBox::Default,this);
            mb.setButtonText(QMessageBox::Yes,tr("Да"));
            mb.setButtonText(QMessageBox::No,tr("Отмена"));
            if (mb.exec()==QMessageBox::No) return;
        }
        rbTime_hours->setChecked(true); rbTime_days->setChecked(false);
        modPlans->fromDB("Temlates");
        modPlans->reflectTemplatesToCb(modPlans->planTemplates(),*cbTemplates);
        selectPattern(cbTemplates->currentText());
    }
    else if (&btn==pbAddProcedure)
    {
      MODULE(Plans);
        if (TCarryPlan *curTemplate = dynamic_cast<TCarryPlan*>(modPlans->findPlanTemplate(cbTemplates->currentText(),true)))
        {
          TCarryProcedure *newPr(new TCarryProcedure(curTemplate,0,curTemplate->nextProcedureNum(),"Процедура"));
            curTemplate->insertProcedure(newPr);
            curTemplate->setSaved(false);
            cbTemplates->setItemIcon(cbTemplates->currentIndex(),ICONPIX(PIX_QUESTION));
            curTemplate->reflectToTree(*twCurrentTemplate,true);
            qtools::expand(*twCurrentTemplate);
          QList<QTreeWidgetItem*> its = twCurrentTemplate->findItems(newPr->scrName(),Qt::MatchCaseSensitive);
            if (its.count()) twCurrentTemplate->setCurrentItem(its.first());
        }
        else
        {
          QMessageBox mb(QMessageBox::Critical,tr("Ошибка"),tr("Не выбран текущий шаблон"),QMessageBox::Yes,this);
            mb.setButtonText(QMessageBox::Yes,tr("Принять"));
            mb.exec();
            return;
        }
    }
    else if (&btn==pbAddWork)
    {
      MODULE(Plans);
        if (TCarryPlan *curTemplate = dynamic_cast<TCarryPlan*>(modPlans->findPlanTemplate(cbTemplates->currentText(),true)))
        {
            if (QTreeWidgetItem *curIt = twCurrentTemplate->currentItem())
            {
              QTreeWidgetItem *prIt(NULL);
                if (TIdent *curIdnt = qtools::ident(*curIt))
                    prIt = (TPlanElementType)curIdnt->tag==petProcedure ? curIt : curIt->parent();

                if (!prIt) return;

                if (TIdent *idnt = qtools::ident(*prIt))
                {
                    if (TCarryProcedure *pr = curTemplate->findProcedure(idnt->num))
                    {
                      TCarryWork *newWrk(new TCarryWork(pr,emtNone,0,pr->nextWorkNum(),"Работа"));
                        pr->insertWork(newWrk);
                        pr->setSaved(false);
                        curTemplate->reflectToTree(*twCurrentTemplate,true);
                        qtools::expand(*twCurrentTemplate);
                      QList<QTreeWidgetItem*> its = twCurrentTemplate->findItems(pr->scrName(),Qt::MatchCaseSensitive);
                        if (its.count())
                        {
                          QTreeWidgetItem *itPr = its.first(), *itWrk(NULL);
                            for (int i=0; i<itPr->childCount(); i++)
                            {
                              QTreeWidgetItem *curIt = itPr->child(i);
                                if (curIt->text(0)==newWrk->scrName())
                                {
                                    itWrk = curIt;
                                    break;
                                }
                            }
                            twCurrentTemplate->setCurrentItem(itWrk ? itWrk : itPr);
                        }
                        curTemplate->setSaved(false);
                        cbTemplates->setItemIcon(cbTemplates->currentIndex(),ICONPIX(PIX_QUESTION));
                    }
                }
            }
            else
            {
              QMessageBox mb(QMessageBox::Critical,tr("Ошибка"),tr("Не выбран текущий узел на дереве"),QMessageBox::Yes,this);
                mb.setButtonText(QMessageBox::Yes,tr("Принять"));
                mb.exec();
                return;
            }
        }
        else
        {
          QMessageBox mb(QMessageBox::Critical,tr("Ошибка"),tr("Не выбран текущий шаблон"),QMessageBox::Yes,this);
            mb.setButtonText(QMessageBox::Yes,tr("Принять"));
            mb.exec();
            return;
        }
    }
    else if (&btn==pbDeleteItem)
    {
      MODULE(Plans);
        if (TCarryPlan *curTemplate = dynamic_cast<TCarryPlan*>(modPlans->findPlanTemplate(cbTemplates->currentText(),true)))
        {
          TCarryProcedureList delProcs;
            delProcs.setAutoDelete(false);
            for (int i=0; i<twCurrentTemplate->topLevelItemCount(); i++)
                if (QTreeWidgetItem *prIt = twCurrentTemplate->topLevelItem(i))
                    if (TIdent *prIdnt = qtools::ident(*prIt))
                        if (TCarryProcedure *curProc = curTemplate->findProcedure(prIdnt->num))
                        {
                            if (prIt->checkState(0)==Qt::Checked) delProcs.append(curProc); // полностью включенная процедура
                            else
                            {
                              TCarryWorkList delWorks;
                                delWorks.setAutoDelete(false);
                                for (int j=0; j<prIt->childCount(); j++)
                                    if (QTreeWidgetItem *wrkIt = prIt->child(j))
                                        if (wrkIt->checkState(0)==Qt::Checked)
                                            if (TIdent *wrkIdnt = qtools::ident(*wrkIt))
                                                if (TCarryWork *curWrk = curProc->findWork(wrkIdnt->num)) delWorks.append(curWrk);
                                foreach (TCarryWork *wrk,delWorks) curProc->removeWork(wrk);
                            }
                        }
            foreach (TCarryProcedure *proc,delProcs) curTemplate->removeProcedure(proc);
            selectPattern("");
        }
    }
    else if (&btn==pbExpand)
    {
        qtools::expand(*twCurrentTemplate);
    }
    else if (&btn==pbCollapse)
    {
        qtools::expand(*twCurrentTemplate,0);
    }
    else if (&btn==pbAddSources)
    {
    }
    else if (&btn==pbClearSources)
    {
    }
    else if (&btn==pbAddResults)
    {
    }
    else if (&btn==pbClearResults)
    {
    }
    else if (&btn==pbOptional)
    {
      bool isChck(pbOptional->isChecked());
        pbOptional->setChecked(isChck);
        pbOptional->setIcon(ICONPIX(!isChck ? "" : PIX_CHECKED));
    }
    else if (&btn==pbControl)
    {
      bool isChck(pbControl->isChecked());
        pbControl->setChecked(isChck);
        pbControl->setIcon(ICONPIX(!isChck ? "" : PIX_CHECKED));
    }
    else if (&btn==pbLinkedElements)
    {
      MODULE(Plans);
        if (TCarryPlan *curTemplate = dynamic_cast<TCarryPlan*>(modPlans->findPlanTemplate(cbTemplates->currentText(),true)))
        {
            if (QTreeWidgetItem *curIt = twCurrentTemplate->currentItem())
            {
                if (TIdent *curIdnt = qtools::ident(*curIt))
                {
                  MODULE(Plans);
                    if ((TPlanElementType)curIdnt->tag==petProcedure)
                    {
                        if (TCarryProcedure *pr = curTemplate->findProcedure(curIdnt->num))
                        {
                          TCarryProcedureList procs;
                            procs.setAutoDelete(false);
                            foreach (TCarryProcedure *curPr, curTemplate->procedures())
                                if (curPr != pr) procs.append(curPr);
                          TfrmLinkedElements dlg(procs,pr->linkedElements());
                            if (dlg.exec() == QDialog::Accepted)
                                selectPlanElement(twCurrentTemplate->currentItem(),NULL);
                        }
                    }
                }
            }
        }
    }
    else if (&btn==pbApplyCurrentItem)
    {
      MODULE(Plans);
        if (TCarryPlan *curTemplate = dynamic_cast<TCarryPlan*>(modPlans->findPlanTemplate(cbTemplates->currentText(),true)))
        {
            if (QTreeWidgetItem *curIt = twCurrentTemplate->currentItem())
            {
                if (TIdent *curIdnt = qtools::ident(*curIt))
                {
                  MODULE(Plans);
                    if ((TPlanElementType)curIdnt->tag==petProcedure)
                    {
                        if (TCarryProcedure *pr = curTemplate->findProcedure(curIdnt->num))
                        {
                            pr->setNum(sbNum->value()); // ??? проверить на совпадение номеров
                            pr->setName(edName->text());
                            pr->setScrName(QString("%1. %2").arg(pr->num(),2,10,QChar(' ')).arg(pr->name()));
                            curIt->setText(0,pr->scrName());
                          QString extNm("");
                            if (TExternProcedureTemplate *ep = modPlans->findExternProcedureTemplate(cbExtProcedure->currentText()))
                            {
                                pr->setExternProcedureNum(ep->num());
                                extNm = ep->scrName();
                            }
                            else pr->setExternProcedureNum(0);
                            curIt->setText(3,extNm);
                            pr->setSourcesTitle(edSources->text());
                            //lwSources
                            pr->setResultsTitle(edResults->text());
                            //lwResults
                          TEmployeeType emplTp;
                            convertEnums::strToEnum(cbEmployee->currentText(),emplTp);
                          TEmployeeRole emplRl(emplTp);
                            emplRl.setUnitId(cbExtUnit->currentData().toInt());
                            pr->setSingleTemplateRole(emplRl);
                            foreach (TCarryWork *wrk,pr->works()) // ??? записать зависимые параметры во все работы процедуры
                            {
                                wrk->setSingleTemplateRole(emplRl);
                            }
                            pr->setSaved(false);
                        }
                    }
                    else // petWork
                    {
                        if (QTreeWidgetItem *prIt = curIt->parent())
                            if (TIdent *prIdnt = qtools::ident(*prIt))
                                if (TCarryProcedure *pr = curTemplate->findProcedure(prIdnt->num))
                                    if (TCarryWork *wrk = pr->findWork(curIdnt->num))
                                    {
                                        wrk->setNum(sbNum->value()); // ??? проверить на совпадение номеров
                                        wrk->setName(edName->text());
                                        wrk->setScrName(QString("%1. %2").arg(wrk->num(),2,10,QChar(' ')).arg(wrk->name()));
                                        curIt->setText(0,wrk->scrName());
                                        wrk->setSourcesTitle(edSources->text());
                                        //lwSources
                                        wrk->setResultsTitle(edResults->text());
                                        //lwResults
                                        wrk->setTemplatePeriod(sbTime_hours->value());
                                        wrk->setOptional(pbOptional->isChecked());
                                        wrk->setControl(pbControl->isChecked());
                                        wrk->setSaved(false);
                                        pr->setSaved(false);

                                        // BUTCHER
                                        QString extNm("");
                                        if (TExternProcedureTemplate *ep = modPlans->findExternProcedureTemplate(cbExtProcedure->currentText())){
                                            wrk->setExternProcedureNum(ep->num());
                                            extNm = ep->scrName();
                                        }else{
                                            wrk->setExternProcedureNum(0);
                                        }
                                        curIt->setText(3,extNm);
                                    }
                    }
                    curTemplate->setSaved(false);
                    cbTemplates->setItemIcon(cbTemplates->currentIndex(),ICONPIX(PIX_QUESTION));
                }
            }
        }
    }
    else if (&btn==pbClearCurrentItem)
    {
      MODULE(Units);
        sbNum->setValue(1);
        edName->setText("");
        cbExtProcedure->setCurrentIndex(0);
        edSources->setText("");
        lwSources->clear();
        edResults->setText("");
        lwResults->clear();
        cbEmployee->setCurrentIndex(0);
        modUnits->findUnitForTemplateInCb(*cbExtUnit,-modUnits->selfUnit()->level());
        sbTime_hours->setValue(0); sbTime_days->setValue(0);
        pbOptional->setChecked(false);
        pbOptional->setIcon(ICONPIX(""));
        pbControl->setChecked(false);
        pbControl->setIcon(ICONPIX(""));
        lblLinkedElements_val->setText("");
    }
    else if (&btn==pbDiagramma)
    {
    }
    else if (&btn==pbSaveCurrentTemplate)
    {
      MODULE(Plans);
        if (TCarryPlan *curTemplate = dynamic_cast<TCarryPlan*>(modPlans->findPlanTemplate(cbTemplates->currentText(),true)))
        {
            if (!curTemplate->isSaved())
            {
                if (!curTemplate->toDB("template"))
                {
                  QMessageBox mb(QMessageBox::Question,tr("Ошибка"),tr("Текущий шаблон не сохранен"),QMessageBox::Ok,this);
                    mb.setButtonText(QMessageBox::Ok,tr("Принять"));
                    mb.exec();
                }
                else cbTemplates->setItemIcon(cbTemplates->currentIndex(),ICONPIX(""));
            }
        }
    }
    else if (&btn==pbSaveAllTemplates)
    {
      MODULE(Plans);
        QString curTemplateName(cbTemplates->currentText());
        foreach (TAbstractPlan *plan,modPlans->planTemplates())
            if (!plan->isSaved())
            {
                if (!plan->toDB("template"))
                {
                  QMessageBox mb(QMessageBox::Question,tr("Ошибка"),QString("Шаблон <%1> не сохранен").arg(plan->scrName()),QMessageBox::Ok,this);
                    mb.setButtonText(QMessageBox::Ok,tr("Принять"));
                    mb.exec();
                }
                else cbTemplates->setItemIcon(cbTemplates->currentIndex(),ICONPIX(""));
            }
        modPlans->reflectTemplatesToCb(modPlans->planTemplates(),*cbTemplates);
        cbTemplates->setCurrentText(curTemplateName);
    }
}
//-----------------------------------------------------------------------------

void WTemplate::selectPattern(const QString &) //scrnm
{
//PR1(0,"selectPattern(%1)",scrnm);
  MODULE(Plans);
    if (TCarryPlan *curTemplate = dynamic_cast<TCarryPlan*>(modPlans->findPlanTemplate(cbTemplates->currentText(),true)))
    {
        edTemplateTime->setText(gen::intToStr(curTemplate->planPeriod()));
        curTemplate->reflectToTree(*twCurrentTemplate,true);
        //twCurrentTemplate->model()->sort(0);
        twCurrentTemplate->sortItems(0,Qt::AscendingOrder);
        qtools::expand(*twCurrentTemplate);
        // ??? отобразить параметры первой процедуры
        if (twCurrentTemplate->topLevelItemCount()) twCurrentTemplate->setCurrentItem(twCurrentTemplate->topLevelItem(0));
    }
    else
    {
        twCurrentTemplate->clear();
        resetTemplates(*pbClearCurrentItem);
        setEnabledControls(false);
        edTemplateTime->setText("");
    }
}
//-----------------------------------------------------------------------------

void WTemplate::setEnabledControls(bool isenbl)
{
  QList<QWidget*> lstW;
    lstW<<lblNum<<sbNum<<lblName<<edName<<lblExtProcedure<<cbExtProcedure<<lblSources<<edSources<<lwSources<<pbAddSources<<pbClearSources<<lblResults<<edResults<<lwResults<<pbAddResults<<pbClearResults<<lblEmployee<<cbEmployee<<lblExtUnit<<cbExtUnit<<lblTime<<sbTime_hours<<sbTime_days<<rbTime_hours<<rbTime_days<<lblOptional<<pbOptional<<lblControl<<pbControl<<pbApplyCurrentItem<<pbClearCurrentItem;
    foreach (QWidget *w,lstW) w->setEnabled(isenbl);
}
//-----------------------------------------------------------------------------

void WTemplate::selectPlanElement(QTreeWidgetItem *curIt, QTreeWidgetItem*)
{
    if (!curIt) return;
  MODULE(Plans);
  TCarryPlan *curTemplate = dynamic_cast<TCarryPlan*>(modPlans->findPlanTemplate(cbTemplates->currentText(),true));

    if (!curTemplate) return;

    if (TIdent *idnt = qtools::ident(*curIt))
    {
        resetTemplates(*pbClearCurrentItem);
        setEnabledControls(true);
      bool isProcedure((TPlanElementType)idnt->tag==petProcedure);
        lblExtProcedure->setEnabled(isProcedure);
        //cbExtProcedure->setEnabled(isProcedure);
        cbExtProcedure->setEnabled(true); //BUTCHER!!!
        lblEmployee->setEnabled(isProcedure);
        cbEmployee->setEnabled(isProcedure);
        lblExtUnit->setEnabled(isProcedure);
        cbExtUnit->setEnabled(isProcedure);
        lblTime->setEnabled(!isProcedure);
        sbTime_hours->setEnabled(!isProcedure);
        sbTime_days->setEnabled(!isProcedure);
        rbTime_hours->setEnabled(!isProcedure);
        rbTime_days->setEnabled(!isProcedure);
        lblOptional->setEnabled(!isProcedure);
        pbOptional->setEnabled(!isProcedure);
        lblControl->setEnabled(!isProcedure);
        pbControl->setEnabled(!isProcedure);
        lblLinkedElements->setEnabled(isProcedure);
        lblLinkedElements_val->setEnabled(isProcedure);
        pbLinkedElements->setEnabled(isProcedure);
      TCarryProcedure *curProc(NULL);
      TCarryWork *curWork(NULL);
        if (isProcedure) curProc = curTemplate->findProcedure(idnt->num);
        else if (QTreeWidgetItem *prntIt = curIt->parent())
        {
            if (TIdent *prntIdnt = qtools::ident(*prntIt))
            {
                curProc = curTemplate->findProcedure(prntIdnt->num);
                if (curProc) curWork = curProc->findWork(idnt->num);
            }
        }
        if (isProcedure && curProc)
        {
            sbNum->setValue(curProc->num());
            edName->setText(curProc->name());
            if (curProc->isExtern())
            {
                if (TExternProcedureTemplate *extProc = modPlans->findExternProcedureTemplate(curProc->externProcedureNum()))
                    cbExtProcedure->setCurrentIndex(cbExtProcedure->findText(extProc->scrName()));
            }
            else cbExtProcedure->setCurrentIndex(0);
            edSources->setText(curProc->sourcesTitle());
            curProc->reflectAttachmentsToLw(curProc->sources(),*lwSources);
            edResults->setText(curProc->resultsTitle());
            curProc->reflectAttachmentsToLw(curProc->results(),*lwResults);
          TEmployeeRole rl = curProc->firstTemplateRole();
          MODULE(Units);
            if (rl.type()==eltNone)
            {
                cbEmployee->setCurrentIndex(0);
                modUnits->findUnitForTemplateInCb(*cbExtUnit,-modUnits->selfUnit()->level());
            }
            else
            {
                cbEmployee->setCurrentIndex(cbEmployee->findText(convertEnums::enumToStr(rl.type())));
                modUnits->findUnitForTemplateInCb(*cbExtUnit,rl.unitId());
            }
            QStringList slHint;
            foreach (int num, curProc->linkedElements())
                if (TCarryProcedure *pr = curTemplate->findProcedure(num))
                    slHint << pr->scrName();
            lblLinkedElements_val->setText(curProc->linkedElements().count() ? QString::number(curProc->linkedElements().count()) : "");
            lblLinkedElements_val->setToolTip(slHint.join("\n"));
        }
        else if (curWork)
        {
            sbNum->setValue(curWork->num());
            edName->setText(curWork->name());
            //cbExtProcedure->setCurrentIndex(0);
            //BUTCHER
            if (curWork->isExtern())
            {
                if (TExternProcedureTemplate *extProc = modPlans->findExternProcedureTemplate(curWork->externProcedureNum()))
                    cbExtProcedure->setCurrentIndex(cbExtProcedure->findText(extProc->scrName()));
            }
            else cbExtProcedure->setCurrentIndex(0);
            edSources->setText(curWork->sourcesTitle());
            curWork->reflectAttachmentsToLw(curWork->sources(),*lwSources);
            edResults->setText(curWork->resultsTitle());
            curWork->reflectAttachmentsToLw(curWork->results(),*lwResults);
          TEmployeeRole rl = curWork->firstTemplateRole();
          MODULE(Units);
            if (rl.type()==eltNone)
            {
                cbEmployee->setCurrentIndex(0);
                modUnits->findUnitForTemplateInCb(*cbExtUnit,-modUnits->selfUnit()->level());
            }
            else
            {
                cbEmployee->setCurrentIndex(cbEmployee->findText(convertEnums::enumToStr(rl.type())));
                modUnits->findUnitForTemplateInCb(*cbExtUnit,rl.unitId());
            }
            sbTime_hours->setValue(curWork->templatePeriod());
            rbTime_hours->setChecked(true); toggledTimeVariant(false);
            periodChanged(*sbTime_hours); // пересчитать часы в рабочие дни
            sbTime_days->setValue(curWork->templatePeriod());
            pbOptional->setChecked(curWork->isOptional());
            pbOptional->setIcon(ICONPIX(curWork->isOptional() ? PIX_CHECKED : ""));
            pbControl->setChecked(curWork->isControl());
            pbControl->setIcon(ICONPIX(curWork->isControl() ? PIX_CHECKED : ""));
        }
        else setEnabledControls(false);
    }
}

void WTemplate::periodChanged(int)
{
    if (QSpinBox *sb = dynamic_cast<QSpinBox*>(sender())) periodChanged(*sb);
}
//-----------------------------------------------------------------------------

void WTemplate::periodChanged(const QSpinBox &sb, int)
{
    if (&sb==sbTime_hours)
    {
        if (rbTime_hours->isChecked()) sbTime_days->setValue((int)ceil(sbTime_hours->value()/8.));
    }
    else if (&sb==sbTime_days)
    {
        if (rbTime_days->isChecked()) sbTime_hours->setValue(sbTime_days->value()*8);
    }
}
//-----------------------------------------------------------------------------

void WTemplate::toggledTimeVariant(bool)
{
    sbTime_hours->setEnabled(rbTime_hours->isChecked());
    sbTime_days->setEnabled(rbTime_days->isChecked());
}
//-----------------------------------------------------------------------------
