#ifndef DEFDBCONST_H
#define DEFDBCONST_H

#define YEAR_PLAN_TYPE_TEST              -1000 // Тестовые планы
#define YEAR_PLAN_TYPE_CARRYTEMPLATE     -1001 // Шаблоны производственных планов
//#define YEAR_PLAN_TYPE_                -???? // ????

// Виды продукции (первый уровень иерархии)
#define PRODUCTION_VIEW_MAPS                 1 // Карты
#define PRODUCTION_VIEW_MANUAL               2 // Руководства и пособия
#define PRODUCTION_VIEW_ONI                  3 // ОНИ
#define PRODUCTION_VIEW_TOPO                 4 // Материалы гидрографических, геофизических и океанографических работ и топографических съемок, материалы ДЗЗ, элементы содержания карты
#define PRODUCTION_VIEW_DOCK                 5 // Руководящие и информационные документы
#define PRODUCTION_VIEW_REQUEST              6 // Заявки (из других подсистем)

// Второй уровень иерархии
#define PRODUCTION_RUBR2_MAPS_SEA            1 // Морские карты
#define PRODUCTION_RUBR2_MAPS_RIVER          2 // Карты внутренних водных путей
#define PRODUCTION_RUBR2_MANUAL_SEA          3 // Морские руководства и пособия
#define PRODUCTION_RUBR2_MANUAL_SCIENCE      4 // Научно-технические пособия
#define PRODUCTION_RUBR2_MANUAL_BASIS        5 // Фундаментальные пособия по географии морей и океанов
#define PRODUCTION_RUBR2_MANUAL_OTHER        6 // Прочие издания
#define PRODUCTION_RUBR2_TOPO_WORK           7 // Материалы гидрографических, геофизических и океанографических работ
#define PRODUCTION_RUBR2_TOPO_SHOOT          8 // Материалы топографических съемок
#define PRODUCTION_RUBR2_TOPO_DZZ            9 // Материалы ДЗЗ (аэрофотосъёмка, космическая съёмка, лазерная съёмка)

// Третий уровень иерархии
#define PRODUCTION_RUBR3_NAVIGATION_SEA      1 // Навигационные морские карты
#define PRODUCTION_RUBR3_HELP_SEA            2 // Морские вспомогательные и справочные карты
#define PRODUCTION_RUBR3_NAVIGATION_RIVER    3 // Навигационные карты внутренних водных путей
#define PRODUCTION_RUBR3_MANUAL_SEA          4 // Морские навигационные руководства и пособия
#define PRODUCTION_RUBR3_MANUAL_FISH         5 // Морские промысловые пособия

// Четвертый уровень иерархии
#define PRODUCTION_RUBR4_MS_MANUAL           1 // Руководства и пособия для плавания
#define PRODUCTION_RUBR4_MS_SEA_HYDRO        2 // Морские гидрометеорологические пособия
#define PRODUCTION_RUBR4_MS_SEA_ASTRO        3 // Морские астрономические пособия
#define PRODUCTION_RUBR4_MS_FOREIGN          4 // Международно-правовые руководства
#define PRODUCTION_RUBR4_MS_HELP             5 // Вспомогательные и справочные пособия для мореплавателей

