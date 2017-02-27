#ifndef ENUMS_H
#define ENUMS_H

#include <QString>
#include "defdbconst.h"

// Тип модуля проекта
enum TProjModuleType {
                        pmtNone        = 0, // Не определен
                        pmtEmployees   = 1, // Модуль работы с должностными лицами
                        pmtUnits       = 2, //                 подразделениями
                        pmtProductions = 3, //                 продукцией
                        pmtPlans       = 4  //                 планами
};

// Уровень подразделения
enum TUnitLevel {
                        ulvNone        = 0,                     // Не определен
                        ulvCompany     = UNIT_LEVEL_COMPANY,    // Предприятие
                        ulvDirection   = UNIT_LEVEL_DIRECTION,  // Направление
                        ulvDepartment  = UNIT_LEVEL_DEPARTMENT, // Отдел
                        ulvSection     = UNIT_LEVEL_SECTION     // Отделение
};

typedef QList<TUnitLevel> TUnitLevelList;

// Тип должности
enum TEmployeeType {
                        eltNone           = 0,                             // Не определен
                        eltChief          = EMPLOYEE_TYPE_CHIEF,           // Начальник
                        eltSubstituent    = EMPLOYEE_TYPE_SUBSTITUENT,     // Заместитель
                        eltMajorEngineer  = EMPLOYEE_TYPE_MAJOR_ENGINEER,  // Главный инженер
                        eltLeadingEditor  = EMPLOYEE_TYPE_LEADING_EDITOR,  // Ведущий редактор
                        eltMajorEditor    = EMPLOYEE_TYPE_MAJOR_EDITOR,    // Главный редактор
                        eltSeniorEditor   = EMPLOYEE_TYPE_SENIOR_EDITOR,   // Старший редактор
                        eltTechEditor     = EMPLOYEE_TYPE_TECH_EDITOR,     // Технический редактор
                        eltResearchEditor = EMPLOYEE_TYPE_RESEARCH_EDITOR, // Редактор изученности
                        eltExpertEditor   = EMPLOYEE_TYPE_EXPERT_EDITOR,   // Редактор технической экспертизы
                        eltEditor         = EMPLOYEE_TYPE_EDITOR,          // Редактор
                        eltOobgdOperator  = EMPLOYEE_TYPE_OOBGD_OPERATOR,  // Оператор ООБГД
                        eltOperator       = EMPLOYEE_TYPE_OPERATOR         // Оператор
};

typedef QList<TEmployeeType> TEmployeeTypeList;

