#ifndef EXCEPTIONS
#define EXCEPTIONS

#include "commondef.h"
#include "SimpleLogger.h"





// Базавый класс исключений
class ENTITYMANAGERSHARED_EXPORT BaseException{
public:
    enum ERR_CODE{
        ERR = 1000,
        NULL_PARAM,
        OPEN_DB_CONNECTION_FAIL,
        ERROR_ALLOCATE_MEMORY,
        SQL_EXCEPTION,
        FILE_NOT_FOUND,
        INSTANCE_EXIST,
        BOOK_EXIST,
        ROLE_EXIST,
        ROLE_USE,
        USER_EXIST,
        EMPTY_GEOM,
        USER_NOT_FOUND,
    };
private:
    ERR_CODE code;
    QString message;
protected:
    explicit BaseException(){
        this->code = ERR;
        this->message = "Error";
    }
    BaseException(const ERR_CODE code,const QString &message){
        this->code = code;
        this->message = message;

        LogObserv* observ = new LogObserv();
        observ->err(message);
        delete observ;
    }
public:
    ERR_CODE getCode() const{return code;}
    QString const &getMessage() const{return message;}
};

namespace CommonException{
    class ENTITYMANAGERSHARED_EXPORT NullParamException:public BaseException{
    public:
        NullParamException():BaseException(NULL_PARAM,"Передан нулевой порядок"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT OpenDBException:public BaseException{
    public:
        OpenDBException(const QString& mes):BaseException(OPEN_DB_CONNECTION_FAIL,"Ошибка соединения с БД: "+mes){;}
        //OpenDBException():BaseException(OPEN_DB_CONNECTION_FAIL,"Ошибка соединения с БД"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT FileNotFoundException:public BaseException{
    public:
        FileNotFoundException(const QString& filename):BaseException(FILE_NOT_FOUND,"Файл "+ filename+" не найден"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT FileOpenException:public BaseException{
    public:
        FileOpenException(const QString& filename,const QString& descr):BaseException(FILE_NOT_FOUND,"Файл "+ filename+" не удалось открыть: "+descr){;}
    };
    class ENTITYMANAGERSHARED_EXPORT AllocateMemoryException:public BaseException{
    public:
        AllocateMemoryException():BaseException(ERROR_ALLOCATE_MEMORY,"Ошибка при выделении памяти"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT SQLException:public BaseException{
    public:
        SQLException(const QString& mes):BaseException(SQL_EXCEPTION,"SQL ERR: "+mes){;}
    };
    class ENTITYMANAGERSHARED_EXPORT GenPKException:public BaseException{
    public:
        GenPKException():BaseException(ERR,"Ошибка генерации ключа"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT ObjNotFoundException:public BaseException{
    public:
        ObjNotFoundException():BaseException(ERR,"Объект не найден"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT FileFormatException:public BaseException{
    public:
        FileFormatException():BaseException(ERR,"Ошибка формата файла"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT DuplicateObjectException:public BaseException{
    public:
        DuplicateObjectException():BaseException(ERR,"Дублирование объекта"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT ZipError:public BaseException{
    public:
        ZipError(const QString& mes):BaseException(ERR,mes){;}
    };
    /*!
     * \brief Не поддерживаемая операция (для ряда классов геометрий функции могут быть не реализованы)
     */
    class ENTITYMANAGERSHARED_EXPORT UnsupportedOperation:public BaseException{
    public:
        UnsupportedOperation(ERR_CODE code,const QString& mes):BaseException(code,mes){;}
        UnsupportedOperation(const QString& mes):BaseException(ERR,mes){;}
        UnsupportedOperation():BaseException(ERR,"Ошибка разбора JSON"){;}
    };
    /*!
     * \brief Выход индекса за границы кол-ва точек геометрии
     */
    class ENTITYMANAGERSHARED_EXPORT IndexOutOfBound:public BaseException{
    public:
        IndexOutOfBound(ERR_CODE code,const QString& mes):BaseException(code,mes){;}
        IndexOutOfBound(const QString& mes):BaseException(ERR,mes){;}
        IndexOutOfBound():BaseException(ERR,"Выход за граници массива"){;}
    };
    /*!
     * \brief Нулевой указатель
     */
    class ENTITYMANAGERSHARED_EXPORT NPE:public BaseException{
    public:
        NPE(ERR_CODE code,const QString& mes):BaseException(code,mes){;}
        NPE(const QString& mes):BaseException(ERR,mes){;}
        NPE():BaseException(ERR,"Нулевой указатель"){;}
    };
}

namespace GeomExceptions{
    /*!
     * \brief Пустая геометрия (нет точек)
     */
    class ENTITYMANAGERSHARED_EXPORT EmptyGeom:public BaseException{
    public:
        EmptyGeom(ERR_CODE code,const QString& mes):BaseException(code,mes){;}
        EmptyGeom(const QString& mes):BaseException(EMPTY_GEOM,mes){;}
        EmptyGeom():BaseException(EMPTY_GEOM,"Пустая геометрия"){;}
    };
}

namespace OSHSException{
    class ENTITYMANAGERSHARED_EXPORT EsteblishedTypeException:public BaseException{
    public:
        EsteblishedTypeException():BaseException(ERR,"Тип должности не соответствует"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT DuplicateIDException:public BaseException{
    public:
        DuplicateIDException():BaseException(ERR,"Идентификатор уже используется"){;}
    };
}
namespace AddressBookException{
    class ENTITYMANAGERSHARED_EXPORT RoleExistException:public BaseException{
    public:
        RoleExistException():BaseException(ROLE_EXIST,"Роль с таким идентификатором уже существует"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT UserExistException:public BaseException{
    public:
        UserExistException():BaseException(USER_EXIST,"Пользователь с таким идентификатором уже существует"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT UserNotFoundException:public BaseException{
    public:
        UserNotFoundException():BaseException(USER_NOT_FOUND,"Пользователь с таким идентификатором не найден"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT RoleUseException:public BaseException{
    public:
        RoleUseException():BaseException(ROLE_USE,"Роль используется"){;}
    };

}
namespace JSONFormatException{
    class ENTITYMANAGERSHARED_EXPORT ParseInputJSONException:public BaseException{
    public:
        ParseInputJSONException(const QString& mes):BaseException(ERR,mes){;}
        ParseInputJSONException():BaseException(ERR,"Ошибка разбора JSON"){;}
    };
    class ENTITYMANAGERSHARED_EXPORT FormatException:public BaseException{
    public:
        FormatException(const QString& mes):BaseException(ERR,mes){;}
        FormatException():BaseException(ERR,"Ошибка разбора JSON"){;}
    };
}
namespace BDFException{
    class ENTITYMANAGERSHARED_EXPORT ParseInputJSONException:public BaseException{
    public:
        ParseInputJSONException(const QString& mes):BaseException(ERR,mes){;}
        ParseInputJSONException():BaseException(ERR,"Ошибка разбора JSON"){;}
    };
}

#endif // EXCEPTIONS

