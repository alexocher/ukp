
//***************************************************************************//
//                                                                           //
//                Реализация базовых классов рисования схемы                 //
//                                                                           //
//***************************************************************************//

#include <math.h>
#include <QPixmap>
#include <QPaintEvent>
#include <QTextDocument>
#include <defMacro>
#include <defConst>
#include <gen>
#include <geod_base>
#include <TSelfDrawWidget>

namespace
{

    QColor pink(255,210,210),
           CLR_BASE(0,0,0),
           CLR_TEXT(255,255,255);

    const int BRD_WIDTH(15);

}

QString enumToStr(TLayerNum ln)
{
    switch (ln)
    {
        case lrnFon       : return QString("Фон");
        case lrnGrid      : return QString("Коорд. сетка");
        case lrnGridLabels: return QString("Метки сетки");
        case lrnRgns      : return QString("Глоб. районы");
        case lrnOrients   : return QString("Ориентиры");
        case lrnGeos      : return QString("Пункты ГС");
        case lrnRouts     : return QString("Маршруты");
        case lrnTargs     : return QString("Цели");
        case lrnRubs      : return QString("Рубежи");
        case lrnOgzRgns   : return QString("Районы ОгЗ");
        case lrnMeetPoints: return QString("Точки встречи");
        case lrnTrgRgns   : return QString("Участки целей");
        case lrnRubRgns   : return QString("Участки руб.");
        case lrnFirePoss  : return QString("Позиции СП");
        case lrnSars      : return QString("САР");
        case lrnNps       : return QString("НП");
        case lrnTrasses   : return QString("Траектории");
        case lrnLines     : return QString("Линии");
        case lrnTexts1    : return QString("Подписи (1)");
        case lrnTexts2    : return QString("Подписи (2)");
        case lrnTexts3    : return QString("Подписи (3)");
        case lrnHints     : return QString("Подсказки");
        case lrnOurForms  : return QString("Свои войска");
        case lrnEnemyForms: return QString("Войска прот.");
        case lrnRazrs     : return QString("Разрывы");
        case lrnStOrients : return QString("Ориентиры нач.");
        case lrnBu        : return QString("Условия безопасности");
        case lrnHelpLines : return QString("Вспомогательные линии");
        case lrnZass      : return QString("Точки и линии засечки");
        case lrnTmp       : return QString("Временные объекты");
        default : return "";
    }
}
//-----------------------------------------------------------------------------

//****************************************************************************
// Структура TZapas - запас для вывода надписей

// КОНСТРУКТОР
TZapas::TZapas(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b), width(left+right), height(top+bottom)
{
}
//-----------------------------------------------------------------------------

void TZapas::setZapas(int l, int t, int r, int b)
{
    left = l; top = t; right = r; bottom = b;
    width = left+right;  height = top+bottom;
}
//-----------------------------------------------------------------------------

//****************************************************************************
// Kласс TDrawElement - элемент рисования

// КОНСТРУКТОРЫ ОБЩЕГО НАЗНАЧЕНИЯ
TDrawElement::TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QPoint pnt, Qt::PenStyle ps, int lw, Qt::BrushStyle bs) : fId(id), fIdHint(0), fDrawLayer(lay), fType(et), fIdent(new TIdent()), fClContour(CLR_TEXT), fClFill(CLR_BASE), fClShadow(Qt::gray), fLineWidth(lw), fPenStyle(ps), fBrushStyle(bs), fText(""), fFlags(0), fIsBright(true), fIsShadow(false), fIsHintVisible(false), fIsChecked(false), fIsVisible(true), fHint("")
{
    fPlPoints.setAutoDelete(true);
    switch (fType)
    {
        case detEllipce: case detTextPoint: case detTextRect: case detRect: case detRoundRect: case detCheckBox: case detCheckCircle: case detCrossCircle: case detHint: case detPixmap: case detScaledPixmap:
            fPlPoints.append(new QPoint(pnt)); break;
        case detPolygon3:
            fPlPoints.append(new QPoint(pnt)); break;
            fPlPoints.append(new QPoint(pnt)); break;
            fPlPoints.append(new QPoint(pnt)); break;
        default : ;
    }
    setKless(1/3.); // fKless, fClContourLess
}
//-----------------------------------------------------------------------------

TDrawElement::TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QColor clCntr, QColor clFill, QPoint pnt, Qt::PenStyle ps, int lw, Qt::BrushStyle bs) : fId(id), fIdHint(0), fDrawLayer(lay), fType(et), fIdent(new TIdent()), fClContour(clCntr), fClFill(clFill), fClShadow(Qt::gray), fLineWidth(lw), fPenStyle(ps), fBrushStyle(bs), fText(""), fFlags(0), fIsBright(true), fIsShadow(false), fIsHintVisible(false), fIsChecked(false), fIsVisible(true), fHint("")
{
    fPlPoints.setAutoDelete(true);
    switch (fType)
    {
        case detEllipce: case detTextPoint: case detTextRect: case detRect: case detRoundRect: case detCheckBox: case detCheckCircle: case detCrossCircle: case detHint: case detPixmap: case detScaledPixmap:
            fPlPoints.append(new QPoint(pnt)); break;
        case detPolygon3:
            fPlPoints.append(new QPoint(pnt)); break;
            fPlPoints.append(new QPoint(pnt)); break;
            fPlPoints.append(new QPoint(pnt)); break;
        default : ;
    }
    setKless(1/3.); // fKless, fClContourLess
}
//-----------------------------------------------------------------------------

TDrawElement::TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QRect rct, Qt::PenStyle ps, int lw, Qt::BrushStyle bs) : fId(id), fIdHint(0), fDrawLayer(lay), fType(et), fIdent(new TIdent()), fClContour(CLR_TEXT), fClFill(CLR_BASE), fClShadow(Qt::gray), fLineWidth(lw), fPenStyle(ps), fBrushStyle(bs), fText(""), fFlags(0), fIsBright(true), fIsShadow(false), fIsHintVisible(false), fIsChecked(false), fIsVisible(true), fHint("")
{
    fPlPoints.setAutoDelete(true);
    setRect(rct); // fRect, fArPoints
    setKless(1/3.); // fKless, fClContourLess
}
//-----------------------------------------------------------------------------

TDrawElement::TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QColor clCntr, QColor clFill, QRect rct, Qt::PenStyle ps, int lw, Qt::BrushStyle bs) : fId(id), fIdHint(0), fDrawLayer(lay), fType(et), fIdent(new TIdent()), fClContour(clCntr), fClFill(clFill), fClShadow(Qt::gray), fLineWidth(lw), fPenStyle(ps), fBrushStyle(bs), fText(""), fFlags(0), fIsBright(true), fIsShadow(false), fIsHintVisible(false), fIsChecked(false), fIsVisible(true), fHint("")
{
    fPlPoints.setAutoDelete(true);
    setRect(rct); // fRect, fArPoints
    setKless(1/3.); // fKless, fClContourLess
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОРЫ ДЛЯ ТЕКСТОВЫХ ЭЛЕМЕНТОВ
TDrawElement::TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QString txt, QPoint pnt, int fl) : fId(id), fIdHint(0), fDrawLayer(lay), fType(et), fIdent(new TIdent()), fClContour(CLR_TEXT), fClFill(CLR_BASE), fClShadow(Qt::gray), fLineWidth(1), fPenStyle(Qt::SolidLine), fBrushStyle(Qt::NoBrush), fText(txt), fFlags(fl), fIsBright(true), fIsShadow(false), fIsHintVisible(false), fIsChecked(false), fIsVisible(true), fHint("")
{
    fPlPoints.setAutoDelete(true);
    switch (fType)
    {
        case detEllipce: case detTextPoint: case detTextRect: case detRect: case detRoundRect: case detCheckBox: case detCheckCircle: case detCrossCircle: case detHint: case detPixmap: case detScaledPixmap:
            fPlPoints.append(new QPoint(pnt)); break;
        case detPolygon3:
            fPlPoints.append(new QPoint(pnt)); break;
            fPlPoints.append(new QPoint(pnt)); break;
            fPlPoints.append(new QPoint(pnt)); break;
        default : ;
    }
    setKless(1/3.); // fKless, fClContourLess
}
//-----------------------------------------------------------------------------

TDrawElement::TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QString txt, QColor clTxt, QPoint pnt, int fl) : fId(id), fIdHint(0), fDrawLayer(lay), fType(et), fIdent(new TIdent()), fClContour(clTxt), fClFill(CLR_BASE), fClShadow(Qt::gray), fLineWidth(1), fPenStyle(Qt::SolidLine), fBrushStyle(Qt::NoBrush), fText(txt), fFlags(fl), fIsBright(true), fIsShadow(false), fIsHintVisible(false), fIsChecked(false), fIsVisible(true), fHint("")
{
    fPlPoints.setAutoDelete(true);
    switch (fType)
    {
        case detEllipce: case detTextPoint: case detTextRect: case detRect: case detRoundRect: case detCheckBox: case detCheckCircle: case detCrossCircle: case detHint: case detPixmap: case detScaledPixmap:
            fPlPoints.append(new QPoint(pnt)); break;
        case detPolygon3:
            fPlPoints.append(new QPoint(pnt)); break;
            fPlPoints.append(new QPoint(pnt)); break;
            fPlPoints.append(new QPoint(pnt)); break;
        default : ;
    }
    setKless(1/3.); // fKless, fClContourLess
}
//-----------------------------------------------------------------------------

