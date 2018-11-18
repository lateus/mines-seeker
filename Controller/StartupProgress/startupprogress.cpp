#include "startupprogress.h"

#include <QCoreApplication>

StartupProgress::StartupProgress(QObject *parent) : QObject(parent)
{

}

double StartupProgress::getProgress() const
{
    return progress;
}

void StartupProgress::setProgress(double value)
{
    if (progress != value) {
        progress = value;
        emit progressChanged(progress);
        qApp->processEvents();
    }
}

QString StartupProgress::getProgressMessage() const
{
    return progressMessage;
}

void StartupProgress::setProgressMessage(const QString &value)
{
    if (progressMessage != value) {
        progressMessage = value;
        emit progressMessageChanged(progressMessage);
        qApp->processEvents();
    }
}

void StartupProgress::emitReadySignal()
{
    emit readyToLoadMainQMLFile();
}
