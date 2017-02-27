
//***************************************************************************//
//                                                                           //
//                           Описание QPtrList                               //
//                                                                           //
//***************************************************************************//

#ifndef QPTRLIST_H
#define QPTRLIST_H

#include <QList>
#include <defUkpCommon>

template<class T>
class UKPCOMMONSHARED_EXPORT QPtrList : public QList<T>
{
protected:
  bool                  fAutoDelete;   // Признак удаления объектов

public:
  QPtrList() : QList<T>(), fAutoDelete(false) {}
  QPtrList(const QPtrList<T> &l) : QList<T>(l), fAutoDelete(false) {}
  ~QPtrList()
  {
      if (fAutoDelete)
          while (!this->isEmpty())
              if (T t = this->takeFirst()) delete t;
  }

  bool autoDelete() const { return fAutoDelete; }
  void setAutoDelete(bool ad) { fAutoDelete = ad; }

  QPtrList<T> &operator=(const QPtrList<T> &l)
  {
      if (&l==this) return *this;
      QList<T>::operator=(l);
      fAutoDelete = l.fAutoDelete;
      return *this;
  }
  void clear()
  {
      if (fAutoDelete)
      {
          while (!this->isEmpty())
              if (T t = this->takeFirst()) delete t;
      }
      else QList<T>::clear();
  }
  void replace(int i, const T &t)
  {
      if (i<0 && i>=this->size()) return;
      if (fAutoDelete) { removeAt(i); insert(i,t); }
      else QList<T>::replace(i,t);
  }
  void removeAt(int i)
  {
      if (i<0 && i>=this->size()) return;
      if (fAutoDelete)
      {
          if (T t = this->takeAt(i)) delete t;
      }
      else QList<T>::removeAt(i);
  }
  int removeAll(const T &t)
  {
      if (fAutoDelete)
      {
        int kolRemoved(0);
          for (int i=this->size()-1; i>=0; i--)
          {
            T curT = this->at(i);
              if (curT==t) { this->removeAt(i); ++kolRemoved; }
          }
          return kolRemoved;
      }
      else return QList<T>::removeAll(t);
  }
  bool removeOne(const T &t)
  {
      if (fAutoDelete)
      {
          for (int i=0; i<this->size(); i++)
          {
            T curT = this->at(i);
              if (curT==t) { removeAt(i); return true; }
          }
          return false;
      }
      else return QList<T>::removeOne(t);
  }

};
#endif // QPTRLIST_H