TDrawElement::TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QString txt, QRect rct, int fl) : fId(id), fIdHint(0), fDrawLayer(lay), fType(et), fIdent(new TIdent()), fClContour(CLR_TEXT), fClFill(CLR_BASE), fClShadow(Qt::gray), fLineWidth(1), fPenStyle(Qt::SolidLine), fBrushStyle(Qt::NoBrush), fText(txt), fFlags(fl), fIsBright(true), fIsShadow(false), fIsHintVisible(false), fIsChecked(false), fIsVisible(true), fHint("")
{
    fPlPoints.setAutoDelete(true);
    setRect(rct); // fRect, fArPoints
    setKless(1/3.); // fKless, fClContourLess
}
//-----------------------------------------------------------------------------

TDrawElement::TDrawElement(int id, TDrawLayer *lay, TDrawElementType et, QString txt, QColor clTxt, QRect rct, int fl)
{
    fPlPoints.setAutoDelete(true);
    fId = id;
    fDrawLayer = lay;
    fIdHint = 0;
    fType = et;
    fIdent = new TIdent();
    fClContour = clTxt;
    fClFill = CLR_BASE;
    fClShadow = Qt::gray;
    fLineWidth = 1;
    fPenStyle = Qt::SolidLine;
    fBrushStyle = Qt::NoBrush;
    fText = txt;
    fFlags = fl;
    setRect(rct); // + fArPoints
    fHintPoint = QPoint();
    fIsBright = true;
    fIsShadow = false;
    fIsHintVisible = false;
    fIsChecked = false;
    fIsVisible = true;
    fHint = "";

    setKless(1/3.); // fKless, fClContourLess
}
//-----------------------------------------------------------------------------

// ДЕСТРУКТОР
TDrawElement::~TDrawElement()
{
    DELETE(fIdent);
}
//-----------------------------------------------------------------------------

// Cброс в исходное состояние
void TDrawElement::reset()
{
    fId = 0;
    fIdHint = 0;
    fDrawLayer = NULL;
    fType = detNone;
    fIdent->setIdent();
    fClContour = CLR_TEXT;
    fClFill = CLR_BASE;
    fClShadow = Qt::gray;
    fLineWidth = 1;
    fPenStyle = Qt::SolidLine;
    fBrushStyle = Qt::NoBrush;
    //QFont fFnt
    fText = "";
    fFlags = 0;
    fPlPoints.clear();
    fRect = QRect();
    fHintPoint = QPoint();
    fIsBright = true;
    fIsShadow = false;
    fIsHintVisible = false;
    fIsChecked = false;
    fIsVisible = true;
    setKless(1/3.); // fKless, fClContourLess
    fHint = "";
}
//-----------------------------------------------------------------------------

// id элемента рисования
int TDrawElement::id() const
{
    return fId;
}
//-----------------------------------------------------------------------------

void TDrawElement::setId(int id)
{
    fId = id;
}
//-----------------------------------------------------------------------------

// id подписываемого элемента (только для detHint)
int TDrawElement::idHint() const
{
    return fIdHint;
}
//-----------------------------------------------------------------------------

void TDrawElement::setIdHint(int id)
{
    fIdHint = id;
}
//-----------------------------------------------------------------------------

// Слой, в котором находится элемент
TDrawLayer *TDrawElement::drawLayer() const
{
    return fDrawLayer;
}
//-----------------------------------------------------------------------------

void TDrawElement::setDrawLayer(TDrawLayer *lay)
{
    fDrawLayer = lay;
}
//-----------------------------------------------------------------------------

// Тип элемента рисования
TDrawElementType TDrawElement::type() const
{
    return fType;
}
//-----------------------------------------------------------------------------

void TDrawElement::setType(TDrawElementType t)
{
    fType = t;
}
//-----------------------------------------------------------------------------

// Цвет контура (текста) - яркий
QColor &TDrawElement::clContour() const
{
    return const_cast<QColor&>(fClContour);
}
//-----------------------------------------------------------------------------

void TDrawElement::setClContour(QColor clr)
{
    fClContour = clr;
}
//-----------------------------------------------------------------------------

// Цвет контура (текста) - бледный
QColor &TDrawElement::clContourLess() const
{
    return const_cast<QColor&>(fClContourLess);
}
//-----------------------------------------------------------------------------

void TDrawElement::setClContourLess(QColor clr)
{
    fClContourLess = clr;
}
//-----------------------------------------------------------------------------

// Цвет заливки
QColor &TDrawElement::clFill() const
{
    return const_cast<QColor&>(fClFill);
}
//-----------------------------------------------------------------------------

void TDrawElement::setClFill(QColor clr)
{
    fClFill = clr;
}
//-----------------------------------------------------------------------------

// Цвет тени
QColor &TDrawElement::clShadow() const
{
    return const_cast<QColor&>(fClShadow);
}
//-----------------------------------------------------------------------------

void TDrawElement::setClShadow(QColor clr)
{
    fClShadow = clr;
}
//-----------------------------------------------------------------------------

// Толщина линии
int TDrawElement::lineWidth() const
{
    return fLineWidth;
}
//-----------------------------------------------------------------------------

void TDrawElement::setLineWidth(int lw)
{
    fLineWidth = lw;
}
//-----------------------------------------------------------------------------

// Стиль пера
Qt::PenStyle TDrawElement::penStyle() const
{
    return fPenStyle;
}
//-----------------------------------------------------------------------------

void TDrawElement::setPenStyle(Qt::PenStyle ps)
{
    fPenStyle = ps;
}
//-----------------------------------------------------------------------------

// Стиль кисти
Qt::BrushStyle TDrawElement::brushStyle() const
{
    return fBrushStyle;
}
//-----------------------------------------------------------------------------

void TDrawElement::setBrushStyle(Qt::BrushStyle bs)
{
    fBrushStyle = bs;
}
//-----------------------------------------------------------------------------

// Список координат для отрисовки элемента
TPointList &TDrawElement::points() const
{
    return const_cast<TPointList&>(fPlPoints);
}
//-----------------------------------------------------------------------------

void TDrawElement::insertPoint(QPoint *pnt)
{
    if (pnt) fPlPoints.append(pnt);
}
//-----------------------------------------------------------------------------

void TDrawElement::clearPoints()
{
    fPlPoints.clear();
}
//-----------------------------------------------------------------------------

// Шрифт
QFont &TDrawElement::fnt() const
{
    return const_cast<QFont&>(fFnt);
}
//-----------------------------------------------------------------------------

void TDrawElement::setFnt(QFont f)
{
    fFnt = f;
}
//-----------------------------------------------------------------------------

// Текст надписи
QString &TDrawElement::text() const
{
    return const_cast<QString&>(fText);
}
//-----------------------------------------------------------------------------

void TDrawElement::setText(QString t)
{
    fText = t;
}
//-----------------------------------------------------------------------------

// Флаги
int TDrawElement::flags() const
{
    return fFlags;
}
//-----------------------------------------------------------------------------

void TDrawElement::setFlags(int fl)
{
    fFlags = fl;
}
//-----------------------------------------------------------------------------

// Прямоугольник для рисования текста (занимаемый элементом, ...)
QRect &TDrawElement::rect() const
{
    return const_cast<QRect&>(fRect);
}
//-----------------------------------------------------------------------------

void TDrawElement::setRect(QRect r)
{
    fRect = r;
    fPlPoints.clear();
    switch (fType)
    {
        case detEllipce: case detTextRect: case detRect: case detRoundRect: case detCheckCircle: case detCrossCircle: case detHint:
            fPlPoints.append(new QPoint(fRect.center()));
            break;
        case detPolygon3:
        {
          int d(fRect.width()/2), d1(gen::round(d*3./5.));
          QPoint pntCntr(fRect.center());
            fPlPoints.append(new QPoint(pntCntr+QPoint(0,-d)));
            fPlPoints.append(new QPoint(pntCntr+QPoint(d,d1)));
            fPlPoints.append(new QPoint(pntCntr+QPoint(-d,d1)));
            break;
        }
        default : ;
    }
}
//-----------------------------------------------------------------------------

void TDrawElement::setRect(QPoint p1, QPoint p2)
{
    setRect(QRect(p1,p2));
}
//-----------------------------------------------------------------------------

// Переключатель рисования объекта ярким (true) или бледным (false) цветом
bool TDrawElement::isBright() const
{
    return fIsBright;
}
//-----------------------------------------------------------------------------

void TDrawElement::setIsBright(bool br)
{
    fIsBright = br;
}
//-----------------------------------------------------------------------------

// Переключатель тени
bool TDrawElement::isShadow() const
{
    return fIsShadow;
}
//-----------------------------------------------------------------------------

void TDrawElement::setIsShadow(bool sh)
{
    fIsShadow = sh;
}
//-----------------------------------------------------------------------------

// Коэффициент уменьшения яркости (по умолчанию в два раза == 0.5)
double TDrawElement::Kless() const
{
    return fKless;
}
//-----------------------------------------------------------------------------

