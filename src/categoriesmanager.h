#ifndef CATEGORIESMANAGER_H
#define CATEGORIESMANAGER_H

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QCursor>

#include "category.h"



namespace Ui {
class CategoriesManager;
}

class CategoriesManager : public QWidget
{
    Q_OBJECT

public:
    explicit CategoriesManager(QWidget *parent = nullptr);
    ~CategoriesManager();

    void updateSheets(QList<Sheet> sheetList);
    QString getSelectedCategoryName() const;
    int indexOfSelectedSheet();
    void clickCategory(BodyPart category);

private:

    QList<Category*> getCategories();
    QList<Sheet> getSheetList();
    void resetCategories();
    void refreshCategory();

    int findNextVisibleCategoryIndex(Category* cat, bool dirUp);

private:
    Ui::CategoriesManager *ui;
    Category* selectedCategory;
    QSpacerItem* m_verticalSpacer;


public slots:


    void selectCategory(Category* category);
    void hideAll();
    void lockAll();


    void categoryUp();
    void categoryDown();
    void portraitNeedsUpdate();


signals:

    void newCategorySelected(const Sheet& sheet);
    void sheetsChangedState(const QList<Sheet>&);


};

#endif // CATEGORIESMANAGER_H





















