
//***************************************************************************//
//                                                                           //
//            Описание данных ячейки таблицы TTableWidgetItemData            //
//                                                                           //
//***************************************************************************//

//***************************************************************************//
//                                                                           //
// TTableWidgetItemData                    - данные для декорирования        //
//   +-TTableWidgetItemDataIdent           -        с идентификатором        //
//                                                                           //
//***************************************************************************//

#ifndef TABLEWIDGETITEMDATAH
#define TABLEWIDGETITEMDATAH

#include <QPainter>
#include <QMetaType>
#include <QDateTimeEdit>
#include <defConst>
#include <defMacro>
#include <TIdent>
#include <defUkpCommon>

// Параметры декорирования ячеек
struct UKPCOMMONSHARED_EXPORT TTableDecor {
  // Режим отображения ячейки
  enum CellReg {
                        crText       = 0, // По умолчанию
                        crPixmap     = 1, // Только картинка
                        crPixmapText = 2, // Картинка и текст
                        crShape      = 3  // Примитив
  };
  // Тип примитива
  enum ShapeType {
                        shtCircle   = 0, // Кружок
                        shtSquare   = 1, // Квадрат
                        shtSqrRomb  = 2, // Квадрат с поворотом на 45 градусов
                        shtStar     = 3, // Звезда
                        shtRomb     = 4, // Ромб
                        shtRect     = 5, // Прямоугольник
                        shtTriangle = 6  // Треугольник
  };
  // Место картинки относительно текста
  enum PixmapPlace {
                        ppSeparate     = 0, // Отдельное расположение
                        ppPixmapLeft   = 1, // Картинка слева
                        ppPixmapRight  = 2, //          справа
                        ppPixmapTop    = 3, //          сверху
                        ppPixmapBottom = 4  //          снизу
  };

  // для crText, crPixmap, crPixmapText
  TTableDecor(CellReg cr, int algnfrst=Qt::AlignCenter, int algnscnd=Qt::AlignCenter); // если crPixmapText: algnfrst-pixmap, algnscnd-text
  // для crPixmapText
  TTableDecor(PixmapPlace pixpl, int pixsp=UI_SP_SM);
  // для crShape
  TTableDecor(ShapeType sht, QColor clrpen, QColor clrbr, int lnw=1, int pixsp=UI_SP_SM);
  TTableDecor(const TTableDecor &td);
  ~TTableDecor();

  CellReg               cellReg;       // Режим отображения ячейки
  ShapeType             shapeType;     // Тип примитива
  PixmapPlace           pixmapPlace;   // Положение рисунка относительно текста
  int                   alignText,     // Выравнивание текста  (если fPixmapPlace==ppSeparate)
                        alignPixmap,   // Выравнивание рисунка (если fPixmapPlace==ppSeparate)
                        pixmapSpace,   // Расстояние от рисунка до текста и от края до примитива
                        lineWidth;     // Толщина линии для примитивов
  QColor                shapePen,      // Цвета примитива: контур
                        shapeBrush;    //                : кисть

  TTableDecor &operator=(const TTableDecor &td);

  QColor *colorText() const;           // Цвета ячейки: текст
  void setColorText(QColor &clr);
  QColor *colorFon() const;            //             : фон
  void setColorFon(QColor &clr);
  void resetColors();                  // Сброс в значения по умолчанию
  QFont *font() const;                 // Шрифт ячейки
  void setFont(QFont &f);
  void resetFont();                    // Сброс в значение по умолчанию

private:
  QColor               *fColorText,    // Цвета ячейки: текст
                       *fColorFon;     //             : фон
  QFont                *fFont;         // Шрифт ячейки

};

// Данные для декорирования
class UKPCOMMONSHARED_EXPORT TTableWidgetItemData
{
public:
  // Тип элемента редактирования
  enum EditorType {
                        etNoEdit        = 0, // Редактирование недоступно
                        etLineEdit      = 1, // LineEdit
                        etSpinBox       = 2, // QSpinBox
                        etDoubleSpinBox = 3, // QDoubleSpinBox
                        etComboBox      = 4, // QComboBox
                        etDateTime      = 5  // QDateTimeEdit
  };