// Типы продукции
#define PRODUCTION_TYPE_MAPS_TOPO            1 // Топографические карты
#define PRODUCTION_TYPE_MAPS_EMK             2 // Специальные электронные морские карты (ЭМК) для ВМФ
#define PRODUCTION_TYPE_MANUAL_GS_NAVY       3 // Уставные документы ГС ВМФ
#define PRODUCTION_TYPE_MANUAL_GUIDE         4 // Руководящие документы, производственно-технические руководства и пособия
#define PRODUCTION_TYPE_MANUAL_RIVER         5 // Руководства и пособия для внутренних водных путей
#define PRODUCTION_TYPE_ONI_FOREIGN_IM       6 // Иностранные ИМ
#define PRODUCTION_TYPE_ONI_NAVAREA          7 // НАВАРЕА, НАВИП
#define PRODUCTION_TYPE_ONI_PRIP             8 // ПРИП
#define PRODUCTION_TYPE_ONI_SHIP             9 // ОНИ, получаемые непосредственно от мореплавателей (донесения командиров, капитанов и штурманов судов)
#define PRODUCTION_TYPE_ONI_AGENSY          10 // Сообщения морских агентств, обслуживающих суда
#define PRODUCTION_TYPE_ONI_GS_FLEET        11 // Сообщения ГС флотов
#define PRODUCTION_TYPE_ONI_NAVAREA_13      12 // Бюллетень навигационных предупреждений НАВАРЕА-13
#define PRODUCTION_TYPE_ONI_NOTICE          13 // Извещения мореплавателям (выпуск, доп. корректура и приложение)
#define PRODUCTION_TYPE_ONI_WARNING         14 // Бюллетень навигационных предупреждений
#define PRODUCTION_TYPE_ONI_NOTICE_EN       15 // Извещения мореплавателям на английском языке
#define PRODUCTION_TYPE_ONI_NUMERIC         16 // Нумерники извещений мореплавателям с приложениями
#define PRODUCTION_TYPE_ONI_LIST_TEMP       17 // Перечни действующих временных и предварительных извещений мореплавателям
#define PRODUCTION_TYPE_ONI_TAB             18 // Вкладка
#define PRODUCTION_TYPE_ONI_SPECIAL_IM      19 // Спец. выпуск ИМ
#define PRODUCTION_TYPE_TOPO_FRAGMENT       20 // Элементы содержания карты в цифровом и аналоговом виде
#define PRODUCTION_TYPE_DOCK_NAVY           21 // Документы Генерального штаба, штабов флотов и др., определяющие режимы плавания
#define PRODUCTION_TYPE_DOCK_COUNTRY        22 // Законы, постановления, инструкции, положения, правила и другие официальные документы, регламентирующие взаимоотношения государств на океанах, морях и ВВП
#define PRODUCTION_TYPE_DOCK_INFO           23 // Информационный бюллетень
#define PRODUCTION_TYPE_REQUEST_QUERY       24 // Рекламации, заявки потребителей
#define PRODUCTION_TYPE_REQUEST_LETTER      25 // Письма, факсы, прочие документы
#define PRODUCTION_TYPE_SCIENCE_WORK        26 // Научные труды и монографии по навигации, гидрографии и океанографии
#define PRODUCTION_TYPE_SCIENCE_LEARNING    27 // Учебные пособия для гидрографов и штурманов
#define PRODUCTION_TYPE_SCIENCE_MILITARY    28 // Военно-научные издания
#define PRODUCTION_TYPE_SCIENCE_PERIODICA   29 // Периодические научно-технические издания
#define PRODUCTION_TYPE_OCEAN_ATLAS         30 // Атлас океанов
#define PRODUCTION_TYPE_OTHER_LEARNING      31 // Прочие пособия по географии морей и океанов
#define PRODUCTION_TYPE_BLANK               32 // Бланковые издания
#define PRODUCTION_TYPE_HISTORY             33 // Документально-исторические издания
#define PRODUCTION_TYPE_REPRESENT           34 // Представительские издания и документы
#define PRODUCTION_TYPE_TW_TABLET_GS        35 // Планшеты ГС
#define PRODUCTION_TYPE_TW_CND_GS           36 // ЦНД ГС
#define PRODUCTION_TYPE_TW_ATLAS            37 // Атласы изученности
#define PRODUCTION_TYPE_TW_PAGES            38 // Сборные листы планшетов
#define PRODUCTION_TYPE_TW_HYDRO_REPORT     39 // Научно-технические и технические отчеты о гидрографической съемке
#define PRODUCTION_TYPE_TW_HYDRO_GS         40 // Материалы, собранные ГС во время производства систематических и попутных гидрографических работ и при корректуре руководств для плавания и карт по местности
#define PRODUCTION_TYPE_TW_TABLET_GEBCO     41 // Планшеты GEBCO
#define PRODUCTION_TYPE_TW_ACT_HYDRO        42 // Акты научно-технической экспертизы и приёмки работ
#define PRODUCTION_TYPE_TW_TABLET_TOPO      43 // Планшеты топографической съемки
#define PRODUCTION_TYPE_TW_TOPO_REPORT      44 // Технические отчеты о геодезических, астрономических, гравиметрических и топографических работах
#define PRODUCTION_TYPE_TW_COORD            45 // Каталоги координат и высот пунктов
#define PRODUCTION_TYPE_TW_NOMENCLATURE     46 // Сборные таблицы номенклатуры топографических карт и картотека планов городов
#define PRODUCTION_TYPE_TW_ACT_TOPO         47 // Акты научно-технической экспертизы и приемки работ
#define PRODUCTION_TYPE_DZZ_AERO            48 // Данные аэрофотосъёмки
#define PRODUCTION_TYPE_DZZ_SPACE           49 // Данные космической съёмки
#define PRODUCTION_TYPE_DZZ_LAZER           50 // Данные лазерной съёмки
#define PRODUCTION_TYPE_NS_NMK              51 // НМК навигационные морские карты
#define PRODUCTION_TYPE_NS_ENK              52 // ЭНК электронные навигационные карты
#define PRODUCTION_TYPE_NS_RADIO            53 // Радионавигационные карты и радионавигационные планшеты
#define PRODUCTION_TYPE_NS_FISH             54 // Навигационно-промысловые карты
#define PRODUCTION_TYPE_NS_CORRECT_ENK      55 // Корректурные наборы к ЭНК
#define PRODUCTION_TYPE_HS_INFO             56 // Справочно-информационные карты
#define PRODUCTION_TYPE_HS_BLANK            57 // Бланковые морские карты
#define PRODUCTION_TYPE_HS_GRID             58 // Карты-сетки
#define PRODUCTION_TYPE_HS_ORTO             59 // Карты для прокладки ортодромий
#define PRODUCTION_TYPE_HS_RESCUE           60 // Карты для спасательных средств
#define PRODUCTION_TYPE_HS_BOTTOM           61 // Карты с наглядным изображением рельефа морского дна
#define PRODUCTION_TYPE_HS_BATHY            62 // Батиметрические карты
#define PRODUCTION_TYPE_HS_WIRES            63 // Карты подводных кабелей
#define PRODUCTION_TYPE_HS_TIME             64 // Карты часовых поясов
#define PRODUCTION_TYPE_HS_WAR_TRAINING     65 // Карты районов боевой подготовки
#define PRODUCTION_TYPE_HS_MEMORY           66 // Карты памятных мест побед и героической гибели кораблей русского и советского флотов
#define PRODUCTION_TYPE_HS_YACHT            67 // Карты для яхт и прогулочных катеров
#define PRODUCTION_TYPE_HS_WORLD            68 // Карта мира
#define PRODUCTION_TYPE_NR_LAKE             69 // Навигационные карты судоходных озер
#define PRODUCTION_TYPE_NR_RIVER            70 // Навигационные речные карты
#define PRODUCTION_TYPE_NR_ENK              71 // ЭНК внутренних путей
#define PRODUCTION_TYPE_MFISH_DESCRIPT      72 // Промысловые описания
#define PRODUCTION_TYPE_MFISH_MOUNTAIN      73 // Описания подводных гор и поднятий морского дна в промысловых районах Мирового океана
#define PRODUCTION_TYPE_MSM_LOCIES          74 // Лоции и приложения к ним
#define PRODUCTION_TYPE_MSM_FIRE            75 // Огни и знаки (Огни)
#define PRODUCTION_TYPE_MSM_RADIO           76 // Радиотехнические средства навигационного оборудования
#define PRODUCTION_TYPE_MSM_REGIME          77 // Режим плавания судов + описание мерных линий
#define PRODUCTION_TYPE_MSM_SHED_RADIO      78 // Расписание передач навигационных предупреждений и гидрометеорологических сообщений радиостанциями
#define PRODUCTION_TYPE_MSM_SHED_FAX        79 // Расписание факсимильных гидрометеорологических передач
#define PRODUCTION_TYPE_MSM_RULES           80 // Правила плавания
#define PRODUCTION_TYPE_MSM_MANUAL_RUS      81 // Руководства для захода судов в российские порты
#define PRODUCTION_TYPE_MSM_MANUAL_NORD     82 // Руководства для плавания в акватории Северного морского пути
#define PRODUCTION_TYPE_MSM_OCEAN_WAY       83 // Океанские пути мира
#define PRODUCTION_TYPE_MSM_TABLE_PHC       84 // Таблицы радионавигационных систем (РНС)
#define PRODUCTION_TYPE_MSM_ATLAS_PHC       85 // Атласы зон точности определения места с помощью РНС
#define PRODUCTION_TYPE_MSSH_ATLAS_PARAM    86 // Атласы океанографических параметров
#define PRODUCTION_TYPE_MSSH_GYDROMETEO     87 // Комплекты гидрометеорологических карт морей и районов океанов
#define PRODUCTION_TYPE_MSSH_ATLAS_ICE      88 // Атласы льдов
#define PRODUCTION_TYPE_MSSH_ATLAS_FLOW     89 // Атласы приливных, поверхностных, приливо-отливных и иных течений
#define PRODUCTION_TYPE_MSSH_TABLE_FLOW     90 // Таблицы приливов и приливных течений
#define PRODUCTION_TYPE_MSSH_ATLAS_DANGER   91 // Атласы опасных и особо опасных гидрометеорологических явлений
#define PRODUCTION_TYPE_MSSH_ATLAS_BOTTOM   92 // Атласы донных осадков
#define PRODUCTION_TYPE_MSSH_OTHER          93 // Прочие гидрометеорологические пособия
#define PRODUCTION_TYPE_MSSA_SEA_YEAR       94 // Морской астрономический ежегодник
#define PRODUCTION_TYPE_MSSA_ALMA           95 // Астронавигационный альманах
#define PRODUCTION_TYPE_MSSA_TBA57          96 // Таблицы вычислений высот и азимутов (ТВА-57)
#define PRODUCTION_TYPE_MSSA_BAC58          97 // Таблицы "Высоты и азимуты светил" (ВАС-58)
#define PRODUCTION_TYPE_MSSA_NORD_SUN       98 // Таблицы восхода и захода Солнца для северных широт 74°-90°
#define PRODUCTION_TYPE_MSSA_NC2026         99 // Астрономические данные N и С на 2006-2026 гг.
#define PRODUCTION_TYPE_MSF_JUSTICE        100 // Руководство "Пределы действия территориальной юрисдикции государств в Мировом океане"
#define PRODUCTION_TYPE_MSF_COLLISION      101 // Международные правила предупреждения столкновения судов в море
#define PRODUCTION_TYPE_MSF_FOREIGN        102 // Сборник международных договоров и законодательных актов по вопросам мореплавания
#define PRODUCTION_TYPE_MSF_RF             103 // Морское законодательство Российской Федерации
#define PRODUCTION_TYPE_MSF_OTHER          104 // Прочие международно-правовые руководства
#define PRODUCTION_TYPE_MSH_CATALOG        105 // Каталоги карт и книг
#define PRODUCTION_TYPE_MSH_SEA_TABLE      106 // Мореходные таблицы
#define PRODUCTION_TYPE_MSH_SEA_DISTANCE   107 // Таблицы морских расстояний
#define PRODUCTION_TYPE_MSH_FIRE           108 // Описание особенностей огней военных кораблей и сигналов, подаваемых кораблями и судами для обеспечения безопасности плавания
#define PRODUCTION_TYPE_MSH_MAMC_A         109 // Описание системы плавучего ограждения в водах России. Система МАМС, регион А
#define PRODUCTION_TYPE_MSH_WAYS           110 // Общие положения об установлении путей движения судов
#define PRODUCTION_TYPE_MSH_SIGNAL         111 // Международный свод сигналов
#define PRODUCTION_TYPE_MSH_LEGEND         112 // Условные знаки
#define PRODUCTION_TYPE_MSH_PORTS          113 // Порты мира
#define PRODUCTION_TYPE_MSH_OYHER          114 // Прочие вспомогательные и справочные пособия для мореплавателей
#define PRODUCTION_TYPE_MSH_GS_FLEET       115 // Издания гидрографических служб флотов

