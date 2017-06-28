#include <defMacro>
#include <TAppCarryPlan>
#include <QSettings>

DiagrammSettings::DiagrammSettings(const QString &projecgroup, const QString &projecname)
{
    QSettings settings(projecgroup, projecname);

    settings.beginGroup("diagramm");

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

    settings.endGroup();
    settings.sync();
}
//-----------------------------------------------------------------------------

int DiagrammSettings::headerHeight() const
{
    return m_headerHeight;
}
//-----------------------------------------------------------------------------

int DiagrammSettings::columnWidth() const
{
    return m_columnWidth;
}
//-----------------------------------------------------------------------------

int DiagrammSettings::rowHeight() const
{
    return m_rowHeight;
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::gridPen() const
{
    return const_cast<QColor &>(m_gridPen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::weekendPen() const
{
    return const_cast<QColor &>(m_weekendPen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::gridBrush() const
{
    return const_cast<QColor &>(m_gridBrush);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::weekendBrush() const
{
    return const_cast<QColor &>(m_weekendBrush);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::planItemsTaskPen() const
{
    return const_cast<QColor &>(m_planItemsTaskPen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::planItemsTaskBrash() const
{
    return const_cast<QColor &>(m_planItemsTaskBrash);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::planItemsPlanPen() const
{
    return const_cast<QColor &>(m_planItemsPlanPen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::planItemsPlanBrash() const
{
    return const_cast<QColor &>(m_planItemsPlanBrash);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::planItemsProcedurePen() const
{
    return const_cast<QColor &>(m_planItemsProcedurePen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::planItemsProcedureBrash() const
{
    return const_cast<QColor &>(m_planItemsProcedureBrash);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::planItemsWorkPen() const
{
    return const_cast<QColor &>(m_planItemsWorkPen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::planItemsWorkBrash() const
{
    return const_cast<QColor &>(m_planItemsWorkBrash);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsTaskPen() const
{
    return const_cast<QColor &>(m_realItemsTaskPen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsTaskBrash() const
{
    return const_cast<QColor &>(m_realItemsTaskBrash);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsPlanPen() const
{
    return const_cast<QColor &>(m_realItemsPlanPen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsPlanBrash() const
{
    return const_cast<QColor &>(m_realItemsPlanBrash);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsProcedurePen() const
{
    return const_cast<QColor &>(m_realItemsProcedurePen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsProcedureBrash() const
{
    return const_cast<QColor &>(m_realItemsProcedureBrash);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsGetWorkPen() const
{
    return const_cast<QColor &>(m_realItemsGetWorkPen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsGetWorkBrash() const
{
    return const_cast<QColor &>(m_realItemsGetWorkBrash);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsBegWorkPen() const
{
    return const_cast<QColor &>(m_realItemsBegWorkPen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsBegWorkBrash() const
{
    return const_cast<QColor &>(m_realItemsBegWorkBrash);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsEndWorkPen() const
{
    return const_cast<QColor &>(m_realItemsEndWorkPen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsEndWorkBrash() const
{
    return const_cast<QColor &>(m_realItemsEndWorkBrash);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsProblemWorkPen() const
{
    return const_cast<QColor &>(m_realItemsProblemWorkPen);
}
//-----------------------------------------------------------------------------

QColor &DiagrammSettings::realItemsProblemWorkBrash() const
{
    return const_cast<QColor &>(m_realItemsProblemWorkBrash);
}
//-----------------------------------------------------------------------------

//******************************************************************************

TAppCarryPlan::TAppCarryPlan(QStringList &params, int n, QString nm) :
    TAppPlan(params, n, nm),
    m_diagrammSettings("modern", "ukpcarryplan")
{
}
//-----------------------------------------------------------------------------

TAppCarryPlan::~TAppCarryPlan()
{

}
//-----------------------------------------------------------------------------

TAppCarryPlan *TAppCarryPlan::instance(QStringList &params, int n, QString nm)
{
    if (!TAbstractApp::project) TAbstractApp::project = new TAppCarryPlan(params, n, nm);
    return (TAppCarryPlan*)TAbstractApp::project;
}
//-----------------------------------------------------------------------------

DiagrammSettings &TAppCarryPlan::diagrammSettings() const
{
    return const_cast<DiagrammSettings &>(m_diagrammSettings);
}
//-----------------------------------------------------------------------------

void TAppCarryPlan::freeInstance()
{
    if (TAbstractApp::project) delete this;
}
//-----------------------------------------------------------------------------

bool TAppCarryPlan::init()
{
    if (!TAppPlan::init()) return false;

    return true;
}
//-----------------------------------------------------------------------------

bool TAppCarryPlan::run()
{
    return true;
}
//-----------------------------------------------------------------------------

void TAppCarryPlan::reset(bool thisonly)
{
    if (!thisonly) TAbstractApp::reset(true);
}
//-----------------------------------------------------------------------------

QString TAppCarryPlan::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TAppCarryPlan::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TAppCarryPlan::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TAppCarryPlan::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
