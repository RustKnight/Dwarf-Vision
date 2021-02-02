#ifndef PORTRAIT_H
#define PORTRAIT_H

#include <sheet.h>
#include <dfcreature.h>
#include "rawconvertor.h"


class Portrait
{
public:

    friend QDataStream &operator << (QDataStream & out, const Portrait& entry) {

        out << entry.hashCategoryPositions;

        out << entry.vSheets.size();

        for (Sheet* sheet : entry.vSheets)
            out << *sheet;

        out << entry.id;

        out << entry.name;

        out << entry.isPlaceholder;

        return out;
    }

    friend QDataStream &operator >> (QDataStream & in, Portrait& entry) {

        in >> entry.hashCategoryPositions;

        int totalSheets;
        in >> totalSheets;

        for (int i = 0; i < totalSheets; ++i){

            Sheet* sheet = new Sheet;
            in >> *sheet;
            entry.vSheets << sheet;
        }

        in >> entry.id;

        in >> entry.name;

        in >> entry.isPlaceholder;

        return in;
    }

    Portrait();
    Portrait(QList<Sheet*> sheetList);
    ~Portrait();
    // todo: copy ctor and assignment ctor

    void updateSheets(QList<Sheet*> sheetList);
    QList<Sheet*> getSheets();
    int getNeckHeight();
    void setNeckHeight(int height);
    void checkAndDoBeardStich();
    void randomizeNeckHeight();
    void refreshFrames();

    void absorpGuiSettings(QList<Sheet*> sourceList);
    void setIgnoreGuiAbsorption(bool truth);

    void absorbDfCreature(const DfCreature& creature, int mode);

    int getID() const;
    QString getName() const;
    bool isSelected() const;
    void setSelected(bool truth);

    void setID(int id);

    void convertToFemalePlaceholder(QPixmap QSplash);



private:
    void refreshSheetsPointers();

private:
    QList<Sheet*> vSheets;
    QHash<BodyPart, int> hashCategoryPositions;
    bool ignoreGuiAbsorption = false;

    int id;
    QString name;
    QString world;
    bool selected = false;
    bool IdAgnostic = false;
    bool isPlaceholder = false;


};

#endif // PORTRAIT_H
