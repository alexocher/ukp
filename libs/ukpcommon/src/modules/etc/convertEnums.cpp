
#include <convertEnums>

// Виды продукции (первый уровень иерархии)
QString convertEnums::enumToStr(TProductionView en)
{
    switch (en)
    {
        case prvMaps:    return "Карты";
        case prvManual:  return "Руководства и пособия";
        case prvOni:     return "ОНИ";
        case prvTopo:    return "Материалы гидрографических, геофизических и океанографических работ и топографических съемок, материалы ДЗЗ, элементы содержания карты";
        case prvDock:    return "Руководящие и информационные документы";
        case prvRequest: return "Заявки (из других подсистем)";
        default:         return ""; // prvNone
    }
}
//-----------------------------------------------------------------------------

void convertEnums::strToEnum(QString, TProductionView &) // nm, en
{

}
//-----------------------------------------------------------------------------

// Второй уровень иерархии
QString convertEnums::enumToStr(TProductionRubr2 en)
{
    switch (en)
    {
        case pr2MapsSea:       return "Морские карты";
        case pr2MapsRiver:     return "Карты внутренних водных путей";
        case pr2ManualSea:     return "Морские руководства и пособия";
        case pr2ManualScience: return "Научно-технические пособия";
        case pr2ManualBasis:   return "Фундаментальные пособия по географии морей и океанов";
        case pr2ManualOther:   return "Прочие издания";
        case pr2TopoWork:      return "Материалы гидрографических, геофизических и океанографических работ";
        case pr2TopoShoot:     return "Материалы топографических съемок";
        case pr2TopoDzz:       return "Материалы ДЗЗ (аэрофотосъёмка, космическая съёмка, лазерная съёмка)";
        default:               return ""; // pr2None
    }
}
//-----------------------------------------------------------------------------

void convertEnums::strToEnum(QString, TProductionRubr2 &) // nm, en
{

}
//-----------------------------------------------------------------------------

// Третий уровень иерархии
QString convertEnums::enumToStr(TProductionRubr3 en)
{
    switch (en)
    {
        case pr3NavigationSea:   return "Навигационные морские карты";
        case pr3HelpSea:         return "Морские вспомогательные и справочные карты";
        case pr3NavigationRiver: return "Навигационные карты внутренних водных путей";
        case pr3ManualSea:       return "Морские навигационные руководства и пособия";
        case pr3ManualFish:      return "Морские промысловые пособия";
        default:                 return ""; // pr3None
    }
}
//-----------------------------------------------------------------------------

void convertEnums::strToEnum(QString, TProductionRubr3 &) // nm, en
{

}
//-----------------------------------------------------------------------------

// Четвертый уровень иерархии
QString convertEnums::enumToStr(TProductionRubr4 en)
{
    switch (en)
    {
        case pr4MsManual:    return "Руководства и пособия для плавания";
        case pr4MsSea_hydro: return "Морские гидрометеорологические пособия";
        case pr4MsSea_astro: return "Морские астрономические пособия";
        case pr4MsForeign:   return "Международно-правовые руководства";
        case pr4MsHelp:      return "Вспомогательные и справочные пособия для мореплавателей";
        default:             return ""; // pr4None
    }
}
//-----------------------------------------------------------------------------

void convertEnums::strToEnum(QString, TProductionRubr4 &) // nm, en
{

}
//-----------------------------------------------------------------------------

