#include <QMessageBox>
#include <QColorDialog>
#include <QSettings>
#include <defMacro>
#include <TAppCarryPlan>
#include <TfrmSettings>

TfrmSettings::TfrmSettings(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
    setupUi(this);

    connect(pbPen, SIGNAL(clicked()), this, SLOT(resetSettings()));
    connect(pbBrush, SIGNAL(clicked()), this, SLOT(resetSettings()));

}
//-----------------------------------------------------------------------------

TfrmSettings::~TfrmSettings()
{
}
//-----------------------------------------------------------------------------

void TfrmSettings::prepare()
{
    DiagrammSettings &ds = PROJCARRYPLAN->diagrammSettings();

    sbHeaderHeight->setValue(ds.headerHeight());
    sbColumnWidth->setValue(ds.columnWidth());
    sbRowHeight->setValue(ds.rowHeight());

    QPalette pal = palette();
    pal.setColor(QPalette::WindowText,ds.gridPen());
    pal.setColor(QPalette::Window,ds.gridBrush());
    lblGrid->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,ds.weekendPen());
    pal.setColor(QPalette::Window,ds.weekendBrush());
    lblWeekend->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,ds.planItemsTaskPen());
    pal.setColor(QPalette::Window,ds.planItemsTaskBrash());
    lblPlanTask->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,ds.planItemsPlanPen());
    pal.setColor(QPalette::Window,ds.planItemsPlanBrash());
    lblPlanPlan->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,ds.planItemsProcedurePen());
    pal.setColor(QPalette::Window,ds.planItemsProcedureBrash());
    lblPlanProcedure->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,ds.planItemsWorkPen());
    pal.setColor(QPalette::Window,ds.planItemsWorkBrash());
    lblPlanWork->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,ds.realItemsTaskPen());
    pal.setColor(QPalette::Window,ds.realItemsTaskBrash());
    lblRealTask->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,ds.realItemsPlanPen());
    pal.setColor(QPalette::Window,ds.realItemsPlanBrash());
    lblRealPlan->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,ds.realItemsProcedurePen());
    pal.setColor(QPalette::Window,ds.realItemsProcedureBrash());
    lblRealProcedure->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,ds.realItemsBegWorkPen());
    pal.setColor(QPalette::Window,ds.realItemsBegWorkBrash());
    lblRealWorkBegin->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,ds.realItemsEndWorkPen());
    pal.setColor(QPalette::Window,ds.realItemsEndWorkBrash());
    lblRealWorkEnd->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,ds.realItemsGetWorkPen());
    pal.setColor(QPalette::Window,ds.realItemsGetWorkBrash());
    lblRealWorkGet->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,ds.realItemsProblemWorkPen());
    pal.setColor(QPalette::Window,ds.realItemsProblemWorkBrash());
    lblRealWorkProblem->setPalette(pal);
}
//-----------------------------------------------------------------------------

void TfrmSettings::resetSettings()
{
    if (QPushButton *btn = dynamic_cast<QPushButton *>(sender())) resetSettings(*btn);
}
//-----------------------------------------------------------------------------