void TDrawElement::setKless(double k)
{
  int h,s,v;

    if (k<0.) k=0.; if (k>1.) k=1.;
    fKless = k;
    fClContour.getHsv(&h,&s,&v);
    fClContourLess.setHsv(h,gen::round(s*fKless),v);
}
//-----------------------------------------------------------------------------

// Текст подсказки
QString &TDrawElement::hint() const
{
    return const_cast<QString&>(fHint);
}
//-----------------------------------------------------------------------------

void TDrawElement::setHint(QString hnt)
{
    fHint = hnt;
}
//-----------------------------------------------------------------------------

// Точка привязки подсказки
QPoint &TDrawElement::hintPoint() const
{
    return const_cast<QPoint&>(fHintPoint);
}
//-----------------------------------------------------------------------------

void TDrawElement::setHintPoint(QPoint pnt)
{
    fHintPoint = pnt;
}
//-----------------------------------------------------------------------------

// Признак видимости подсказки
bool TDrawElement::isHintVisible() const
{
    return fIsHintVisible;
}
//-----------------------------------------------------------------------------

void TDrawElement::setHintVisible(bool vs)
{
    fIsHintVisible = vs;
}
//-----------------------------------------------------------------------------

// Признак отметки
bool TDrawElement::isChecked() const
{
    return fIsChecked;
}
//-----------------------------------------------------------------------------

void TDrawElement::setChecked(bool chk)
{
    fIsChecked = chk;
}
//-----------------------------------------------------------------------------

// Признак видимости
bool TDrawElement::isVisible() const
{
    return fIsVisible;
}
//-----------------------------------------------------------------------------

void TDrawElement::setVisible(bool vis)
{
    fIsVisible = vis;
}
//-----------------------------------------------------------------------------

// Идентификатор (для связи с внешними объектами)
TIdent &TDrawElement::ident() const
{
    return *fIdent;
}
//-----------------------------------------------------------------------------

void TDrawElement::setIdent(const TIdent *idnt)
{
    if (!idnt) { fIdent->setIdent(); return; }
    fIdent->id = idnt->id;
    fIdent->num = idnt->num;
    fIdent->name = idnt->name;
    fIdent->tag = idnt->tag;
}
//-----------------------------------------------------------------------------

void TDrawElement::setIdent(int i, int n, QString nm, int tg)
{
    fIdent->setIdent(i,n,nm,tg);
}
//-----------------------------------------------------------------------------

// Отрисовка элемента
// fRect и fArPoints, как правило, д.б. заданы
void TDrawElement::draw(QPainter &p)
{
  QPen pen(p.pen());
  QBrush brush(p.brush());
  int dx(0), dy(0);
  const int kolPoints = fPlPoints.size();

    pen.setWidth(fLineWidth);
    pen.setStyle(fPenStyle);
    pen.setColor(fIsBright ? fClContour : fClContourLess);
    p.setPen(pen);
    brush.setStyle(fBrushStyle);
    brush.setColor(fClFill);
    p.setBrush(brush);
    p.setFont(fFnt);
    switch (fType)
    {
        case detLomLine:
            for (int i=0; i<kolPoints-1; i++)
                p.drawLine(*fPlPoints.at(i),*fPlPoints.at(i+1));
            break;
        case detPolyline:
            if (fPlPoints.size())
            {
                QVector<QPoint> vPoints(kolPoints);
                  for (int i=0; i<kolPoints; i++)
                      vPoints[i] = *fPlPoints.at(i);
                  p.drawPolyline(QPolygon(vPoints));
            }
            break;
        case detPolygon:
        case detPolygon3:
            if (kolPoints)
            {
              QVector<QPoint> vPoints(kolPoints);
                for (int i=0; i<kolPoints; i++)
                    vPoints[i] = *fPlPoints.at(i);
                p.drawPolygon(QPolygon(vPoints));
            }
            break;
        case detEllipce:
            //brush.setStyle(Qt::SolidPattern);
            //brush.setColor(Qt::white);
            p.setBrush(brush);
            p.drawEllipse(fRect);
            brush.setStyle(fBrushStyle);
            brush.setColor(fClFill);
            p.setBrush(brush);
            p.drawEllipse(fRect);
            break;
        case detTextPoint:
            if (kolPoints)
            {
              QFontMetrics fm(p.font());
              QRect r = fm.boundingRect(fText); // QSize sz = fm.size(fFlags,fText);
              QSize sz = r.size();
                if (fFlags & Qt::AlignLeft) dx = 0;
                else if (fFlags & Qt::AlignRight) dx = -sz.width();
                else dx = -sz.width()/2; // fFlags & Qt::AlignHCenter
                if (fFlags & Qt::AlignTop) dy = sz.height();
                else if (fFlags & Qt::AlignBottom) dy = 0;
                else dy = sz.height()/2; // fFlags & Qt::AlignVCenter
              QPoint pnt(*fPlPoints.first()+QPoint(dx,dy));
                p.drawText(pnt,fText);
                fRect = QRect(pnt-QPoint(0,sz.height()),sz);
            }
            break;
        case detTextRect:
            p.drawText(fRect,fFlags,fText);
            break;
        case detRect:
            p.drawRect(fRect);
            break;
        case detRoundRect:
            p.drawRoundRect(fRect);
            break;
        case detCheckBox:
            if (kolPoints)
            {
              QFontMetrics fm(p.font());
              QRect r = fm.boundingRect(fText), rr;
              int hText = r.height(), wText = r.width();
              QPoint pnt(*fPlPoints.first()+QPoint(0,hText>DREL_CHCK_SIDE ? hText-DREL_CHCK_SIDE : 0)); // Квадратик с (без) галочкой
                brush.setStyle(Qt::SolidPattern);
                p.setBrush(brush);
                rr = QRect(pnt,pnt+QPoint(DREL_CHCK_SIDE-1,DREL_CHCK_SIDE-1));
                p.drawRect(rr);
                pnt += QPoint(DREL_CHCK_SIDE/2,DREL_CHCK_SIDE/2); // центр квадрата
                if (fIsChecked) // Галочка
                {
                    p.drawLine(QPoint(pnt+QPoint(-4,0)),QPoint(pnt+QPoint(-1,3)));
                    p.drawLine(QPoint(pnt+QPoint(-4,-1)),QPoint(pnt+QPoint(-1,2)));
                    p.drawLine(QPoint(pnt+QPoint(-3,-1)),QPoint(pnt+QPoint(-1,1)));
                    p.drawLine(QPoint(pnt+QPoint(-1,1)),QPoint(pnt+QPoint(4,-4)));
                    p.drawLine(QPoint(pnt+QPoint(-1,2)),QPoint(pnt+QPoint(4,-3)));
                    p.drawLine(QPoint(pnt+QPoint(-1,3)),QPoint(pnt+QPoint(4,-2)));
                }
                pen.setColor(fIsBright ? fClContour : fClContourLess);
                p.setPen(pen);
                pnt = *fPlPoints.first()+QPoint(DREL_CHCK_SIDE+DREL_TEXT_SPACE,hText>DREL_CHCK_SIDE ? hText : DREL_CHCK_SIDE); // Текст в точке
                brush.setStyle(fBrushStyle);
                p.setBrush(brush);
                p.drawText(pnt,fText);
                fRect = QRect(*fPlPoints.first(),QSize(DREL_CHCK_SIDE+DREL_TEXT_SPACE+wText,hText)); // Расчет fRect
            }
            break;
        case detCheckCircle:
        {
            p.drawEllipse(fRect);
          QPoint pnt(fRect.center());
            fPlPoints.clear();
            fPlPoints.append(new QPoint(pnt));
            if (fIsChecked) // Галочка
            {
                p.drawLine(QPoint(pnt+QPoint(-4,0)),QPoint(pnt+QPoint(-1,3)));
                p.drawLine(QPoint(pnt+QPoint(-4,-1)),QPoint(pnt+QPoint(-1,2)));
                p.drawLine(QPoint(pnt+QPoint(-3,-1)),QPoint(pnt+QPoint(-1,1)));
                p.drawLine(QPoint(pnt+QPoint(-1,1)),QPoint(pnt+QPoint(4,-4)));
                p.drawLine(QPoint(pnt+QPoint(-1,2)),QPoint(pnt+QPoint(4,-3)));
                p.drawLine(QPoint(pnt+QPoint(-1,3)),QPoint(pnt+QPoint(4,-2)));
            }
            break;
        }
        case detCrossPolygon:
            if (kolPoints)
            {
              QRect rct = defPointsRect();
                pen.setColor(Qt::red);
                p.setPen(pen);
                brush.setColor(pink);
                brush.setStyle(Qt::SolidPattern);
              QVector<QPoint> vPoints(kolPoints);
                for (int i=0; i<kolPoints; i++)
                    vPoints[i] = *fPlPoints.at(i);
                p.setBrush(brush);
                p.drawPolygon(QPolygon(vPoints));
                rct.setTopLeft(rct.topLeft()+QPoint(-DREL_TEXT_OUTDRAW,-DREL_TEXT_OUTDRAW));
                rct.setBottomRight(rct.bottomRight()+QPoint(DREL_TEXT_OUTDRAW,DREL_TEXT_OUTDRAW));
                p.drawLine(rct.topLeft(),rct.bottomRight());
                p.drawLine(rct.bottomLeft(),rct.topRight());
            }
            break;
        case detCrossLomLine:
        {
          QRect rct = defPointsRect();
            pen.setColor(Qt::red);
            p.setPen(pen);
            rct.setTopLeft(rct.topLeft()+QPoint(-DREL_TEXT_OUTDRAW,-DREL_TEXT_OUTDRAW));
            rct.setBottomRight(rct.bottomRight()+QPoint(DREL_TEXT_OUTDRAW,DREL_TEXT_OUTDRAW));
            for (int i=0; i<kolPoints-1; i++)
                p.drawLine(*fPlPoints.at(i),*fPlPoints.at(i+1));
            p.drawLine(rct.topLeft(),rct.bottomRight());
            p.drawLine(rct.bottomLeft(),rct.topRight());
        }
        case detCrossCircle:
            pen.setColor(Qt::red);
            p.setPen(pen);
            brush.setColor(pink);
            brush.setStyle(Qt::SolidPattern);
            p.setBrush(brush);
            p.drawEllipse(fRect);
            p.drawLine(fRect.topLeft(),fRect.bottomRight());
            p.drawLine(fRect.topRight(),fRect.bottomLeft());
            break;
        case detHint: // ??? Сделать, чтобы fRect отобразился в видимой части виджета (реализация в самом виджете)
            if (kolPoints)
            {
              QFontMetrics fm(p.font());
              QSize sz = fm.size(fFlags,fText);
              QRect txtRect;
                txtRect = QRect(*fPlPoints.first()+QPoint(-sz.width()/2,-sz.height()/2),sz);
                fRect.setTopLeft(txtRect.topLeft()+QPoint(-DREL_TEXT_OUTDRAW,-DREL_TEXT_OUTDRAW)); // ??? запас по краям
                fRect.setBottomRight(txtRect.bottomRight()+QPoint(DREL_TEXT_OUTDRAW,DREL_TEXT_OUTDRAW));
                p.drawLine(*fPlPoints.first(),fHintPoint); // ??? пока просто линия (сделать сектор к центру одной из сторон)
                p.drawRoundRect(fRect);
                p.drawText(txtRect,fFlags,fText);
            }
            break;
        case detPixmap:
            if (kolPoints)
            {
              QPixmap pix(fText);
                if (!pix.isNull())
                {
                  QSize sz = pix.size();
                    if (fFlags & Qt::AlignLeft) dx = 0;
                    else if (fFlags & Qt::AlignRight) dx = -sz.width();
                    else dx = -sz.width()/2; // fFlags & Qt::AlignHCenter
                    if (fFlags &Qt:: AlignTop) dy = 0;
                    else if (fFlags & Qt::AlignBottom) dy = -sz.height();
                    else dy = -sz.height()/2; // fFlags & Qt::AlignVCenter
                  QPoint pnt(*fPlPoints.first()+QPoint(dx,dy));
                    p.drawPixmap(pnt,pix);
                    fRect = QRect(pnt,sz);
                }
            }
            break;
        default : ;
    }
}
//-----------------------------------------------------------------------------

