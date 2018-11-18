#ifndef ARCADE_H
#define ARCADE_H

#include <QThread>
#include <QTimer>
#include <QTime>
#include <QSettings>
#include <QVersionNumber>
#include <QCoreApplication>

#include "../Board/board.h"

class Arcade : public QThread
{
    Q_OBJECT

    Q_PROPERTY(Board* board READ getBoard WRITE setBoard NOTIFY boardChanged)
    Q_PROPERTY(QString stringTiming READ getStringTiming NOTIFY stringTimingChanged)
    Q_PROPERTY(bool running READ getRunning NOTIFY runningChanged)
    Q_PROPERTY(bool paused READ getPaused NOTIFY pausedChanged)
    Q_PROPERTY(bool waitingForOrder READ getWaitingForOrder NOTIFY waitingForOrderChanged)
    Q_PROPERTY(int recordMs READ getRecordMs NOTIFY recordMsChanged)
    Q_PROPERTY(QString recordString READ getRecordString NOTIFY recordStringChanged)

    Q_PROPERTY(QString username READ getUsername WRITE setUsername NOTIFY usernameChanged)

public:
    explicit Arcade(QObject *parent = nullptr);
    ~Arcade();

    QString getStringTiming() const;
    void setStringTiming(const QString &value); // not exposed to QML engine

    Board* getBoard();
    void setBoard(Board *boardPtr);

    bool getPaused() const;
    void setPaused(bool value);

    bool getRunning() const;
    void setRunning(bool value);

    bool getWaitingForOrder() const;
    void setWaitingForOrder(bool value);

    int getRecordMs() const;
    void setRecordMs(int value);

    QString getRecordString() const;

    QString getUsername() const;
    void setUsername(const QString &value);

    // Implement this when multithreading
//protected:
//    void run() override;

//private:
//    int changeModeMode = int(Board::BEGINNER);
//    int changeModeCustomRows    = Board::beginnerRows;
//    int changeModeCustomColumns = Board::beginnerColumns;
//    int changeModeCustomMines   = Board::beginnerMines;

signals:
    void readyToStart();
    void stringTimingChanged(const QString &str);
    void boardChanged(Board *boardPtr);
    void runningChanged(bool newState);
    void pausedChanged(bool newState);
    void waitingForOrderChanged(bool newState);
    void recordMsChanged(int ms);
    void recordStringChanged(const QString &str);

    void usernameChanged(const QString &newUsername);

    void bigUnlock();
    void flaggingDenied();

    void win();
    void lose();
    void finished();

public slots:
    void newGame();
    void pauseGame();
    void changeMode(int mode, int customRows = Board::beginnerRows, int customColumns = Board::beginnerColumns, int customMines = Board::beginnerMines);
    void startTiming();
    void whenWin();
    void whenLose();
    void whenFinished();
    void loadArcadeSettings();
    void saveArcadeSettings();

private slots:
    void advanceTiming();
    void stopTimer();
    void resetTiming();

private:
    Board board;
    QString stringTiming;
    bool running = false;
    bool paused = false;
    bool waitingForOrder = false;

    QTime timing; // do not expose to QML engine
    QTimer timingUpdater; // do not expose to QML engine
    int remainingTimeWhenPaused = -1; // do not expose to QML engine
    int recordMs = 0;

    // settings
    QString username;

    // private helper functions
    void configureEndOfGame();
    void configureTiming();
};

#endif // ARCADE_H
