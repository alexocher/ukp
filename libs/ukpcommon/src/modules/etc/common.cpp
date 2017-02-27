
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <common>
#include <defMacro>

std::string common::readFile(const QString &fnm)
{
PR1(0,"current: %1",QDir::currentPath());
PR1(0,"file: %1",fnm);
  QFile file(fnm);
  if (!file.open(QIODevice::ReadOnly)) return "";
PR_FN_LN
  qint64 len(file.size());
PR1(4,"L: %1",fnm);
  char *s(new char[len]);
  QDataStream stream(&file);
    stream.readRawData(s,len);
  std::string resStr(s,len);
    file.close();
    return resStr;
}
//-----------------------------------------------------------------------------

bool common::writeFile(const QString &fnm, std::string &data)
{
    if (FILE *file = fopen(fnm.toLocal8Bit().data(),"wb")) // Старое содержимое стирается. Если файла нет - он создается.
    {
        if (fwrite(data.c_str(),1,data.size(),file)!=data.size()) { fclose(file); return false; }
        fclose(file);
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------

QJsonValue common::value(const std::string &json, const QString &nm)
{
  QJsonDocument jsdDocument = QJsonDocument::fromJson(QByteArray::fromStdString(json));
  QJsonObject jsoRoot = jsdDocument.object();
    return jsoRoot.value(nm);
/*
#if defined(WORK_WITH_LOCAL_DATA)
  QStringList mainUnits;
  std::string filedata(common::readFile(PROJ->dataPath()+"data/var/orgshtat.json"));
    if (!filedata.empty())
    {
      QJsonDocument jsdDocument = QJsonDocument::fromJson(QByteArray::fromStdString(filedata));
      QJsonObject jsoRoot = jsdDocument.object();
      QJsonValue jsvUnits = jsoRoot.value("units");
        if (jsvUnits.isArray())
        {
          QJsonArray jsaUnits = jsvUnits.toArray();
            for (int i=0; i<jsaUnits.count(); i++)
            {
              QJsonObject jsoUnit = jsaUnits[i].toObject();
                TUnit *newUn(new TUnit(jsoUnit.value("id").toInt(),jsoUnit.value("name").toString(),jsoUnit.value("shortName").toString()));
                  newUn->setISort(jsoUnit.value("parent").toInt()); // для последующей связи
                  newUn->setLevel((TUnitLevel)jsoUnit.value("level").toInt());
                  newUn->setShtatEmployeeCount(jsoUnit.value("emloyeeCount").toInt());
                  fUnits.append(newUn);
            }
            foreach (TUnit *un,fUnits)
            {
                if (!un->iSort())
                {
                    fMainUnit = un; // последнее найденное
                    mainUnits<<un->name();
                    continue;
                }
                if (TUnit *unChief = findUnit(un->iSort()))
                {
                    un->setChiefUnit(unChief);
                    unChief->insertSubUnit(un);
                }
                else INSERT_ERROR(QString("Не найден родитель (id: %1) для <%2>").arg(un->iSort()).arg(un->name()),false);
            }
        }
    }
    // ??? if (!fSelfUnit) { INSERT_ERROR("Не найдено собственное подразделение",true); return false; }
    if (!mainUnits.count()) { INSERT_ERROR("Не найдено подразделение уровня предприятия",true); return false; } // ??? if (!fMainUnit)
    else if (mainUnits.count()>1)
    {
        INSERT_ERROR("Найдено более одного подразделения уровня предприятия",true);
        foreach (QString nm,mainUnits) INSERT_ERROR(QString("    %1").arg(nm),false);
        return false;
    }
#else
*/
}
//-----------------------------------------------------------------------------