// Перемещение вверх в своем слое
void TDrawElement::moveTop()
{
    if (!fDrawLayer) return;
  TDrawElementList &els = fDrawLayer->els();
    els.setAutoDelete(false);
    els.removeOne(this);
    els.append(this);
    els.setAutoDelete(true);
}
//-----------------------------------------------------------------------------

// Определение точки с минимальным X и минимальным Y (при необходимости добавить приращение)
QPoint TDrawElement::minXminY(int dx, int dy)
{
  int resX(MAX_LONG), resY(MAX_LONG);
    if (!fPlPoints.count()) return QPoint();
    foreach (QPoint *curPnt,fPlPoints)
    {
        if (curPnt->x()<resX) resX = curPnt->x();
        if (curPnt->y()<resY) resY = curPnt->y();
    }
    return QPoint(resX+dx,resY+dy);
}
//-----------------------------------------------------------------------------

// Определение точки с минимальным X и максимальным Y
QPoint TDrawElement::minXmaxY(int dx, int dy)
{
  int resX(MAX_LONG), resY(MIN_LONG);
    if (!fPlPoints.count()) return QPoint();
    foreach (QPoint *curPnt,fPlPoints)
    {
        if (curPnt->x()<resX) resX = curPnt->x();
        if (curPnt->y()>resY) resY = curPnt->y();
    }
    return QPoint(resX+dx,resY+dy);
}
//-----------------------------------------------------------------------------

// Определение точки с максимальным X и минимальным Y
QPoint TDrawElement::maxXminY(int dx, int dy)
{
  int resX(MIN_LONG), resY(MAX_LONG);
    if (!fPlPoints.count()) return QPoint();
    foreach (QPoint *curPnt,fPlPoints)
    {
        if (curPnt->x()>resX) resX = curPnt->x();
        if (curPnt->y()<resY) resY = curPnt->y();
    }
    return QPoint(resX+dx,resY+dy);
}
//-----------------------------------------------------------------------------

// Определение точки с максимальным X и максимальным Y
QPoint TDrawElement::maxXmaxY(int dx, int dy)
{
  int resX(MIN_LONG), resY(MIN_LONG);
    if (!fPlPoints.count()) return QPoint();
    foreach (QPoint *curPnt,fPlPoints)
    {
        if (curPnt->x()>resX) resX = curPnt->x();
        if (curPnt->y()>resY) resY = curPnt->y();
    }
    return QPoint(resX+dx,resY+dy);
}
//-----------------------------------------------------------------------------

// Определение центральной точки
QPoint TDrawElement::center(int dx, int dy)
{
  int minX(MAX_LONG), minY(MAX_LONG), maxX(MIN_LONG), maxY(MIN_LONG);
    if (!fPlPoints.count()) return QPoint();
    foreach (QPoint *curPnt,fPlPoints)
    {
        if (curPnt->x()<minX) minX = curPnt->x();
        if (curPnt->y()<minY) minY = curPnt->y();
        if (curPnt->x()>maxX) maxX = curPnt->x();
        if (curPnt->y()>maxY) maxY = curPnt->y();
    }
    return QPoint((minX+maxX)/2+dx,(minY+maxY)/2+dy);
}
//-----------------------------------------------------------------------------

// Принадлежит ли точка элементу рисования
bool TDrawElement::isMyCoord(QPoint pnt)
{
  int kolPoints(fPlPoints.count()), pntX(pnt.x()), pntY(pnt.y()), x1(0), x2(0), y1(0), y2(0);
    switch (fType)
    {
        case detLomLine: case detCrossLomLine: // Точка на линии (fArPoints[n])
            for (int i=0; i<kolPoints-1; i++)
            {
                x1 = fPlPoints.at(i)->x(); x2 = fPlPoints.at(i+1)->x(); y1 = fPlPoints.at(i)->y(); y2 = fPlPoints.at(i+1)->y();
                if (abs((pntX-x1)*(y2-y1)-(pntY-y1)*(x2-x1))<=DREL_RECT_OUTCLICK) return true; // ??? д.б. == 0
            }
            return false;
        case detPolyline: case detPolygon: case detCrossPolygon: // ... внутри полигона (fArPoints[n])
        {
//          TCalcDblXY dpnt(pnt.x(),pnt.y());
//          TCalcDblXYList dpnts;
//            dpnts.setAutoDelete(true);
//            foreach (QPoint *curPnt,fPlPoints) dpnts.append(new TCalcDblXY(curPnt->x(),curPnt->y()));
//            return geom_base::isPntInPoly(dpnt,dpnts);
              return true; // ???
        }
        case detEllipce: case detCheckCircle: case detCrossCircle: // ... в радиусе точки fPlPoints[0]
            if (kolPoints)
            {
//              TCalcDblXY dpnt(pnt.x(),pnt.y()), dpnt0(fPlPoints.first()->x(),fPlPoints.first()->y());
//                return floor(geod_base::ogzd(dpnt,dpnt0))<(MAX2(fRect.width(),fRect.height())/2+DREL_RECT_OUTCLICK);
              return true; // ???
            }
            else return false;
        case detTextRect: case detRect: case detRoundRect: case detPixmap: case detScaledPixmap: // ... в fRect
            x1 = fRect.topLeft().x(); x2 = fRect.bottomRight().x(); y1 = fRect.topLeft().y(); y2 = fRect.bottomRight().y();
            return pntX>=x1 && pntX<=x2 && pntY>=y1 && pntY<=y2;
        case detTextPoint: case detCheckBox: case detHint: // ... в fRect + DREL_RECT_OUT
            x1 = fRect.topLeft().x()-DREL_RECT_OUTCLICK; x2 = fRect.bottomRight().x()+DREL_RECT_OUTCLICK; y1 = fRect.topLeft().y()-DREL_RECT_OUTCLICK; y2 = fRect.bottomRight().y()+DREL_RECT_OUTCLICK;
            return pntX>=x1 && pntX<=x2 && pntY>=y1 && pntY<=y2;
        default : return false; // detNone
    }
}
//-----------------------------------------------------------------------------

// Касается ли элемент рисования прямоугольника rct
bool TDrawElement::inRect(QRect rct)
{
    // return fRect.intersects(rct);
    return true;
    rct=rct;
}
//-----------------------------------------------------------------------------