// Тип штатной должности
enum TEstablishedPostType {
                        eptNone                        = 0,                                                  // Не определен
                        eptChiefCompany                = ESTABLISHED_POST_TYPE_CHIEF_COMPANY,                // Начальник Центрального картографического производства ВМФ
                        eptSubstituentCompanyMap       = ESTABLISHED_POST_TYPE_SUBSTITUENT_COMPANY_MAP,      // Заместитель начальника производства (по подготовке к изданию морских карт)
                        eptSubstituentCompanyBook      = ESTABLISHED_POST_TYPE_SUBSTITUENT_COMPANY_BOOK,     // Заместитель начальника производства (по подготовке к изданию руководств и пособий для плавания)
                        eptMajorEngineer               = ESTABLISHED_POST_TYPE_MAJOR_ENGINEER,               // Главный инженер
                        eptChiefDepartment             = ESTABLISHED_POST_TYPE_CHIEF_DEPARTMENT,             // Начальник отдела
                        eptSubstituentDepartment       = ESTABLISHED_POST_TYPE_SUBSTITUENT_DEPARTMENT,       // Заместитель начальника отдела
                        eptChiefSection                = ESTABLISHED_POST_TYPE_CHIEF_SECTION,                // Начальник отделения
                        eptSeniorEditorMap             = ESTABLISHED_POST_TYPE_SENIOR_EDITOR_MAP,            // Старший редактор морских карт
                        eptEditorMap                   = ESTABLISHED_POST_TYPE_EDITOR_MAP,                   // Редактор морских карт
                        eptSeniorEditorBook            = ESTABLISHED_POST_TYPE_SENIOR_EDITORBOOK,            // Старший редактор руководств для плавания
                        eptEditorBook                  = ESTABLISHED_POST_TYPE_EDITOR_BOOK,                  // Редактор руководств для плавания
                        eptSeniorTechEditorMapBook     = ESTABLISHED_POST_TYPE_SENIOR_TECH_EDITOR_MAP_BOOK,  // Старший технический редактор КМ и руководств для плавания
                        eptTechEditorMapBook           = ESTABLISHED_POST_TYPE_TECH_EDITOR_MAP_BOOK,         // Технический редактор КМ и руководств для плавания
                        eptSeniorCorrectorMapBook      = ESTABLISHED_POST_TYPE_SENIOR_CORRECTOR_MAP_BOOK,    // Старший корректор КМ и руководств для плавания
                        eptCorrectorMapBook            = ESTABLISHED_POST_TYPE_CORRECTOR_MAP_BOOK,           // Корректор КМ и руководств для плавания
                        eptLeadingEngineer             = ESTABLISHED_POST_TYPE_LEADING_ENGINEER,             // Ведущий инженер
                        eptEngineer                    = ESTABLISHED_POST_TYPE_ENGINEER,                     // Инженер
                        eptEngineerTechnologist        = ESTABLISHED_POST_TYPE_ENGINEER_TECHNOLOGIST,        // Инженер-технолог
                        eptLeadingEngineerProgrammer   = ESTABLISHED_POST_TYPE_LEADING_ENGINEER_PROGRAMMER,  // Ведущий инженер-программист
                        eptEngineerProgrammer          = ESTABLISHED_POST_TYPE_ENGINEER_PROGRAMMER,          // Инженер-программист
                        eptTechnicianProgrammer        = ESTABLISHED_POST_TYPE_TECHNICIAN_PROGRAMMER,        // Техник-программист
                        eptGeodesist                   = ESTABLISHED_POST_TYPE_GEODESIST,                    // Геодезист
                        eptGeophysicist                = ESTABLISHED_POST_TYPE_GEOPHYSICIST,                 // Геофизик
                        eptSeniorReferent              = ESTABLISHED_POST_TYPE_SENIOR_REFERENT,              // Старший референт-переводчик КМ и руководств для плавания
                        eptReferent                    = ESTABLISHED_POST_TYPE_REFERENT,                     // Референт-переводчик КМ и руководств для плавания
                        eptCartographer                = ESTABLISHED_POST_TYPE_CARTOGRAPHER,                 // Картограф–составитель КМ и руководств для плавания
                        eptLeadingEngineerTechnologist = ESTABLISHED_POST_TYPE_LEADING_ENGINEER_TECHNOLOGIST // Ведущий инженер-технолог
                        //eptOobgdOperator               = 28, // Оператор ООБГД
                        //eptOperator                    = 29  // ??? Оператор
};

// Тип промежутка рабочего времени
enum TWorkPeriodType {
                        wptNone           = 0, // Не определен
                        wptOperating_end  = 1, // Выполнение работ закончено
                        wptOperating_run  = 2, // Выполнение работ производится
                        wptOperating_plan = 3, // Выполнение работ спланировано
                        wptAbsent         = 4  // Отсутствие на работе (отпуск, командировка, ...)
                        //wptFree           = 5 // Свободный промежуток - оставшееся время
};

// Виды продукции (первый уровень иерархии)
enum TProductionView {
                        prvNone    = 0,                      // Не определен
                        prvMaps    = PRODUCTION_VIEW_MAPS,   // Карты
                        prvManual  = PRODUCTION_VIEW_MANUAL, // Руководства и пособия
                        prvOni     = PRODUCTION_VIEW_ONI,    // ОНИ
                        prvTopo    = PRODUCTION_VIEW_TOPO,   // Материалы гидрографических, геофизических и океанографических работ и топографических съемок, материалы ДЗЗ, элементы содержания карты
                        prvDock    = PRODUCTION_VIEW_DOCK,   // Руководящие и информационные документы
                        prvRequest = PRODUCTION_VIEW_REQUEST // Заявки (из других подсистем)
};

