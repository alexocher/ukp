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
    QPalette pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().gridPen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().gridBrush());
    lblGrid->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().weekendPen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().weekendBrush());
    lblWeekend->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().planItemsTaskPen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().planItemsTaskBrash());
    lblPlanTask->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().planItemsPlanPen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().planItemsPlanBrash());
    lblPlanPlan->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().planItemsProcedurePen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().planItemsProcedureBrash());
    lblPlanProcedure->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().planItemsWorkPen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().planItemsWorkBrash());
    lblPlanWork->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().realItemsTaskPen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().realItemsTaskBrash());
    lblRealTask->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().realItemsPlanPen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().realItemsPlanBrash());
    lblRealPlan->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().realItemsProcedurePen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().realItemsProcedureBrash());
    lblRealProcedure->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().realItemsBegWorkPen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().realItemsBegWorkBrash());
    lblRealWorkBegin->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().realItemsEndWorkPen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().realItemsEndWorkBrash());
    lblRealWorkEnd->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().realItemsGetWorkPen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().realItemsGetWorkBrash());
    lblRealWorkGet->setPalette(pal);
    pal = palette();
    pal.setColor(QPalette::WindowText,PROJCARRYPLAN->diagrammSettings().realItemsProblemWorkPen());
    pal.setColor(QPalette::Window,PROJCARRYPLAN->diagrammSettings().realItemsProblemWorkBrash());
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
        bool isPen(&btn==pbPen);

        QColor color;

        if (rbGrid->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().gridPen() : PROJCARRYPLAN->diagrammSettings().gridBrush();
        else if (rbWeekend->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().weekendPen() : PROJCARRYPLAN->diagrammSettings().weekendBrush();
        else if (rbPlanTask->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().planItemsTaskPen() : PROJCARRYPLAN->diagrammSettings().planItemsTaskBrash();
        else if (rbPlanPlan->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().planItemsPlanPen() : PROJCARRYPLAN->diagrammSettings().planItemsPlanBrash();
        else if (rbPlanProcedure->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().planItemsProcedurePen() : PROJCARRYPLAN->diagrammSettings().planItemsProcedureBrash();
        else if (rbPlanWork->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().planItemsWorkPen() : PROJCARRYPLAN->diagrammSettings().planItemsWorkBrash();
        else if (rbRealTask->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().realItemsTaskPen() : PROJCARRYPLAN->diagrammSettings().realItemsTaskBrash();
        else if (rbRealPlan->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().realItemsPlanPen() : PROJCARRYPLAN->diagrammSettings().realItemsPlanBrash();
        else if (rbRealProcedure->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().realItemsProcedurePen() : PROJCARRYPLAN->diagrammSettings().realItemsProcedureBrash();
        else if (rbRealWorkBegin->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().realItemsBegWorkPen() : PROJCARRYPLAN->diagrammSettings().realItemsBegWorkBrash();
        else if (rbRealWorkEnd->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().realItemsEndWorkPen() : PROJCARRYPLAN->diagrammSettings().realItemsEndWorkBrash();
        else if (rbRealWorkGet->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().realItemsGetWorkPen() : PROJCARRYPLAN->diagrammSettings().realItemsGetWorkBrash();
        else if (rbRealWorkProblem->isChecked())
            color = isPen ? PROJCARRYPLAN->diagrammSettings().realItemsProblemWorkPen() : PROJCARRYPLAN->diagrammSettings().realItemsProblemWorkBrash();

        color = QColorDialog::getColor(color, this, "Выбор цвета", QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);

        if (color.isValid())
        {
            QVariant value;
            QSettings settings("modern", "ukpcarryplan");
            settings.beginGroup("diagramm");
            if (rbGrid->isChecked())
            {
                value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
                settings.setValue(isPen ? "gridPen" : "gridBrush", value);
                if (isPen) PROJCARRYPLAN->diagrammSettings().setGridPen(COLOR(value.toString()));
                else PROJCARRYPLAN->diagrammSettings().setGridBrush(COLOR(value.toString()));
            }
            else if (rbWeekend->isChecked())
            {

            }
            else if (rbPlanTask->isChecked())
            {

            }
            else if (rbPlanPlan->isChecked())
            {

            }
            else if (rbPlanProcedure->isChecked())
            {

            }
            else if (rbPlanWork->isChecked())
            {

            }
            else if (rbRealTask->isChecked())
            {

            }
            else if (rbRealPlan->isChecked())
            {

            }
            else if (rbRealProcedure->isChecked())
            {

            }
            else if (rbRealWorkBegin->isChecked())
            {

            }
            else if (rbRealWorkEnd->isChecked())
            {

            }
            else if (rbRealWorkGet->isChecked())
            {

            }
            else if (rbRealWorkProblem->isChecked())
            {

            }
/*

    QVariant value = settings.value("headerHeight");
    if (!value.isValid())
    {
        value = 60;
        settings.setValue("headerHeight", value);
    }
    m_headerHeight = value.toInt();

    value = settings.value("columnWidth");
    if (!value.isValid())
    {
        value = 30;
        settings.setValue("columnWidth", value);
    }
    m_columnWidth = value.toInt();

    value = settings.value("rowHeight");
    if (!value.isValid())
    {
        value = 40;
        settings.setValue("rowHeight", value);
    }
    m_rowHeight = value.toInt();

    value = settings.value("gridPen");
    if (!value.isValid())
    {
      QColor color(Qt::darkGray);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("gridPen", value);
    }
    m_gridPen = COLOR(value.toString());

    value = settings.value("bridBrush");
    if (!value.isValid())
    {
      QColor color(Qt::white);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("bridBrush", value);
    }
    m_gridBrush = COLOR(value.toString());

    value = settings.value("weekendPen");
    if (!value.isValid())
    {
      QColor color(Qt::darkRed);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("weekendPen", value);
    }
    m_weekendPen = COLOR(value.toString());

    value = settings.value("weekendBrush");
    if (!value.isValid())
    {
      QColor color(Qt::lightGray);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("weekendBrush", value);
    }
    m_weekendBrush = COLOR(value.toString());

    value = settings.value("planItemsTaskPen");
    if (!value.isValid())
    {
      QColor color(Qt::darkGray);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("planItemsTaskPen", value);
    }
    m_planItemsTaskPen = COLOR(value.toString());

    value = settings.value("planItemsTaskBrash");
    if (!value.isValid())
    {
      QColor color(Qt::darkGray);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("planItemsTaskBrash", value);
    }
    m_planItemsTaskBrash = COLOR(value.toString());

    value = settings.value("planItemsPlanPen");
    if (!value.isValid())
    {
      QColor color(Qt::darkGray);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("planItemsPlanPen", value);
    }
    m_planItemsPlanPen = COLOR(value.toString());

    value = settings.value("planItemsPlanBrash");
    if (!value.isValid())
    {
      QColor color(Qt::darkGray);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("planItemsPlanBrash", value);
    }
    m_planItemsPlanBrash = COLOR(value.toString());

    value = settings.value("planItemsProcedurePen");
    if (!value.isValid())
    {
      QColor color(Qt::darkGray);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("planItemsProcedurePen", value);
    }
    m_planItemsProcedurePen = COLOR(value.toString());

    value = settings.value("planItemsProcedureBrash");
    if (!value.isValid())
    {
      QColor color(Qt::darkGray);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("planItemsProcedureBrash", value);
    }
    m_planItemsProcedureBrash = COLOR(value.toString());

    value = settings.value("planItemsWorkPen");
    if (!value.isValid())
    {
      QColor color(Qt::blue);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("planItemsWorkPen", value);
    }
    m_planItemsWorkPen = COLOR(value.toString());

    value = settings.value("planItemsWorkBrash");
    if (!value.isValid())
    {
      QColor color(Qt::blue);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("planItemsWorkBrash", value);
    }
    m_planItemsWorkBrash = COLOR(value.toString());

    value = settings.value("realItemsTaskPen");
    if (!value.isValid())
    {
      QColor color(Qt::darkRed);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsTaskPen", value);
    }
    m_realItemsTaskPen = COLOR(value.toString());

    value = settings.value("realItemsTaskBrash");
    if (!value.isValid())
    {
      QColor color(Qt::darkRed);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsTaskBrash", value);
    }
    m_realItemsTaskBrash = COLOR(value.toString());

    value = settings.value("realItemsPlanPen");
    if (!value.isValid())
    {
      QColor color(Qt::darkRed);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsPlanPen", value);
    }
    m_realItemsPlanPen = COLOR(value.toString());

    value = settings.value("realItemsPlanBrash");
    if (!value.isValid())
    {
      QColor color(Qt::darkRed);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsPlanBrash", value);
    }
    m_realItemsPlanBrash = COLOR(value.toString());

    value = settings.value("realItemsProcedurePen");
    if (!value.isValid())
    {
      QColor color(Qt::darkRed);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsProcedurePen", value);
    }
    m_realItemsProcedurePen = COLOR(value.toString());

    value = settings.value("realItemsProcedureBrash");
    if (!value.isValid())
    {
      QColor color(Qt::darkRed);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsProcedureBrash", value);
    }
    m_realItemsProcedureBrash = COLOR(value.toString());

    value = settings.value("realItemsGetWorkPen");
    if (!value.isValid())
    {
      QColor color(Qt::darkGreen);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsGetWorkPen", value);
    }
    m_realItemsGetWorkPen = COLOR(value.toString());

    value = settings.value("realItemsGetWorkBrash");
    if (!value.isValid())
    {
      QColor color(Qt::darkGreen);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsGetWorkBrash", value);
    }
    m_realItemsGetWorkBrash = COLOR(value.toString());

    value = settings.value("realItemsBegWorkPen");
    if (!value.isValid())
    {
      QColor color(Qt::green);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsBegWorkPen", value);
    }
    m_realItemsBegWorkPen = COLOR(value.toString());

    value = settings.value("realItemsBegWorkBrash");
    if (!value.isValid())
    {
      QColor color(Qt::green);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsBegWorkBrash", value);
    }
    m_realItemsBegWorkBrash = COLOR(value.toString());

    value = settings.value("realItemsEndWorkPen");
    if (!value.isValid())
    {
      QColor color(Qt::blue);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsEndWorkPen", value);
    }
    m_realItemsEndWorkPen = COLOR(value.toString());

    value = settings.value("realItemsEndWorkBrash");
    if (!value.isValid())
    {
      QColor color(Qt::blue);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsEndWorkBrash", value);
    }
    m_realItemsEndWorkBrash = COLOR(value.toString());

    value = settings.value("realItemsProblemWorkPen");
    if (!value.isValid())
    {
      QColor color(Qt::red);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsProblemWorkPen", value);
    }
    m_realItemsProblemWorkPen = COLOR(value.toString());

    value = settings.value("realItemsProblemWorkBrash");
    if (!value.isValid())
    {
      QColor color(Qt::red);
        value = QString("%1.%2.%3.%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        settings.setValue("realItemsProblemWorkBrash", value);
    }
    m_realItemsProblemWorkBrash = COLOR(value.toString());

*/
            settings.endGroup();
            settings.sync();
        }
    }
}
//-----------------------------------------------------------------------------

void TfrmSettings::apply()
{
    accept();
}
//-----------------------------------------------------------------------------
