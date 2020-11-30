#ifndef CLOTHESCHROMER_H
#define CLOTHESCHROMER_H

#include "chromer.h"

class ClothesChromer : public Chromer
{
public:
    ClothesChromer();


public:
    void saveColors() override;
    QPixmap checkAndChromarize(Sheet *sheet) override;
    void loadColors() override;
    void addColor(ColorData &color) override;
    void resetColors() override;
    void checkAndAssignColor(Sheet *sheet) override;
    void checkAndReadColor(const Sheet* sheet) override;

private:
    QString fileName;
    QList<BodyPart> vColorTaskList;



};



class ClothesSecChromer : public Chromer
{
public:
    ClothesSecChromer();


public:
    void saveColors() override;
    QPixmap checkAndChromarize(Sheet *sheet) override;
    void loadColors() override;
    void addColor(ColorData &color) override;
    void resetColors() override;
    void checkAndAssignColor(Sheet *sheet) override;
    void checkAndReadColor(const Sheet* sheet) override;

private:
    QString fileName;
    QList<BodyPart> vColorTaskList;
};



#endif // CLOTHESCHROMER_H