// Типы продукции
QString convertEnums::enumToStr(TProductionType en)
{
    switch (en)
    {
        case prtMapsTopo:         return "Топографические карты";
        case prtMapsEmk:          return "Специальные электронные морские карты (ЭМК) для ВМФ";
        case prtManualGsNavy:     return "Уставные документы ГС ВМФ";
        case prtManualGuide:      return "Руководящие документы, производственно-технические руководства и пособия";
        case prtManualRiver:      return "Руководства и пособия для внутренних водных путей";
        case prtOniForeignIm:     return "Иностранные ИМ";
        case prtOniNavarea:       return "НАВАРЕА, НАВИП";
        case prtOniPrip:          return "ПРИП";
        case prtOniShip:          return "ОНИ, получаемые непосредственно от мореплавателей (донесения командиров, капитанов и штурманов судов)";
        case prtOniAgensy:        return "Сообщения морских агентств, обслуживающих суда";
        case prtOniGs_fleet:      return "Сообщения ГС флотов";
        case prtOniNavarea13:     return "Бюллетень навигационных предупреждений НАВАРЕА-13";
        case prtOniNotice:        return "Извещения мореплавателям (выпуск, доп. корректура и приложение)";
        case prtOniWarning:       return "Бюллетень навигационных предупреждений";
        case prtOniNoticeEn:      return "Извещения мореплавателям на английском языке";
        case prtOniNumeric:       return "Нумерники извещений мореплавателям с приложениями";
        case prtOniListTemp:      return "Перечни действующих временных и предварительных извещений мореплавателям";
        case prtOniTab:           return "Вкладка";
        case prtOniSpecialIm:     return "Спец. выпуск ИМ";
        case prtTopoFragment:     return "Элементы содержания карты в цифровом и аналоговом виде";
        case prtDockNavy:         return "Документы Генерального штаба, штабов флотов и др., определяющие режимы плавания";
        case prtDockCountry:      return "Законы, постановления, инструкции, положения, правила и другие официальные документы, регламентирующие взаимоотношения государств на океанах, морях и ВВП";
        case prtDockInfo:         return "Информационный бюллетень";
        case prtRequestQuery:     return "Рекламации, заявки потребителей";
        case prtRequestLetter:    return "Письма, факсы, прочие документы";
        case prtScienceWork:      return "Научные труды и монографии по навигации, гидрографии и океанографии";
        case prtScienceLearning:  return "Учебные пособия для гидрографов и штурманов";
        case prtScienceMilitary:  return "Военно-научные издания";
        case prtSciencePeriodica: return "Периодические научно-технические издания";
        case prtOceanAtlas:       return "Атлас океанов";
        case prtOtherLearning:    return "Прочие пособия по географии морей и океанов";
        case prtBlank:            return "Бланковые издания";
        case prtHistory:          return "Документально-исторические издания";
        case prtRepresent:        return "Представительские издания и документы";
        case prtTwTabletGs:       return "Планшеты ГС";
        case prtTwCndGs:          return "ЦНД ГС";
        case prtTwAtlas:          return "Атласы изученности";
        case prtTwPages:          return "Сборные листы планшетов";
        case prtTwHydroReport:    return "Научно-технические и технические отчеты о гидрографической съемке";
        case prtTwHydroGs:        return "Материалы, собранные ГС во время производства систематических и попутных гидрографических работ и при корректуре руководств для плавания и карт по местности";
        case prtTwTabletGebco:    return "Планшеты GEBCO";
        case prtTwActHydro:       return "Акты научно-технической экспертизы и приёмки работ";
        case prtTwTabletTopo:     return "Планшеты топографической съемки";
        case prtTwTopoReport:     return "Технические отчеты о геодезических, астрономических, гравиметрических и топографических работах";
        case prtTwCoord:          return "Каталоги координат и высот пунктов";
        case prtTwNomenclature:   return "Сборные таблицы номенклатуры топографических карт и картотека планов городов";
        case prtTwActTopo:        return "Акты научно-технической экспертизы и приемки работ";
        case prtDzzAero:          return "Данные аэрофотосъёмки";
        case prtDzzSpace:         return "Данные космической съёмки";
        case prtDzzLazer:         return "Данные лазерной съёмки";
        case prtNsNmk:            return "НМК навигационные морские карты";
        case prtNsEnk:            return "ЭНК электронные навигационные карты";
        case prtNsRadio:          return "Радионавигационные карты и радионавигационные планшеты";
        case prtNsFish:           return "Навигационно-промысловые карты";
        case prtNsCorrectEnk:     return "Корректурные наборы к ЭНК";
        case prtHsInfo:           return "Справочно-информационные карты";
        case prtHsBlank:          return "Бланковые морские карты";
        case prtHsGrid:           return "Карты-сетки";
        case prtHsOrto:           return "Карты для прокладки ортодромий";
        case prtHsRescue:         return "Карты для спасательных средств";
        case prtHsBottom:         return "Карты с наглядным изображением рельефа морского дна";
        case prtHsBathy:          return "Батиметрические карты";
        case prtHsWires:          return "Карты подводных кабелей";
        case prtHsTime:           return "Карты часовых поясов";
        case prtHsWarTraining:    return "Карты районов боевой подготовки";
        case prtHsMemory:         return "Карты памятных мест побед и героической гибели кораблей русского и советского флотов";
        case prtHsYacht:          return "Карты для яхт и прогулочных катеров";
        case prtHsWorld:          return "Карта мира";
        case prtNrLake:           return "Навигационные карты судоходных озер";
        case prtNrRiver:          return "Навигационные речные карты";
        case prtNrEnk:            return "ЭНК внутренних путей";
        case prtMfishDescript:    return "Промысловые описания";
        case prtMfishMountain:    return "Описания подводных гор и поднятий морского дна в промысловых районах Мирового океана";
        case prtMsmLocies:        return "Лоции и приложения к ним";
        case prtMsmFire:          return "Огни и знаки (Огни)";
        case prtMsmRadio:         return "Радиотехнические средства навигационного оборудования";
        case prtMsmRegime:        return "Режим плавания судов + описание мерных линий";
        case prtMsmShedRadio:     return "Расписание передач навигационных предупреждений и гидрометеорологических сообщений радиостанциями";
        case prtMsmShedFax:       return "Расписание факсимильных гидрометеорологических передач";
        case prtMsmRules:         return "Правила плавания";
        case prtMsmManualRus:     return "Руководства для захода судов в российские порты";
        case prtMsmManualNord:    return "Руководства для плавания в акватории Северного морского пути";
        case prtMsmOceanWay:      return "Океанские пути мира";
        case prtMsmTablePhc:      return "Таблицы радионавигационных систем (РНС)";
        case prtMsmAtlasPhc:      return "Атласы зон точности определения места с помощью РНС";
        case prtMsshAtlasParam:   return "Атласы океанографических параметров";
        case prtMsshGydrometeo:   return "Комплекты гидрометеорологических карт морей и районов океанов";
        case prtMsshAtlasIce:     return "Атласы льдов";
        case prtMsshAtlasFlow:    return "Атласы приливных, поверхностных, приливо-отливных и иных течений";
        case prtMsshTableFlow:    return "Таблицы приливов и приливных течений";
        case prtMsshAtlasDanger:  return "Атласы опасных и особо опасных гидрометеорологических явлений";
        case prtMsshAtlasBottom:  return "Атласы донных осадков";
        case prtMsshOther:        return "Прочие гидрометеорологические пособия";
        case prtMssaSeaYear:      return "Морской астрономический ежегодник";
        case prtMssaAlma:         return "Астронавигационный альманах";
        case prtMssaTba57:        return "Таблицы вычислений высот и азимутов (ТВА-57)";
        case prtMssaBac58:        return "Таблицы \"Высоты и азимуты светил\" (ВАС-58)";
        case prtMssaNord_sun:     return "Таблицы восхода и захода Солнца для северных широт 74°-90°";
        case prtMssaNc2026:       return "Астрономические данные N и С на 2006-2026 гг.";
        case prtMsfJustice:       return "Руководство \"Пределы действия территориальной юрисдикции государств в Мировом океане\"";
        case prtMsfCollision:     return "Международные правила предупреждения столкновения судов в море";
        case prtMsfForeign:       return "Сборник международных договоров и законодательных актов по вопросам мореплавания";
        case prtMsfRf:            return "Морское законодательство Российской Федерации";
        case prtMsfOther:         return "Прочие международно-правовые руководства";
        case prtMshCatalog:       return "Каталоги карт и книг";
        case prtMshSeaTable:      return "Мореходные таблицы";
        case prtMshSeaDistance:   return "Таблицы морских расстояний";
        case prtMshFire:          return "Описание особенностей огней военных кораблей и сигналов, подаваемых кораблями и судами для обеспечения безопасности плавания";
        case prtMshMamcA:         return "Описание системы плавучего ограждения в водах России. Система МАМС, регион А";
        case prtMshWays:          return "Общие положения об установлении путей движения судов";
        case prtMshSignal:        return "Международный свод сигналов";
        case prtMshLegend:        return "Условные знаки";
        case prtMshPorts:         return "Порты мира";
        case prtMshOyher:         return "Прочие вспомогательные и справочные пособия для мореплавателей";
        case prtMshGsFleet:       return "Издания гидрографических служб флотов";
        default:                  return ""; // prtNone
    }
}
//-----------------------------------------------------------------------------