// Второй уровень иерархии
enum TProductionRubr2 {
                        pr2None          = 0,                               // Не определен
                        pr2MapsSea       = PRODUCTION_RUBR2_MAPS_SEA,       // Морские карты
                        pr2MapsRiver     = PRODUCTION_RUBR2_MAPS_RIVER,     // Карты внутренних водных путей
                        pr2ManualSea     = PRODUCTION_RUBR2_MANUAL_SEA,     // Морские руководства и пособия
                        pr2ManualScience = PRODUCTION_RUBR2_MANUAL_SCIENCE, // Научно-технические пособия
                        pr2ManualBasis   = PRODUCTION_RUBR2_MANUAL_BASIS,   // Фундаментальные пособия по географии морей и океанов
                        pr2ManualOther   = PRODUCTION_RUBR2_MANUAL_OTHER,   // Прочие издания
                        pr2TopoWork      = PRODUCTION_RUBR2_TOPO_WORK,      // Материалы гидрографических, геофизических и океанографических работ
                        pr2TopoShoot     = PRODUCTION_RUBR2_TOPO_SHOOT,     // Материалы топографических съемок
                        pr2TopoDzz       = PRODUCTION_RUBR2_TOPO_DZZ        // Материалы ДЗЗ (аэрофотосъёмка, космическая съёмка, лазерная съёмка)
};

// Третий уровень иерархии
enum TProductionRubr3 {
                        pr3None            = 0,                                 // Не определен
                        pr3NavigationSea   = PRODUCTION_RUBR3_NAVIGATION_SEA,   // Навигационные морские карты
                        pr3HelpSea         = PRODUCTION_RUBR3_HELP_SEA,         // Морские вспомогательные и справочные карты
                        pr3NavigationRiver = PRODUCTION_RUBR3_NAVIGATION_RIVER, // Навигационные карты внутренних водных путей
                        pr3ManualSea       = PRODUCTION_RUBR3_MANUAL_SEA,       // Морские навигационные руководства и пособия
                        pr3ManualFish      = PRODUCTION_RUBR3_MANUAL_FISH       // Морские промысловые пособия
};

// Четвертый уровень иерархии
enum TProductionRubr4 {
                        pr4None        = 0,                             // Не определен
                        pr4MsManual    = PRODUCTION_RUBR4_MS_MANUAL,    // Руководства и пособия для плавания
                        pr4MsSea_hydro = PRODUCTION_RUBR4_MS_SEA_HYDRO, // Морские гидрометеорологические пособия
                        pr4MsSea_astro = PRODUCTION_RUBR4_MS_SEA_ASTRO, // Морские астрономические пособия
                        pr4MsForeign   = PRODUCTION_RUBR4_MS_FOREIGN,   // Международно-правовые руководства
                        pr4MsHelp      = PRODUCTION_RUBR4_MS_HELP       // Вспомогательные и справочные пособия для мореплавателей
};

