
//***************************************************************************//
//                                                                           //
//           Реализация данных ячейки таблицы TTableWidgetItemData           //
//                                                                           //
//***************************************************************************//

#include <math.h>
#include <QPalette>
#include <defMacro>
#include <defConst>
#include <geod_base>
#include <TTableWidgetItemData>

//*****************************************************************************
// Структура TTableDecor - Параметры декорирования ячеек

// КОНСТРУКТОР для crText, crPixmap, crPixmapText
// если crPixmapText: algnfrst-pixmap, algnscnd-text
TTableDecor::TTableDecor(CellReg cr, int algnfrst, int algnscnd) :
cellReg(cr),
shapeType(shtCircle),
pixmapPlace(ppSeparate),
alignText(cr==crPixmapText ? algnscnd : algnfrst),
alignPixmap(algnfrst),
pixmapSpace(0),
lineWidth(1),
shapePen(PAL_TEXT),
shapeBrush(PAL_BASE),
fColorText(NULL),
fColorFon(NULL),
fFont(NULL)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР для crPixmapText
TTableDecor::TTableDecor(PixmapPlace pixpl, int pixsp) :
cellReg(crPixmapText),
shapeType(shtCircle),
pixmapPlace(pixpl),
alignText(Qt::AlignCenter),
alignPixmap(Qt::AlignCenter),
pixmapSpace(pixsp),
lineWidth(1),
shapePen(PAL_TEXT),
shapeBrush(PAL_BASE),
fColorText(NULL),
fColorFon(NULL),
fFont(NULL)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР для crShape
TTableDecor::TTableDecor(ShapeType sht, QColor clrpen, QColor clrbr, int lnw, int pixsp) :
cellReg(crShape),
shapeType(sht),
pixmapPlace(ppSeparate),
alignText(Qt::AlignCenter),
alignPixmap(Qt::AlignCenter),
pixmapSpace(pixsp),
lineWidth(lnw),
shapePen(clrpen),
shapeBrush(clrbr),
fColorText(NULL),
fColorFon(NULL),
fFont(NULL)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableDecor::TTableDecor(const TTableDecor &td) :
cellReg(td.cellReg),
shapeType(td.shapeType),
pixmapPlace(td.pixmapPlace),
alignText(td.alignText),
alignPixmap(td.alignPixmap),
pixmapSpace(td.pixmapSpace),
lineWidth(td.lineWidth),
shapePen(td.shapePen),
shapeBrush(td.shapeBrush),
fColorText(NULL),
fColorFon(NULL),
fFont(NULL)
{
    if (td.fColorText) fColorText = new QColor(*td.fColorText);
    if (td.fColorFon) fColorFon = new QColor(*td.fColorFon);
    if (td.fFont) fFont = new QFont(*td.fFont);
}
//-----------------------------------------------------------------------------

// ДЕСТРУКТОР
TTableDecor::~TTableDecor()
{
    DELETE(fColorText); DELETE(fColorFon); DELETE(fFont);
}
//-----------------------------------------------------------------------------

TTableDecor &TTableDecor::operator=(const TTableDecor &td)
{
    if (this==&td) return *this;
    cellReg = td.cellReg;
    shapeType = td.shapeType;
    pixmapPlace = td.pixmapPlace;
    alignText = td.alignText;
    alignPixmap = td.alignPixmap;
    pixmapSpace = td.pixmapSpace;
    lineWidth = td.lineWidth;
    shapePen = td.shapePen;
    shapeBrush = td.shapeBrush;
    DELETE(fColorText); DELETE(fColorFon); DELETE(fFont);
    if (td.fColorText) fColorText = new QColor(*td.fColorText);
    if (td.fColorFon) fColorFon = new QColor(*td.fColorFon);
    if (td.fFont) fFont = new QFont(*td.fFont);
    return *this;
}
//-----------------------------------------------------------------------------

// Цвета ячейки: текст
QColor *TTableDecor::colorText() const
{
    return fColorText;
}
//-----------------------------------------------------------------------------

void TTableDecor::setColorText(QColor &clr)
{
    DELETE(fColorText);
    fColorText = new QColor(clr);
}
//-----------------------------------------------------------------------------