void convertEnums::strToEnum(QString, TProductionType &) // nm, en
{

}
//-----------------------------------------------------------------------------

// Типы должностей
QString convertEnums::enumToStr(TEmployeeType en)
{
    switch (en)
    {
        case eltChief:          return "Начальник";
        case eltSubstituent:    return "Заместитель";
        case eltMajorEngineer:  return "Главный инженер";
        case eltLeadingEditor:  return "Ведущий редактор";
        case eltMajorEditor:    return "Главный редактор";
        case eltSeniorEditor:   return "Старший редактор";
        case eltTechEditor:     return "Технический редактор";
        case eltResearchEditor: return "Редактор изученности";
        case eltExpertEditor:   return "Редактор технической экспертизы";
        case eltEditor:         return "Редактор";
        case eltOobgdOperator:  return "Оператор ООБГД";
        case eltOperator:       return "Оператор";
        default:                return ""; // eltNone
    }
}
//-----------------------------------------------------------------------------

void convertEnums::strToEnum(QString nm, TEmployeeType &en)
{
    if (nm==QString("Начальник")) en = eltChief;
    else if (nm==QString("Заместитель")) en = eltSubstituent;
    else if (nm==QString("Главный инженер")) en = eltMajorEngineer;
    else if (nm==QString("Ведущий редактор")) en = eltLeadingEditor;
    else if (nm==QString("Главный редактор")) en = eltMajorEditor;
    else if (nm==QString("Старший редактор")) en = eltSeniorEditor;
    else if (nm==QString("Технический редактор")) en = eltTechEditor;
    else if (nm==QString("Редактор изученности")) en = eltResearchEditor;
    else if (nm==QString("Редактор технической экспертизы")) en = eltExpertEditor;
    else if (nm==QString("Редактор")) en = eltEditor;
    else if (nm==QString("Оператор ООБГД")) en = eltOobgdOperator;
    else if (nm==QString("Оператор")) en = eltOperator;
    else en = eltNone;
}
//-----------------------------------------------------------------------------

