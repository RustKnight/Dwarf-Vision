#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>



#include <Qt3DInput/qabstractphysicaldevice.h>
#include <Qt3DInput/qt3dinput_global.h>
#include <Qt3DInput/QMouseDevice>
#include <Qt3DInput/QMouseHandler>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileSystemWatcher>
#include <QScreen>
#include <QToolTip>
#include <QDesktopServices>


#include "assetdb.h"
#include "portraitmanager.h"
#include "globals.h"
#include "filtercontroller.h"
#include "dfcreature.h"
#include "collection.h"
#include "dfmessagereader.h"
#include "myfilewatcher.h"


enum FabulationState {FULL, PARTIAL, NONE};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ticket getTicket() const;

    void init();

public slots:

    void onPreviousPortrait();
    void onNextPortrait();
    void onGenerateClick();
    void onGenderToggle();
    void onListWidgetFrameChange(const QString& frameStr);
    void onCultureChange(const QString& culture);
    void onFilterClick(QWidget *stackedPage);
    void onAllCheckBoxClick(QAbstractButton* allButton);
    void generateGallery();
    void prepareSheetsAndPassToPortraitManager(const QList<Sheet>& sheetList);    
    void syncSheets(QList<Sheet*> pSheetsList);
    void onFilterListClick(QListWidgetItem *item);
    void savePNG();
    void savePNG_commandLine(QString nameArg);
    void createFolders();
    void onCodeSideNeckHeightChange(int height);
    void savePortraits();
    void loadPortraits(QString world);
    void onDwarfFortressSignal(bool forceExecute = false);
    void onDwarfFortressSignal_DEBUG();
    void changeWorld(QString newWorld);
    void drawCollection();
    void changeGameMaxPortraitSize();
    void onGameChangeClick();
    void onPurgePortraitsClick();
    void onFabulationInfo();
    void onFabulationSliderChange(int newState);
    void toggleSecondaryLayersVisibility();

    Sheet *findSheetInList(QList<Sheet*> list, BodyPart bp);
    QList<Sheet*> findMultipleSheetsInList(QList<Sheet*> list, QList<BodyPart> bpList);

    void resizeEvent(QResizeEvent *);
    void moveEvent(QMoveEvent *event);
    void changeEvent(QEvent *event);





signals:
    void filtersChanged (QStringList filters);

public:
    Ui::MainWindow *ui;
    AssetDB assetDB;
    PortraitManager portraitManager;
    FilterController* filterController;
    bool inGameMode = false;
    Collection collection;
    DfMessageReader dfMsgReader;
    QString messageTxtPath;
    QFileSystemWatcher watcher;
    MyFileWatcher* myFileWatcher;

    QHash<int, Portrait*> portraitDB;
    QString currentWorld = "-1";

    int maxPortraitSize = 195;//170;

    FabulationState fabulation = FULL;


};
#endif // MAINWINDOW_H















