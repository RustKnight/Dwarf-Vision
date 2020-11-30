#ifndef IRISCHROMER_H
#define IRISCHROMER_H

#include "chromer.h"

class IrisChromer : public Chromer
{
public:
    IrisChromer();


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

#endif // IRISCHROMER_H