// Типы продукции
enum TProductionType {
                        prtNone                = 0,                                  // Не определен
                        prtMapsTopo            = PRODUCTION_TYPE_MAPS_TOPO,          // Топографические карты
                        prtMapsEmk             = PRODUCTION_TYPE_MAPS_EMK,           // Специальные электронные морские карты (ЭМК) для ВМФ
                        prtManualGsNavy        = PRODUCTION_TYPE_MANUAL_GS_NAVY,     // Уставные документы ГС ВМФ
                        prtManualGuide         = PRODUCTION_TYPE_MANUAL_GUIDE,       // Руководящие документы, производственно-технические руководства и пособия
                        prtManualRiver         = PRODUCTION_TYPE_MANUAL_RIVER,       // Руководства и пособия для внутренних водных путей
                        prtOniForeignIm        = PRODUCTION_TYPE_ONI_FOREIGN_IM,     // Иностранные ИМ
                        prtOniNavarea          = PRODUCTION_TYPE_ONI_NAVAREA,        // НАВАРЕА, НАВИП
                        prtOniPrip             = PRODUCTION_TYPE_ONI_PRIP,           // ПРИП
                        prtOniShip             = PRODUCTION_TYPE_ONI_SHIP,           // ОНИ, получаемые непосредственно от мореплавателей (донесения командиров, капитанов и штурманов судов)
                        prtOniAgensy           = PRODUCTION_TYPE_ONI_AGENSY,         // Сообщения морских агентств, обслуживающих суда
                        prtOniGs_fleet         = PRODUCTION_TYPE_ONI_GS_FLEET,       // Сообщения ГС флотов
                        prtOniNavarea13        = PRODUCTION_TYPE_ONI_NAVAREA_13,     // Бюллетень навигационных предупреждений НАВАРЕА-13
                        prtOniNotice           = PRODUCTION_TYPE_ONI_NOTICE,         // Извещения мореплавателям (выпуск, доп. корректура и приложение)
                        prtOniWarning          = PRODUCTION_TYPE_ONI_WARNING,        // Бюллетень навигационных предупреждений
                        prtOniNoticeEn         = PRODUCTION_TYPE_ONI_NOTICE_EN,      // Извещения мореплавателям на английском языке
                        prtOniNumeric          = PRODUCTION_TYPE_ONI_NUMERIC,        // Нумерники извещений мореплавателям с приложениями
                        prtOniListTemp         = PRODUCTION_TYPE_ONI_LIST_TEMP,      // Перечни действующих временных и предварительных извещений мореплавателям
                        prtOniTab              = PRODUCTION_TYPE_ONI_TAB,            // Вкладка
                        prtOniSpecialIm        = PRODUCTION_TYPE_ONI_SPECIAL_IM,     // Спец. выпуск ИМ
                        prtTopoFragment        = PRODUCTION_TYPE_TOPO_FRAGMENT,      // Элементы содержания карты в цифровом и аналоговом виде
                        prtDockNavy            = PRODUCTION_TYPE_DOCK_NAVY,          // Документы Генерального штаба, штабов флотов и др., определяющие режимы плавания
                        prtDockCountry         = PRODUCTION_TYPE_DOCK_COUNTRY,       // Законы, постановления, инструкции, положения, правила и другие официальные документы, регламентирующие взаимоотношения государств на океанах, морях и ВВП
                        prtDockInfo            = PRODUCTION_TYPE_DOCK_INFO,          // Информационный бюллетень
                        prtRequestQuery        = PRODUCTION_TYPE_REQUEST_QUERY,      // Рекламации, заявки потребителей
                        prtRequestLetter       = PRODUCTION_TYPE_REQUEST_LETTER,     // Письма, факсы, прочие документы
                        prtScienceWork         = PRODUCTION_TYPE_SCIENCE_WORK,       // Научные труды и монографии по навигации, гидрографии и океанографии
                        prtScienceLearning     = PRODUCTION_TYPE_SCIENCE_LEARNING,   // Учебные пособия для гидрографов и штурманов
                        prtScienceMilitary     = PRODUCTION_TYPE_SCIENCE_MILITARY,   // Военно-научные издания
                        prtSciencePeriodica    = PRODUCTION_TYPE_SCIENCE_PERIODICA,  // Периодические научно-технические издания
                        prtOceanAtlas          = PRODUCTION_TYPE_OCEAN_ATLAS,        // Атлас океанов
                        prtOtherLearning       = PRODUCTION_TYPE_OTHER_LEARNING,     // Прочие пособия по географии морей и океанов
                        prtBlank               = PRODUCTION_TYPE_BLANK,              // Бланковые издания
                        prtHistory             = PRODUCTION_TYPE_HISTORY,            // Документально-исторические издания
                        prtRepresent           = PRODUCTION_TYPE_REPRESENT,          // Представительские издания и документы
                        prtTwTabletGs          = PRODUCTION_TYPE_TW_TABLET_GS,       // Планшеты ГС
                        prtTwCndGs             = PRODUCTION_TYPE_TW_CND_GS,          // ЦНД ГС
                        prtTwAtlas             = PRODUCTION_TYPE_TW_ATLAS,           // Атласы изученности
                        prtTwPages             = PRODUCTION_TYPE_TW_PAGES,           // Сборные листы планшетов
                        prtTwHydroReport       = PRODUCTION_TYPE_TW_HYDRO_REPORT,    // Научно-технические и технические отчеты о гидрографической съемке
                        prtTwHydroGs           = PRODUCTION_TYPE_TW_HYDRO_GS,        // Материалы, собранные ГС во время производства систематических и попутных гидрографических работ и при корректуре руководств для плавания и карт по местности
                        prtTwTabletGebco       = PRODUCTION_TYPE_TW_TABLET_GEBCO,    // Планшеты GEBCO
                        prtTwActHydro          = PRODUCTION_TYPE_TW_ACT_HYDRO,       // Акты научно-технической экспертизы и приёмки работ
                        prtTwTabletTopo        = PRODUCTION_TYPE_TW_TABLET_TOPO,     // Планшеты топографической съемки
                        prtTwTopoReport        = PRODUCTION_TYPE_TW_TOPO_REPORT,     // Технические отчеты о геодезических, астрономических, гравиметрических и топографических работах
                        prtTwCoord             = PRODUCTION_TYPE_TW_COORD,           // Каталоги координат и высот пунктов
                        prtTwNomenclature      = PRODUCTION_TYPE_TW_NOMENCLATURE,    // Сборные таблицы номенклатуры топографических карт и картотека планов городов
                        prtTwActTopo           = PRODUCTION_TYPE_TW_ACT_TOPO,        // Акты научно-технической экспертизы и приемки работ
                        prtDzzAero             = PRODUCTION_TYPE_DZZ_AERO,           // Данные аэрофотосъёмки
                        prtDzzSpace            = PRODUCTION_TYPE_DZZ_SPACE,          // Данные космической съёмки
                        prtDzzLazer            = PRODUCTION_TYPE_DZZ_LAZER,          // Данные лазерной съёмки
                        prtNsNmk               = PRODUCTION_TYPE_NS_NMK,             // НМК навигационные морские карты
                        prtNsEnk               = PRODUCTION_TYPE_NS_ENK,             // ЭНК электронные навигационные карты
                        prtNsRadio             = PRODUCTION_TYPE_NS_RADIO,           // Радионавигационные карты и радионавигационные планшеты
                        prtNsFish              = PRODUCTION_TYPE_NS_FISH,            // Навигационно-промысловые карты
                        prtNsCorrectEnk        = PRODUCTION_TYPE_NS_CORRECT_ENK,     // Корректурные наборы к ЭНК
                        prtHsInfo              = PRODUCTION_TYPE_HS_INFO,            // Справочно-информационные карты
                        prtHsBlank             = PRODUCTION_TYPE_HS_BLANK,           // Бланковые морские карты
                        prtHsGrid              = PRODUCTION_TYPE_HS_GRID,            // Карты-сетки
                        prtHsOrto              = PRODUCTION_TYPE_HS_ORTO,            // Карты для прокладки ортодромий
                        prtHsRescue            = PRODUCTION_TYPE_HS_RESCUE,          // Карты для спасательных средств
                        prtHsBottom            = PRODUCTION_TYPE_HS_BOTTOM,          // Карты с наглядным изображением рельефа морского дна
                        prtHsBathy             = PRODUCTION_TYPE_HS_BATHY,           // Батиметрические карты
                        prtHsWires             = PRODUCTION_TYPE_HS_WIRES,           // Карты подводных кабелей
                        prtHsTime              = PRODUCTION_TYPE_HS_TIME,            // Карты часовых поясов
                        prtHsWarTraining       = PRODUCTION_TYPE_HS_WAR_TRAINING,    // Карты районов боевой подготовки
                        prtHsMemory            = PRODUCTION_TYPE_HS_MEMORY,          // Карты памятных мест побед и героической гибели кораблей русского и советского флотов
                        prtHsYacht             = PRODUCTION_TYPE_HS_YACHT,           // Карты для яхт и прогулочных катеров
                        prtHsWorld             = PRODUCTION_TYPE_HS_WORLD,           // Карта мира
                        prtNrLake              = PRODUCTION_TYPE_NR_LAKE,            // Навигационные карты судоходных озер
                        prtNrRiver             = PRODUCTION_TYPE_NR_RIVER,           // Навигационные речные карты
                        prtNrEnk               = PRODUCTION_TYPE_NR_ENK,             // ЭНК внутренних путей
                        prtMfishDescript       = PRODUCTION_TYPE_MFISH_DESCRIPT,     // Промысловые описания
                        prtMfishMountain       = PRODUCTION_TYPE_MFISH_MOUNTAIN,     // Описания подводных гор и поднятий морского дна в промысловых районах Мирового океана
                        prtMsmLocies           = PRODUCTION_TYPE_MSM_LOCIES,         // Лоции и приложения к ним
                        prtMsmFire             = PRODUCTION_TYPE_MSM_FIRE,           // Огни и знаки (Огни)
                        prtMsmRadio            = PRODUCTION_TYPE_MSM_RADIO,          // Радиотехнические средства навигационного оборудования
                        prtMsmRegime           = PRODUCTION_TYPE_MSM_REGIME,         // Режим плавания судов + описание мерных линий
                        prtMsmShedRadio        = PRODUCTION_TYPE_MSM_SHED_RADIO,     // Расписание передач навигационных предупреждений и гидрометеорологических сообщений радиостанциями
                        prtMsmShedFax          = PRODUCTION_TYPE_MSM_SHED_FAX,       // Расписание факсимильных гидрометеорологических передач
                        prtMsmRules            = PRODUCTION_TYPE_MSM_RULES,          // Правила плавания
                        prtMsmManualRus        = PRODUCTION_TYPE_MSM_MANUAL_RUS,     // Руководства для захода судов в российские порты
                        prtMsmManualNord       = PRODUCTION_TYPE_MSM_MANUAL_NORD,    // Руководства для плавания в акватории Северного морского пути
                        prtMsmOceanWay         = PRODUCTION_TYPE_MSM_OCEAN_WAY,      // Океанские пути мира
                        prtMsmTablePhc         = PRODUCTION_TYPE_MSM_TABLE_PHC,      // Таблицы радионавигационных систем (РНС)
                        prtMsmAtlasPhc         = PRODUCTION_TYPE_MSM_ATLAS_PHC,      // Атласы зон точности определения места с помощью РНС
                        prtMsshAtlasParam      = PRODUCTION_TYPE_MSSH_ATLAS_PARAM,   // Атласы океанографических параметров
                        prtMsshGydrometeo      = PRODUCTION_TYPE_MSSH_GYDROMETEO,    // Комплекты гидрометеорологических карт морей и районов океанов
                        prtMsshAtlasIce        = PRODUCTION_TYPE_MSSH_ATLAS_ICE,     // Атласы льдов
                        prtMsshAtlasFlow       = PRODUCTION_TYPE_MSSH_ATLAS_FLOW,    // Атласы приливных, поверхностных, приливо-отливных и иных течений
                        prtMsshTableFlow       = PRODUCTION_TYPE_MSSH_TABLE_FLOW,    // Таблицы приливов и приливных течений
                        prtMsshAtlasDanger     = PRODUCTION_TYPE_MSSH_ATLAS_DANGER,  // Атласы опасных и особо опасных гидрометеорологических явлений
                        prtMsshAtlasBottom     = PRODUCTION_TYPE_MSSH_ATLAS_BOTTOM,  // Атласы донных осадков
                        prtMsshOther           = PRODUCTION_TYPE_MSSH_OTHER,         // Прочие гидрометеорологические пособия
                        prtMssaSeaYear         = PRODUCTION_TYPE_MSSA_SEA_YEAR,      // Морской астрономический ежегодник
                        prtMssaAlma            = PRODUCTION_TYPE_MSSA_ALMA,          // Астронавигационный альманах
                        prtMssaTba57           = PRODUCTION_TYPE_MSSA_TBA57,         // Таблицы вычислений высот и азимутов (ТВА-57)
                        prtMssaBac58           = PRODUCTION_TYPE_MSSA_BAC58,         // Таблицы "Высоты и азимуты светил" (ВАС-58)
                        prtMssaNord_sun        = PRODUCTION_TYPE_MSSA_NORD_SUN,      // Таблицы восхода и захода Солнца для северных широт 74°-90°
                        prtMssaNc2026          = PRODUCTION_TYPE_MSSA_NC2026,        // Астрономические данные N и С на 2006-2026 гг.
                        prtMsfJustice          = PRODUCTION_TYPE_MSF_JUSTICE,        // Руководство "Пределы действия территориальной юрисдикции государств в Мировом океане"
                        prtMsfCollision        = PRODUCTION_TYPE_MSF_COLLISION,      // Международные правила предупреждения столкновения судов в море
                        prtMsfForeign          = PRODUCTION_TYPE_MSF_FOREIGN,        // Сборник международных договоров и законодательных актов по вопросам мореплавания
                        prtMsfRf               = PRODUCTION_TYPE_MSF_RF,             // Морское законодательство Российской Федерации
                        prtMsfOther            = PRODUCTION_TYPE_MSF_OTHER,          // Прочие международно-правовые руководства
                        prtMshCatalog          = PRODUCTION_TYPE_MSH_CATALOG,        // Каталоги карт и книг
                        prtMshSeaTable         = PRODUCTION_TYPE_MSH_SEA_TABLE,      // Мореходные таблицы
                        prtMshSeaDistance      = PRODUCTION_TYPE_MSH_SEA_DISTANCE,   // Таблицы морских расстояний
                        prtMshFire             = PRODUCTION_TYPE_MSH_FIRE,           // Описание особенностей огней военных кораблей и сигналов, подаваемых кораблями и судами для обеспечения безопасности плавания
                        prtMshMamcA            = PRODUCTION_TYPE_MSH_MAMC_A,         // Описание системы плавучего ограждения в водах России. Система МАМС, регион А
                        prtMshWays             = PRODUCTION_TYPE_MSH_WAYS,           // Общие положения об установлении путей движения судов
                        prtMshSignal           = PRODUCTION_TYPE_MSH_SIGNAL,         // Международный свод сигналов
                        prtMshLegend           = PRODUCTION_TYPE_MSH_LEGEND,         // Условные знаки
                        prtMshPorts            = PRODUCTION_TYPE_MSH_PORTS,          // Порты мира
                        prtMshOyher            = PRODUCTION_TYPE_MSH_OYHER,          // Прочие вспомогательные и справочные пособия для мореплавателей
                        prtMshGsFleet          = PRODUCTION_TYPE_MSH_GS_FLEET        // Издания гидрографических служб флотов
};