// Типы должностей
#define EMPLOYEE_TYPE_CHIEF                  1 // Начальник
#define EMPLOYEE_TYPE_SUBSTITUENT            2 // Заместитель
#define EMPLOYEE_TYPE_MAJOR_ENGINEER         3 // Главный инженер
#define EMPLOYEE_TYPE_LEADING_EDITOR         4 // Ведущий редактор
#define EMPLOYEE_TYPE_MAJOR_EDITOR           5 // Главный редактор
#define EMPLOYEE_TYPE_SENIOR_EDITOR          6 // Старший редактор
#define EMPLOYEE_TYPE_TECH_EDITOR            7 // Технический редактор
#define EMPLOYEE_TYPE_RESEARCH_EDITOR        8 // Редактор изученности
#define EMPLOYEE_TYPE_EXPERT_EDITOR          9 // Редактор технической экспертизы
#define EMPLOYEE_TYPE_EDITOR                10 // Редактор
#define EMPLOYEE_TYPE_OOBGD_OPERATOR        11 // Оператор ООБГД
#define EMPLOYEE_TYPE_OPERATOR              12 // Оператор

// Тип штатной должности
#define ESTABLISHED_POST_TYPE_CHIEF_COMPANY                  1 // Начальник Центрального картографического производства ВМФ
#define ESTABLISHED_POST_TYPE_SUBSTITUENT_COMPANY_MAP        2 // Заместитель начальника производства (по подготовке к изданию морских карт)
#define ESTABLISHED_POST_TYPE_SUBSTITUENT_COMPANY_BOOK       3 // Заместитель начальника производства (по подготовке к изданию руководств и пособий для плавания)
#define ESTABLISHED_POST_TYPE_MAJOR_ENGINEER                 4 // Главный инженер
#define ESTABLISHED_POST_TYPE_CHIEF_DEPARTMENT               5 // Начальник отдела
#define ESTABLISHED_POST_TYPE_SUBSTITUENT_DEPARTMENT         6 // Заместитель начальника отдела
#define ESTABLISHED_POST_TYPE_CHIEF_SECTION                  7 // Начальник отделения
#define ESTABLISHED_POST_TYPE_SENIOR_EDITOR_MAP              8 // Старший редактор морских карт
#define ESTABLISHED_POST_TYPE_EDITOR_MAP                     9 // Редактор морских карт
#define ESTABLISHED_POST_TYPE_SENIOR_EDITORBOOK             10 // Старший редактор руководств для плавания
#define ESTABLISHED_POST_TYPE_EDITOR_BOOK                   11 // Редактор руководств для плавания
#define ESTABLISHED_POST_TYPE_SENIOR_TECH_EDITOR_MAP_BOOK   12 // Старший технический редактор КМ и руководств для плавания
#define ESTABLISHED_POST_TYPE_TECH_EDITOR_MAP_BOOK          13 // Технический редактор КМ и руководств для плавания
#define ESTABLISHED_POST_TYPE_SENIOR_CORRECTOR_MAP_BOOK     14 // Старший корректор КМ и руководств для плавания
#define ESTABLISHED_POST_TYPE_CORRECTOR_MAP_BOOK            15 // Корректор КМ и руководств для плавания
#define ESTABLISHED_POST_TYPE_LEADING_ENGINEER              16 // Ведущий инженер
#define ESTABLISHED_POST_TYPE_ENGINEER                      17 // Инженер
#define ESTABLISHED_POST_TYPE_ENGINEER_TECHNOLOGIST         18 // Инженер-технолог
#define ESTABLISHED_POST_TYPE_LEADING_ENGINEER_PROGRAMMER   19 // Ведущий инженер-программист
#define ESTABLISHED_POST_TYPE_ENGINEER_PROGRAMMER           20 // Инженер-программист
#define ESTABLISHED_POST_TYPE_TECHNICIAN_PROGRAMMER         21 // Техник-программист
#define ESTABLISHED_POST_TYPE_GEODESIST                     22 // Геодезист
#define ESTABLISHED_POST_TYPE_GEOPHYSICIST                  23 // Геофизик
#define ESTABLISHED_POST_TYPE_SENIOR_REFERENT               24 // Старший референт-переводчик КМ и руководств для плавания
#define ESTABLISHED_POST_TYPE_REFERENT                      25 // Референт-переводчик КМ и руководств для плавания
#define ESTABLISHED_POST_TYPE_CARTOGRAPHER                  26 // Картограф–составитель КМ и руководств для плавания
#define ESTABLISHED_POST_TYPE_LEADING_ENGINEER_TECHNOLOGIST 27 // Ведущий инженер-технолог


