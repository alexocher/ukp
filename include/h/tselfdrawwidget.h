
//***************************************************************************//
//                                                                           //
//                 Описание базовых классов рисования схемы                  //
//                                                                           //
//***************************************************************************//

#ifndef SELFDRAWWIDGETH
#define SELFDRAWWIDGETH

//***************************************************************************//
//                                                                           //
// Иерархия классов, описанных в модуле                                      //
//                                                                           //
// TDrawElement                        - элемент рисования                   //
// QObject                                                                   //
//   |-TDrawLayer                      - слой рисования                      //
// QWidget                                                                   //
//   |-TSelfDrawWidget                 - самоотрисовывающаяся панель         //
//       |-TSchemeXYWidget             - Панель-схема с координатами         //
//       |-TSchemeADWidget             - Панель-схема с полярными коорд.     //
//                                                                           //
//***************************************************************************//

#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QTreeWidget>
#include <enums>
#include <QPtrList>
#include <TIdent>
#include <defUkpCommon>

// Опережающее объявление классов
class TDrawElement;
class TDrawLayer;
class TSelfDrawWidget;

/*
Перечень типов
    TDrawClassType      // Тип класса, соответствующего элементу рисования
    TDrawElementType    // Тип элемента рисования
    TLayerNum           // Именованные номера слоев
    TZapas              // Запас для вывода надписей
*/

#define DREL_DMTR_LBL        23        // Диаметр метки координатной сетки
#define DREL_DMTR_CRCL       17        // Диаметр огневой позиции
#define DREL_DMTR_PNT         9        // Диаметр точки с координатами
#define DREL_CHCK_SIDE       13        // Длина стороны квадрата checkBox'a
#define DREL_RECT_OUTCLICK    1        // Количество пикселей за пределами объекта (для функции isMyCoord())
#define DREL_TEXT_OUTDRAW     5        // Количество пикселей за пределами объекта (для подсказки и зачеркивания)
#define DREL_TEXT_SPACE       3        // Расстояние от квадратика до текста в checkBox

//***************************************************************************//
// Описание типов                                                            //
//***************************************************************************//

// Именованные номера слоев
enum TLayerNum {
                        lrnFon          =  0, // Фон
                        lrnGrid         =  1, // Координатная сетка
                        lrnGridLabels   =  2, // Метки координатной сетки
                        lrnRgns         =  3, // Глобальные районы
                        lrnOrients      =  4, // Ориентиры
                        lrnGeos         =  5, // Пункты геодезической сети
                        lrnRouts        =  6, // Маршруты
                        lrnTargs        =  7, // Цели
                        lrnRubs         =  8, // Рубежи
                        lrnOgzRgns      =  9, // Районы ОгЗ
                        lrnMeetPoints   = 10, // Точки встречи
                        lrnTrgRgns      = 11, // Участки целей
                        lrnRubRgns      = 12, // Участки рубежей
                        lrnFirePoss     = 13, // Огневые позиции
                        lrnSars         = 14, // Средства разведки
                        lrnNps          = 15, // Наблюдательные пункты
                        lrnTrasses      = 16, // Траектории
                        lrnLines        = 17, // Линии
                        lrnTexts1       = 18, // Подписи. 1-й слой
                        lrnTexts2       = 19, //          2-й слой
                        lrnTexts3       = 20, //          3-й слой
                        lrnHints        = 21, // Подсказки
                        lrnOurForms     = 22, // Свои войска
                        lrnEnemyForms   = 23, // Войска противника
                        lrnRazrs        = 24, // Разрывы
                        lrnStOrients    = 25, // Ориентиры начальников
                        lrnBu           = 26, // Условия безопасности
                        lrnHelpLines    = 27, // Вспомогательные линии
                        lrnZass         = 28, // Точки и линии засечки
                        lrnTmp          = 29  // Временные объкты
};

