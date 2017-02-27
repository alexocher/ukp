#ifndef FDIALOG_NNM_H
#define FDIALOG_NNM_H

#include <QFrame>
#include "ui_fdialog_nnm.h"

class TfrmDialog_NNm : public QDialog, private Ui::TfrmDialog_NNm
{
    Q_OBJECT
private:
    int                 fPropNum;      // Заданный номер
    QString             fPropName;     // Заданное наименование

public:
    explicit TfrmDialog_NNm(QWidget *parent=0, Qt::WindowFlags f=0);
    ~TfrmDialog_NNm();

    void prepare(const QString &title, int n, const QString &nm);
    int propNum() const;               // Заданный номер
    QString propName() const;          // Заданное наименование

public slots:
    void apply();

};

#endif // FDIALOG_NNM_H
