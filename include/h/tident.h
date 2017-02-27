
//***************************************************************************//
//                                                                           //
//                             Описание TIdent                               //
//                                                                           //
//***************************************************************************//

#ifndef TIDENTH
#define TIDENTH

#include <QString>
#include <QMetaType>
#include <QPtrList>
#include <defUkpCommon>

struct UKPCOMMONSHARED_EXPORT TIdent
{
  TIdent(int i=0, int n=0, QString nm="", int tg=0);
  TIdent(const TIdent &idnt);
  ~TIdent() {}

  int                   id,            // id
                        num;           // Номер
  QString               name;          // Наименование
  int                   tag;           // Вспомогательное поле

  TIdent &operator=(const TIdent &idnt);
  bool operator==(const TIdent &idnt) const;
  void setIdent(int i=0, int n=0, QString nm="", int tg=0); // Задание значений
  void setIdent(const TIdent &idnt);   // Задание значений
  QString toStr();                     // Преобразование в строку
  void fromStr(QString s);             // Заполнение полей из строки
};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TIdent*> TIdentList;

Q_DECLARE_METATYPE(TIdent)

/*
  TIdent idn1;
  QVariant var;
    var.setValue(idn1); // copy idn1 into the variant
    ...
  TIdent idn2 = var.value<TIdent>(); // retrieve the value
*/

#endif // TIDENTH
