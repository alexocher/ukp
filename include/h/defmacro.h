
//***************************************************************************//
//                                                                           //
//                                 Макросы                                   //
//                                                                           //
//***************************************************************************//

#ifndef DEFMACRO_H
#define DEFMACRO_H

#include <QtDebug>
#include <TAbstractApp>

//#define EMPTY QString("")

// Элементы интерфейса
#define UI_FNT_SZ TAbstractApp::project->uiData().fontSize
#define UI_BTN_SZ TAbstractApp::project->uiData().btnSize
#define UI_BTNOK_SZ TAbstractApp::project->uiData().btnOkSize
#define UI_SQR_SZ TAbstractApp::project->uiData().sqrElSize
#define UI_BIGXY_SZ TAbstractApp::project->uiData().edBigXYSize
#define UI_SMXY_SZ TAbstractApp::project->uiData().edSmallXYSize
#define UI_ED_H TAbstractApp::project->uiData().edHeight
#define UI_BTNTAB_H TAbstractApp::project->uiData().btnTabHeight
#define UI_SP_BIG TAbstractApp::project->uiData().spaceBig
#define UI_SP_SM TAbstractApp::project->uiData().spaceSmall

// Цвета палитры
#define PAL_BASE           TAbstractApp::project->currentPalette().base
#define PAL_PANEL          TAbstractApp::project->currentPalette().panel
#define PAL_BUTTON         TAbstractApp::project->currentPalette().button
#define PAL_BUTTON_ALT     TAbstractApp::project->currentPalette().altButton
#define PAL_TITLE          TAbstractApp::project->currentPalette().title
#define PAL_TITLE_ACT      TAbstractApp::project->currentPalette().actTitle
#define PAL_TEXT           TAbstractApp::project->currentPalette().text
#define PAL_TEXT_HILIGHT   TAbstractApp::project->currentPalette().hilightText
#define PAL_TEXT_RED       TAbstractApp::project->currentPalette().textRed
#define PAL_TEXT_BLUE      TAbstractApp::project->currentPalette().textBlue
#define PAL_TEXT_ALT       TAbstractApp::project->currentPalette().altText
#define PAL_TITLE_TEXT     PAL_TEXT
#define PAL_TITLE_TEXT_ACT TAbstractApp::project->currentPalette().actTitleText
#define PAL_LABEL_TAB      TAbstractApp::project->currentPalette().tabLabel
#define PAL_HILIGHT        TAbstractApp::project->currentPalette().hilight
#define PAL_YELLOW_LT      QColor(255.255.127)

// Флаги
#define TWI_EDIT_YES(it) (it)->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
#define TWI_EDIT_NO(it) (it)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

// Установка шрифта
#define FONT(el,sz,b) {QFont fnt(el->font());if(sz>0)fnt.setPointSize(sz);fnt.setBold(b);el->setFont(fnt);}
#define FONT_BOLD(el,b) {QFont fnt(el->font());fnt.setBold(b);el->setFont(fnt);}
#define FONT_FROM(el,fromel,sz,b) {QFont fnt((fromel)->font());if(sz>0)fnt.setPointSize(sz);fnt.setBold(b);(el)->setFont(fnt);}

// Приложения ...
#define PROJ         TAbstractApp::project
#define PROJUKP      ((TAppUkp*)TAbstractApp::project)
#define PROJPLAN     ((TAppPlan*)TAbstractApp::project)
#define PROJCARRYPLAN ((TAppCarryPlan*)TAbstractApp::project)

// Склейка ...
#define JOINT(frst,scnd) frst##scnd

// Модули проекта
#define MODULE(Type) TModule##Type *mod##Type = dynamic_cast<TModule##Type*>(PROJ->findModule(pmt##Type))

// Выбор минимального (максимального) значения
#define MAX2(a,b) ((a)>(b)?(a):(b))
#define MAX3(a,b,c) ((MAX2(a,b))>(c)?(MAX2(a,b)):(c))
#define MAX4(a,b,c,d) MAX2(MAX2(a,b),MAX2(c,d))
#define MIN2(a,b) ((a)>(b)?(b):(a))
#define MIN3(a,b,c) ((MIN2(a,b))<(c)?(MIN2(a,b)):(c))
#define MIN4(a,b,c,d) MIN2(MIN2(a,b),MIN2(c,d))

// Удаление объекта c присвоением указателю NULL
#define DELETE(ob) {if((ob)){delete ob;ob=NULL;}}