typedef QList<TProductionType> TProductionTypeList;

// Тип вложения
enum TAttachmentType {
                        attNone      =  0, // Не определен
                        attDocument  = 10, // Документ
                        attBdfData   = 20, // Данные БДФ
                        attOobgdData = 30  // Данные ООБГД
};

// Сосотояние выполнения элемента плана
enum TCarryOutCondition {
                        cocNone      = 0,                           // Не определено
                        cocSend      = CONDITION_CARRYOUT_SEND,     // Задача поставлена
                        cocGet       = CONDITION_CARRYOUT_GET,      // Задача получена
                        cocCarryBeg  = CONDITION_CARRYOUT_CARRYBEG, // Приступил к выполнению
                        cocCarryEnd  = CONDITION_CARRYOUT_CARRYEND, // Выполнил
                        cocProblem   = CONDITION_CARRYOUT_PROBLEM   // Проблемы (причины - TCarryOutProblem)
};

// Причина невозможности выполнения элемента плана
enum TCarryOutProblem {
                        copNone      = 0,                         // Не определена
                        copBadTask   = CARRYOUT_PROBLEM_BADTASK,  // Неподходящая задача
                        copEmplMiss  = CARRYOUT_PROBLEM_EMPLMISS, // Отсутствуют исполнители
                        copBadData   = CARRYOUT_PROBLEM_BADDATA,  // Отсутствуют (неполные) исходные данные
                        copUnknError = CARRYOUT_PROBLEM_UNKNERROR // Неизвестная ошибка
};

