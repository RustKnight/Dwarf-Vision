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

    QList<Sheet> getSheets(AssetDB* assetDb);

    void setID (int id);
    void setName (QString name);
    void setWorldName(QString world);
    void setSelected (bool truth);
    void setBp (const QList<int>& bpList);

    void checkHairConsistency();
    void checkBeardConsistency();
    void checkMoustacheConsistency();

    void setHair(const QString& s);
    void setBeard(const QString& s);
    void setMoustache(const QString& s);
    void setSideburns(const QString& s);
    void setHairColor (const QString& s);
    void setIrisColor (const QString& s);
    void setGender(bool gender);


    QString getHair() const;
    QString getBeard() const;
    QString getMoustache() const;
    QString getSideburns() const;
    QString getHairColor() const;
    QString getIrisColor() const;
    bool isMale();

    QList<int> getBpList() const;

    int getID() const;
    QString getName() const;
    QString getWorldName() const;
    bool isSelected() const;

    // it will just have a list with adjectives that will be read by a new random portrait,
    // which will set its frames according to this list
    void setAppearance(QList<QList<QStringList>> selectedFilters);


private:
    QList<int> bpValuesList;
    QList<QList<QStringList>> selectedFilters;

    int id;
    QString name;
    QString world;
    bool selected;
    bool male;

    QString hair;
    QString beard;
    QString moustache;
    QString sideburns;

    QString hairColor;
    QString irisColor;

};

#endif // DFCREATURE_H











