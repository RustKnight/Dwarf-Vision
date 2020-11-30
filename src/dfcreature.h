#ifndef DFCREATURE_H
#define DFCREATURE_H

#include "Ticket.h"
#include "sheet.h"
#include "assetdb.h"


class DfCreature
{
public:
    DfCreature();

    friend bool operator==(const DfCreature& lhs, const DfCreature& rhs) {
        return lhs.getID() == rhs.getID();
    }
    friend bool operator!=(const DfCreature& lhs, const DfCreature& rhs) {
        return lhs.getID() != rhs.getID();
    }

    void simulateDwarf();
    QList<Sheet> getSheets(AssetDB* assetDb);

    void setID (int id);
    void setName (QString name);
    void setWorldName(QString world);
    void setSelected (bool truth);

    int getID() const;
    QString getName() const;
    QString getWorldName() const;
    bool isSelected() const;

    void setAppearance(QList<QList<QStringList>> selectedFilters);


private:
    QList<QString> bodyPartsList;
    QList<QStringList> debugPartsList;
    QList<QList<QStringList>> selectedFilters;

    int id;
    QString name;
    QString world;
    bool selected;
};

#endif // DFCREATURE_H
