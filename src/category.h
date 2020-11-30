#ifndef CATEGORY_H
#define CATEGORY_H

#include <QWidget>
#include <QPushButton>
#include <QIcon>

#include "sheet.h"




namespace Ui {
class Category;
}

class Category : public QWidget
{
    Q_OBJECT

public:
    explicit Category(QWidget *parent = nullptr);
    ~Category();

    void setCategoryName(QString category);
    QString getCategoryName() const;
    void setSelected(bool truth);
    void setSheet(Sheet sheet);
    bool matchesEnum (BodyPart enu);
    const Sheet& getSheet();
    void toggleHide();
    void toggleLock();
    void applyIcon(QString iconName);
    void createDrawOrderArrows();
    void deleteDrawOrderArrows();
    void mySetHidden(bool truth);
    bool myIsHidden() const;
    QPoint getArrowUpPos() const;
    QPoint getArrowDownPos() const;



private:
    Ui::Category *ui;
    Sheet sheet;
    bool isSelected = false;
    bool hidden = false;

    void updateSheet();


public slots:
    void buttonPressed();
    void updateSheetAndNotify();


signals:
    void thisCategoryClicked(Category* category);
    void guiStateChanged();

    void moveMeUp();
    void moveMeDown();


};

#endif // CATEGORY_H
