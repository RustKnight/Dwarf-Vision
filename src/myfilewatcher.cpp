#include "myfilewatcher.h"

MyFileWatcher::MyFileWatcher(QString fileString) : watchFile {fileString}, timer {new QTimer(this)}
{


}


void MyFileWatcher::onTimerTrigger()
{
    emit readDfMessage();
}

void MyFileWatcher::start()
{
    timer->start(50);
    connect(timer, &QTimer::timeout, this, &MyFileWatcher::onTimerTrigger);
}