void TfrmSettings::resetSettings(const QPushButton &btn)
{
    if (&btn==pbPen || &btn==pbBrush)
    {
        DiagrammSettings &ds = PROJCARRYPLAN->diagrammSettings();

        bool isPen(&btn==pbPen);

        QColor color;

        if (rbGrid->isChecked())
            color = isPen ? ds.gridPen() : ds.gridBrush();
        else if (rbWeekend->isChecked())
            color = isPen ? ds.weekendPen() : ds.weekendBrush();
        else if (rbPlanTask->isChecked())
            color = isPen ? ds.planItemsTaskPen() : ds.planItemsTaskBrash();
        else if (rbPlanPlan->isChecked())
            color = isPen ? ds.planItemsPlanPen() : ds.planItemsPlanBrash();
        else if (rbPlanProcedure->isChecked())
            color = isPen ? ds.planItemsProcedurePen() : ds.planItemsProcedureBrash();
        else if (rbPlanWork->isChecked())
            color = isPen ? ds.planItemsWorkPen() : ds.planItemsWorkBrash();
        else if (rbRealTask->isChecked())
            color = isPen ? ds.realItemsTaskPen() : ds.realItemsTaskBrash();
        else if (rbRealPlan->isChecked())
            color = isPen ? ds.realItemsPlanPen() : ds.realItemsPlanBrash();
        else if (rbRealProcedure->isChecked())
            color = isPen ? ds.realItemsProcedurePen() : ds.realItemsProcedureBrash();
        else if (rbRealWorkBegin->isChecked())
            color = isPen ? ds.realItemsBegWorkPen() : ds.realItemsBegWorkBrash();
        else if (rbRealWorkEnd->isChecked())
            color = isPen ? ds.realItemsEndWorkPen() : ds.realItemsEndWorkBrash();
        else if (rbRealWorkGet->isChecked())
            color = isPen ? ds.realItemsGetWorkPen() : ds.realItemsGetWorkBrash();
        else if (rbRealWorkProblem->isChecked())
            color = isPen ? ds.realItemsProblemWorkPen() : ds.realItemsProblemWorkBrash();

        color = QColorDialog::getColor(color, this, "Выбор цвета", QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);

        if (color.isValid())
        {
            if (rbGrid->isChecked())
            {
                if (isPen) ds.setGridPen(color);
                else ds.setGridBrush(color);
            }
            else if (rbWeekend->isChecked())
            {
                if (isPen) ds.setWeekendPen(color);
                else ds.setWeekendBrush(color);
            }
            else if (rbPlanTask->isChecked())
            {
                if (isPen) ds.setPlanItemsTaskPen(color);
                else ds.setPlanItemsTaskBrash(color);
            }
            else if (rbPlanPlan->isChecked())
            {
                if (isPen) ds.setPlanItemsPlanPen(color);
                else ds.setPlanItemsPlanBrash(color);
            }
            else if (rbPlanProcedure->isChecked())
            {
                if (isPen) ds.setPlanItemsProcedurePen(color);
                else ds.setPlanItemsProcedureBrash(color);
            }
            else if (rbPlanWork->isChecked())
            {
                if (isPen) ds.setPlanItemsWorkPen(color);
                else ds.setPlanItemsWorkBrash(color);
            }
            else if (rbRealTask->isChecked())
            {
                if (isPen) ds.setRealItemsTaskPen(color);
                else ds.setRealItemsTaskBrash(color);
            }
            else if (rbRealPlan->isChecked())
            {
                if (isPen) ds.setRealItemsPlanPen(color);
                else ds.setRealItemsPlanBrash(color);
            }
            else if (rbRealProcedure->isChecked())
            {
                if (isPen) ds.setRealItemsProcedurePen(color);
                else ds.setRealItemsProcedureBrash(color);
            }
            else if (rbRealWorkBegin->isChecked())
            {
                if (isPen) ds.setRealItemsBegWorkPen(color);
                else ds.setRealItemsBegWorkBrash(color);
            }
            else if (rbRealWorkEnd->isChecked())
            {
                if (isPen) ds.setRealItemsEndWorkPen(color);
                else ds.setRealItemsEndWorkBrash(color);
            }
            else if (rbRealWorkGet->isChecked())
            {
                if (isPen) ds.setRealItemsGetWorkPen(color);
                else ds.setRealItemsGetWorkBrash(color);
            }
            else if (rbRealWorkProblem->isChecked())
            {
                if (isPen) ds.setRealItemsProblemWorkPen(color);
                else ds.setRealItemsProblemWorkBrash(color);
            }

            prepare();
        }
    }
}
//-----------------------------------------------------------------------------

void TfrmSettings::apply()
{
    QSettings settings("modern", "ukpcarryplan");
    settings.beginGroup("diagramm");

    DiagrammSettings &ds = PROJCARRYPLAN->diagrammSettings();

    QVariant value;
    QColor color;

    value = sbHeaderHeight->value();
    settings.setValue("headerHeight", value);

    value = sbColumnWidth->value();
    settings.setValue("columnWidth", value);

    value = sbRowHeight->value();
    settings.setValue("rowHeight", value);

    color = ds.gridPen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("gridPen", value);
    color = ds.gridBrush();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("gridBrush", value);

    color = ds.weekendPen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("weekendPen", value);
    color = ds.weekendBrush();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("weekendBrush", value);

    color = ds.planItemsTaskPen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("planItemsTaskPen", value);
    color = ds.planItemsTaskBrash();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("planItemsTaskBrash", value);

    color = ds.planItemsPlanPen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("planItemsPlanPen", value);
    color = ds.planItemsPlanBrash();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("planItemsPlanBrash", value);

    color = ds.planItemsProcedurePen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("planItemsProcedurePen", value);
    color = ds.planItemsProcedureBrash();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("planItemsProcedureBrash", value);

    color = ds.planItemsWorkPen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("planItemsWorkPen", value);
    color = ds.planItemsWorkBrash();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("planItemsWorkBrash", value);

    color = ds.realItemsTaskPen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsTaskPen", value);
    color = ds.realItemsTaskBrash();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsTaskBrash", value);

    color = ds.realItemsPlanPen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsPlanPen", value);
    color = ds.realItemsPlanBrash();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsPlanBrash", value);

    color = ds.realItemsProcedurePen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsProcedurePen", value);
    color = ds.realItemsProcedureBrash();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsProcedureBrash", value);

    color = ds.realItemsBegWorkPen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsBegWorkPen", value);
    color = ds.realItemsBegWorkBrash();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsBegWorkBrash", value);

    color = ds.realItemsEndWorkPen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsEndWorkPen", value);
    color = ds.realItemsEndWorkBrash();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsEndWorkBrash", value);

    color = ds.realItemsGetWorkPen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsGetWorkPen", value);
    color = ds.realItemsGetWorkBrash();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsGetWorkBrash", value);

    color = ds.realItemsProblemWorkPen();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsProblemWorkPen", value);
    color = ds.realItemsProblemWorkBrash();
    value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    settings.setValue("realItemsProblemWorkBrash", value);

    settings.endGroup();
    settings.sync();

    accept();
}
//-----------------------------------------------------------------------------
