#ifndef flinked_elements_h
#define flinked_elements_h

#include <TCarryProcedure>
#include <QFrame>
#include "ui_flinkedelements.h"

class TfrmLinkedElements : public QDialog, private Ui::TfrmLinkedElements
{
    Q_OBJECT
private:

    const TCarryProcedureList &fProcedures;
    QList<int> &fLinkedProcedures;

public:

    TfrmLinkedElements(const TCarryProcedureList &procedures, QList<int> &linkedProcedures, QWidget *parent=0, Qt::WindowFlags f=0);
    ~TfrmLinkedElements();

public slots:

    void apply();

};

#endif // flinked_elements_h
