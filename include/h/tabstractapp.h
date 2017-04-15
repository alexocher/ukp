#ifndef TABSTRACTAPP_H
#define TABSTRACTAPP_H

#include <QTime>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QSize>
#include <QColor>
#include <QPalette>
#include <enums>
#include <TModuleEmployees>
#include <TModuleUnits>
#include <TModuleProductions>
#include <defUkpCommon>

// Ошибки проекта
struct UKPCOMMONSHARED_EXPORT TProjError
{
    TProjError(int n, QString nm="", bool iscrit=false, QString fnc="", QString ln="");
    TProjError(const TProjError &per);

    int                 num;           // Номер ошибки
    QString             name,          // Наименование ошибки
                        func,          // Функция, в которой возникла ошибка
                        line;          // Номер строки, в которой возникла ошибка
    bool                isCritic;      // Признак критической ошибки

    TProjError &operator=(const TProjError &per);
    void toConsole();                  // Вывод ошибок в консоль
    QString toStr();                   // Преобразование в строку
};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TProjError*> TProjectErrorList;

// Данные интерфейса
struct UKPCOMMONSHARED_EXPORT TUiData
{
  TUiData();

  int                   fontSize;      // Размер шрифта по умолчанию
  QSize                 btnSize,       // Размер обычной кнопки
                        btnOkSize,     //        кнопки ПРИНЯТЬ
                        sqrElSize,     //        квадратного элемента
                        edBigXYSize,   //        окна больших координат
                        edSmallXYSize; //        окна маленьких координат
  int                   edHeight,      // Высота окна редактирования
                        btnTabHeight,  // Высота кнопки переключения закладок
                        spaceBig,      // Расстояние между элементами управления (большое)
                        spaceSmall;    //                                        (маленькое)
};

// Цвета палитры
struct UKPCOMMONSHARED_EXPORT TPaletteColors
{
  TPaletteColors(QString nm="PAL1", QString mnnm="Зеленая");
  TPaletteColors(const TPaletteColors &pc);

  QString               name,
                        mnName;        // Наименование палитры в меню
  bool                  isActive;      // Признак активной палитры
  QColor                base,          // Цвет фона текстового поля
                        panel,         // Цвет панели
                        button,        // Цвет кнопки
                        altButton,     // Альтернативный цвет кнопки
                        title,         // Цвет заголоsвка окна и группы элементов
                        actTitle,      // Цвет заголовка активного окна
                        actTitleText,  // Цвет текста заголоsвка активного окна
                        hilight,       // Цвет выделения
                        text,          // Цвет текста
                        hilightText,   // Цвет текста выделения
                        textRed,       // Цвет текста-акцента 1 (maroon)
                        textBlue,      // Цвет текста-акцента 2 (navy)
                        altText;       // Цвет приглушенного текста

  TPaletteColors &operator=(const TPaletteColors &pc);
  TPaletteColors &operator=(const QPalette &pal);

  QString toStr();                     // Преобразование в строку
};

class UKPCOMMONSHARED_EXPORT TAbstractApp : public TAbstractObject
{
    Q_OBJECT
private:
    TAbstractApp(const TAbstractApp &obj);
    TAbstractApp &operator=(const TAbstractApp &obj);

protected:
    QString             fDataPath;     // Путь к директории данных data
    QStringList         fParams;       // Параметры запуска
    TModuleList         fPlModules;    // Модули проекта
    TModuleEmployees   *fModuleEmployees; // Модуль работы с персоналом
    TModuleUnits       *fModuleUnits;  //                  с подразделениями
    TModuleProductions *fModuleProductions; //             с продукцией
    TProjectErrorList   fPlErrors;     // Список ошибок приложения
    QIntValidator      *fIvZeroAndMore, // Положительные целые
                       *fIvAll;        // Целые во всем диапазоне
    QDoubleValidator   *fDvAll;        // Вещественные во всем диапазоне
    bool                fIsRunning;    // Флаг запущенного проекта
    TUiData             fUiData;       // Данные интерфейса
    TPaletteColors      fCurrentPalette; // Текущая палитра
    QTime               fWorkDayBegin; // Начало рабочего дня

    TAbstractApp(QStringList &params, int n=0, QString nm="");
    ~TAbstractApp();

    // Модули проекта
    void insertModule(TAbstractModule *m); // ... добавление модуля
    void removeModule(TAbstractModule *m); // ... удаление модуля
    void clearModules();               // ... очистка списка модулей

public:
    static TAbstractApp *project;      // pattern singleton
    virtual void freeInstance()=0;

    QString &dataPath() const;         // Путь к директории данных data
    QStringList &params() const;       // Параметры запуска
    TAbstractModule *findModule(TProjModuleType mt); // Модуль проекта
    TProjectErrorList &errors() const; // Список ошибок приложения
    void insertError(QString nm, bool iscrit, QString fnc, QString ln); // Добавить ошибку приложения в список
    void printErrors();                // Вывод ошибок проекта
    bool presentCriticalErrors();      // Поиск критических ошибок проекта
    const QTime &workDayBegin() const; // Начало рабочего дня
    void setWorkDayBegin(const QTime &t);

    TUiData &uiData() const;           // Данные интерфейса
    TPaletteColors &currentPalette() const; // Текущая палитра

    virtual bool init();               // Функция начальной инициализации
    virtual bool run()=0;              // Начало работы проекта

public: // TAbstractObject interface

    virtual void reset(bool thisonly);
    virtual QString toStr();
    virtual QString toHtml(bool fullinfo=true);
    virtual bool toDB(QString param);
    virtual bool fromDB(QString param);

signals :
  void sig_progressValue(QString,int); // Данные ProgressBar

};

#endif // TABSTRACTAPP_H
