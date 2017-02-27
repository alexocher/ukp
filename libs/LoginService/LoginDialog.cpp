#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QInputDialog>
LoginDialog::LoginDialog(QWidget *parent): QDialog(parent){

    QVBoxLayout *mainLayout = new QVBoxLayout();

    QLabel *l1 = new QLabel("Логин",this);
    l1->setAlignment(Qt::AlignHCenter);

    QLabel *l2 = new QLabel("Пароль",this);
    l2->setAlignment(Qt::AlignHCenter);

    m_txtLogin = new QLineEdit(this);
    m_txtLogin->setAlignment(Qt::AlignCenter);

    m_txtPasword = new QLineEdit(this);
    m_txtPasword->setAlignment(Qt::AlignCenter);
    m_txtPasword->setEchoMode(QLineEdit::Password);
    m_txtPasword->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);

    QHBoxLayout *btnLayout = new QHBoxLayout();

    m_btnOK = new QPushButton("OK",this);
    connect(m_btnOK,SIGNAL(clicked(bool)),this,SLOT(btn_ok()));

    m_btnCancel = new QPushButton("Отмена",this);
    connect(m_btnCancel,SIGNAL(clicked(bool)),this,SLOT(btn_cancel()));

    btnLayout->addWidget(m_btnOK);
    btnLayout->addWidget(m_btnCancel);

    mainLayout->addWidget(l1);
    mainLayout->addWidget(m_txtLogin);
    mainLayout->addWidget(l2);
    mainLayout->addWidget(m_txtPasword);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);

    bOK = false;
}

LoginDialog::~LoginDialog(){

}
bool LoginDialog::isOK() const{
    return bOK;
}
const QString& LoginDialog::getLogin() const{
    return _login;
}
const QString& LoginDialog::getPassword() const{
    return _paswword;
}
void LoginDialog::btn_ok(){
    _login = m_txtLogin->text();
    _paswword = m_txtPasword->text();
    bOK = true;
    close();
}
void LoginDialog::btn_cancel(){
    close();
}