// Проверка нажатия функциональных клавищ
// ( порядок всегда CTRL ALT SHIFT )
// ... отдельные (нажатые или ненажатые)
#define IS_CTRL(k) (k->state() & ControlButton)
#define IS_NOTCTRL(k) (!(k->state() & ControlButton))
#define IS_ALT(k) (k->state() & AltButton)
#define IS_NOTALT(k) (!(k->state() & AltButton))
#define IS_SHIFT(k) (k->state() & ShiftButton)
#define IS_NOTSHIFT(k) (!(k->state() & ShiftButton))
// ... сочетания (нажатые)
#define IS_CTRL_ALT(k) ((k->state() & ControlButton) && (k->state() & AltButton))
#define IS_CTRL_SHIFT(k) ((k->state() & ControlButton) && (k->state() & ShiftButton))
#define IS_ALT_SHIFT(k) ((k->state() & AltButton) && (k->state() & ShiftButton))
#define IS_CTRL_ALT_SHIFT(k) ((k->state() & ControlButton) && (k->state() & AltButton) && (k->state() & ShiftButton))
// ... сочетания (ненажатые)
#define IS_NOTCTRL_NOTALT(k) (!(k->state() & ControlButton) && !(k->state() & AltButton))
#define IS_NOTCTRL_NOTSHIFT(k) (!(k->state() & ControlButton) && !(k->state() & ShiftButton))
#define IS_NOTALT_NOTSHIFT(k) (!(k->state() & AltButton) && !(k->state() & ShiftButton))
#define IS_NOTCTRL_NOTALT_NOTSHIFT(k) (!(k->state() & ControlButton) && !(k->state() & AltButton) && !(k->state() & ShiftButton))
// ... сочетания (нажатые и ненажатые)
#define IS_NOTCTRL_ALT(k) (!(k->state() & ControlButton) && (k->state() & AltButton))
#define IS_CTRL_NOTALT(k) ((k->state() & ControlButton) && !(k->state() & AltButton))
#define IS_NOTCTRL_SHIFT(k) (!(k->state() & ControlButton) && (k->state() & ShiftButton))
#define IS_CTRL_NOTSHIFT(k) ((k->state() & ControlButton) && !(k->state() & ShiftButton))
#define IS_NOTALT_SHIFT(k) (!(k->state() & AltButton) && (k->state() & ShiftButton))
#define IS_ALT_NOTSHIFT(k) ((k->state() & AltButton) && !(k->state() & ShiftButton))
#define IS_NOTCTRL_NOTALT_SHIFT(k) (!(k->state() & ControlButton) && !(k->state() & AltButton) && (k->state() & ShiftButton))
#define IS_NOTCTRL_ALT_NOTSHIFT(k) (!(k->state() & ControlButton) && (k->state() & AltButton) && !(k->state() & ShiftButton))
#define IS_CTRL_NOTALT_SHIFT(k) ((k->state() & ControlButton) && !(k->state() & AltButton) && (k->state() & ShiftButton))
#define IS_CTRL_ALT_NOTSHIFT(k) ((k->state() & ControlButton) && (k->state() & AltButton) && !(k->state() & ShiftButton))
#define IS_CTRL_NOTALT_NOTSHIFT(k) ((k->state() & ControlButton) && !(k->state() & AltButton) && !(k->state() & ShiftButton))

// Задание картинок
#define ICONPIX(pix) QIcon(QPixmap(pix))
#define PIXMAP(pix) QPixmap::fromMimeSource(pix)
#define ICONSET(pix) QIconSet(PIXMAP(pix))
#define ICONSET_LG(pix) QIconSet(PIXMAP(pix),QIconSet::Large)