QString convertEnums::enumToStr(TCarryOutCondition en)
{
    switch (en)
    {
        case cocSend:     return "Задача поставлена";
        case cocGet:      return "Задача получена";
        case cocCarryBeg: return "Приступил к выполнению";
        case cocCarryEnd: return "Выполнил";
        case cocProblem:  return "Проблемы";
        default:          return ""; // cocNone
    }
}
//-----------------------------------------------------------------------------

void convertEnums::strToEnum(QString nm, TCarryOutCondition &en)
{
    if (nm==QString("Задача поставлена")) en = cocSend;
    else if (nm==QString("Задача получена")) en = cocGet;
    else if (nm==QString("Приступил к выполнению")) en = cocCarryBeg;
    else if (nm==QString("Выполнил")) en = cocCarryEnd;
    else if (nm==QString("Проблемы")) en = cocProblem;
    else en = cocNone;
}
//-----------------------------------------------------------------------------

QString convertEnums::enumToStr(TCarryOutProblem en)
{
    switch (en)
    {
        case copBadTask:   return "Неподходящая задача";
        case copEmplMiss:  return "Отсутствуют исполнители";
        case copBadData:   return "Отсутствуют (неполные) исходные данные";
        case copUnknError: return "Неизвестная ошибка";
        default:           return ""; // copNone
    }
}
//-----------------------------------------------------------------------------

