#include "assetdb.h"
#include <QCoreApplication>


AssetDB::AssetDB()
{


    //QString path = QCoreApplication::applicationDirPath() + "/resources";
    QString path = ":/resources";
    QDir dir (path);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    QFileInfoList raceFoldersInfo = dir.entryInfoList();

    readTotalRaces(raceFoldersInfo);


    for (int c = 0; c < culturesList.size(); c++){
        assetDB.push_back(QList < QList < QList < NamedPixmap >>> ());

        for (int g = 0; g < INVALID_GENDER; g++) {
            assetDB[c].push_back(QList < QList < NamedPixmap >> ());

            for (int p = 0; p < INVALID_ASSET; p++){
                assetDB[c][g].push_back(QList < NamedPixmap > ());

            }
        }
    }


}

void AssetDB::readTotalRaces(QFileInfoList raceFolders)
{

    QStringList culturesList;


     for (const QFileInfo& folderInfo : raceFolders){

         QString folderName = folderInfo.fileName();

         culturesList << folderName;
     }

     this->culturesList = culturesList;

}

int AssetDB::getRaceIndex(QString pngName)
{

    QStringList pngNameSplit = pngName.split("_");

    for (int i = 0; i < culturesList.size(); ++i)
        if (pngName.contains(pngNameSplit[i]))
            return i;

    // failed
    return -111;
}

const QStringList &AssetDB::getRacesList()
{

    return culturesList;

}

void AssetDB::shortenBodyPart(QString &pngName)
{

    QStringList split = pngName.split("_");

    split.removeAt(BODYPART);
    split.removeAt(GENDER);
    split.removeAt(RACE);

    pngName = split.join("_");

}


void AssetDB::loadResources()
{

    // look in resources folder

    // store a list of Qpixmaps according to their matching assetEnum

    // assets are grouped in folders only by race
    // for each folder in resources
    // read the name of the png
        // access assetDB dimensions via culture_gender_bodyPart
        // store

    //QString path = QCoreApplication::applicationDirPath() + QDir::separator() + "resources";
    QString path = ":/resources/";
    QDir dir (path);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    QFileInfoList raceFoldersInfo = dir.entryInfoList();


    for (const QFileInfo& folderInfo : raceFoldersInfo){

        QDir folder (folderInfo.filePath());
        folder.setFilter(QDir::NoDotAndDotDot | QDir::Files);
        QFileInfoList pngList = folder.entryInfoList();


        for (const QFileInfo& pngInfo : pngList){


            QString pngName = pngInfo.baseName();

            Ticket ticket (pngName);     
            if (ticket.isInvalid())
                continue;

            shortenBodyPart(pngName);

            assetDB[getRaceIndex(pngName)][ticket.gender][ticket.bodypart].push_back(NamedPixmap(pngInfo.filePath(), pngName));
        }
    }


}

void AssetDB::checkAssetsNameCorrectness(QStringList allFilters)
{

    QStringList erroneousFiles;

    // loop through all namedPixmap lists
    for (int c = 0; c < culturesList.size(); c++){

        for (int g = 0; g < INVALID_GENDER; g++) {

            for (int p = 0; p < INVALID_ASSET; p++){

                // for each namedPixmap
                const QList<NamedPixmap>& namedPixList = assetDB[c][g][p];

                for (const NamedPixmap& namedPix : namedPixList){

                    // ignore "empty" named files
                    if (namedPix.name.contains("empty"))
                        continue;

                    QString fileName = namedPix.name;

                    // split name
                    QStringList splitList = fileName.split("_");

                    // each split must have a match in filter list
                    bool matchFound = false;

                    for (QString split : splitList){

                        for (QString filter : allFilters){

                            // if found, stop and move to next split
                            if (split.compare(filter, Qt::CaseInsensitive) == 0 ){
                                matchFound = true;
                                break;
                            }
                        }


                        // split missed all filters!
                        if (!matchFound){

                            // add file to error list
                            if (!erroneousFiles.contains(fileName))
                                erroneousFiles << fileName;

                            // break; file bad. move to next file
                            break;
                        }

                    }
                }

            }
        }
    }

    // inform user via msgBox on problematic file
    if (!erroneousFiles.isEmpty()){

        QString message = "Asset(s) name contains unknown filters!\nSearch in assets folder for:\n";

        for (QString str : erroneousFiles)
            message.append("\n" + str);

        QMessageBox messageBox;
        messageBox.warning(0,"Error", message);
        messageBox.setFixedSize(500,200);
    }

}



Sheet AssetDB::getSheet(Ticket ticket, QList<QStringList>& filters)
{

    Sheet sheet(ticket.gender, ticket.bodypart);
    bool noPNGsFound = true;

    for (const NamedPixmap& bodyPart : assetDB[ticket.cultureIndex][ticket.gender][ticket.bodypart]){

        if (bodyPart.passedFilters(filters) || bodyPart.name.contains("empty")){

            sheet.addFrame(bodyPart.pixmap, bodyPart.name);
            noPNGsFound = false;
        }
    }


    if (noPNGsFound)
        sheet.addDummy("No assets found using current filters!");

    return sheet;
}



QList<Sheet> AssetDB::getAllSheets(Ticket ticket)
{

    // bodyPart is needed for sheet registration

    // for each Qlist<png>
    // create a new Sheet
    // add sheet in return list


    // in order to create a custom sheet (ex: only concave noses), DB must hold QPixmap and their specific bodyPart name (concave_hooked)
    // DB might be needed to store a Pixmap & NameString struct instead

    QList<Sheet> result;

    for (int bodyPart = 0; bodyPart < assetDB[ticket.cultureIndex][ticket.gender].size(); ++bodyPart){

        // add frames to sheet
        Sheet sheet(ticket.gender, (BodyPart)bodyPart);
        for (const NamedPixmap& bodyPart : assetDB[ticket.cultureIndex][ticket.gender][bodyPart]){

            sheet.addFrame(bodyPart.pixmap, bodyPart.name);
        }

        if (assetDB[ticket.cultureIndex][ticket.gender][bodyPart].isEmpty())
            sheet.addDummy("Could not find any assets for this category!");

        result << sheet;
    }

    return result;
}




































