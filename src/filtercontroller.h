#ifndef FILTERCONTROLLER_H
#define FILTERCONTROLLER_H

#include <QDialog>
#include <QListWidgetItem>
#include <QTextStream>
#include <QDir>


#include "sheet.h"


namespace Ui {
class FilterController;
}

class FilterController : public QDialog
{
    Q_OBJECT

public:
    explicit FilterController(QWidget *parent = nullptr);
    ~FilterController();

public slots:
    void onCategoryChange(const Sheet& sheet);
    QStringList getAllFiltersNoSpace();
    void syncFilterListWidget();
    void emitClickedStackedPage();
    void emitClickedAllBtn();
    QString removeFilter(QString workString, QString removeThis);

    void outputFiltersToText();


signals:
    void filterButtonClicked(QWidget* stackedPage);
    void filterButtonClickedAll(QAbstractButton* allBtn);
    void filterListWidgetItemClicked(QListWidgetItem* item);

public:
    Ui::FilterController *ui;
};

#endif // FILTERCONTROLLER_H