void convertEnums::strToEnum(QString nm, TCarryOutProblem &en)
{
    if (nm==QString("Неподходящая задача")) en = copBadTask;
    else if (nm==QString("Отсутствуют исполнители")) en = copEmplMiss;
    else if (nm==QString("Отсутствуют (неполные) исходные данные")) en = copBadData;
    else if (nm==QString("Неизвестная ошибка")) en = copUnknError;
    else en = copNone;
}
//-----------------------------------------------------------------------------

QString convertEnums::enumToStr(TStatus en)
{
    switch (en)
    {
        case stsOnExamination:       return "«На экспертизу»";
        case stsOnEvaluation:        return "«На оценку информативности»";
        case stsOnCheck:             return "«На проверку»";
        case stsOnCompletion:        return "«На доработку»";
        case stsCellsFormed:         return "«Ячейки сформированы»";
        case stsChecked:             return "«Проверено»";
        case stsCheckedSeniorEditor: return "«Проверено старшим редактором»";
        case stsCheckedMainEdition:  return "«Проверено главной редакцией (ОТК)»";
        case stsChangeEdition:       return "«Изменить редакцию ООБГД»";
        case stsAssert:              return "«Утверждено»";
        default:                     return ""; // stsNone
    }
}
//-----------------------------------------------------------------------------

void convertEnums::strToEnum(QString nm, TStatus &en)
{
    if (nm==QString("«На экспертизу»")) en=stsOnExamination;
    else if (nm==QString("«На оценку информативности»")) en=stsOnEvaluation;
    else if (nm==QString("«На проверку»")) en=stsOnCheck;
    else if (nm==QString("«На доработку»")) en=stsOnCompletion;
    else if (nm==QString("«Ячейки сформированы»")) en=stsCellsFormed;
    else if (nm==QString("«Проверено»")) en=stsChecked;
    else if (nm==QString("«Проверено старшим редактором»")) en=stsCheckedSeniorEditor;
    else if (nm==QString("«Проверено главной редакцией (ОТК)»")) en=stsCheckedMainEdition;
    else if (nm==QString("«Изменить редакцию ООБГД»")) en=stsChangeEdition;
    else if (nm==QString("«Утверждено»")) en=stsAssert;
    else en = stsNone;
}
//-----------------------------------------------------------------------------

QString convertEnums::enumToStr(TExternalModuleType en)
{
    switch (en)
    {
        case emtInternetBrowser: return "Интернет-браузер";
        case emtScannerProgramm: return "Приложение для сканирования";
        case emtPdfProgramm:     return "Adobe Acrobat или аналог";
        case emtOcrProgramm:     return "ABBYY FineReader или аналог";
        case emtCaris:           return "Caris HIPS&SIPS";
        case emtOkean:           return "АС «Океан»";
        case emtCtrlOOBGD:       return "«Управление ООБГД»";
        case emtDigitPGS:        return "«ПК оцифровки ПГС»";
        case emtWord:            return "Microsoft Word";
        case emtInDesign:        return "Adobe InDesign";
        case emtNeva:            return "ГИС «Нева»";
        default:                 return ""; // emtNone
    }
}
//-----------------------------------------------------------------------------

void convertEnums::strToEnum(QString nm, TExternalModuleType &en)
{
    if (nm==QString("Интернет-браузер")) en = emtInternetBrowser;
    else if (nm==QString("Приложение для сканирования")) en = emtScannerProgramm;
    else if (nm==QString("Adobe Acrobat или аналог")) en = emtPdfProgramm;
    else if (nm==QString("ABBYY FineReader или аналог")) en = emtOcrProgramm;
    else if (nm==QString("Caris HIPS&SIPS")) en = emtCaris;
    else if (nm==QString("АС «Океан»")) en = emtOkean;
    else if (nm==QString("«Управление ООБГД»")) en = emtCtrlOOBGD;
    else if (nm==QString("«ПК оцифровки ПГС»")) en = emtDigitPGS ;
    else if (nm==QString("Microsoft Word")) en = emtWord ;
    else if (nm==QString("Adobe InDesign")) en = emtInDesign ;
    else if (nm==QString("ГИС «Нева»")) en = emtNeva ;
    else en = emtNone;
}
//-----------------------------------------------------------------------------