// Вывод информации в консоль
#define PR_BR qWarning("")
#define PR(sp,s) {QString sSp(""),sS(s),sd("");for(int z=0;z<(sp);z++)sSp+=" ";sd=sSp+sS;qDebug().noquote()<<sd;}
#define PR1(sp,s,a1) {QString sSp(""),sS(s),sd("");for(int z=0;z<(sp);z++)sSp+=" ";sd=sSp+sS.arg(a1);qDebug().noquote()<<sd;}
#define PR2(sp,s,a1,a2) {QString sSp(""),sS(s),sd("");for(int z=0;z<(sp);z++)sSp+=" ";sd=sSp+sS.arg(a1).arg(a2);qDebug().noquote()<<sd;}
#define PR3(sp,s,a1,a2,a3) {QString sSp(""),sS(s),sd("");for(int z=0;z<(sp);z++)sSp+=" ";sd=sSp+sS.arg(a1).arg(a2).arg(a3);qDebug().noquote()<<sd;}
#define PR4(sp,s,a1,a2,a3,a4) {QString sSp(""),sS(s),sd("");for(int z=0;z<(sp);z++)sSp+=" ";sd=sSp+sS.arg(a1).arg(a2).arg(a3).arg(a4);qDebug().noquote()<<sd;}
#define PR5(sp,s,a1,a2,a3,a4,a5) {QString sSp(""),sS(s),sd("");for(int z=0;z<(sp);z++)sSp+=" ";sd=sSp+sS.arg(a1).arg(a2).arg(a3).arg(a4).arg(a5);qDebug().noquote()<<sd;}
#define PR6(sp,s,a1,a2,a3,a4,a5,a6) {QString sSp(""),sS(s),sd("");for(int z=0;z<(sp);z++)sSp+=" ";sd=sSp+sS.arg(a1).arg(a2).arg(a3).arg(a4).arg(a5).arg(a6);qDebug().noquote()<<sd;}
#define PR7(sp,s,a1,a2,a3,a4,a5,a6,a7) {QString sSp(""),sS(s),sd("");for(int z=0;z<(sp);z++)sSp+=" ";sd=sSp+sS.arg(a1).arg(a2).arg(a3).arg(a4).arg(a5).arg(a6).arg(a7);qDebug().noquote()<<sd;}
#define PR8(sp,s,a1,a2,a3,a4,a5,a6,a7,a8) {QString sSp(""),sS(s),sd("");for(int z=0;z<(sp);z++)sSp+=" ";sd=sSp+sS.arg(a1).arg(a2).arg(a3).arg(a4).arg(a5).arg(a6).arg(a7).arg(a8);}qDebug().noquote()<<sd;}
#define PR9(sp,s,a1,a2,a3,a4,a5,a6,a7,a8,a9) {QString sSp(""),sS(s),sd("");for(int z=0;z<(sp);z++)sSp+=" ";sd=sSp+sS.arg(a1).arg(a2).arg(a3).arg(a4).arg(a5).arg(a6).arg(a7).arg(a8).arg(a9);qDebug().noquote()<<sd;}

// Вывод информации в файл
#define RU(s) QString(s)
#define FPR_BR ts<<endl;
#define FPR1(sp,a1) {QString sSp(""),sd("");for(int z=0;z<(sp);z++)sSp+=" ";ts<<sSp<<a1<<endl;}
#define FPR2(sp,a1,a2) {QString sSp(""),sd("");for(int z=0;z<(sp);z++)sSp+=" ";ts<<sSp<<a1<<a2<<endl;}
#define FPR3(sp,a1,a2,a3) {QString sSp(""),sd("");for(int z=0;z<(sp);z++)sSp+=" ";ts<<sSp<<a1<<a2<<a3<<endl;}
#define FPR4(sp,a1,a2,a3,a4) {QString sSp(""),sd("");for(int z=0;z<(sp);z++)sSp+=" ";ts<<sSp<<a1<<a2<<a3<<a4<<endl;}
#define FPR5(sp,a1,a2,a3,a4,a5) {QString sSp(""),sd("");for(int z=0;z<(sp);z++)sSp+=" ";ts<<sSp<<a1<<a2<<a3<<a4<<a5<<endl;}
#define FPR6(sp,a1,a2,a3,a4,a5,a6) {QString sSp(""),sd("");for(int z=0;z<(sp);z++)sSp+=" ";ts<<sSp<<a1<<a2<<a3<<a4<<a5<<a6<<endl;}
#define FPR7(sp,a1,a2,a3,a4,a5,a6,a7)  {QString sSp(""),sd("");for(int z=0;z<(sp);z++)sSp+=" ";ts<<sSp<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<endl;}
#define FPR8(sp,a1,a2,a3,a4,a5,a6,a7,a8) {QString sSp(""),sd("");for(int z=0;z<(sp);z++)sSp+=" ";ts<<sSp<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<endl;}
#define FPR9(sp,a1,a2,a3,a4,a5,a6,a7,a8,a9) {QString sSp(""),sd("");for(int z=0;z<(sp);z++)sSp+=" ";ts<<sSp<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<endl;}

