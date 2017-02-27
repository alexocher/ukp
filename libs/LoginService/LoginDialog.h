#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class LoginDialog : public QDialog{
    Q_OBJECT
private:
    QPushButton *m_btnOK;
    QPushButton *m_btnCancel;

    QLineEdit *m_txtLogin;
    QLineEdit *m_txtPasword;

    QString _login;
    QString _paswword;
    bool bOK;
public:
    LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    const QString& getLogin() const;
    const QString& getPassword() const;
    bool isOK() const;
public slots:
    void btn_ok();
    void btn_cancel();

};

#endif // LOGINDIALOG_H
