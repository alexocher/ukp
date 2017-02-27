#ifndef CONVERTENUMS_H
#define CONVERTENUMS_H

#include <QString>
#include <enums>
#include <defUkpCommon>

class UKPCOMMONSHARED_EXPORT convertEnums
{
public:

    static QString enumToStr(TProductionView en);  // Виды продукции (первый уровень иерархии)
    static void strToEnum(QString nm, TProductionView &en);
    static QString enumToStr(TProductionRubr2 en); // Второй уровень иерархии
    static void strToEnum(QString nm, TProductionRubr2 &en);
    static QString enumToStr(TProductionRubr3 en); // Третий уровень иерархии
    static void strToEnum(QString nm, TProductionRubr3 &en);
    static QString enumToStr(TProductionRubr4 en); // Четвертый уровень иерархии
    static void strToEnum(QString nm, TProductionRubr4 &en);
    static QString enumToStr(TProductionType en);  // Типы продукции
    static void strToEnum(QString nm, TProductionType &en);
    static QString enumToStr(TEmployeeType en);    // Типы должностей
    static void strToEnum(QString nm, TEmployeeType &en);
    static QString enumToStr(TCarryOutCondition en); // Сосотояние выполнения элемента плана
    static void strToEnum(QString nm, TCarryOutCondition &en);
    static QString enumToStr(TCarryOutProblem en); // Причина невозможности выполнения элемента плана
    static void strToEnum(QString nm, TCarryOutProblem &en);
    static QString enumToStr(TStatus en);          // Статус
    static void strToEnum(QString nm, TStatus &en);
    static QString enumToStr(TExternalModuleType en); // Типы внещних модулей
    static void strToEnum(QString nm, TExternalModuleType &en);

};

#endif // CONVERTENUMS_H
