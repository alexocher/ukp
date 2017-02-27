#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QJsonValue>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT common
{
public:

    //.............................................................................
    // Работа с файлами

    // Чтение файла
    static std::string readFile(const QString &fnm);

    // Запись файла
    static bool writeFile(const QString &fnm, std::string &data);

    //.............................................................................
    // Работа с JSON
    static QJsonValue value(const std::string &json, const QString &nm);

};


#endif // COMMON_H