// Тип класса, соответствующего элементу рисования
enum TDrawClassType {
                        dctNone       =  0, // Не задан
                        dctEnObj      =  1, // Объект поражения
                        dctFormObj    =  2  // Средство поражения
};

// Запас для вывода надписей
struct UKPCOMMONSHARED_EXPORT TZapas
{
  TZapas(int l=0, int t=0, int r=0, int b=0);

  int                   left,
                        top,
                        right,
                        bottom,
                        width,
                        height;
  void setZapas(int l=0, int t=0, int r=0, int b=0);
};

//***************************************************************************//
// Описание классов                                                          //
//***************************************************************************//

typedef UKPCOMMONSHARED_EXPORT QPtrList<QPoint*> TPointList;

// Элемент рисования
class UKPCOMMONSHARED_EXPORT TDrawElement
{
protected:
  // Основные (общие) данные
  int                   fId,           // id элемента рисования
                        fIdHint;       // id подписываемого элемента (только для detHint)
  TDrawLayer           *fDrawLayer;    // Слой, в котором находится элемент
  TDrawElementType      fType;         // Тип элемента рисования
  TIdent               *fIdent;        // Идентификатор (для связи с внешними объектами)
  QColor                fClContour,    // Цвет контура (текста) - нормальной яркости
                        fClContourLess, //                      - бледный
                        fClFill,       // Цвет заливки
                        fClShadow;     // Цвет тени
  int                   fLineWidth;    // Толщина линии
  Qt::PenStyle          fPenStyle;     // Стиль пера
  Qt::BrushStyle        fBrushStyle;   // Стиль кисти
  // Данные для текстовых типов (+ для detPixmap)
  QFont                 fFnt;          // Шрифт
  QString               fText;         // Текст надписи (название картинки)
  int                   fFlags;        // Выравнивание и флаги текста (Qt::AlignmentFlags | Qt::TextFlags)
                                       // Выравнивание текста (Qt::AlignmentFlags)
                                       // Для текста в fRect
                                       // ---------------------------------------------------------------------------------------
                                       // | (AlignLeft | AlignTop    ) (AlignHCenter | AlignTop   ) (AlignRight | AlignTop    ) |
                                       // |                                                                                     |
                                       // | (AlignLeft | AlignVCenter) (        AlignCenter       ) (AlignRight | AlignVCenter) |
                                       // |                                                                                     |
                                       // | (AlignLeft | AlignBottom ) (AlignHCenter | AlignBottom) (AlignRight | AlignBottom ) |
                                       // ---------------------------------------------------------------------------------------
                                       // Для текста в точке или картинки (положение точки относительно текста (картинки))
                                       //...........................
                                       // AlignLeft | AlignTop
                                       //          (*)
                                       //             ТЕКСТ
                                       //...........................
                                       // AlignHCenter | AlignTop
                                       //          (*)
                                       //         ТЕКСТ
                                       //...........................
                                       // AlignRight | AlignTop
                                       //          (*)
                                       //     ТЕКСТ
                                       //...........................
                                       // AlignLeft | AlignVCenter
                                       //          (*)ТЕКСТ
                                       //...........................
                                       // AlignCenter
                                       //       ТЕК(*)СТ
                                       //...........................
                                       // AlignRight | AlignVCenter
                                       //     ТЕКСТ(*)
                                       //...........................
                                       // AlignLeft | AlignBottom
                                       //             ТЕКСТ
                                       //          (*)
                                       //...........................
                                       // AlignHCenter | AlignBottom
                                       //         ТЕКСТ
                                       //          (*)
                                       //...........................
                                       // AlignRight | AlignBottom
                                       //     ТЕКСТ
                                       //          (*)
                                       //...........................
                                       // Флаги текста (Qt::TextFlags)
  // Пространственные данные
  TPointList            fPlPoints;     // Координаты для отрисовки элемента
                                       // detNone         - 0
                                       // detLomLine      - n
                                       // detPolyline     - n
                                       // detPolygon      - n
                                       // detEllipce      - 1 (центр)
                                       // detTextPoint    - 1 (c учетом выравнивания)
                                       // detTextRect     - 1 (центр)
                                       // detRect         - 1 (центр)
                                       // detRoundRect    - 1 (центр)
                                       // detCheckBox     - 1 (левая верхняя)
                                       // detCheckCircle  - 1 (центр)
                                       // detCrossPolygon - n
                                       // detCrossLomLine - n
                                       // detCrossCircle  - 1 (центр)
                                       // detHint         - 1 (центр)
                                       // detPixmap       - 1 (c учетом выравнивания)
  QRect                 fRect;         // Прямоугольник для рисования текста (занимаемый элементом, ...)
                                       // (detTextPoint,detTextRect,detRect,detRoundRect,detCheckBox,detHint,detPixmap)
  QPoint                fHintPoint;    // Точка привязки подсказки (определяется при связывании с подписываемым элементом, fIdHint)
  // Дополнительные данные
  bool                  fIsBright,     // Переключатель рисования объекта ярким (true) или бледным (false) цветом
                        fIsShadow,     // Переключатель тени
                        fIsHintVisible, // Признак видимости подсказки
                        fIsChecked,    // Признак отметки
                        fIsVisible;    // Признак видимости
  double                fKless;        // Коэффициент уменьшения яркости (по умолчанию в три раза == 1/3.)
                                       // !!! Для черного не работает ( нужно использовать setClContourLess() )
  QString               fHint;         // Текст подсказки

public:
  // Конструкторы общего назначения
  TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QPoint pnt=QPoint(), Qt::PenStyle ps=Qt::SolidLine, int lw=1, Qt::BrushStyle bs=Qt::NoBrush);
  TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QColor clCntr, QColor clFill, QPoint pnt=QPoint(), Qt::PenStyle ps=Qt::SolidLine, int lw=1, Qt::BrushStyle bs=Qt::NoBrush);
  TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QRect rct, Qt::PenStyle ps=Qt::SolidLine, int lw=1, Qt::BrushStyle bs=Qt::NoBrush);
  TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QColor clCntr, QColor clFill, QRect rct, Qt::PenStyle ps=Qt::SolidLine, int lw=1, Qt::BrushStyle bs=Qt::NoBrush);
  // Конструкторы для текстовых элементов
  TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QString txt, QPoint pnt=QPoint(), int fl=0);
  TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QString txt, QColor clTxt, QPoint pnt=QPoint(), int fl=0);
  TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QString txt, QRect rct, int fl=0);
  TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QString txt, QColor clTxt, QRect rct, int fl=0);
  virtual ~TDrawElement();

  // Функции чтения-записи
  int id() const;                      // id элемента рисования
  void setId(int id);
  int idHint() const;                  // id подписываемого элемента (только для detHint)
  void setIdHint(int id);
  TDrawLayer *drawLayer() const;       // Слой, в котором находится элемент
  void setDrawLayer(TDrawLayer *lay);
  TDrawElementType type() const;       // Тип элемента рисования
  void setType(TDrawElementType t);
  QColor &clContour() const;           // Цвет контура (текста) - яркий
  void setClContour(QColor clr);
  QColor &clContourLess() const;       // Цвет контура (текста) - бледный
  void setClContourLess(QColor clr);
  QColor &clFill() const;              // Цвет заливки
  void setClFill(QColor clr);
  QColor &clShadow() const;            // Цвет тени
  void setClShadow(QColor clr);
  int lineWidth() const;               // Толщина линии
  void setLineWidth(int );
  Qt::PenStyle penStyle() const;       // Стиль пера
  void setPenStyle(Qt::PenStyle ps);
  Qt::BrushStyle brushStyle() const;   // Стиль кисти
  void setBrushStyle(Qt::BrushStyle bs);
  TPointList &points() const;          // Список координат для отрисовки элемента
  void insertPoint(QPoint *pnt);
  void clearPoints();
  QFont &fnt() const;                  // Шрифт
  void setFnt(QFont fnt);
  QString &text() const;               // Текст надписи (название картинки)
  void setText(QString t);
  int flags() const;                   // Флаги
  void setFlags(int fl);
  QRect &rect() const;                 // Прямоугольник для рисования текста (занимаемый элементом, ...)
  virtual void setRect(QRect r);
  virtual void setRect(QPoint p1, QPoint p2);
  bool isBright() const;               // Переключатель рисования объекта ярким (true) или бледным (false) цветом
  void setIsBright(bool br);
  bool isShadow() const;               // Переключатель тени
  void setIsShadow(bool sh);
  double Kless() const;                // Коэффициент уменьшения яркости (по умолчанию в три раза == 1/3.)
  void setKless(double k);
  QString &hint() const;               // Текст подсказки
  void setHint(QString hnt);
  QPoint &hintPoint() const;           // Точка привязки подсказки
  void setHintPoint(QPoint pnt);
  bool isHintVisible() const;          // Признак видимости подсказки
  void setHintVisible(bool vs);
  bool isChecked() const;              // Признак отметки
  void setChecked(bool chk);
  bool isVisible() const;              // Признак видимости
  void setVisible(bool vis);
  TIdent &ident() const;               // Идентификатор (для связи с внешними объектами)
  void setIdent(const TIdent *idnt);
  void setIdent(int i=0, int n=0, QString nm="", int tg=0);

  // Другие функции
  void reset();                        // Cброс в исходное состояние
  virtual void draw(QPainter &p);      // Отрисовка элемента
  void moveTop();                      // Перемещение вверх в своем слое
  QPoint minXminY(int dx=0, int dy=0); // Определение точки с минимальным X и минимальным Y (при необходимости добавить приращение)
  QPoint minXmaxY(int dx=0, int dy=0); // Определение точки с минимальным X и максимальным Y
  QPoint maxXminY(int dx=0, int dy=0); // Определение точки с максимальным X и минимальным Y
  QPoint maxXmaxY(int dx=0, int dy=0); // Определение точки с максимальным X и максимальным Y
  QPoint center(int dx=0, int dy=0);   // Определение центральной точки
  bool isMyCoord(QPoint pnt);          // Принадлежит ли точка элементу рисования
  bool inRect(QRect rct);              // Касается ли элемент рисования прямоугольника rct
  void setGeometry(int oldSc, int newSc, TZapas *zapas=NULL); // Изменение размеров и положения элемента при изменении масштаба
  QRect defPointsRect();               // Определение QRect по fArPoints
};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TDrawElement*> TDrawElementList;

