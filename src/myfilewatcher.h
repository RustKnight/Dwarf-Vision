#ifndef MYFILEWATCHER_H
#define MYFILEWATCHER_H

#include <QObject>
#include <QDir>
#include <QTimer>


class MyFileWatcher : public QObject
{   
    Q_OBJECT

public:
    MyFileWatcher(QString fileString);


public slots:
    void onTimerTrigger();
    void start();

signals:
    void readDfMessage();


private:

    QFile watchFile;
    QTimer* timer;
};

#endif // MYFILEWATCHER_H