// Типы внещних модулей
enum TExternalModuleType {
                        emtNone            = 0,                                    // Не определен
                        emtInternetBrowser = EXTERNAL_MODULE_TYPE_INTERNETBROWSER, // интернет-браузер
                        emtScannerProgramm = EXTERNAL_MODULE_TYPE_SCANNERPROGRAMM, // приложение для сканирования (в составе штатного ПО сканера)
                        emtPdfProgramm     = EXTERNAL_MODULE_TYPE_PDFPROGRAMM,     // Adobe Acrobat или аналог для работы с файлами формата PDF
                        emtOcrProgramm     = EXTERNAL_MODULE_TYPE_OCRPROGRAMM,     // ABBYY FineReader или аналог для распознавания текстов
                        emtCaris           = EXTERNAL_MODULE_TYPE_CARIS,           // Caris HIPS&SIPS
                        emtOkean           = EXTERNAL_MODULE_TYPE_OKEAN,           // АС «Океан»
                        emtCtrlOOBGD       = EXTERNAL_MODULE_TYPE_CTRLOOBGD,       // «Управление ООБГД»
                        emtDigitPGS        = EXTERNAL_MODULE_TYPE_DIGITPGS,        // «ПК оцифровки ПГС»
                        emtWord            = EXTERNAL_MODULE_TYPE_WORD,            // Microsoft Word
                        emtInDesign        = EXTERNAL_MODULE_TYPE_INDESIGN,        // Adobe InDesign
                        emtNeva            = EXTERNAL_MODULE_TYPE_NEVA             // ГИС «Нева»
};

