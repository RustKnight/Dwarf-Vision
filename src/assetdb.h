#ifndef ASSETDB_H
#define ASSETDB_H


#include <QList>
#include <QPixmap>
#include <QDir>
#include <QFile>
#include <QRegularExpression>

#include "Ticket.h"
#include "sheet.h"


struct NamedPixmap{

    NamedPixmap(QPixmap pix, QString name) : pixmap {pix}, name {name}
    {

        if (pixmap.size() != g_assetSize){


            // Check size of asset. Warn. Option to stop.
            QString var = QString::number(pixmap.size().width()) + "x" + QString::number(pixmap.size().height());
            QString varExpected = QString::number(g_assetSize.width()) + "x" + QString::number(g_assetSize.height());

            QMessageBox messageBox;
            messageBox.warning(0,"Warning", "Asset is " + var + "\nExpected size is " + varExpected + "!\nFile name: " + name);
            messageBox.setFixedSize(500,200);

            QMessageBox::StandardButton reply;
            reply = messageBox.question(nullptr, "", "Ignore and continue?",
                                            QMessageBox::Yes|QMessageBox::No);

            if (reply == QMessageBox::No)
                exit(0);
        }
    }


    bool passedFilters (QList<QStringList>& filterLists) const{

        // split bodyParts name
        QStringList splitList = name.split("_");
        bool filterFoundStatus;


        // check splitList against filters
        for (QStringList& filterList: filterLists){

            filterFoundStatus = false;

            for (QString& filter : filterList){

                for (QString& split: splitList){

                    if (split.compare(filter, Qt::CaseInsensitive) == 0){

                        filterFoundStatus = true;
                        goto nextFilterList;
                    }
                    // finised all splits
                }
                // finised all filters from list (for this loop)
            }

            // if false, we failed to match the current filter in the filters for-loop; return false
            if (filterFoundStatus == false)
                return filterFoundStatus;

            nextFilterList:;
        }

        // if here, all filters found, return true
        return true;
    }

    QPixmap pixmap;
    QString name;
};

class AssetDB
{
public:
    AssetDB();


    void loadResources();

    void checkAssetsNameCorrectness(QStringList allFilters);

    Sheet getSheet (Ticket ticket, QList<QStringList>& filters);
    QList<Sheet> getAllSheets (Ticket ticket);

    void readTotalRaces(QFileInfoList allPNGs);
    int getRaceIndex (QString pngName);

    const QStringList& getRacesList();

    void shortenBodyPart(QString& pngName);

private:


    QList < QList < QList < QList <NamedPixmap> > > >  assetDB;
    QStringList culturesList;
};


#endif // ASSETDB_H
