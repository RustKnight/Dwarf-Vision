#ifndef MYSTRINGLIST_H
#define MYSTRINGLIST_H


#include <QListWidget>

#include <sheet.h>

class MyStringList : public QListWidget
{
    Q_OBJECT
public:
    explicit MyStringList(QWidget *parent = nullptr);



public slots:
    void updateStringList(const Sheet& sheet);
    void changeStrIndex(int index);


private:
    // this is needed because the onCategoryChange is triggered BEFORE changeStrIndex, due to connect() order
    int itemIndex = 0;

};

#endif // MYSTRINGLIST_H
