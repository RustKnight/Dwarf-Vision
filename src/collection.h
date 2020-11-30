#ifndef COLLECTION_H
#define COLLECTION_H

#include <QDialog>
#include <QLabel>
#include <QPainter>
#include <QMenu>
#include <QMouseEvent>
#include <QScreen>

#include "portrait.h"
#include "globals.h"




// needs refactoring to proper oop behaviour and get rid of bool hacks
// have different behaviour depending if in game mode or edit mode


class MyLabel : public QLabel{


    Q_OBJECT
public:
    MyLabel(QWidget* parent = nullptr, Portrait* portraitData = nullptr, bool hasSubMenus = false) : QLabel(parent), portraitData{portraitData}, subMenus {hasSubMenus}{

        if (hasSubMenus){

            QAction *deleteMe = new QAction("Remove from Collection", this);
            connect(deleteMe, &QAction::triggered, this, &MyLabel::deleteMe);
            this->addAction(deleteMe);

            QAction *editMe = new QAction("Edit Portrait", this);
            connect(editMe, &QAction::triggered, this, &MyLabel::editMe);
            this->addAction(editMe);

            QAction *closeCollection = new QAction("Close Collection", this);
            connect(closeCollection, &QAction::triggered, this, &MyLabel::closeMe);
            this->addAction(closeCollection);
        }
    }


signals:
    void deleteMe();
    void editMe();
    void closeMe();

protected:
    void enterEvent(QEvent *ev) override;
    void leaveEvent(QEvent *ev) override;
    void mouseReleaseEvent ( QMouseEvent * ev ) override;


public:

    QPixmap beforeOverlay;
    Portrait* portraitData;
    bool subMenus;
};




class Overlay : public QWidget {

    Q_OBJECT

public:
    Overlay(QWidget* parent = nullptr) : QWidget(parent)
    {

        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_NoSystemBackground);
        setAttribute(Qt::WA_TranslucentBackground);
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    }


    void updateSize(QSize newSize);

protected:
    void paintEvent(QPaintEvent *) override {

        QPainter(this).fillRect(rect(), QColor{0, 0, 0, 100});

    }
};



namespace Ui {
class Collection;
}

class Collection : public QDialog
{
    Q_OBJECT

    enum outOfSight {OUT_OF_SIGHT = -1000};

public:
    explicit Collection(QWidget *parent = nullptr, bool gameMode = false);
    ~Collection();

public slots:
    void addPortrait(Portrait* portrait, QPixmap pixmap);
    void showCollection(bool addSubmenus);
    void showTempCollection(QList<QPixmap> pixPortraits, QList<Portrait *> pPortrais);
    void moveBottomLeft();
    void deletePortraitAndLabel();
    void deleteLabel(MyLabel* label);
    void editPortrait();
    QList<Portrait *> getPortraits();
    void deleteAllPortraits();
    void closeDialog();
    void minimizeMe();
    void exitProgram();


    void setGameMode (bool truth);


protected:
    void enterEvent(QEvent *ev) override;
    void leaveEvent(QEvent *ev) override;


    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)  override;
    //void mouseReleaseEvent ( QMouseEvent * ev ) override;

signals:
    void editThisPortrait(Portrait* portrait);
    void needSizeChange();

private:
    Ui::Collection *ui;
    QList<Portrait*> vCollectionData;
    QList<QPixmap> vCollectionPix;

    QList<MyLabel*> vLabels;    //not in use
    bool gameMode;
    Overlay overlay;

    QPoint customPosition;

    int mouseClickXPos;
    int mouseClickYPos;

    QMenu menu;
};





#endif // COLLECTION_H