// Строка свойства и значения
#define SPROPERTY_INT(w1,w2,sp,i)   QString("<TR><TD width=%1>%2</TD><TD width=%3><B>%4</B></TD></TR>").arg(w1).arg(QString(sp)).arg(w2).arg(i)
#define SPROPERTY_FLOAT(w1,w2,sp,d) QString("<TR><TD width=%1>%2</TD><TD width=%3><B>%4</B></TD></TR>").arg(w1).arg(QString(sp)).arg(w2).arg(d)
#define SPROPERTY_STR(w1,w2,sp,s)   QString("<TR><TD width=%1>%2</TD><TD width=%3><B>%4</B></TD></TR>").arg(w1).arg(QString(sp)).arg(w2).arg(QString(s))
#define SPROPERTY_BOOL(w1,w2,sp,b)  QString("<TR><TD width=%1>%2</TD><TD width=%3><B>%4</B></TD></TR>").arg(w1).arg(QString(sp)).arg(w2).arg(b?"TRUE":"FALSE")
#define SPROPERTY_DT(w1,w2,sp,dt)   QString("<TR><TD width=%1>%2</TD><TD width=%3><B>%4</B></TD></TR>").arg(w1).arg(QString(sp)).arg(w2).arg(dt.toString("hh:mm:ss dd:MM:yy"))

#define SPROPERTY_EXT_INT(w1,w2,w3,sp,si,i)   QString("<TR><TD width=%1>%2</TD><TD width=%3><B>%4</B></TD><TD width=%5><B>%6</B></TD></TR>").arg(w1).arg(QString(sp)).arg(w2).arg(QString(si)).arg(w3).arg(i)
#define SPROPERTY_EXT_FLOAT(w1,w2,w3,sp,sd,d) QString("<TR><TD width=%1>%2</TD><TD width=%3><B>%4</B></TD><TD width=%5><B>%6</B></TD></TR>").arg(w1).arg(QString(sp)).arg(w2).arg(QString(sd)).arg(w3).arg(d)
#define SPROPERTY_EXT_STR(w1,w2,w3,sp,s,ss)   QString("<TR><TD width=%1>%2</TD><TD width=%3><B>%4</B></TD><TD width=%5><B>%6</B></TD></TR>").arg(w1).arg(QString(sp)).arg(w2).arg(QString(s)).arg(w3).arg(QString(ss))
#define SPROPERTY_EXT_BOOL(w1,w2,w3,sp,b)  QString("<TR><TD width=%1>%2</TD><TD width=%3><B>%4</B></TD><TD width=%5><B>%6</B></TD></TR>").arg(w1).arg(QString(sp)).arg(w2).arg(b).arg(w3).arg(b?"ДА":"НЕТ")
#define SPROPERTY_EXT_DT(w1,w2,w3,sp,dt)   QString("<TR><TD width=%1>%2</TD><TD width=%3><B>%4</B></TD><TD width=%5><B>%6</B></TD></TR>").arg(w1).arg(QString(sp)).arg(w2).arg(dt.toTime_t()).arg(w3).arg(dt.toString("hh:mm:ss dd:MM:yy"))

// Изменение размеров форм
#define FRM_MINMAX_WIDTH(w) {setMinimumWidth(w);setMaximumWidth(minimumWidth());}
#define FRM_MINMAX_HEIGHT(h) {setMinimumHeight(h);setMaximumHeight(minimumHeight());}
#define FRM_RESIZE(sw,sh) ({int w,h;if(sw!=SZ_WIDTH_SELF){w=sw==SZ_WIDTH_SCR?project->scrSize().scrWidth:project->scrSize().wrkWidth;setMinimumWidth(w);setMaximumWidth(w);}if(sh!=SZ_HEIGHT_SELF){h=sh==SZ_HEIGHT_SCR?project->scrSize().scrHeight:project->scrSize().wrkHeight;setMinimumHeight(h);setMaximumHeight(h);}})
#define EL_MINMAX_WIDTH(el,w) {el->setMinimumWidth(w);el->setMaximumWidth(el->minimumWidth());}
#define EL_MINMAX_HEIGHT(el,h) {el->setMinimumHeight(h);el->setMaximumHeight(el->minimumHeight());}
#define EL_RESIZE(el,w,h) {EL_MINMAX_WIDTH(el,w);EL_MINMAX_HEIGHT(el,h)}
#define FRM_MINIMIZE {qApp->processEvents(); EL_RESIZE(this,0,0); qApp->processEvents();}
#define FRM_RESTORE {qApp->processEvents(); EL_RESIZE(this,FRM_W,FRM_H); qApp->processEvents();}