  // Данные QSpinBox
  struct SpinBoxData {
    SpinBoxData(int min, int max, int st) : minValue(min), maxValue(max), step(st) {}
    SpinBoxData(const SpinBoxData &d) : minValue(d.maxValue), maxValue(d.maxValue), step(d.step) {}
    int                 minValue,      // минимальное значение
                        maxValue,      // максимальное значение
                        step;          // шаг изменения
  };

  // Данные QDoubleSpinBox
  struct DoubleSpinBoxData {
    DoubleSpinBoxData(double min, double max, double st) : minValue(min), maxValue(max), step(st) {}
    DoubleSpinBoxData(const DoubleSpinBoxData &d) : minValue(d.maxValue), maxValue(d.maxValue), step(d.step) {}
    double              minValue,      // минимальное значение
                        maxValue,      // максимальное значение
                        step;          // шаг изменения
  };

  // Данные QComboBox
  struct ComboBoxData {
    ComboBoxData(const QStringList &sl) : strings(sl) {}
    ComboBoxData(const ComboBoxData &d) : strings(d.strings) {}
    QStringList         strings;       // список значений
  };

  // Данные QDateTimeEdit
  enum DateTimeFormat {                     // ЧЧ:ММ:СС ДД:ММ:ГГ
                        dtfHHMMSSDDMMYY = 0, // 00:00:00 00.00.00
                        dtfHHMMDDMMYY   = 1, //    00:00 00.00.00
                        dtfHHMMDDMM     = 2, //    00:00 00.00
                        dtfDDMMYY       = 3, //          00.00.00
                        dtfDDMM         = 4, //          00.00
                        dtfHHMMSS       = 5, // 00:00:00
                        dtfHHMM         = 6, // 00:00
                        dtfMMSS         = 7  //    00:00
  };
  struct DateTimeData {
    DateTimeData(DateTimeFormat dtft, bool iscldr=false);
    DateTimeData(DateTimeFormat dtft, QDateTime mindt, bool iscldr=false);
    DateTimeData(DateTimeFormat dtft, QDateTime mindt, QDateTime maxdt, bool iscldr=false);
    DateTimeData(const DateTimeData &d);
    DateTimeFormat     dtFormat;      // Формат вывода даты и времени
    bool                isCalendar,    // Признак редактирования в календаре
                        isMinDt,       // Признак задания минимального времени
                        isMaxDt;       // Признак задания максимального времени
    QDateTime           minDt,         // Минимальное время
                        maxDt;         // Максимальное время
    QString &displayFormat() const { return const_cast<QString&>(fDisplayFormat); }
    QDateTime dateTimeFromText(const QString &text) const;
  private:
    QString             fDisplayFormat; // Формат вывода даты и времени
  };

protected:
  QString               fText;         // Текст
  QPixmap              *fPixmap;       // Картинка
  TTableDecor           fDecor;        // Элементы декорирования
  // ... данные редактирования
  EditorType            fEditorType;   // Тип элемента редактирования
  SpinBoxData          *fSpinBoxData;  // Данные QSpinBox
  DoubleSpinBoxData    *fDoubleSpinBoxData; // Данные QDoubleSpinBox
  ComboBoxData         *fComboBoxData; // Данные QComboBox
  DateTimeData         *fDateTimeData; // Данные QDateTimeEdit