// Сложный элемент рисования
class UKPCOMMONSHARED_EXPORT TDrawComplexElement : public TDrawElement
{
protected:
  bool                  fIsHtml;       // Признак формата HTML
  int                   fBrdWidth;     // Ширина границы вокруг текста подсказки
  QColor                fClText;       // Цвет текста (контур и заливка - fClContour, fClFill
  uint                  fScale;        // Масштаб рисунка, %

public :
  // Конструктор для подсказки TDrawElementType::detHint
  //
  //   4   5        6  7        8   9
  //    +--+--------+--+--------+--+
  //    |  |                    |  |
  // 23 +--+--------------------+--* 10
  //    |  | 0                1 |  |
  //    |  |                    |  |
  // 22 +  |                    |  + 11
  //    |  |        fRect       |  |
  // 21 +  |                    |  + 12
  //    |  |                    |  |
  //    |  | 3                2 |  |
  // 20 +--+--------------------+--* 13
  //    |  |                    |  |
  //    +--+--------+--+--------+--+
  //  19  18       17  16       15  14
  //
  TDrawComplexElement(int id, TDrawLayer *lay, TDrawElementType et, QString txt, QColor clTxt, QColor clCntr, QColor clFill, QRect rct, QPoint lnkpnt=QPoint(), bool isHtml=true);
  // Конструктор для подсказки TDrawElementType::detScaledPixmap
  TDrawComplexElement(int id, TDrawLayer *lay, TDrawElementType et, QString pixnm, QPoint pnt, uint scl=100, int fl=0);
  virtual ~TDrawComplexElement();