// Изменение размеров и положения элемента при изменении масштаба
void TDrawElement::setGeometry(int oldSc, int newSc, TZapas *zapas)
{
    if (!oldSc || !newSc) return;

  double dScale = (double)newSc/oldSc, dXold, dYold, dXnew, dYnew;
  int zapLeft = zapas ? zapas->left : 0, zapTop = zapas ? zapas->top : 0;

    foreach (QPoint *curPnt,fPlPoints)
    {
        dXold = curPnt->x()-zapLeft; dYold = curPnt->y()-zapTop;
        dXnew = dXold*dScale; dYnew = dYold*dScale;
        curPnt->setX(gen::round(dXnew+zapLeft)); curPnt->setX(gen::round(dYnew+zapTop));
    }
    // fRect (учесть масштабирование)
    // масштабируемые: detLomLine, detPolyline, detPolygon, detCrossPolygon
    // немасштабируемые: detEllipce, detTextPoint, detTextRect, detRect, detRoundRect, detCheckBox, detCheckCircle, detCrossCircle, detHint, detPixmap
  QPoint curPnt;
    switch (fType)
    {
        case detLomLine: case detPolyline: case detPolygon: case detCrossPolygon:
            curPnt = fRect.topLeft();
            dXold = curPnt.x()-zapLeft; dYold = curPnt.y()-zapTop;
            dXnew = dXold*dScale; dYnew = dYold*dScale;
            fRect.setTopLeft(QPoint(gen::round(dXnew+zapLeft),gen::round(dYnew+zapTop)));
            curPnt = fRect.bottomRight();
            dXold = curPnt.x()-zapLeft; dYold = curPnt.y()-zapTop;
            dXnew = dXold*dScale; dYnew = dYold*dScale;
            fRect.setBottomRight(QPoint(gen::round(dXnew+zapLeft),gen::round(dYnew+zapTop)));
            break;
        default :
            curPnt = fRect.topLeft();
            dXold = curPnt.x()-zapLeft; dYold = curPnt.y()-zapTop;
            dXnew = dXold*dScale; dYnew = dYold*dScale;
            fRect.setTopLeft(QPoint(gen::round(dXnew+zapLeft),gen::round(dYnew+zapTop)));
            fRect.setBottomRight(fRect.bottomRight()+(fRect.topLeft()-curPnt)); // смещение такое же, как и у fRect.topLeft()
    }
}
//-----------------------------------------------------------------------------

// Определение QRect по fArPoints
QRect TDrawElement::defPointsRect()
{
    if (!fPlPoints.count()) return QRect();

  int xmin(MAX_LONG), xmax(MIN_LONG), ymin(MAX_LONG), ymax(MIN_LONG);
  QRect r;

    foreach (QPoint *curPnt,fPlPoints)
    {
        if (curPnt->x()<xmin) xmin = curPnt->x();
        if (curPnt->x()>xmax) xmax = curPnt->x();
        if (curPnt->y()<ymin) ymin = curPnt->y();
        if (curPnt->y()>ymax) ymax = curPnt->y();
    }
    r.setTopLeft(QPoint(xmin,ymin)); r.setBottomRight(QPoint(xmax,ymax));
    return r;
}
//-----------------------------------------------------------------------------


//*****************************************************************************
// Kласс TDrawComplexElement - сложный элемент рисования

// КОНСТРУКТОР
TDrawComplexElement::TDrawComplexElement(int id, TDrawLayer *lay, TDrawElementType et, QString txt, QColor clTxt, QColor clCntr, QColor clFill, QRect rct, QPoint lnkpnt, bool isHtml) : TDrawElement(id,lay,et,txt,clCntr,rct), fIsHtml(isHtml), fBrdWidth(BRD_WIDTH), fClText(clTxt), fScale(100)
{
    fClFill = clFill;
    fHintPoint = lnkpnt;
    fBrushStyle = Qt::SolidPattern;
    this->setRect(rct);
}
//-----------------------------------------------------------------------------

TDrawComplexElement::TDrawComplexElement(int id, TDrawLayer *lay, TDrawElementType et, QString pixnm, QPoint pnt, uint scl, int fl) : TDrawElement(id,lay,et,pixnm,pnt,fl), fIsHtml(false), fBrdWidth(0), fScale(scl)
{
}
//-----------------------------------------------------------------------------

// ДЕСТРУКТОР
TDrawComplexElement::~TDrawComplexElement()
{
}
//-----------------------------------------------------------------------------

void TDrawComplexElement::reset()
{
    TDrawElement::reset();
    fIsHtml = true;
    fBrdWidth = BRD_WIDTH;
    fClText = CLR_TEXT;
    fScale = 100;
}
//-----------------------------------------------------------------------------

bool TDrawComplexElement::isHtml() const
{
    return fIsHtml;
}
//-----------------------------------------------------------------------------

void TDrawComplexElement::setHtml(bool ih)
{
    fIsHtml = ih;
}
//-----------------------------------------------------------------------------

int TDrawComplexElement::brdWidth() const
{
    return fBrdWidth;
}
//-----------------------------------------------------------------------------

void TDrawComplexElement::setBrdWidth(int w)
{
    fBrdWidth = w;
}
//-----------------------------------------------------------------------------

QColor &TDrawComplexElement::clText() const
{
    return const_cast<QColor&>(fClText);
}
//-----------------------------------------------------------------------------

void TDrawComplexElement::setClText(QColor clr)
{
    fClText = clr;
}
//-----------------------------------------------------------------------------

uint TDrawComplexElement::scale() const
{
    return fScale;
}
//-----------------------------------------------------------------------------

void TDrawComplexElement::setScale(uint sc)
{
    fScale = sc;
}
//-----------------------------------------------------------------------------

// Расчет данных по fRect
// 1. fType==detHint
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
// 2. ...
//
void TDrawComplexElement::setRect(QRect r)
{
    fRect = r;
    if (fType==detHint) // Расчет 20-ти относительных координат [0..23]
    {
      int rw(fRect.width()), rh(fRect.height());
        fPlPoints.clear();
        fPlPoints.append(new QPoint(0,0));                       // [ 0]
        fPlPoints.append(new QPoint(rw,0));                      // [ 1]
        fPlPoints.append(new QPoint(rw,rh));                     // [ 2]
        fPlPoints.append(new QPoint(0,rh));                      // [ 3]
        fPlPoints.append(new QPoint(-fBrdWidth,-fBrdWidth));     // [ 4]
        fPlPoints.append(new QPoint(0,-fBrdWidth));              // [ 5]
        fPlPoints.append(new QPoint(2*rw/5,-fBrdWidth));         // [ 6]
        fPlPoints.append(new QPoint(3*rw/5,-fBrdWidth));         // [ 7]
        fPlPoints.append(new QPoint(rw,-fBrdWidth));             // [ 8]
        fPlPoints.append(new QPoint(rw+fBrdWidth,-fBrdWidth));   // [ 9]
        fPlPoints.append(new QPoint(rw+fBrdWidth,0));            // [10]
        fPlPoints.append(new QPoint(rw+fBrdWidth,2*rh/5));       // [11]
        fPlPoints.append(new QPoint(rw+fBrdWidth,3*rh/5));       // [12]
        fPlPoints.append(new QPoint(rw+fBrdWidth,rh));           // [13]
        fPlPoints.append(new QPoint(rw+fBrdWidth,rh+fBrdWidth)); // [14]
        fPlPoints.append(new QPoint(rw,rh+fBrdWidth));           // [15]
        fPlPoints.append(new QPoint(3*rw/5,rh+fBrdWidth));       // [16]
        fPlPoints.append(new QPoint(2*rw/5,rh+fBrdWidth));       // [17]
        fPlPoints.append(new QPoint(0,rh+fBrdWidth));            // [18]
        fPlPoints.append(new QPoint(-fBrdWidth,rh+fBrdWidth));   // [19]
        fPlPoints.append(new QPoint(-fBrdWidth,rh));             // [20]
        fPlPoints.append(new QPoint(-fBrdWidth,3*rh/5));         // [21]
        fPlPoints.append(new QPoint(-fBrdWidth,2*rh/5));         // [22]
        fPlPoints.append(new QPoint(-fBrdWidth,0));              // [23]
    }
    else TDrawElement::setRect(r);
}
//-----------------------------------------------------------------------------

void TDrawComplexElement::setRect(QPoint p1, QPoint p2)
{
    this->setRect(QRect(p1,p2));
}
//-----------------------------------------------------------------------------

