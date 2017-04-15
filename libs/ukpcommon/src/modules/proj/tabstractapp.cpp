
#include <defConst>
#include <defMacro>
#include <TAbstractApp>

TAbstractApp *TAbstractApp::project(NULL);           // pattern singleton

//*****************************************************************************

TProjError::TProjError(int n, QString nm, bool iscrit, QString fnc, QString ln) : num(n), name(nm), func(fnc), line(ln), isCritic(iscrit)
{

}
//-----------------------------------------------------------------------------

TProjError::TProjError(const TProjError &per) : num(per.num), name(per.name), func(per.func), line(per.line), isCritic(per.isCritic)
{

}
//-----------------------------------------------------------------------------

TProjError &TProjError::operator=(const TProjError &per)
{
    if (this==&per) return *this;
    num = per.num;
    name = per.name;
    func = per.func;
    line = per.line;
    isCritic = per.isCritic;
    return *this;
}
//-----------------------------------------------------------------------------

void TProjError::toConsole()
{
    qWarning() << (isCritic ? CLR_RED : CLR_MAGENTA) << num << name << CLR_GRAY << func << line << CLR_RESET;
}
//-----------------------------------------------------------------------------

QString TProjError::toStr()
{
    return QString("%1. %2. [%3][%4]").arg(num).arg(name).arg(func).arg(line);
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// Структура TUiData - данные интерфейса

// КОНСТРУКТОР
TUiData::TUiData() :
fontSize(0),
btnSize(0,0),
btnOkSize(0,0),
sqrElSize(0,0),
edBigXYSize(0,0),
edSmallXYSize(0,0),
edHeight(0),
btnTabHeight(0),
spaceBig(0),
spaceSmall(0)
{
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// Структура TPaletteColors - цвета палитры

// КОНСТРУКТОР
#if defined(CPL_XFIRE)
TPaletteColors::TPaletteColors(QString nm, QString mnnm) : name(nm), mnName(mnnm), isActive(false),
base(255,255,255),
panel(184,184,184),
button(221,221,221),
altButton(202,202,202),
title(126,126,126),
actTitle(42,42,42),
actTitleText(255.255,255),
hilight(221,221,221),
text(0,0,0),
hilightText(0,0,0),
textRed(170,0,0),
textBlue(0,0,127),
altText(110,110,110)
#else
TPaletteColors::TPaletteColors(QString nm, QString mnnm) : name(nm), mnName(mnnm), isActive(false),
base(218,222,206),
panel(160,189,179),
button(222,216,184),
altButton(185,207,199),
title(234,230,222),
actTitle(49,72,64),
actTitleText(255,255,127),
hilight(211,191,112),
text(0,0,0),
hilightText(0,0,0),
textRed(170,0,0),
textBlue(0,0,127),
altText(110,110,110)
#endif
{
}
//-----------------------------------------------------------------------------

// КОНСТРУКТОР
TPaletteColors::TPaletteColors(const TPaletteColors &pc) : name(pc.name), mnName(pc.mnName), isActive(pc.isActive), base(pc.base), panel(pc.panel), button(pc.button), altButton(pc.altButton), title(pc.title), actTitle(pc.actTitle), actTitleText(pc.actTitleText), hilight(pc.hilight), text(pc.text), hilightText(pc.hilightText), textRed(pc.textRed), textBlue(pc.textBlue), altText(pc.altText)
{
}
//-----------------------------------------------------------------------------

TPaletteColors &TPaletteColors::operator=(const TPaletteColors &pc)
{
    if (&pc==this) return *this;
    name = pc.name;
    mnName = pc.mnName;
    isActive = pc.isActive;
    base = pc.base;
    panel = pc.panel;
    button = pc.button;
    altButton = pc.altButton;
    title = pc.title;
    actTitle = pc.actTitle;
    actTitleText = pc.actTitleText;
    hilight = pc.hilight;
    text = pc.text;
    hilightText = pc.hilightText;
    textRed = pc.textRed;
    textBlue = pc.textBlue;
    altText = pc.altText;
    return *this;
}
//-----------------------------------------------------------------------------

TPaletteColors &TPaletteColors::operator=(const QPalette &pal)
{
    base = pal.base().color();
    panel = pal.window().color();
    button = pal.button().color();
    text = pal.buttonText().color();
    hilight = pal.highlight().color();
    hilightText = pal.highlightedText().color();
    textRed = QColor(170,0,0);
    textBlue = QColor(0,0,127);
    altText = QColor(110,110,110);
//  QString sVal(PROJ->iniValue(QString("PALSYS_TITLE_%1").arg(PROJ->iniSuffix()))), sRed(sVal.section(".",0,0)), sGreen(sVal.section(".",1,1)), sBlue(sVal.section(".",2,2));
//    title = QColor(sRed.toInt(),sGreen.toInt(),sBlue.toInt());
//    sVal = PROJ->iniValue(QString("PALSYS_ALT_BUTTON_%1").arg(PROJ->iniSuffix())); sRed = sVal.section(".",0,0); sGreen = sVal.section(".",1,1); sBlue = sVal.section(".",2,2);
//    altButton = QColor(sRed.toInt(),sGreen.toInt(),sBlue.toInt());
//    sVal = PROJ->iniValue(QString("PALSYS_ACT_TITLE_%1").arg(PROJ->iniSuffix())); sRed = sVal.section(".",0,0); sGreen = sVal.section(".",1,1); sBlue = sVal.section(".",2,2);
//    actTitle = QColor(sRed.toInt(),sGreen.toInt(),sBlue.toInt());
    //sVal = PROJ->iniValue(QString("PALSYS_ACT_TITLE_TEXT_%1").arg(PROJ->iniSuffix())); sRed = sVal.section(".",0,0); sGreen = sVal.section(".",1,1); sBlue = sVal.section(".",2,2);
    //actTitleText = QColor(sRed.toInt(),sGreen.toInt(),sBlue.toInt());
    actTitleText = base;
    return *this;
}
//-----------------------------------------------------------------------------

// Преобразование в строку
QString TPaletteColors::toStr()
{
    return QString("Palette.base: %1\nPalette.panel: %2\nPalette.button: %3\nPalette.altButton: %4\nPalette.title: %5\nPalette.hilight: %6\nPalette.text: %7\nPalette.textRed: %8\nPalette.altText: %9").arg(QString("%1.%2.%3").arg(base.red()).arg(base.green()).arg(base.blue())).arg(QString("%1.%2.%3").arg(panel.red()).arg(panel.green()).arg(panel.blue())).arg(QString("%1.%2.%3").arg(button.red()).arg(button.green()).arg(button.blue())).arg(QString("%1.%2.%3").arg(altButton.red()).arg(altButton.green()).arg(altButton.blue())).arg(QString("%1.%2.%3").arg(title.red()).arg(title.green()).arg(title.blue())).arg(QString("%1.%2.%3").arg(title.red()).arg(title.green()).arg(title.blue())).arg(QString("%1.%2.%3").arg(text.red()).arg(text.green()).arg(text.blue())).arg(QString("%1.%2.%3").arg(textRed.red()).arg(textRed.green()).arg(textRed.blue())).arg(QString("%1.%2.%3").arg(altText.red()).arg(altText.green()).arg(altText.blue()));
}
//-----------------------------------------------------------------------------

//*****************************************************************************

TAbstractApp::TAbstractApp(QStringList &params, int n, QString nm) : TAbstractObject(0,n,nm), fParams(params), fModuleEmployees(new TModuleEmployees(pmtEmployees)), fModuleUnits(new TModuleUnits(pmtUnits)), fModuleProductions(new TModuleProductions(pmtProductions)), fIvZeroAndMore(NULL), fIvAll(NULL), fDvAll(NULL), fIsRunning(false)
{
#if defined(Q_OS_LINUX)
    fDataPath = "/usr/share/ukp/";
#elif defined(Q_OS_WIN32)
    fDataPath = "../../data/var/";
#elif defined(Q_OS_OSX)
    fDataPath = "../../../../../data/var/";
#endif
    fPlModules.setAutoDelete(true);
    fPlErrors.setAutoDelete(true);
    fPlModules.append(fModuleEmployees);
    fPlModules.append(fModuleUnits);
    fPlModules.append(fModuleProductions);
}
//-----------------------------------------------------------------------------

TAbstractApp::~TAbstractApp()
{
    TAbstractApp::project = NULL;
}
//-----------------------------------------------------------------------------

void TAbstractApp::reset(bool thisonly)
{
    if (!thisonly) TAbstractObject::reset(true);
    fDataPath = "";
    fParams.clear();
    fPlModules.clear();
    fPlErrors.clear();
    DELETE(fIvZeroAndMore);
    DELETE(fIvAll);
    DELETE(fDvAll);
    fIsRunning = false;
}
//-----------------------------------------------------------------------------

bool TAbstractApp::init()
{
    emit sig_progressValue("Инициализация окружения ...",10);
    fIvZeroAndMore = new QIntValidator(0,MAX_LONG,this);
    fIvAll = new QIntValidator(MIN_LONG,MAX_LONG,this);
    fDvAll = new QDoubleValidator(MIN_LONG,MAX_LONG,2,this);
    if (!fModuleEmployees->createSelfEmployee()) return false;
    if (!fModuleUnits->init()) return false;
    fModuleEmployees->init();
    fModuleProductions->init();
    //loadIni();
    return true;
}
//-----------------------------------------------------------------------------

bool TAbstractApp::run()
{
    return true;
}
//-----------------------------------------------------------------------------

QStringList &TAbstractApp::params() const
{
    return const_cast<QStringList&>(fParams);
}
//-----------------------------------------------------------------------------

void TAbstractApp::insertModule(TAbstractModule *m)
{
    if (m && fPlModules.indexOf(m)==-1) fPlModules.append(m);
}
//-----------------------------------------------------------------------------

void TAbstractApp::removeModule(TAbstractModule *m)
{
    fPlModules.removeOne(m);
}
//-----------------------------------------------------------------------------

void TAbstractApp::clearModules()
{
    fPlModules.clear();
}

QString &TAbstractApp::dataPath() const
{
    return const_cast<QString&>(fDataPath);
}
//-----------------------------------------------------------------------------

TAbstractModule *TAbstractApp::findModule(TProjModuleType mt)
{
    foreach (TAbstractModule *curMod,fPlModules)
        if (curMod->type()==mt) return curMod;
    return NULL;
}
//-----------------------------------------------------------------------------

TProjectErrorList &TAbstractApp::errors() const
{
    return const_cast<TProjectErrorList&>(fPlErrors);
}
//-----------------------------------------------------------------------------

void TAbstractApp::insertError(QString nm, bool iscrit, QString fnc, QString ln)
{
    fPlErrors.append(new TProjError(fPlErrors.count()+1,nm,iscrit,fnc,ln));
}
//-----------------------------------------------------------------------------

void TAbstractApp::printErrors()
{
    if (!fPlErrors.count()) return;
    PR(0,"================================================================================");
    foreach (TProjError *curErr,fPlErrors) curErr->toConsole();
    PR(0,"================================================================================");
}
//-----------------------------------------------------------------------------

bool TAbstractApp::presentCriticalErrors()
{
    foreach (TProjError *curErr,fPlErrors)
        if (curErr->isCritic) return true;
    return false;
}
//-----------------------------------------------------------------------------

// Начало рабочего дня
const QTime &TAbstractApp::workDayBegin() const
{
    return const_cast<QTime&>(fWorkDayBegin);
}
//-----------------------------------------------------------------------------

void TAbstractApp::setWorkDayBegin(const QTime &t)
{
    fWorkDayBegin = t;
}
//-----------------------------------------------------------------------------

// Данные интерфейса
TUiData &TAbstractApp::uiData() const
{
    return const_cast<TUiData&>(fUiData);
}
//-----------------------------------------------------------------------------

TPaletteColors &TAbstractApp::currentPalette() const
{
    return const_cast<TPaletteColors&>(fCurrentPalette);
}
//-----------------------------------------------------------------------------

QString TAbstractApp::toStr()
{
    return "";
}
//-----------------------------------------------------------------------------

QString TAbstractApp::toHtml(bool)
{
    return "";
}
//-----------------------------------------------------------------------------

bool TAbstractApp::toDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------

bool TAbstractApp::fromDB(QString)
{
    return true;
}
//-----------------------------------------------------------------------------