  bool isHtml() const;                 // Признак формата HTML
  void setHtml(bool ih);
  int brdWidth() const;                // Ширина границы вокруг текста подсказки
  void setBrdWidth(int w);
  QColor &clText() const;              // Цвет текста (контур и заливка - fClContour, fClFill
  void setClText(QColor clr);
  uint scale() const;                  // Масштаб рисунка, %
  void setScale(uint sc);

  virtual void setRect(QRect r);
  virtual void setRect(QPoint p1, QPoint p2);

  void reset();                        // Cброс в исходное состояние
  virtual void draw(QPainter &p);      // Отрисовка элемента

};

// Слой рисования
class UKPCOMMONSHARED_EXPORT TDrawLayer : public QObject
{
  Q_OBJECT
private :
  int                   fNum;          // Номер
  QString               fName;         // Hаименование
  int                   fISort;        // Поле для сортировки: целое
  TDrawElementList      fPlDrawEls;   // Список элементов рисования
  bool                  fIsVisible;    // Признак видимости слоя
public :
  TDrawLayer(int n=0, QString nm="", int is=0);
  ~TDrawLayer();

  // Функции чтения-записи
  int num() const;                     // Номер
  void setNum(int n);
  QString &name() const;               // Hаименование
  void setName(QString nm);
  int iSort() const;                   // Целое поле для сортировки
  void setISort(int is);
  TDrawElementList &els() const;       // Список элементов рисования
  bool insertEl(TDrawElement *el);     // Добавить элемент рисования
  bool removeEl(TDrawElement *el);     // Удалить элемент рисования
  void clearEls();                     // Очистить список элементов рисования
  TDrawElement *findEl(int id);        // Поиск элемента рисования по id
  TDrawElement *findEl(TIdent idnt);   // Поиск элемента рисования по данным идентификатора
  TDrawElement *findEl(TDrawElementType tp, TIdent idnt); // Поиск элемента рисования по типу и данным идентификатора
  bool isVisible() const;              // Признак видимости слоя
  void setIsVisible(bool vs);

