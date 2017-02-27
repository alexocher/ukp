#ifndef ENTITYMANAGER_GLOBAL_H
#define ENTITYMANAGER_GLOBAL_H

#include <QtCore/qglobal.h>
//#include <iostream>
#include <QString>

#if defined(ENTITYMANAGER_LIBRARY)
#  define ENTITYMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ENTITYMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif






// Коды операций и состояний
enum EM_OPERATION_RETURNED_STATUS{
    opOK    = 1000,         // успешное завершение
    opFAIL,                 // необработанная ошибка    
    opYEARPLAN_NOT_EXIST,   // годовой план не найден
    opOSHS_EMPTY,           // дерево ОШС не заполнено
};



// состояние узла
class ENTITYMANAGERSHARED_EXPORT EM_InternalState{
public:
    // Внутреннее состяние узла
    enum INTERNAL_STATE{
        INTERNAL_STATE_NEW_ITEM,
        INTERNAL_STATE_MODIFY_ITEM,
        INTERNAL_STATE_COMMIT_ITEM
    };
private:
    INTERNAL_STATE _internal_state; // внутреннее состояние узла (В БД НЕ ХРАНИТСЯ)
protected:
    EM_InternalState(INTERNAL_STATE state){_internal_state = state;}
    INTERNAL_STATE getInternalState() const{return _internal_state;}
    virtual void setInternalState(INTERNAL_STATE state){_internal_state = state;}
public:
    bool isModify(){return _internal_state==INTERNAL_STATE_MODIFY_ITEM;}
    bool isCommit(){return _internal_state==INTERNAL_STATE_COMMIT_ITEM;}
};



// Класс содержит реализацию свойства ключа
class ENTITYMANAGERSHARED_EXPORT AbstractEntity{
private:
    int _suid;
protected:
    AbstractEntity(int _suid){this->_suid = _suid;}
    void suid(int _suid){this->_suid = _suid;}
    int suid()const{return _suid;}
public:
    int SUID()const{return suid();}
};



#endif // ENTITYMANAGER_GLOBAL_H
