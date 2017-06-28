#ifndef FSETTINGS_H
#define FSETTINGS_H

#include <QFrame>
#include "ui_fsettings.h"

class TfrmSettings : public QDialog, private Ui::TfrmSettings
{
    Q_OBJECT
private:

    void resetSettings(const QPushButton &btn);

public:
    explicit TfrmSettings(QWidget *parent=0, Qt::WindowFlags f=0);
    ~TfrmSettings();

public slots:

    void resetSettings();

    void apply();

};

#endif // FSETTINGS_H