// Запись ошибки в список (с выводом в консоль)
#define INSERT_ERROR(nm,iscr) {PROJ->insertError((nm),(iscr),QString(__PRETTY_FUNCTION__),QString("%1").arg(__LINE__));\
qWarning().noquote() << CLR_RED << "ERROR. " << (nm) << CLR_GRAY << " [" << __PRETTY_FUNCTION__ << ":" << __LINE__ << "]" << CLR_RESET;}
//PR6(0,"%1ERROR. %2 %3[%4:%5]%6",CLR_RED,(nm),CLR_GRAY,__PRETTY_FUNCTION__,__LINE__,CLR_RESET);}
#define INS_ERROR(nm,iscr) {insertError((nm),(iscr),QString(__PRETTY_FUNCTION__),QString("%1").arg(__LINE__));\
qWarning().noquote() << CLR_RED << "ERROR. " << (nm) << CLR_GRAY << " [" << __PRETTY_FUNCTION__ << ":" << __LINE__ << "]" << CLR_RESET;}
//PR6(0,"%1ERROR. %2 %3[%4:%5]%6",CLR_RED,(nm),CLR_GRAY,__PRETTY_FUNCTION__,__LINE__,CLR_RESET);}

/*
Управление цветом при выводе в консоль
    \033      ESC-последовательность управления консолью
    \033[m     отмена модификаторов цвета
    \033[0m    отмена модификаторов цвета
    \033[1m    яркий
    \033[4m    подчёркнутый в PUTTY, яркий в FreeBSD
    \033[5m    светлый в PUTTY, без эффекта в FreeBSD
    \033[7m    инверсия
Непонятно что
    \033[12m   после этого консоль PUTTY превратится в стадо кракозябр...
    \033[21m   подчёркнутый в PUTTY, без эффекта в FreeBSD
Цвет символов
    \033[30m   чёрный шрифт
    \033[31m   красный шрифт
    \033[32m   зелёный шрифт
    \033[33m   жёлтый шрифт в PUTTY, коричневый в FreeBSD (если не яркий)
    \033[34m   синий шрифт
    \033[35m   малиновый шрифт
    \033[36m   голубой шрифт
    \033[37m   серый шрифт
Цвет фона
    \033[40m   чёрный фон
    \033[41m   красный фон
    \033[42m   зелёный фон
    \033[43m   жёлтый фон в PUTTY, коричневый в FreeBSD
    \033[44m   синий фон
    \033[45m   малиновый фон
    \033[46m   голубой фон
    \033[47m   серый фон
Пример
    printf("\033[1;34;47mЯркий (1) синий текст (34) на сером фоне (47)\033[0m\n");
*/

// Макросы вывода в консоль
#define CLR_RESET "\033[0m"

#define CLR_BLACK      "\033[30m"
#define CLR_RED        "\033[31m"
#define CLR_GREEN      "\033[32m"
#define CLR_YELLOW     "\033[33m"
#define CLR_NAVY       "\033[34m"
#define CLR_MAGENTA    "\033[35m"
#define CLR_BLUE       "\033[36m"
#define CLR_GRAY       "\033[37m"

#define CLR_RED_LT     "\033[1;31m"
#define CLR_GREEN_LT   "\033[1;32m"
#define CLR_YELLOW_LT  "\033[1;33m"
#define CLR_NAVY_LT    "\033[1;34m"
#define CLR_MAGENTA_LT "\033[1;35m"
#define CLR_BLUE_LT    "\033[1;36m"

#define PR_LN              qWarning() << CLR_GRAY << "[" << __LINE__ << "]" << CLR_RESET;
#define PR_FN              qWarning() << CLR_GRAY << "[" << __PRETTY_FUNCTION__ << "]" << CLR_RESET;
#define PR_FN_LN           qWarning() << CLR_GRAY << "[" << __PRETTY_FUNCTION__ << "][" << __LINE__ << "]" << CLR_RESET;
#define PR_FN_LN_MSG(s,cl) qWarning() << CLR_GRAY << "[" << __PRETTY_FUNCTION__ << "][" << __LINE__ << "]" << CLR_RESET << cl << s << CLR_RESET;

#define PRINT_BYTEARRAY_CODES(ar,sz) for(uint z=0;z<(uint)sz;z++){printf("%4d\t",(int)((unsigned char)ar[z]));if((z+1)%5==0 || (int)z==((int)sz-1))printf("\n");}

#endif // DEFMACRO_H
