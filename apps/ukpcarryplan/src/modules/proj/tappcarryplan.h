#ifndef TAPPCARRYPLAN_H
#define TAPPCARRYPLAN_H

#include <TAppPlan>
#include <QSettings>

class DiagrammSettings
{
public:

    DiagrammSettings(const QString &projecgroup, const QString &projecname);

    DiagrammSettings() = delete;
    DiagrammSettings(const DiagrammSettings &) = delete;
    DiagrammSettings &operator=(const DiagrammSettings &) = delete;
    DiagrammSettings(DiagrammSettings &&) = delete;
    DiagrammSettings &operator=(DiagrammSettings &&) = delete;

    int headerHeight() const;
    int columnWidth() const;
    int rowHeight() const;

    QColor &gridPen() const;
    QColor &weekendPen() const;
    QColor &gridBrush() const;
    QColor &weekendBrush() const;

    QColor &planItemsTaskPen() const;
    QColor &planItemsTaskBrash() const;
    QColor &planItemsPlanPen() const;
    QColor &planItemsPlanBrash() const;
    QColor &planItemsProcedurePen() const;
    QColor &planItemsProcedureBrash() const;
    QColor &planItemsWorkPen() const;
    QColor &planItemsWorkBrash() const;

    QColor &realItemsTaskPen() const;
    QColor &realItemsTaskBrash() const;
    QColor &realItemsPlanPen() const;
    QColor &realItemsPlanBrash() const;
    QColor &realItemsProcedurePen() const;
    QColor &realItemsProcedureBrash() const;

    QColor &realItemsGetWorkPen() const;
    QColor &realItemsGetWorkBrash() const;
    QColor &realItemsBegWorkPen() const;
    QColor &realItemsBegWorkBrash() const;
    QColor &realItemsEndWorkPen() const;
    QColor &realItemsEndWorkBrash() const;
    QColor &realItemsProblemWorkPen() const;
    QColor &realItemsProblemWorkBrash() const;

private:

    int m_headerHeight;
    int m_columnWidth;
    int m_rowHeight;

    QColor m_gridPen;
    QColor m_weekendPen;
    QColor m_bridBrush;
    QColor m_weekendBrush;

    QColor m_planItemsTaskPen;
    QColor m_planItemsTaskBrash;
    QColor m_planItemsPlanPen;
    QColor m_planItemsPlanBrash;
    QColor m_planItemsProcedurePen;
    QColor m_planItemsProcedureBrash;
    QColor m_planItemsWorkPen;
    QColor m_planItemsWorkBrash;

    QColor m_realItemsTaskPen;
    QColor m_realItemsTaskBrash;
    QColor m_realItemsPlanPen;
    QColor m_realItemsPlanBrash;
    QColor m_realItemsProcedurePen;
    QColor m_realItemsProcedureBrash;

    QColor m_realItemsGetWorkPen;
    QColor m_realItemsGetWorkBrash;
    QColor m_realItemsBegWorkPen;
    QColor m_realItemsBegWorkBrash;
    QColor m_realItemsEndWorkPen;
    QColor m_realItemsEndWorkBrash;
    QColor m_realItemsProblemWorkPen;
    QColor m_realItemsProblemWorkBrash;

};

class TAppCarryPlan : public TAppPlan
{
private:
    TAppCarryPlan(const TAppCarryPlan &obj);
    TAppCarryPlan &operator=(const TAppCarryPlan &obj);

    DiagrammSettings m_diagrammSettings;

protected:
    TAppCarryPlan(QStringList &params, int n = 0, QString nm = "");
    ~TAppCarryPlan();

public:
    static TAppCarryPlan *instance(QStringList &params, int n = 0, QString nm = "");

    DiagrammSettings &diagrammSettings() const;

public: // TAbstractApp interface
    virtual void freeInstance();
    virtual bool init();
    virtual bool run();

public: // TAbstractObject interface
    virtual void reset(bool thisonly);
    virtual QString toStr();
    virtual QString toHtml(bool fullinfo = true);
    virtual bool toDB(QString param);
    virtual bool fromDB(QString param);

};

#endif // TAPPCARRYPLAN_H