  // Другие функции
  void reset();                        // Cброс в исходное состояние
  void findEls(QPoint pnt, TDrawElementList &pl); // Поиск элементов рисования в точке
};

typedef UKPCOMMONSHARED_EXPORT QPtrList<TDrawLayer*> TDrawLayerList;

// Самоотрисовывающаяся панель
class UKPCOMMONSHARED_EXPORT TSelfDrawWidget : public QWidget
{
  Q_OBJECT
protected :
  TDrawLayerList        fPlLayers,     // Список всех слоев
                        fPlVisLayers;  // Список видимых слоев
  QColor                fClrBase,      // Цвета: фона
                        fClrText;      //        текста

  TDrawLayer *findVisLayer(int n);     // Поиск видимого слоя
  void refreshVisLayers();             // Перезанести слои в список видимых (в нужной последовательности)

public :
  TSelfDrawWidget(const QList<TLayerNum> &lays, QColor clrbs, QColor clrtxt, const QStringList *names=NULL, QWidget *parent=0, int w=0, int h=0, Qt::WindowFlags f=0);
  ~TSelfDrawWidget();

  // Функции чтения-записи
  TDrawLayerList &layers() const;      // Список слоев
  TDrawLayer *findLayer(int n);        // Поиск слоя
  TDrawLayerList &visLayers() const;   // Список видимых слоев
  void showLayer(int n);               // Добавить слой номер n к видимым
  void showLayers(QList<int> &vlNums); // Сделать слои видимыми
  void showLayers();                   // Сделать все слои видимыми
  void hideLayer(int n);               // Удалить слой номер n из видимых
  void hideLayers(QList<int> &vlNums); // Удалить видимые слои
  void hideLayers();                   // Удалить все видимые слои
  void moveUp(int n);                  // Сделать слой номер n верхним (последним в списке)
  void moveDown(int n);                // Сделать слой номер n нижним (первым в списке)
  void prepSortLayers(QList<int> &vlNums); // Подготовить слои к сортировке (setISort(from vlNums))
  void sortLayers();                   // Упорядочить видимые слои (по iSort())
  bool insertEl(int n, TDrawElement *el); // Добавить элемент рисования на слой номер n
  bool removeEl(TDrawElement *el);     // Удалить элемент рисования
  bool clearLayer(int n);              // Очистить слой номер n
  void clearLayers();                  // Очистить все слои
  TDrawElement *findEl(int id, bool invislays=false); // Поиск элемента рисования по id (по умолчанию - во всех слоях)
  TDrawElement *findEl(TIdent idnt, bool invislays=false); // Поиск элемента рисования по данным идентификатора (по умолчанию - во всех слоях)
  TDrawElement *findEl(TDrawElementType tp, TIdent idnt, bool invislays=false); // Поиск элемента рисования по типу и данным идентификатора (по умолчанию - во всех слоях)
  QColor clrBase() const;              // Цвета: фона
  QColor clrText() const;              //        текста