void TDrawComplexElement::draw(QPainter &p)
{
  QPen pen(p.pen());
  QBrush brush(p.brush());

    pen.setWidth(fLineWidth);
    pen.setStyle(fPenStyle);
    pen.setColor(fIsBright ? fClContour : fClContourLess);
    p.setPen(pen);
    brush.setStyle(fBrushStyle);
    brush.setColor(fClFill);
    p.setBrush(brush);
    p.setFont(fFnt);
    switch (fType)
    {
        case detHint:
        {
//          int transX(fRect.x()), transY(fRect.y()), w(fRect.width()), h(fRect.height()), bw2(2*fBrdWidth);
//            p.translate(transX,transY);
//          QPoint lnkPnt(fHintPoint-QPoint(transX,transY)), cntrPnt(w/2,h/2);
//          QRect txtRct(*fPlPoints.at(0),*fPlPoints.at(2)), topRct(*fPlPoints.at(5),*fPlPoints.at(1)), rightRct(*fPlPoints.at(1),*fPlPoints.at(13)), bottomRct(*fPlPoints.at(3),*fPlPoints.at(15)), leftRct(*fPlPoints.at(23),*fPlPoints.at(3));
//            // заливка фона
//            p.fillRect(txtRct,fClFill);
//            p.fillRect(topRct,fClFill); p.fillRect(rightRct,fClFill); p.fillRect(bottomRct,fClFill); p.fillRect(leftRct,fClFill);
//            p.setPen(Qt::NoPen);
//            p.drawEllipse(*fPlPoints.at(0),fBrdWidth,fBrdWidth);
//            p.drawEllipse(*fPlPoints.at(1),fBrdWidth,fBrdWidth);
//            p.drawEllipse(*fPlPoints.at(2),fBrdWidth,fBrdWidth);
//            p.drawEllipse(*fPlPoints.at(3),fBrdWidth,fBrdWidth);
//            // поиск стороны сноски
//          bool isSide[4] = { false,false,false,false }; // Top, Right, Bottom, Left
//          QPoint &pntLT = *fPlPoints.at(4), &pntRT = *fPlPoints.at(9), &pntRB = *fPlPoints.at(14), &pntLB = *fPlPoints.at(19);
//          TPairCalcDblXY sides[4] = { TPairCalcDblXY(TCalcDblXY(pntLT.x(),pntLT.y()),TCalcDblXY(pntRT.x(),pntRT.y())),
//                                      TPairCalcDblXY(TCalcDblXY(pntRT.x(),pntRT.y()),TCalcDblXY(pntRB.x(),pntRB.y())),
//                                      TPairCalcDblXY(TCalcDblXY(pntRB.x(),pntRB.y()),TCalcDblXY(pntLB.x(),pntLB.y())),
//                                      TPairCalcDblXY(TCalcDblXY(pntLB.x(),pntLB.y()),TCalcDblXY(pntLT.x(),pntLT.y()))};
//          TCalcDblXY dcenter(cntrPnt.x(),cntrPnt.y()), dlnkpos(lnkPnt.x(),lnkPnt.y());
//            for (int i=0; i<4; i++)
//            {
//              TCalcDblXY crossPos1, crossPos2;
//                if (geom_base::isCrossOtrOtr(dcenter,dlnkpos,sides[i].posit1,sides[i].posit2,crossPos1,crossPos2))
//                {
//                    if (!crossPos2.isZero()) continue; // касательная
//                    else { isSide[i] = true; break; }
//                }
//            }
//            // заливка фона сноски
//            if (isSide[0]) // Top
//            {
//              QPoint points[3] = { *fPlPoints.at(6),lnkPnt,*fPlPoints.at(7) };
//                p.drawPolygon(points,3);
//            }
//            else if (isSide[1]) // Right
//            {
//              QPoint points[3] = { *fPlPoints.at(11),lnkPnt,*fPlPoints.at(12) };
//                p.drawPolygon(points,3);
//            }
//            else if (isSide[2]) // Bottom
//            {
//              QPoint points[3] = { *fPlPoints.at(16),lnkPnt,*fPlPoints.at(17) };
//                p.drawPolygon(points,3);
//            }
//            else if (isSide[3]) // Left
//            {
//              QPoint points[3] = { *fPlPoints.at(21),lnkPnt,*fPlPoints.at(22) };
//                p.drawPolygon(points,3);
//            }
//            // рисование контура
//            p.setPen(Qt::SolidLine);
//          int from0(0*16), from90(90*16), from180(180*16), from270(270*16), sect(90*16);
//            p.drawArc(-fBrdWidth,-fBrdWidth,bw2,bw2,from90,sect);
//            p.drawArc(w-fBrdWidth,-fBrdWidth,bw2,bw2,from0,sect);
//            p.drawArc(w-fBrdWidth,h-fBrdWidth,bw2,bw2,from270,sect);
//            p.drawArc(-fBrdWidth,h-fBrdWidth,bw2,bw2,from180,sect);
//            if (!isSide[0]) p.drawLine(*fPlPoints.at(5),*fPlPoints.at(8));
//            if (!isSide[1]) p.drawLine(*fPlPoints.at(10),*fPlPoints.at(13));
//            if (!isSide[2]) p.drawLine(*fPlPoints.at(15),*fPlPoints.at(18));
//            if (!isSide[3]) p.drawLine(*fPlPoints.at(20),*fPlPoints.at(23));
//            if (isSide[0])
//            {
//                p.drawLine(*fPlPoints.at(5),*fPlPoints.at(6));
//                p.drawLine(*fPlPoints.at(6),lnkPnt);
//                p.drawLine(lnkPnt,*fPlPoints.at(7));
//                p.drawLine(*fPlPoints.at(7),*fPlPoints.at(8));
//            }
//            else if (isSide[1])
//            {
//                p.drawLine(*fPlPoints.at(10),*fPlPoints.at(11));
//                p.drawLine(*fPlPoints.at(11),lnkPnt);
//                p.drawLine(lnkPnt,*fPlPoints.at(12));
//                p.drawLine(*fPlPoints.at(12),*fPlPoints.at(13));
//            }
//            else if (isSide[2])
//            {
//                p.drawLine(*fPlPoints.at(15),*fPlPoints.at(16));
//                p.drawLine(*fPlPoints.at(16),lnkPnt);
//                p.drawLine(lnkPnt,*fPlPoints.at(17));
//                p.drawLine(*fPlPoints.at(17),*fPlPoints.at(18));
//            }
//            else if (isSide[3])
//            {
//                p.drawLine(*fPlPoints.at(20),*fPlPoints.at(21));
//                p.drawLine(*fPlPoints.at(21),lnkPnt);
//                p.drawLine(lnkPnt,*fPlPoints.at(22));
//                p.drawLine(*fPlPoints.at(22),*fPlPoints.at(23));
//            }
//          QTextDocument txtdoc;
//            if (fIsHtml) txtdoc.setHtml(fText);
//            else txtdoc.setPlainText(fText);
//            txtdoc.drawContents(&p);
            break;
        }
        case detScaledPixmap:
        {
            if (fPlPoints.count())
            {
              QPixmap pix(fText);
                if (!pix.isNull())
                {
                  QSize sz(gen::round(pix.size().width()*fScale/100.),gen::round(pix.size().height()*fScale/100.));
                  QPixmap sclPix = pix.scaled(sz); // (const QSize &size, Qt::AspectRatioMode aspectRatioMode=Qt::IgnoreAspectRatio, Qt::TransformationMode transformMode=Qt::FastTransformation)
                    if (!sclPix.isNull())
                    {
                      int dx(0), dy(0);
                        if (fFlags & Qt::AlignLeft) dx = 0;
                        else if (fFlags & Qt::AlignRight) dx = -sz.width();
                        else dx = -sz.width()/2; // fFlags & Qt::AlignHCenter
                        if (fFlags &Qt:: AlignTop) dy = 0;
                        else if (fFlags & Qt::AlignBottom) dy = -sz.height();
                        else dy = -sz.height()/2; // fFlags & Qt::AlignVCenter
                      QPoint pnt(*fPlPoints.first()+QPoint(dx,dy));
                        p.drawPixmap(pnt,sclPix);
                        fRect = QRect(pnt,sz);
                    }
                }
            }
            break;
        }
        default: TDrawElement::draw(p);
    }
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// Kласс TDrawLayer - Слой рисования

// КОНСТРУКТОР
TDrawLayer::TDrawLayer(int n, QString nm, int is) : QObject(), fNum(n), fName(nm), fISort(is), fIsVisible(false)
{
    fPlDrawEls.setAutoDelete(true);
}
//-----------------------------------------------------------------------------

// ДЕСТРУКТОР
TDrawLayer::~TDrawLayer()
{
}
//-----------------------------------------------------------------------------

// Cброс в исходное состояние
void TDrawLayer::reset()
{
    fNum = 0;
    fName = "";
    fISort = 0;
    fPlDrawEls.clear();
    fIsVisible = false;
}
//-----------------------------------------------------------------------------

// Номер
int TDrawLayer::num() const
{
    return fNum;
}
//-----------------------------------------------------------------------------

void TDrawLayer::setNum(int n)
{
    fNum = n;
}
//-----------------------------------------------------------------------------

// Hаименование
QString &TDrawLayer::name() const
{
    return const_cast<QString&>(fName);
}
//-----------------------------------------------------------------------------

void TDrawLayer::setName(QString nm)
{
    fName = nm;
}
//-----------------------------------------------------------------------------

// Целое поле для сортировки
int TDrawLayer::iSort() const
{
    return fISort;
}
//-----------------------------------------------------------------------------

void TDrawLayer::setISort(int is)
{
    fISort = is;
}
//-----------------------------------------------------------------------------

// Список элементов рисования
TDrawElementList &TDrawLayer::els() const
{
    return const_cast<TDrawElementList&>(fPlDrawEls);
}
//-----------------------------------------------------------------------------

// Добавить элемент рисования
bool TDrawLayer::insertEl(TDrawElement *el)
{
    if (!el) return false;
    if(fPlDrawEls.indexOf(el)==-1)
    {fPlDrawEls.append(el); return true; }
    else return false;
}
//-----------------------------------------------------------------------------

// Удалить элемент рисования
bool TDrawLayer::removeEl(TDrawElement *el)
{
    if (el && fPlDrawEls.indexOf(el)==-1) return false;
    fPlDrawEls.removeOne(el);
    return true;
}
//-----------------------------------------------------------------------------

// Очистить список элементов рисования
void TDrawLayer::clearEls()
{
    fPlDrawEls.clear();
}
//-----------------------------------------------------------------------------

// Поиск элемента рисования по id
TDrawElement *TDrawLayer::findEl(int id)
{
    foreach (TDrawElement *curEl,fPlDrawEls)
        if (curEl->id()==id) return curEl;
    return NULL;
}
//-----------------------------------------------------------------------------

// Поиск элемента рисования по данным идентификатора
TDrawElement *TDrawLayer::findEl(TIdent idnt)
{
    foreach (TDrawElement *curEl,fPlDrawEls)
        if (curEl->ident()==idnt) return curEl;
    return NULL;
}
//-----------------------------------------------------------------------------

// Поиск элемента рисования по типу и данным идентификатора
TDrawElement *TDrawLayer::findEl(TDrawElementType tp, TIdent idnt)
{
    foreach (TDrawElement *curEl,fPlDrawEls)
        if (curEl->type()==tp && curEl->ident()==idnt) return curEl;
    return NULL;
}
//-----------------------------------------------------------------------------

// Признак видимости слоя
bool TDrawLayer::isVisible() const
{
    return fIsVisible;
}
//-----------------------------------------------------------------------------

void TDrawLayer::setIsVisible(bool vs)
{
    fIsVisible = vs;
}
//-----------------------------------------------------------------------------

// Поиск элементов рисования в точке
void TDrawLayer::findEls(QPoint pnt, TDrawElementList &pl)
{
    pl.clear();
    foreach (TDrawElement *curEl,fPlDrawEls)
        if (curEl->isMyCoord(pnt)) pl.append(curEl);
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// Kласс TSelfDrawWidget - Самоотрисовывающаяся панель

// КОНСТРУКТОР
TSelfDrawWidget::TSelfDrawWidget(const QList<TLayerNum> &lays, QColor clrbs, QColor clrtxt, const QStringList *names, QWidget *parent, int w, int h, Qt::WindowFlags f) : QWidget(parent,f), fClrBase(clrbs), fClrText(clrtxt)
{
  TLayerNum curN;
  QString curNm;

    resize(w,h);
    CLR_BASE = clrbs;
    CLR_TEXT = clrtxt;
    fPlLayers.setAutoDelete(true);
    fPlVisLayers.setAutoDelete(false);
    for (int i=0; i<lays.count(); i++)
    {
        curN = lays.at(i);
        curNm = names ? (names->count()<i ? names->at(i) : enumToStr(curN)) : enumToStr(curN);
        fPlLayers.append(new TDrawLayer((int)curN,curNm,i));
    }
    if (!fPlLayers.count()) fPlLayers.append(new TDrawLayer((int)lrnFon,"Фон",0));
    showLayers(); // показать все слои
}
//-----------------------------------------------------------------------------

// ДЕСТРУКТОР
TSelfDrawWidget::~TSelfDrawWidget()
{
}
//-----------------------------------------------------------------------------

// Cброс в исходное состояние
void TSelfDrawWidget::reset()
{
    fPlVisLayers.clear();
    fPlLayers.clear();
}
//-----------------------------------------------------------------------------

// Поиск слоя
TDrawLayer *TSelfDrawWidget::findLayer(int n)
{
    foreach (TDrawLayer *curLay,fPlLayers)
        if (curLay->num()==n) return curLay;
    return NULL;
}
//-----------------------------------------------------------------------------

// Перезанести слои в список видимых (в нужной последовательности)
void TSelfDrawWidget::refreshVisLayers()
{
    fPlVisLayers.clear();
    foreach (TDrawLayer *curLay,fPlLayers)
        if (curLay->isVisible()) fPlVisLayers.append(curLay);
}
//-----------------------------------------------------------------------------

// Поиск видимого слоя
TDrawLayer *TSelfDrawWidget::findVisLayer(int n)
{
    foreach (TDrawLayer *curLay,fPlVisLayers)
        if (curLay->num()==n) return curLay;
    return NULL;
}
//-----------------------------------------------------------------------------

// Список слоев
TDrawLayerList &TSelfDrawWidget::layers() const
{
    return const_cast<TDrawLayerList&>(fPlLayers);
}
//-----------------------------------------------------------------------------

// Список видимых слоев
TDrawLayerList &TSelfDrawWidget::visLayers() const
{
    return const_cast<TDrawLayerList&>(fPlVisLayers);
}
//-----------------------------------------------------------------------------

// Добавить слой номер n к видимым
// ( добавить на свое место )
void TSelfDrawWidget::showLayer(int n)
{
    TDrawLayer *curLay(findLayer(n));
    if (curLay && fPlVisLayers.indexOf(curLay)==-1)
    {
        curLay->setIsVisible(true);
        refreshVisLayers();
    }
}
//-----------------------------------------------------------------------------

// Сделать слои видимыми
void TSelfDrawWidget::showLayers(QList<int> &vlNums)
{
    hideLayers();
    for (int i=0; i<vlNums.count(); i++)
        if (TDrawLayer *curLay = findLayer(vlNums.at(i))) curLay->setIsVisible(true);
    refreshVisLayers();
}
//-----------------------------------------------------------------------------

// Сделать все слои видимыми
void TSelfDrawWidget::showLayers()
{
    hideLayers();
    foreach (TDrawLayer *curLay,fPlLayers)
    {
        curLay->setIsVisible(true);
        fPlVisLayers.append(curLay);
    }
}
//-----------------------------------------------------------------------------

// Удалить слой номер n из видимых
void TSelfDrawWidget::hideLayer(int n)
{
    if (TDrawLayer *curLay = findVisLayer(n))
    {
        curLay->setIsVisible(false);
        fPlVisLayers.removeOne(curLay);
    }
}
//-----------------------------------------------------------------------------

// Удалить видимымые слои
void TSelfDrawWidget::hideLayers(QList<int> &vlNums)
{
    for (int i=0; i<vlNums.count(); i++) hideLayer(vlNums.at(i));
}
//-----------------------------------------------------------------------------

// Удалить все видимымые слои
void TSelfDrawWidget::hideLayers()
{
    foreach (TDrawLayer *curLay,fPlVisLayers) curLay->setIsVisible(false);
    fPlVisLayers.clear();
}
//-----------------------------------------------------------------------------

// Сделать слой номер n верхним
void TSelfDrawWidget::moveUp(int n)
{
    if (TDrawLayer *curLay = findVisLayer(n))
        if (fPlVisLayers.last()!=curLay)
        {
            fPlVisLayers.removeOne(curLay);
            fPlVisLayers.append(curLay);
        }
}
//-----------------------------------------------------------------------------

// Сделать слой номер n нижним
void TSelfDrawWidget::moveDown(int n)
{
    if (TDrawLayer *curLay = findVisLayer(n))
        if (fPlVisLayers.first()!=curLay)
        {
            fPlVisLayers.removeOne(curLay);
            fPlVisLayers.prepend(curLay);
        }
}
//-----------------------------------------------------------------------------

// Подготовить слои к сортировке (setITag(from vlNums))
void TSelfDrawWidget::prepSortLayers(QList<int> &vlNums)
{
  int is=0;
    for (int i=0; i<vlNums.count(); i++)
        if (TDrawLayer *curLay = findVisLayer(vlNums.at(i))) curLay->setISort(is++);
}
//-----------------------------------------------------------------------------

// Упорядочить видимые слои (по iSort())
void TSelfDrawWidget::sortLayers()
{
    qStableSort(fPlVisLayers.begin(),fPlVisLayers.end(),obj::FSortMinToMaxInt<TDrawLayer*>());
}
//-----------------------------------------------------------------------------

// Добавить элемент рисования на слой номер n
bool TSelfDrawWidget::insertEl(int n, TDrawElement *el)
{
    if (TDrawLayer *curLay = findLayer(n)) return curLay->insertEl(el);
    return false;
}
//-----------------------------------------------------------------------------

// Удалить элемент рисования со слоя номер n
bool TSelfDrawWidget::removeEl(TDrawElement *el)
{
    foreach (TDrawLayer *curLay,fPlLayers) return curLay->removeEl(el);
    return false;
}
//-----------------------------------------------------------------------------

// Очистить слой номер n
bool TSelfDrawWidget::clearLayer(int n)
{
    if (TDrawLayer *curLay = findLayer(n)) { curLay->clearEls(); return true; }
    return false;
}
//-----------------------------------------------------------------------------

// Очистить все слои
void TSelfDrawWidget::clearLayers()
{
    foreach (TDrawLayer *curLay,fPlLayers) curLay->clearEls();
}
//-----------------------------------------------------------------------------

// Поиск элемента рисования по id (во всех слоях или только в видимых)
TDrawElement *TSelfDrawWidget::findEl(int id, bool invislays)
{
  TDrawLayerList &layers = invislays ? fPlVisLayers : fPlLayers;
    foreach (TDrawLayer *curLay,layers)
        if (TDrawElement *curEl = curLay->findEl(id)) return curEl;
    return NULL;
}
//-----------------------------------------------------------------------------

// Поиск элемента рисования по данным идентификатора (во всех слоях или только в видимых)
TDrawElement *TSelfDrawWidget::findEl(TIdent idnt, bool invislays)
{
  TDrawLayerList &layers = invislays ? fPlVisLayers : fPlLayers;
    foreach (TDrawLayer *curLay,layers)
        if (TDrawElement *curEl = curLay->findEl(idnt)) return curEl;
    return NULL;
}
//-----------------------------------------------------------------------------

// Поиск элемента рисования по типу и данным идентификатора (по умолчанию - во всех слоях)
TDrawElement *TSelfDrawWidget::findEl(TDrawElementType tp, TIdent idnt, bool invislays)
{
  TDrawLayerList &layers = invislays ? fPlVisLayers : fPlLayers;
    foreach (TDrawLayer *curLay,layers)
        if (TDrawElement *curEl = curLay->findEl(tp,idnt)) return curEl;
    return NULL;
}
//-----------------------------------------------------------------------------

// Поиск элементов рисования в точке на всех слоях или только на видимых (по умолчанию)
void TSelfDrawWidget::findEls(QPoint pnt, TDrawElementList &pl, bool onall)
{
  TDrawElementList curPl;
    pl.clear();
    if (onall)
        foreach (TDrawLayer *curLay,fPlLayers)
        {
            curLay->findEls(pnt,curPl);
            foreach (TDrawElement *curEl,curPl) pl.append(curEl);
        }
    else
        foreach (TDrawLayer *curLay,fPlVisLayers)
        {
            curLay->findEls(pnt,curPl);
            foreach (TDrawElement *curEl,curPl) pl.append(curEl);
        }
}
//-----------------------------------------------------------------------------

// Поиск элементов рисования в точке на слое n
void TSelfDrawWidget::findEls(QPoint pnt, int n, TDrawElementList &pl)
{
    if (TDrawLayer *curLay = findVisLayer(n)) curLay->findEls(pnt,pl);
}
//-----------------------------------------------------------------------------

// Цвета: фона
QColor TSelfDrawWidget::clrBase() const
{
    return fClrBase;
}
//-----------------------------------------------------------------------------

//        текста
QColor TSelfDrawWidget::clrText() const
{
    return fClrText;
}
//-----------------------------------------------------------------------------

// Установление единого цвета контура
void TSelfDrawWidget::setColor_contour(QColor clr, bool invislays)
{
  TDrawLayerList &layers = invislays ? fPlVisLayers : fPlLayers;
    foreach (TDrawLayer *curLay,layers)
    {
      TDrawElementList &els = curLay->els();
        foreach (TDrawElement *curEl,els) curEl->setClContour(clr);
    }
}
//-----------------------------------------------------------------------------

// ... цвета заливки
void TSelfDrawWidget::setColor_fill(QColor clr, bool invislays)
{
  TDrawLayerList &layers = invislays ? fPlVisLayers : fPlLayers;
    foreach (TDrawLayer *curLay,layers)
    {
      TDrawElementList &els = curLay->els();
        foreach (TDrawElement *curEl,els) curEl->setClFill(clr);
    }
}
//-----------------------------------------------------------------------------

// ... толщины линии
void TSelfDrawWidget::setLineWidth(int w, bool invislays)
{
  TDrawLayerList &layers = invislays ? fPlVisLayers : fPlLayers;
    foreach (TDrawLayer *curLay,layers)
    {
      TDrawElementList &els = curLay->els();
        foreach (TDrawElement *curEl,els) curEl->setLineWidth(w);
    }
}
//-----------------------------------------------------------------------------

// ... стиля пера
void TSelfDrawWidget::setPenStyle(Qt::PenStyle ps, bool invislays)
{
  TDrawLayerList &layers = invislays ? fPlVisLayers : fPlLayers;
    foreach (TDrawLayer *curLay,layers)
    {
      TDrawElementList &els = curLay->els();
        foreach (TDrawElement *curEl,els) curEl->setPenStyle(ps);
    }
}
//-----------------------------------------------------------------------------

// ... стиля кисти
void TSelfDrawWidget::setBrushStyle(Qt::BrushStyle bs, bool invislays)
{
  TDrawLayerList &layers = invislays ? fPlVisLayers : fPlLayers;
    foreach (TDrawLayer *curLay,layers)
    {
      TDrawElementList &els = curLay->els();
        foreach (TDrawElement *curEl,els) curEl->setBrushStyle(bs);
    }
}
//-----------------------------------------------------------------------------

// Создать слои
void TSelfDrawWidget::recreateLayers(QList<int> &lays, QStringList *names)
{
  TLayerNum curN;
  QString curNm;

    fPlLayers.clear(); fPlVisLayers.clear();
    for (int i=0; i<lays.count(); i++)
    {
        curN = (TLayerNum)lays.at(i);
        curNm = names ? (names->count()<i ? names->at(i) : enumToStr(curN)) : enumToStr(curN);
        fPlLayers.append(new TDrawLayer(curN,curNm,i));
    }
    if (!fPlLayers.count()) fPlLayers.append(new TDrawLayer((int)lrnFon,"Фон",0));
    showLayers(); // показать все слои
}
//-----------------------------------------------------------------------------

// Определение максимального id
int TSelfDrawWidget::maxId()
{
  int resId(0);
    foreach (TDrawLayer *curLay,fPlLayers)
    {
      TDrawElementList &els = curLay->els();
        foreach (TDrawElement *curEl,els)
            if (curEl->id()>resId) resId = curEl->id();
    }
    return resId;
}
//-----------------------------------------------------------------------------

// Отображение в QListView слоев
void TSelfDrawWidget::reflectLayersToTree(QTreeWidget &tw)
{
  QTreeWidgetItem *twiNew(NULL);
    tw.clear();
    foreach (TDrawLayer *curLay,fPlLayers)
    {
        twiNew = new QTreeWidgetItem(&tw,twiNew);
        twiNew->setText(0,curLay->name());
        twiNew->setFlags(twiNew->flags() | Qt::ItemIsUserCheckable);
        twiNew->setData(0,Qt::UserRole,qVariantFromValue(TIdent(0,curLay->num(),curLay->name())));
        twiNew->setCheckState(0,curLay->isVisible() ? Qt::Checked : Qt::Unchecked);
    }
}
//-----------------------------------------------------------------------------

// Обработчик события перерисовки панели
// ( автоматически отрисовываются только видимые слои     )
// ( первым рисуется первый в списке слой (первый-нижний) )
void TSelfDrawWidget::paintEvent(QPaintEvent *e)
{
  QPainter p(this);
  const QRect &drawRect = e->rect();

    p.fillRect(drawRect,QBrush(palette().base()));
    foreach (TDrawLayer *curLay,fPlVisLayers)
    {
      TDrawElementList &els = curLay->els();
        foreach (TDrawElement *curEl,els)
            if (curEl->isVisible() && curEl->inRect(drawRect)) curEl->draw(p);
    }
}
//-----------------------------------------------------------------------------

// Нажатие кнопки мыши
void TSelfDrawWidget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
}
//-----------------------------------------------------------------------------

// Отпускание кнопки мыши
void TSelfDrawWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
}
//-----------------------------------------------------------------------------