typedef QList<TExternalModuleType> TExternalModuleTypeList;

// Тип документа
enum TDocumentType {
                        docNone           =  0, // Не определен
                        // специальные документы
                        docGenPlan        =  1, // План производства (годовой)
                        docCarryPlan      =  2, // План работ по проекту
                        docFormular       =  3, // Формуляр
                        docTechTask       =  4, // Техническое задание (составная часть формуляра ???)
                        docQueryEmployees =  5, // Запрос на привлечение внешних специалистов
                        docQueryOobgd     =  6, // Запрос на обновление ООБГД
                        docQuerySources   =  7, // Запрос на приобретение источников
                        // документы общего назначения
                        docQuery          = 50, // Запрос
                        docConclusion     = 51, // Заключение
                        docComment        = 52, // Комментарий
                        docNote           = 53  // Заметка
};

// Статус
enum TStatus {
                        stsNone                =  0, // Не определен
                        stsOnExamination       =  1, // «На экспертизу»
                        stsOnEvaluation        =  2, // «На оценку информативности»
                        stsOnCheck             =  3, // «На проверку»
                        stsOnCompletion        =  4, // «На доработку»
                        stsCellsFormed         =  5, // «Ячейки сформированы»
                        stsChecked             =  6, // «Проверено»
                        stsCheckedSeniorEditor =  7, // «Проверено старшим редактором»
                        stsCheckedMainEdition  =  8, // «Проверено главной редакцией (ОТК)»
                        stsChangeEdition       =  9, // «Изменить редакцию ООБГД»
                        stsAssert              = 10  // «Утверждено»
};

