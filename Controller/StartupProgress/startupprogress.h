#ifndef STARTUPPROGRESS_H
#define STARTUPPROGRESS_H

#include <QObject>

class StartupProgress : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double progress READ getProgress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(QString progressMessage READ getProgressMessage WRITE setProgressMessage NOTIFY progressMessageChanged)

public:
    explicit StartupProgress(QObject *parent = nullptr);

    double getProgress() const;
    void setProgress(double value);

    QString getProgressMessage() const;
    void setProgressMessage(const QString &value);

signals:
    void progressChanged(double value);
    void progressMessageChanged(const QString &msg);
    void readyToLoadMainQMLFile();

public slots:
    void emitReadySignal();

private:
    double progress = 0.0;
    QString progressMessage = tr("Initializing...");
};

#endif // STARTUPPROGRESS_H
