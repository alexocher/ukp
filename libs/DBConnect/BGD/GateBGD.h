#ifndef GATEBGD_H
#define GATEBGD_H
#include "dbconnect.h"
#include "../BaseGate/BaseGate.h"
/**
 * \brief Класс для подключения и выполнения запросов к ООБГД. Все результаты получаются в сыром виде (JSON).
 * \warning Проверки входных параметров не производятся
 *
 * \version 0.1
 */

class SHARED_EXPORT GateBGD:public BaseGate{
public:
    enum BGD_REQUEST_STATUS_CODE{
        BGD_REQUEST_STATUS_CODE_OK = 1000,
        BGD_REQUEST_STATUS_CODE_EMPTY_DATA,
        BGD_REQUEST_STATUS_CODE_ERROR,
    };
private:   
    // убрали конструктор копирования и оператор присваивания
    GateBGD(const GateBGD &o);
    GateBGD &operator=(const GateBGD &o);

    BGD_REQUEST_STATUS_CODE parse_ok(const QByteArray &a);
public:


    ~GateBGD();
    /*!
     * \brief GateBDF
     * \param login - логин к БДФ
     * \param password - пароль к БДФ
     * \param url - адрес (без протокола)
     */
    GateBGD();
    /*!
     * \brief version получить версию API
     * \return JSON согласно схемы ООБГД
     */
    QByteArray version();
    /*!
     * \brief branches получить список всех веток редакций
     * \return JSON согласно схемы ООБГД
     */
    QByteArray branches();
    /*!
     * \brief branche получение данных ветки редакций
     * \param _branche - ветка
     * \return JSON согласно схемы ООБГД
     */
    QByteArray branch(const QString& _branch);
    /*!
     * \brief masterBranchContent получение данных из ветки редакций
     * \param _branch - ветка
     * \return JSON согласно схемы ООБГД
     */
    QByteArray branchContent(const QString& _branch);
    /*!
     * \brief mainBranch получение данных ветки редакций
     * \return JSON согласно схемы ООБГД
     */
    QByteArray mainBranch();
    /*!
     * \brief mainBranchContent получение данных из ветки редакций
     * \return JSON согласно схемы ООБГД
     */
    QByteArray mainBranchContent();
    /*!
     * \brief commit обновление ветки редакций (commit)
     * \param data - JSON схема операции
     * \return JSON согласно схемы ООБГД
     */
    QByteArray commit(const QByteArray& data);
    /*!
     * \brief log получение истории изменений ветки редакций
     * \param _branch - ветка
     * \return JSON согласно схемы ООБГД
     */
    QByteArray log(const QString& _branch);
    /*!
     * \brief newBranch ветвление рабочей ветки от главной ветки редакций
     * \param _branch - ветка
     * \return JSON согласно схемы ООБГД
     */
    QByteArray newBranch(const QString& _branch);
    /*!
     * \brief delBranch удаление ветки
     * \param _branch - ветка
     * \return JSON согласно схемы ООБГД
     */
    QByteArray delBranch(const QString& _branch);
    /*!
     * \brief diff различия между ветками
     * \param _branch1 - ветка 1
     * \param _branch2 - ветка 2
     * \return JSON согласно схемы ООБГД
     */
    QByteArray diff(const QString& _branch1,const QString& _branch2);
    /*!
     * \brief diff различия с главной веткой
     * \param _branch - ветка
     * \return JSON согласно схемы ООБГД
     */
    QByteArray diff(const QString& _branch);
};

#endif // GATEBGD_H
