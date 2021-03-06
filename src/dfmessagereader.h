#ifndef DFMESSAGEREADER_H
#define DFMESSAGEREADER_H

#include "dfcreature.h"
#include "QTextStream"
#include <QElapsedTimer>


enum msgIndex { IS_SELECTED, ID, NAME, GEND, AGE, DESCRIPTION, BODYPARTS_VALUES, HAIRSTYLE, COLORS, CURLINESS };


class DfMessageReader
{
public:
    DfMessageReader();

    void readMessage(QFile& message);    // populates creatureList

    QList<DfCreature> getCreatures() const;

    QStringList parseApperance(QString text);

    bool sameAsLastMessage();

private:
    QList<DfCreature> creatureList;
    QList<DfCreature> lastCreatureList;

};

#endif // DFMESSAGEREADER_H