//             : фон
QColor *TTableDecor::colorFon() const
{
    return fColorFon;
}
//-----------------------------------------------------------------------------

void TTableDecor::setColorFon(QColor &clr)
{
    DELETE(fColorFon);
    fColorFon = new QColor(clr);
}
//-----------------------------------------------------------------------------

// Сброс в значения по умолчанию
void TTableDecor::resetColors()
{
    DELETE(fColorText); DELETE(fColorFon);
}
//-----------------------------------------------------------------------------

// Шрифт ячейки
QFont *TTableDecor::font() const
{
    return fFont;
}
//-----------------------------------------------------------------------------

void TTableDecor::setFont(QFont &f)
{
    DELETE(fFont);
    fFont = new QFont(f);
}
//-----------------------------------------------------------------------------

// Сброс в значение по умолчанию
void TTableDecor::resetFont()
{
    DELETE(fFont);
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// Структура TTableWidgetItemData::TDateTimeData - Данные QDateTimeEdit

// КОНСТРУКТОР
TTableWidgetItemData::DateTimeData::DateTimeData(DateTimeFormat dtft, bool iscldr) : dtFormat(dtft), isCalendar(iscldr), isMinDt(false), isMaxDt(false)
{
    switch (dtFormat)
    {
        case dtfHHMMDDMMYY: fDisplayFormat = "hh:mm dd.MM.yy"; break;
        case dtfHHMMDDMM:   fDisplayFormat = "hh:mm dd.MM"; break;
        case dtfDDMMYY:     fDisplayFormat = "dd.MM.yy"; break;
        case dtfDDMM:       fDisplayFormat = "dd.MM"; break;
        case dtfHHMMSS:     fDisplayFormat = "hh:mm:ss"; break;
        case dtfHHMM:       fDisplayFormat = "hh:mm"; break;
        case dtfMMSS:       fDisplayFormat = "mm:ss"; break;
        default:            fDisplayFormat = "hh:mm:ss dd.MM.yy"; // dtfHHMMSSDDMMYY
    }
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemData::DateTimeData::DateTimeData(DateTimeFormat dtft, QDateTime mindt, bool iscldr) : dtFormat(dtft), isCalendar(iscldr), isMinDt(false), isMaxDt(false), minDt(mindt)
{
    switch (dtFormat)
    {
        case dtfHHMMDDMMYY: fDisplayFormat = "hh:mm dd.MM.yy"; break;
        case dtfHHMMDDMM:   fDisplayFormat = "hh:mm dd.MM"; break;
        case dtfDDMMYY:     fDisplayFormat = "dd.MM.yy"; break;
        case dtfDDMM:       fDisplayFormat = "dd.MM"; break;
        case dtfHHMMSS:     fDisplayFormat = "hh:mm:ss"; break;
        case dtfHHMM:       fDisplayFormat = "hh:mm"; break;
        case dtfMMSS:       fDisplayFormat = "mm:ss"; break;
        default:            fDisplayFormat = "hh:mm:ss dd.MM.yy"; // dtfHHMMSSDDMMYY
    }
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemData::DateTimeData::DateTimeData(DateTimeFormat dtft, QDateTime mindt, QDateTime maxdt, bool iscldr) : dtFormat(dtft), isCalendar(iscldr), isMinDt(true), isMaxDt(true), minDt(mindt), maxDt(maxdt)
{
    switch (dtFormat)
    {
        case dtfHHMMDDMMYY: fDisplayFormat = "hh:mm dd.MM.yy"; break;
        case dtfHHMMDDMM:   fDisplayFormat = "hh:mm dd.MM"; break;
        case dtfDDMMYY:     fDisplayFormat = "dd.MM.yy"; break;
        case dtfDDMM:       fDisplayFormat = "dd.MM"; break;
        case dtfHHMMSS:     fDisplayFormat = "hh:mm:ss"; break;
        case dtfHHMM:       fDisplayFormat = "hh:mm"; break;
        case dtfMMSS:       fDisplayFormat = "mm:ss"; break;
        default:            fDisplayFormat = "hh:mm:ss dd.MM.yy"; // dtfHHMMSSDDMMYY
    }
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemData::DateTimeData::DateTimeData(const DateTimeData &d) : dtFormat(d.dtFormat), isCalendar(d.isCalendar), isMinDt(d.isMinDt), isMaxDt(d.isMaxDt), minDt(d.minDt), maxDt(d.maxDt), fDisplayFormat(d.fDisplayFormat)
{
}
//-----------------------------------------------------------------------------

// Преобразование строки во время
QDateTime TTableWidgetItemData::DateTimeData::dateTimeFromText(const QString &text) const
{
  int yy(0), MM(0), dd(0), hh(0), mm(0), ss(0);
  QStringList slDt = text.split(" ");
    switch (dtFormat)
    {
        case dtfHHMMDDMMYY: // "hh:mm dd.MM.yy"
            if (slDt.count()==2)
            {
              QStringList slTime = slDt.at(0).split(":"), slDate = slDt.at(1).split(".");
                if (slTime.count()==2 && slDate.count()==3)
                {
                    yy = slDate.at(2).toInt();
                    MM = slDate.at(1).toInt();
                    dd = slDate.at(0).toInt();
                    hh = slTime.at(0).toInt();
                    mm = slTime.at(1).toInt();
                }
            }
            break;
        case dtfHHMMDDMM: // "hh:mm dd.MM"
            if (slDt.count()==2)
            {
              QStringList slTime = slDt.at(0).split(":"), slDate = slDt.at(1).split(".");
                if (slTime.count()==2 && slDate.count()==2)
                {
                    MM = slDate.at(1).toInt();
                    dd = slDate.at(0).toInt();
                    hh = slTime.at(0).toInt();
                    mm = slTime.at(1).toInt();
                }
            }
            break;
        case dtfDDMMYY: // "dd.MM.yy"
            if (slDt.count()==1)
            {
              QStringList slDate = slDt.at(0).split(".");
                if (slDate.count()==3)
                {
                    yy = slDate.at(2).toInt();
                    MM = slDate.at(1).toInt();
                    dd = slDate.at(0).toInt();
                }
            }
            break;
        case dtfDDMM: // "dd.MM"
            if (slDt.count()==1)
            {
              QStringList slDate = slDt.at(0).split(".");
                if (slDate.count()==2)
                {
                    MM = slDate.at(1).toInt();
                    dd = slDate.at(0).toInt();
                }
            }
            break;
        case dtfHHMMSS: // "hh:mm:ss"
            if (slDt.count()==1)
            {
              QStringList slTime = slDt.at(0).split(":");
                if (slTime.count()==3)
                {
                    hh = slTime.at(0).toInt();
                    mm = slTime.at(1).toInt();
                    ss = slTime.at(2).toInt();
                }
            }
            break;
        case dtfHHMM: // "hh:mm"
            if (slDt.count()==1)
            {
              QStringList slTime = slDt.at(0).split(":");
                if (slTime.count()==2)
                {
                    hh = slTime.at(0).toInt();
                    mm = slTime.at(1).toInt();
                }
            }
            break;
        case dtfMMSS: // "mm:ss"
            if (slDt.count()==1)
            {
              QStringList slTime = slDt.at(0).split(":");
                if (slTime.count()==2)
                {
                    mm = slTime.at(0).toInt();
                    ss = slTime.at(1).toInt();
                }
            }
            break;
        default: // "hh:mm:ss dd.MM.yy"
            if (slDt.count()==2)
            {
              QStringList slTime = slDt.at(0).split(":"), slDate = slDt.at(1).split(".");
                if (slTime.count()==3 && slDate.count()==3)
                {
                    yy = slDate.at(2).toInt();
                    MM = slDate.at(1).toInt();
                    dd = slDate.at(0).toInt();
                    hh = slTime.at(0).toInt();
                    mm = slTime.at(1).toInt();
                    ss = slTime.at(2).toInt();
                }
            }
    }
  QDate date(yy,MM,dd);
  QTime time(hh,mm,ss);
    return QDateTime(date,time);
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// Класс TTableWidgetItemData - Данные для декорирования

// КОНСТРУКТОР
TTableWidgetItemData::TTableWidgetItemData() :
fText(""),
fPixmap(NULL),
fDecor(TTableDecor::crText),
fEditorType(etNoEdit),
fSpinBoxData(NULL),
fDoubleSpinBoxData(NULL),
fComboBoxData(NULL),
fDateTimeData(NULL)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemData::TTableWidgetItemData(const TTableWidgetItemData &twd) :
fText(twd.fText),
fPixmap(twd.fPixmap ? new QPixmap(*twd.fPixmap) : NULL),
fDecor(twd.fDecor),
fEditorType(twd.fEditorType),
fSpinBoxData(twd.fSpinBoxData ? new SpinBoxData(*twd.fSpinBoxData) : NULL),
fDoubleSpinBoxData(twd.fDoubleSpinBoxData ? new DoubleSpinBoxData(*twd.fDoubleSpinBoxData) : NULL),
fComboBoxData(twd.fComboBoxData ? new ComboBoxData(*twd.fComboBoxData) : NULL),
fDateTimeData(twd.fDateTimeData ? new DateTimeData(*twd.fDateTimeData) : NULL)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemData::TTableWidgetItemData(QPixmap *pix, int algn) :
fText(""),
fPixmap(pix ? new QPixmap(*pix) : NULL),
fDecor(TTableDecor::crPixmap,algn),
fEditorType(etNoEdit),
fSpinBoxData(NULL),
fDoubleSpinBoxData(NULL),
fComboBoxData(NULL),
fDateTimeData(NULL)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemData::TTableWidgetItemData(const QString &txt, int algn) :
fText(txt),
fPixmap(NULL),
fDecor(TTableDecor::crText,algn),
fEditorType(etNoEdit),
fSpinBoxData(NULL),
fDoubleSpinBoxData(NULL),
fComboBoxData(NULL),
fDateTimeData(NULL)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemData::TTableWidgetItemData(QPixmap *pix, const QString &txt, TTableDecor::PixmapPlace pixplace, int pixsp) :
fText(txt),
fPixmap(pix ? new QPixmap(*pix) : NULL),
fDecor(pixplace,pixsp),
fEditorType(etNoEdit),
fSpinBoxData(NULL),
fDoubleSpinBoxData(NULL),
fComboBoxData(NULL),
fDateTimeData(NULL)
{
}
//-----------------------------------------------------------------------------

TTableWidgetItemData::TTableWidgetItemData(QPixmap *pix, const QString &txt, int algnpix, int algntxt) :
fText(txt),
fPixmap(pix ? new QPixmap(*pix) : NULL),
fDecor(TTableDecor::crPixmapText,algnpix,algntxt),
fEditorType(etNoEdit),
fSpinBoxData(NULL),
fDoubleSpinBoxData(NULL),
fComboBoxData(NULL),
fDateTimeData(NULL)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemData::TTableWidgetItemData(TTableDecor::ShapeType sht, const QColor &clpen, const QColor &clbr, const QString &txt, int pixsp, int lnw) :
fText(txt),
fPixmap(NULL),
fDecor(sht,clpen,clbr,lnw,pixsp),
fEditorType(etNoEdit),
fSpinBoxData(NULL),
fDoubleSpinBoxData(NULL),
fComboBoxData(NULL),
fDateTimeData(NULL)
{
}
//-----------------------------------------------------------------------------

// ДЕСТРУКТОР
TTableWidgetItemData::~TTableWidgetItemData()
{
    DELETE(fPixmap);
    DELETE(fSpinBoxData); DELETE(fDoubleSpinBoxData); DELETE(fComboBoxData); DELETE(fDateTimeData);
}
//-----------------------------------------------------------------------------

// Текст
QString &TTableWidgetItemData::text() const
{
    return const_cast<QString&>(fText);
}
//-----------------------------------------------------------------------------

void TTableWidgetItemData::setText(QString txt)
{
    fText = txt;
}
//-----------------------------------------------------------------------------

// Элементы декорирования
TTableDecor &TTableWidgetItemData::decor() const
{
    return const_cast<TTableDecor&>(fDecor);
}
//-----------------------------------------------------------------------------

// ... выравнивание
void TTableWidgetItemData::setAlign(int algn)
{
    fDecor.alignPixmap = fDecor.alignText = algn;
}
//-----------------------------------------------------------------------------

void TTableWidgetItemData::setAlign(int algnpix, int algntxt)
{
    fDecor.alignPixmap = algnpix;
    fDecor.alignText = algntxt;
}
//-----------------------------------------------------------------------------

// Цвет текста
void TTableWidgetItemData::setForeground(QColor &clr)
{
    fDecor.setColorText(clr);
}
//-----------------------------------------------------------------------------

void TTableWidgetItemData::setForeground(Qt::GlobalColor clr)
{
  QColor c(clr);
    fDecor.setColorText(c);
}
//-----------------------------------------------------------------------------

// Цвет фона
void TTableWidgetItemData::setBackground(QColor &clr)
{
    fDecor.setColorFon(clr);
}
//-----------------------------------------------------------------------------

void TTableWidgetItemData::setBackground(Qt::GlobalColor clr)
{
  QColor c(clr);
    fDecor.setColorFon(c);
}
//-----------------------------------------------------------------------------

// Сброс в значения по умолчанию
void TTableWidgetItemData::resetColors()
{
    fDecor.resetColors();
}
//-----------------------------------------------------------------------------

// Шрифт ячейки
void TTableWidgetItemData::setFont(QFont &f)
{
    fDecor.setFont(f);
}
//-----------------------------------------------------------------------------

// Сброс в значение по умолчанию
void TTableWidgetItemData::resetFont()
{
    fDecor.resetFont();
}
//-----------------------------------------------------------------------------

// Тип элемента редактирования
TTableWidgetItemData::EditorType TTableWidgetItemData::editorType()
{
    return fEditorType;
}
//-----------------------------------------------------------------------------

// Данные редактирования
void TTableWidgetItemData::setEditorProperty(int min, int max, int step)
{
    DELETE(fSpinBoxData); DELETE(fDoubleSpinBoxData); DELETE(fComboBoxData); DELETE(fDateTimeData);
    fSpinBoxData = new SpinBoxData(min,max,step);
}
//-----------------------------------------------------------------------------

void TTableWidgetItemData::setEditorProperty(double min, double max, double step)
{
    DELETE(fSpinBoxData); DELETE(fDoubleSpinBoxData); DELETE(fComboBoxData); DELETE(fDateTimeData);
    fDoubleSpinBoxData = new DoubleSpinBoxData(min,max,step);
}
//-----------------------------------------------------------------------------

void TTableWidgetItemData::setEditorProperty(const QStringList &sl)
{
    DELETE(fSpinBoxData); DELETE(fDoubleSpinBoxData); DELETE(fComboBoxData); DELETE(fDateTimeData);
    fComboBoxData = new ComboBoxData(sl);
}
//-----------------------------------------------------------------------------

void TTableWidgetItemData::setEditorProperty(DateTimeFormat dtft, bool iscldr)
{
    DELETE(fSpinBoxData); DELETE(fDoubleSpinBoxData); DELETE(fComboBoxData); DELETE(fDateTimeData);
    fDateTimeData = new DateTimeData(dtft,iscldr);
}
//-----------------------------------------------------------------------------

void TTableWidgetItemData::setEditorProperty(DateTimeFormat dtft, QDateTime mindt, bool iscldr)
{
    DELETE(fSpinBoxData); DELETE(fDoubleSpinBoxData); DELETE(fComboBoxData); DELETE(fDateTimeData);
    fDateTimeData = new DateTimeData(dtft,mindt,iscldr);
}
//-----------------------------------------------------------------------------

void TTableWidgetItemData::setEditorProperty(DateTimeFormat dtft, QDateTime mindt, QDateTime maxdt, bool iscldr)
{
    DELETE(fSpinBoxData); DELETE(fDoubleSpinBoxData); DELETE(fComboBoxData); DELETE(fDateTimeData);
    fDateTimeData = new DateTimeData(dtft,mindt,maxdt,iscldr);
}
//-----------------------------------------------------------------------------

// Данные QSpinBox
TTableWidgetItemData::SpinBoxData *TTableWidgetItemData::spinBoxData() const
{
    return fSpinBoxData;
}
//-----------------------------------------------------------------------------

// Данные QDoubleSpinBox
TTableWidgetItemData::DoubleSpinBoxData *TTableWidgetItemData::doubleSpinBoxData() const
{
    return fDoubleSpinBoxData;
}
//-----------------------------------------------------------------------------

// Данные QComboBox
TTableWidgetItemData::ComboBoxData *TTableWidgetItemData::comboBoxData() const
{
    return fComboBoxData;
}
//-----------------------------------------------------------------------------

// Данные QDateTimeEdit
TTableWidgetItemData::DateTimeData *TTableWidgetItemData::dateTimeData() const
{
    return fDateTimeData;
}
//-----------------------------------------------------------------------------

TTableWidgetItemData &TTableWidgetItemData::operator=(const TTableWidgetItemData &twd)
{
    if (this==&twd) return *this;
    DELETE(fPixmap); DELETE(fSpinBoxData); DELETE(fDoubleSpinBoxData); DELETE(fComboBoxData); DELETE(fDateTimeData);
    fText = twd.fText;
    if (twd.fPixmap) fPixmap = new QPixmap(*twd.fPixmap);
    fDecor = twd.fDecor;
    fEditorType = twd.fEditorType;
    if (twd.fSpinBoxData) fSpinBoxData = new SpinBoxData(*twd.fSpinBoxData);
    if (twd.fDoubleSpinBoxData) fDoubleSpinBoxData = new DoubleSpinBoxData(*twd.fDoubleSpinBoxData);
    if (twd.fComboBoxData) fComboBoxData = new ComboBoxData(*twd.fComboBoxData);
    if (twd.fDateTimeData) fDateTimeData = new DateTimeData(*twd.fDateTimeData);
    return *this;
}
//-----------------------------------------------------------------------------

void TTableWidgetItemData::paint(QPainter *painter, const QRect &rect, const QPalette &palette, bool isselect) const
{
    painter->save();
    if (!isselect && fDecor.colorFon()) painter->fillRect(rect,*fDecor.colorFon());
    if (fDecor.font()) painter->setFont(*fDecor.font());
    painter->translate(rect.x(),rect.y());
  int rctW(rect.width()), rctH(rect.height()), cntrX(rctW/2), cntrY(rctH/2);
    switch (fDecor.cellReg)
    {
        case TTableDecor::crText: case TTableDecor::crPixmap: case TTableDecor::crPixmapText:
        {
            painter->setRenderHint(fPixmap ? QPainter::Antialiasing : QPainter::TextAntialiasing,true);
            painter->setPen(fDecor.colorText() ? *fDecor.colorText() : palette.color(QPalette::WindowText));
            if (!fPixmap || fText.isEmpty()) const_cast<TTableWidgetItemData*>(this)->fDecor.pixmapPlace = TTableDecor::ppSeparate;
            if (fDecor.pixmapPlace==TTableDecor::ppSeparate) // раздельно нарисовать картинку и текст
            {
              int elX(0), elY(0);
                if (fPixmap)
                {
                  if (fDecor.alignPixmap & Qt::AlignLeft) elX = 0;
                    else if (fDecor.alignPixmap & Qt::AlignRight) elX = rctW-fPixmap->width();
                    else elX = cntrX-fPixmap->width()/2;
                    if (fDecor.alignPixmap & Qt::AlignTop) elY = 0;
                    else if (fDecor.alignPixmap & Qt::AlignBottom) elY = rctH-fPixmap->height();
                    else elY = cntrY-fPixmap->height()/2;
                    painter->drawPixmap(elX,elY,*fPixmap);
                }
                if (!fText.isEmpty())
                {
                    if (isselect) painter->setPen(palette.color(QPalette::Window));
                    painter->drawText(QRect(QPoint(0,0),QSize(rctW,rctH)),fDecor.alignText,fText);
                }
            }
            else // есть и fPixmap, и fText (рассчитать места для совместного рисования картинки и текста)
            {
              int pixW(fPixmap->width()), pixH(fPixmap->height()), bndW(0), bndH(0);
                if (fDecor.pixmapPlace==TTableDecor::ppPixmapLeft || fDecor.pixmapPlace==TTableDecor::ppPixmapRight)
                {
                    bndW = rctW-pixW-fDecor.pixmapSpace;
                    bndH = rctH;
                }
                else
                {
                    bndW = rctW;
                    bndH = rctH-pixH-fDecor.pixmapSpace;
                }
              QFontMetrics fm(painter->font());
              QRect br(fm.boundingRect(0,0,bndW,bndH,fDecor.alignText,fText));
              int txtW(br.width()), txtH(br.height()),
                  sumW(fDecor.pixmapPlace==TTableDecor::ppPixmapLeft || fDecor.pixmapPlace==TTableDecor::ppPixmapRight ? pixW+fDecor.pixmapSpace+txtW : 0),
                  sumH(fDecor.pixmapPlace==TTableDecor::ppPixmapTop || fDecor.pixmapPlace==TTableDecor::ppPixmapBottom ? pixH+fDecor.pixmapSpace+txtH : 0);
              QPoint pntPix, pntTxt;
                switch (fDecor.pixmapPlace)
                {
                    case TTableDecor::ppPixmapLeft:
                        pntPix.setX((rctW-sumW)/2);
                        pntPix.setY((rctH-pixH)/2);
                        pntTxt.setX((rctW-sumW)/2+pixW+fDecor.pixmapSpace);
                        pntTxt.setY((rctH-txtH)/2);
                        break;
                    case TTableDecor::ppPixmapRight:
                        pntPix.setX((rctW-sumW)/2+txtW+fDecor.pixmapSpace);
                        pntPix.setY((rctH-pixH)/2);
                        pntTxt.setX((rctW-sumW)/2);
                        pntTxt.setY((rctH-txtH)/2);
                        break;
                    case TTableDecor::ppPixmapTop:
                        pntPix.setX((rctW-pixW)/2);
                        pntPix.setY((rctH-sumH)/2);
                        pntTxt.setX((rctW-txtW)/2);
                        pntTxt.setY((rctH-sumH)/2+pixH+fDecor.pixmapSpace);
                        break;
                    case TTableDecor::ppPixmapBottom:
                        pntPix.setX((rctW-pixW)/2);
                        pntPix.setY((rctH-sumH)/2+txtH+fDecor.pixmapSpace);
                        pntTxt.setX((rctW-txtW)/2);
                        pntTxt.setY((rctH-sumH)/2);
                        break;
                    default: ;
                }
                painter->drawPixmap(pntPix,*fPixmap);
              QRect rctTxt(pntTxt,QSize(txtW,txtH));
                if (isselect) painter->setPen(palette.color(QPalette::Window));
                painter->drawText(rctTxt,fDecor.alignText,fText);
            }
            break;
        }
        case TTableDecor::crShape:
        {
          int side(qMin(rctW,rctH)), htr(0);
            painter->setRenderHint(QPainter::Antialiasing,true);
          QPen pen(painter->pen());
            pen.setColor(fDecor.shapePen);
            pen.setWidth(fDecor.lineWidth);
            painter->setPen(pen);
            painter->setBrush(fDecor.shapeBrush);
            if (fDecor.shapeType==TTableDecor::shtCircle) painter->drawEllipse(cntrX-side/2+fDecor.pixmapSpace,cntrY-side/2+fDecor.pixmapSpace,side-2*fDecor.pixmapSpace,side-2*fDecor.pixmapSpace);
            else if (fDecor.shapeType==TTableDecor::shtRect) painter->drawRect(fDecor.pixmapSpace,fDecor.pixmapSpace,rctW-2*fDecor.pixmapSpace,rctH-2*fDecor.pixmapSpace);
            else if (fDecor.shapeType==TTableDecor::shtSquare) painter->drawRect(cntrX-side/2+fDecor.pixmapSpace,cntrY-side/2+fDecor.pixmapSpace,side-2*fDecor.pixmapSpace,side-2*fDecor.pixmapSpace);
            else if (fDecor.shapeType==TTableDecor::shtRomb)
            {
              QPoint arpnts[4] = { QPoint(fDecor.pixmapSpace,cntrY), QPoint(cntrX,fDecor.pixmapSpace), QPoint(rctW-fDecor.pixmapSpace,cntrY), QPoint(cntrX,rctH-fDecor.pixmapSpace) };
                painter->drawPolygon(arpnts,4,Qt::WindingFill);
            }
            else if (fDecor.shapeType==TTableDecor::shtSqrRomb)
            {
              int os2(side/2-fDecor.pixmapSpace);
              QPoint arpnts[4] = { QPoint(cntrX-os2,cntrY), QPoint(cntrX,cntrY-os2), QPoint(cntrX+os2,cntrY), QPoint(cntrX,cntrY+os2) };
                painter->drawPolygon(arpnts,4,Qt::WindingFill);
            }
            else if (fDecor.shapeType==TTableDecor::shtTriangle)
            {
                htr = qRound((side-2*fDecor.pixmapSpace)*sqrt(3.)/2.);
              QPoint arpnts[3] = { QPoint(cntrX,cntrY-htr/2), QPoint(cntrX+side/2-fDecor.pixmapSpace,cntrY+htr/2), QPoint(cntrX-side/2+fDecor.pixmapSpace,cntrY+htr/2) };
                painter->drawPolygon(arpnts,3,Qt::WindingFill);
            }
            else if (fDecor.shapeType==TTableDecor::shtStar)
            {
              int r(side/2-fDecor.pixmapSpace);
              TPositDblAD posAD(3*PI/2.,r);
              QPoint arpnts[10], cntr(cntrX,cntrY+(r-qRound(r*cos(PI/5.)))/2);
                for (int i=0; i<10; i++)
                {
                    posAD.d = i%2==1 ? r*2/5 : r;
                    arpnts[i] = geod_base::pgz(cntr,posAD);
                    posAD.a += PI/5.;
                }
                painter->drawPolygon(arpnts,10,Qt::WindingFill);
            }
            if (!fText.isEmpty())
            {
              QRect rctTxt(QPoint(0,htr/4),QSize(rctW,rctH-htr/4));
                painter->setPen(fDecor.colorText() ? *fDecor.colorText() : palette.color(QPalette::WindowText));
                painter->drawText(rctTxt,Qt::AlignCenter | Qt::TextWordWrap,fText);
            }
            break;
        }
        default: ;
    }
    painter->restore();
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// Класс TTableWidgetItemDataIdent - Данные с идентификатором

// КОНСТРУКТОР
TTableWidgetItemDataIdent::TTableWidgetItemDataIdent() : TTableWidgetItemData()
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemDataIdent::TTableWidgetItemDataIdent(const TTableWidgetItemDataIdent &twid) : TTableWidgetItemData(twid), fIdent(twid.fIdent)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemDataIdent::TTableWidgetItemDataIdent(const TIdent &idnt, QPixmap *pix, int algn) : TTableWidgetItemData(pix,algn), fIdent(idnt)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemDataIdent::TTableWidgetItemDataIdent(const TIdent &idnt, const QString &txt, int algn) : TTableWidgetItemData(txt,algn), fIdent(idnt)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemDataIdent::TTableWidgetItemDataIdent(const TIdent &idnt, QPixmap *pix, const QString &txt, int algnpix, int algntxt) : TTableWidgetItemData(pix,txt,algnpix,algntxt), fIdent(idnt)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemDataIdent::TTableWidgetItemDataIdent(const TIdent &idnt, QPixmap *pix, const QString &txt, TTableDecor::PixmapPlace pixplace, int pixsp) : TTableWidgetItemData(pix,txt,pixplace,pixsp), fIdent(idnt)
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TTableWidgetItemDataIdent::TTableWidgetItemDataIdent(const TIdent &idnt, TTableDecor::ShapeType sht, const QColor &clpen, const QColor &clbr, const QString &txt, int pixsp, int lnw) : TTableWidgetItemData(sht,clpen,clbr,txt,pixsp,lnw), fIdent(idnt)
{
}
//-----------------------------------------------------------------------------

// ДЕСТРУКТОР
TTableWidgetItemDataIdent::~TTableWidgetItemDataIdent()
{
}
//-----------------------------------------------------------------------------

// Идентификатор
TIdent &TTableWidgetItemDataIdent::ident() const
{
    return const_cast<TIdent&>(fIdent);
}
//-----------------------------------------------------------------------------

TTableWidgetItemDataIdent &TTableWidgetItemDataIdent::operator=(const TTableWidgetItemDataIdent &twid)
{
    if (this==&twid) return *this;
    TTableWidgetItemData::operator =(twid);
    fIdent = twid.fIdent;
    return *this;
}
//-----------------------------------------------------------------------------
