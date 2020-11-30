#ifndef BACKGROUNDCHROMER_H
#define BACKGROUNDCHROMER_H

#include "chromer.h"

class BackgroundChromer : public Chromer
{
public:
    BackgroundChromer();


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

#endif // BACKGROUNDCHROMER_H