  // Другие функции
  void setColor_contour(QColor clr, bool invislays=false); // Установление единого цвета контура
  void setColor_fill(QColor clr, bool invislays=false); // ... цвета заливки
  void setLineWidth(int w, bool invislays=false); // ... толщины линии
  void setPenStyle(Qt::PenStyle ps, bool invislays=false); // ... стиля пера
  void setBrushStyle(Qt::BrushStyle bs, bool invislays=false); // ... стиля кисти
  void recreateLayers(QList<int> &lays, QStringList *names=NULL);
  void reset();                        // Cброс в исходное состояние
  void findEls(QPoint pnt, TDrawElementList &pl, bool onall=false); // Поиск элементов рисования в точке на всех слоях или только на видимых (по умолчанию)
  void findEls(QPoint pnt, int n, TDrawElementList &pl); // Поиск элементов рисования в точке на слое n
  int maxId();                         // Определение максимального id
  void reflectLayersToTree(QTreeWidget &tw); // Отображение в QTreeWidget слоев

protected :

  // Обработчики событий
  virtual void paintEvent(QPaintEvent *e);            // Перерисовка панели
  virtual void mousePressEvent(QMouseEvent *e);       // Нажатие кнопки мыши
  virtual void mouseReleaseEvent(QMouseEvent *e);     // Отпускание кнопки мыши
  virtual void mouseDoubleClickEvent(QMouseEvent *e); // Двойной клик
  virtual void mouseMoveEvent(QMouseEvent *e);        // Перемещение мыши
  virtual void wheelEvent(QWheelEvent *e);            // Прокрутка колесика
  virtual void keyPressEvent(QKeyEvent *e);           // Нажатие клавиши
  virtual void keyReleaseEvent(QKeyEvent *e);         // Отпускание клавиши
  virtual void focusInEvent(QFocusEvent *e);          // Получение фокуса (+ setFocusPolicy() и  setFocus())
  virtual void focusOutEvent(QFocusEvent *e);         // Потеря фокуса
  virtual void enterEvent(QEvent *e);                 // Курсор находится в компоненте
  virtual void leaveEvent(QEvent *e);                 // Курсор уходит из компонента
  virtual void resizeEvent(QResizeEvent *e);          // Изменение размеров
  virtual void closeEvent(QCloseEvent *e);            // Закрытие
  virtual void showEvent(QShowEvent *e);              // Показ
  virtual void hideEvent(QHideEvent *e);              // Скрытие
};

#endif // SELFDRAWWIDGETH