typedef QList<TStatus> TStatusList;

// Тип элемента плана
enum TPlanElementType {
                        petNone      =  0, // Не определен
                        petProject   =  1, // Проект
                        petPlan      =  2, // План
                        petProcedure =  3, // Процедура
                        petWork      =  4  // Работа
};

// Тип элемента рисования
enum TDrawElementType {
                        detNone         =  0, // Не определен
                        detLomLine      =  1, // Ломаная линия
                        detPolyline     =  2, // Замкнутая ломаная линия
                        detPolygon      =  3, // Полигон с заполнением
                        detPolygon3     =  4, // Треугольник
                        detEllipce      =  5, // Эллипс
                        detTextPoint    =  6, // Текст в точке
                        detTextRect     =  7, // Текст в прямоугольнике
                        detRect         =  8, // Прямоугольник
                        detRoundRect    =  9, // Закругленный прямоугольник
                        detCheckBox     = 10, // Текст с галочкой
                        detCheckCircle  = 11, // Кружочек с галочкой
                        detCrossPolygon = 12, // Перечеркнутый прямоугольник
                        detCrossCircle  = 13, // Перечеркнутый кружочек
                        detCrossLomLine = 14, // Перечеркнутая ломаная линия
                        detHint         = 15, // Подсказка
                        detPixmap       = 16, // Картинка
                        detScaledPixmap = 17  // Масштабируемая картинка
};

#endif // ENUMS_H