// Двойной клик
void TSelfDrawWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);
}
//-----------------------------------------------------------------------------

// Перемещение мыши
void TSelfDrawWidget::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
}
//-----------------------------------------------------------------------------

// Прокрутка колесика
void TSelfDrawWidget::wheelEvent(QWheelEvent *e)
{
    QWidget::wheelEvent(e);
}
//-----------------------------------------------------------------------------

// Нажатие клавиши
void TSelfDrawWidget::keyPressEvent(QKeyEvent *e)
{
    QWidget::keyPressEvent(e);
}
//-----------------------------------------------------------------------------

// Отпускание клавиши
void TSelfDrawWidget::keyReleaseEvent(QKeyEvent *e)
{
    QWidget::keyReleaseEvent(e);
}
//-----------------------------------------------------------------------------

// Получение фокуса (+ setFocusPolicy() и  setFocus())
void TSelfDrawWidget::focusInEvent(QFocusEvent *e)
{
    QWidget::focusInEvent(e);
}
//-----------------------------------------------------------------------------

// Потеря фокуса
void TSelfDrawWidget::focusOutEvent(QFocusEvent *e)
{
    QWidget::focusOutEvent(e);
}
//-----------------------------------------------------------------------------

// Курсор находится в компоненте
void TSelfDrawWidget::enterEvent(QEvent *e)
{
    QWidget::enterEvent(e);
}
//-----------------------------------------------------------------------------

// Курсор уходит из компонента
void TSelfDrawWidget::leaveEvent(QEvent *e)
{
    QWidget::leaveEvent(e);
}
//-----------------------------------------------------------------------------

// Изменение размеров
void TSelfDrawWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}
//-----------------------------------------------------------------------------

// Закрытие
void TSelfDrawWidget::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
}
//-----------------------------------------------------------------------------

// Показ
void TSelfDrawWidget::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
}
//-----------------------------------------------------------------------------

// Сокрытие
void TSelfDrawWidget::hideEvent(QHideEvent *e)
{
    QWidget::hideEvent(e);
}
//-----------------------------------------------------------------------------