  TTableDecor &decor() const;          // Элементы декорирования

public:
  TTableWidgetItemData();
  TTableWidgetItemData(const TTableWidgetItemData&);
  TTableWidgetItemData(QPixmap *pix, int algn=Qt::AlignCenter);
  TTableWidgetItemData(const QString &txt, int algn=Qt::AlignCenter);
  TTableWidgetItemData(QPixmap *pix, const QString &txt, int algnpix=Qt::AlignCenter, int algntxt=Qt::AlignCenter);
  TTableWidgetItemData(QPixmap *pix, const QString &txt, TTableDecor::PixmapPlace pixplace, int pixsp=UI_SP_SM);
  TTableWidgetItemData(TTableDecor::ShapeType sht, const QColor &clpen, const QColor &clbr, const QString &txt="", int pixsp=UI_SP_SM, int lnw=1);
  virtual ~TTableWidgetItemData();

  QString &text() const;               // Текст
  void setText(QString txt);

  void setAlign(int algn);             // Выравнивание
  void setAlign(int algnpix, int algntxt);
  void setForeground(QColor &clr);     // Цвет текста
  void setForeground(Qt::GlobalColor clr);
  void setBackground(QColor &clr);     // Цвет фона
  void setBackground(Qt::GlobalColor clr);
  void resetColors();                  // Сброс в значения по умолчанию
  void setFont(QFont &f);              // Шрифт ячейки
  void resetFont();                    // Сброс в значение по умолчанию

  EditorType editorType();             // Тип элемента редактирования
  void setEditorProperty(int min, int max, int step); // Данные редактирования
  void setEditorProperty(double min, double max, double step);
  void setEditorProperty(const QStringList &sl);
  void setEditorProperty(DateTimeFormat dtft, bool iscldr=false);
  void setEditorProperty(DateTimeFormat dtft, QDateTime mindt, bool iscldr=false);
  void setEditorProperty(DateTimeFormat dtft, QDateTime mindt, QDateTime maxdt, bool iscldr=false);
  SpinBoxData *spinBoxData() const;    // Данные QSpinBox
  DoubleSpinBoxData *doubleSpinBoxData() const; // Данные QDoubleSpinBox
  ComboBoxData *comboBoxData() const;  // Данные QComboBox
  DateTimeData *dateTimeData() const;  // Данные QDateTimeEdit

  TTableWidgetItemData &operator=(const TTableWidgetItemData&);
  void paint(QPainter *painter, const QRect &rect, const QPalette &palette, bool isselect) const;

};

// Данные с идентификатором
class UKPCOMMONSHARED_EXPORT TTableWidgetItemDataIdent : public TTableWidgetItemData
{
protected:
  TIdent                fIdent;        // Идентификатор

public:
  TTableWidgetItemDataIdent();
  TTableWidgetItemDataIdent(const TTableWidgetItemDataIdent&);
  TTableWidgetItemDataIdent(const TIdent &idnt, QPixmap *pix, int algn=Qt::AlignCenter);
  TTableWidgetItemDataIdent(const TIdent &idnt, const QString &txt, int algn=Qt::AlignCenter);
  TTableWidgetItemDataIdent(const TIdent &idnt, QPixmap *pix, const QString &txt, int algnpix=Qt::AlignCenter, int algntxt=Qt::AlignCenter);
  TTableWidgetItemDataIdent(const TIdent &idnt, QPixmap *pix, const QString &txt, TTableDecor::PixmapPlace pixplace, int pixsp=UI_SP_SM);
  TTableWidgetItemDataIdent(const TIdent &idnt, TTableDecor::ShapeType sht, const QColor &clpen, const QColor &clbr, const QString &txt="", int pixsp=UI_SP_SM, int lnw=1);
  ~TTableWidgetItemDataIdent();

  TIdent &ident() const;               // Идентификатор

  TTableWidgetItemDataIdent &operator=(const TTableWidgetItemDataIdent&);

};

Q_DECLARE_METATYPE(TTableWidgetItemData)
Q_DECLARE_METATYPE(TTableWidgetItemDataIdent)

/*
  TTableWidgetItemData twd1;
  QVariant var;
    var.setValue(twd1); // copy s into the variant
    ...
  TTableWidgetItemData twd2 = var.value<TTableWidgetItemData>(); // retrieve the value
*/

#endif // TABLEWIDGETITEMDATAH
