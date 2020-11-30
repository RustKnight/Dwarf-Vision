#ifndef HAIRCHROMER_H
#define HAIRCHROMER_H

#include "chromer.h"

class HairChromer : public Chromer
{
public:
    HairChromer();


public:
    void saveColors() override;
    QPixmap checkAndChromarize(Sheet *sheet) override;
    void loadColors() override;
    void addColor(ColorData &color) override;
    void resetColors() override;
    void checkAndReadColor(const Sheet* sheet) override;
    void checkAndAssignColor(Sheet *sheet) override;

private:
    QString fileName;
    QList<BodyPart> vColorTaskList;
};



#endif // HAIRCHROMER_H
