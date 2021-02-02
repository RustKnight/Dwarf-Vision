#ifndef COLORIZER_H
#define COLORIZER_H

#include <QWidget>

#include <QColor>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QColorDialog>
#include <QListWidget>

#include "sheet.h"
#include "colorizer/hairchromer.h"
#include "colorizer/irischromer.h"
#include "colorizer/clotheschromer.h"
#include "colorizer/backgroundchromer.h"


// DONE needs to initialize DF colors at start

// DONE hold a color list from which to read and write colors
// DONE color assets that need coloring using saved colors

// DONE dynamically be able to change color VIA: QT color picker save it in color list
    // must be able to modify values of base, highlight, dark individually
    // alter lerp function


// load colors from hair_colors.txt to combobox
// reflect in comboBox what color is selected and change currentColor from the combobox


// hair_colors.txt is just meant to store colors - NOT to edit them via text
// if some colors need tweaking, it will be done via GUI, by changing vHairColors's values
// after this is done - via a save button, Colorizer will delete all text from hair_colors.txt and reRewrite colors as their current state


// EXPAND COLORIZER TO HANDLE OTHER ASSETS THAN HAIR

// be able to easily colorize individual assets with their own color (Eg: make beard dark blond but hair light blond)
// handle colors for eyes or possibly clothes

// use strategy (aka interface) and composition
// Colorizer will have an object for each task(function) that needs substantial change
    // when GUI changes what we want to colorize (hair, beard, eyes), Colorizer changes component pointers to match the according strategy
    // Colorizer must switch it's components accoding to what sheet needs color

    // do we need a factory for componenet instantiation?


// load colors from correct txt file
// save files to correct txt file
// change GUI labels
// own list for relevant colorable assets

// lerp needs to express clearly when it is saved (On each modification/on color save, etc.)
// (?) own lerp value for each color

// DONE add a new color to the txt file, by selecting it via the colorDialog
    // NOTE: newly added colors are added only by their BASE colors


//DONE txt color values need to be stored in source. When missing new ones should be copied into the working folder.
    //SUSPENDED if txt colors are corrupted, propose refreshing it with the source txt file. Warn that this will delete current contents.
                // sort of dumb since you would lose all data
                // would be better to just cut out damaged data and try to read the rest
    // DONE restore default colors button for selected radio chromer button


// DONE lock color randomization
    // implemented in PortraitManager -> could have been done in Colorizer thou...



// instead of keeping an array of chromers, hold just one base chromer pointer and swap it as needed with derived classes

enum colorManagersIndex {HAIR_CHROMER, IRIS_CHROMER, CLOTHES_CHROMER, CLOTHES_SECONDARY_CHROMER, BACKGROUND_CHROMER};


namespace Ui {
class Colorizer;
}

class Colorizer : public QWidget
{
    Q_OBJECT

public:
    explicit Colorizer(QWidget *parent = nullptr);
    ~Colorizer();

    void randomizeUsedColor();
    void loadColors();
    QPixmap colorize(Sheet* sheet);

private slots:
    void saveColorsToText();
    void swapCurrentChromer(colorManagersIndex index);
    void updateGui();

public slots:
    void alterColor(const QColor &color);
    void updateLerpVal(double val);
    void saveColor(const QColor &color);
    void openColorDialog();
    void changeSelectedColorIndex(int index);
    void onChromerRadioChange();
    void addNewColor();
    void deleteColor();
    void resetColors();
    void overwriteAllColors(Sheet* sh);
    void overwriteColor(Sheet* sh);
    void readSheetsForColors(const QList<Sheet*> sheetList);
    void onColorLockedClick(bool btnState);

    void changeHairColorTo(QString color);
    void changeIrisColorTo(QString color);

    void changeIrisColor (QList<Sheet*> sheetList, QString newColor);
    void changeHairColor (QList<Sheet*> sheetList, QString newColor);

signals:
    void selectedColorChanged();

private:
    Ui::Colorizer *ui;
    QColorDialog colorDialog;

    QList<Chromer*> vChromers;
    int chrIndex;
};



#endif // COLORIZER_H