// Уровень подразделения
#define UNIT_LEVEL_COMPANY                   1 // Предприятие
#define UNIT_LEVEL_DIRECTION                 2 // Направление
#define UNIT_LEVEL_DEPARTMENT                3 // Отдел
#define UNIT_LEVEL_SECTION                   4 // Отделение

// Сосотояние выполнения элемента плана
#define CONDITION_CARRYOUT_SEND              1 // Задача поставлена
#define CONDITION_CARRYOUT_GET               2 // Задача получена
#define CONDITION_CARRYOUT_CARRYBEG          3 // Приступил к выполнению
#define CONDITION_CARRYOUT_CARRYEND          4 // Выполнил
#define CONDITION_CARRYOUT_PROBLEM           5 // Проблемы (причины - TCarryOutProblem)

// Причина невозможности выполнения элемента плана
#define CARRYOUT_PROBLEM_BADTASK             1 // Неподходящая задача
#define CARRYOUT_PROBLEM_EMPLMISS            2 // Отсутствуют исполнители
#define CARRYOUT_PROBLEM_BADDATA             3 // Отсутствуют (неполные) исходные данные
#define CARRYOUT_PROBLEM_UNKNERROR           4 // Неизвестная ошибка

// Типы внещних модулей
#define EXTERNAL_MODULE_TYPE_INTERNETBROWSER  1 // интернет-браузер
#define EXTERNAL_MODULE_TYPE_SCANNERPROGRAMM  2 // приложение для сканирования (в составе штатного ПО сканера)
#define EXTERNAL_MODULE_TYPE_PDFPROGRAMM      3 // Adobe Acrobat или аналог для работы с файлами формата PDF
#define EXTERNAL_MODULE_TYPE_OCRPROGRAMM      4 // ABBYY FineReader или аналог для распознавания текстов
#define EXTERNAL_MODULE_TYPE_CARIS            5 // Caris HIPS&SIPS
#define EXTERNAL_MODULE_TYPE_OKEAN            6 // АС «Океан»
#define EXTERNAL_MODULE_TYPE_CTRLOOBGD        7 // «Управление ООБГД»
#define EXTERNAL_MODULE_TYPE_DIGITPGS         8 // «ПК оцифровки ПГС»
#define EXTERNAL_MODULE_TYPE_WORD             9 // Microsoft Word
#define EXTERNAL_MODULE_TYPE_INDESIGN        10 // Adobe InDesign
#define EXTERNAL_MODULE_TYPE_NEVA            11 // ГИС «Нева»

#endif // DEFDBCONST_H
