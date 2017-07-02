#ifndef TAPPCARRYPLAN_H
#define TAPPCARRYPLAN_H

#include <TAppPlan>
#include <QSettings>

class DiagrammSettings
{
public:

    DiagrammSettings(const QString &projecgroup, const QString &projecname);

//    DiagrammSettings() = delete;
//    DiagrammSettings(const DiagrammSettings &) = delete;
//    DiagrammSettings &operator=(const DiagrammSettings &) = delete;

    int headerHeight() const;
    void setHeaderHeight(int h);

    int columnWidth() const;
    void setColumnWidth(int w);

    int rowHeight() const;
    void setRowHeight(int h);

    QColor &gridPen() const;
    void setGridPen(const QColor &color);

    QColor &gridBrush() const;
    void setGridBrush(const QColor &color);

    QColor &weekendPen() const;
    void setWeekendPen(const QColor &color);

    QColor &weekendBrush() const;
    void setWeekendBrush(const QColor &color);

    QColor &planItemsTaskPen() const;
    void setPlanItemsTaskPen(const QColor &color);

    QColor &planItemsTaskBrash() const;
    void setPlanItemsTaskBrash(const QColor &color);

    QColor &planItemsPlanPen() const;
    void setPlanItemsPlanPen(const QColor &color);

    QColor &planItemsPlanBrash() const;
    void setPlanItemsPlanBrash(const QColor &color);

    QColor &planItemsProcedurePen() const;
    void setPlanItemsProcedurePen(const QColor &color);

    QColor &planItemsProcedureBrash() const;
    void setPlanItemsProcedureBrash(const QColor &color);

    QColor &planItemsWorkPen() const;
    void setPlanItemsWorkPen(const QColor &color);

    QColor &planItemsWorkBrash() const;
    void setPlanItemsWorkBrash(const QColor &color);

    QColor &realItemsTaskPen() const;
    void setRealItemsTaskPen(const QColor &color);

    QColor &realItemsTaskBrash() const;
    void setRealItemsTaskBrash(const QColor &color);

    QColor &realItemsPlanPen() const;
    void setRealItemsPlanPen(const QColor &color);

    QColor &realItemsPlanBrash() const;
    void setRealItemsPlanBrash(const QColor &color);

    QColor &realItemsProcedurePen() const;
    void setRealItemsProcedurePen(const QColor &color);

    QColor &realItemsProcedureBrash() const;
    void setRealItemsProcedureBrash(const QColor &color);

    QColor &realItemsGetWorkPen() const;
    void setRealItemsGetWorkPen(const QColor &color);

    QColor &realItemsGetWorkBrash() const;
    void setRealItemsGetWorkBrash(const QColor &color);

    QColor &realItemsBegWorkPen() const;
    void setRealItemsBegWorkPen(const QColor &color);

    QColor &realItemsBegWorkBrash() const;
    void setRealItemsBegWorkBrash(const QColor &color);

    QColor &realItemsEndWorkPen() const;
    void setRealItemsEndWorkPen(const QColor &color);

    QColor &realItemsEndWorkBrash() const;
    void setRealItemsEndWorkBrash(const QColor &color);

    QColor &realItemsProblemWorkPen() const;
    void setRealItemsProblemWorkPen(const QColor &color);

    QColor &realItemsProblemWorkBrash() const;
    void setRealItemsProblemWorkBrash(const QColor &color);

private:

    int m_headerHeight;
    int m_columnWidth;
    int m_rowHeight;

    QColor m_gridPen;
    QColor m_gridBrush;
    QColor m_weekendPen;
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
