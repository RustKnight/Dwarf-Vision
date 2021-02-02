#ifndef PORTRAITMANAGER_H
#define PORTRAITMANAGER_H

#include <QWidget>
#include <QGraphicsScene>
#include <QPainter>
#include <QMap>
#include <QGraphicsPixmapItem>
#include <QRectF>
#include <qdialog.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QGraphicsView>
#include <QElapsedTimer>
#include <QFileDialog>

#include "colorizer.h"
#include "sheet.h"
#include "portrait.h"
#include "collection.h"


// also, if you go back on a previous portrait, and press generate, it will crash/modify that current portrait in selection


class PortraitManager : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PortraitManager(QWidget *parent = nullptr, Colorizer* colorizer = nullptr);


    void updateFrame(int SheetIndex, QString newFrame);
    void applyColorizerPointer(Colorizer* colorizer);
    void applyGraphicsView (QGraphicsView* graphicsView);


public slots:

    void updateSheetList(QList<Sheet*> sheetList);

    void onGenerateButtonClicked();
    void randomizePortraitSheets();

    void checkAndDoBeardStich();
    Portrait* createPortrait();
    void drawPortrait(Portrait *portrait);
    QPixmap portraitToPixmap(Portrait* portrait, float size);
    void RedrawPortrait();


    void DBstorePortrait(Portrait * portrait);
    void drawGallery(int qty, int size);
    void setGuiSheetsNeckHeight(int height);
    void changeNeckHeight(int height);
    void toggleColorRandomization(bool truth);
    void next();
    void previous();
    QPixmap getPixmapCurrentPortrait();
    void updateColorForCurrentPortrait();
    void changePortraitColor(Portrait* portrait, QString hairColor, QString irisColor);


    void onCategoryChange();

    void addPortraitToCollection();
    void showCollection(float portraitSize);
    void findDrawEditPortrait(Portrait * portrait);
    void saveCurrentPortrait();
    void loadPortrait();


signals:
    void focusedCategoryChangedFrame();
    void portraitRandomized(int);
    void newPortraitSelected(const QList<Sheet*> sheets);
    void currentlySelectedFrameIndex(int);
    void neckHeightChanged(int);

private:
    void deleteAndAssignNewSheetsList(QList<Sheet*> pList);

private:
    QList<Sheet*> vGuiSheetList;
    Colorizer* colorizer;
    QGraphicsView* graphicsView;
    bool randomizeColor;

    QList<Portrait*> portraitDB;
    int portraitIndex = 0;
    Collection collection;

    QStringList loadedFiles;
};

#endif // PORTRAITMANAGER_H
















