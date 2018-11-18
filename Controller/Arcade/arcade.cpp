#include "arcade.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>

Arcade::Arcade(QObject *parent) : QThread(parent), stringTiming(QTime(0, 0, 0, 0).toString("mm:ss")), timing(QTime(0, 0, 0, 0))
{
    configureEndOfGame();
    configureTiming();

    connect(&board, &Board::runningChanged, this, &Arcade::setRunning);
    connect(&board, &Board::bigUnlock, this, &Arcade::bigUnlock);
    connect(&board, &Board::flaggingDenied, this, &Arcade::flaggingDenied);

    loadArcadeSettings();

    QTimer::singleShot(100, this, &Arcade::readyToStart);
}

Arcade::~Arcade()
{
    saveArcadeSettings();
}

void Arcade::configureEndOfGame()
{
    connect(&board, &Board::gameOverByMineActivation, this, &Arcade::whenLose);
    connect(&board, &Board::gameCleared, this, &Arcade::whenWin);
    connect(&board, &Board::finished, this, &Arcade::whenFinished);
}

void Arcade::configureTiming()
{
    timingUpdater.setInterval(1000);
    connect(&timingUpdater, &QTimer::timeout, this, &Arcade::advanceTiming);
    connect(&board, &Board::start, this, &Arcade::startTiming);
    connect(&board, &Board::finished, this, &Arcade::stopTimer);
}

void Arcade::newGame()
{
    board.newGame();
    stopTimer();
    resetTiming();
    setWaitingForOrder(false);
}

void Arcade::pauseGame()
{
    remainingTimeWhenPaused = timingUpdater.remainingTime();
    stopTimer();
    setPaused(true);
    // hide the board in the UI
}

void Arcade::changeMode(int mode, int customRows, int customColumns, int customMines)
{
    stopTimer();
    resetTiming();
    setWaitingForOrder(false);
    board.setMode(Board::BOARDMODE(mode), customRows, customColumns, customMines); // this will end the current game, if any
}

void Arcade::whenWin()
{
    int ms = 1000 - timingUpdater.remainingTime();
    if (ms < 0) {
        ms = 1;
    }
    setRecordMs(timing.hour() > 0 ? 999*59*59 : timing.msecsSinceStartOfDay() + ms);
    emit win();
}

void Arcade::whenLose()
{
    emit lose();
}

void Arcade::whenFinished()
{
    setWaitingForOrder(true);
    emit finished();
}

void Arcade::loadArcadeSettings()
{
    QSettings s;
    if (!s.contains("Arcade/Mode")) {
        board.setup();
    } else {
        int mode = s.value("Arcade/Mode").toInt();
        int cr   = s.value("Arcade/CustomGame/Rows").toInt();
        int cc   = s.value("Arcade/CustomGame/Columns").toInt();
        int cm   = s.value("Arcade/CustomGame/Mines").toInt();
        board.setCustomGameRows(cr);
        board.setCustomGameColumns(cc);
        board.setCustomGameMines(cm);
        changeMode(mode, cr, cc, cm);
    }

    QString usrn = s.value("Arcade/Username").toString();
    if (usrn.isEmpty()) {
        QFileInfo f(QDir::homePath());
        setUsername(f.baseName());
    } else {
        setUsername(usrn);
    }
}

void Arcade::saveArcadeSettings()
{
    QSettings s;
    s.setValue("Arcade/Mode", int(board.getMode()));
    s.setValue("Arcade/CustomGame/Rows", board.getCustomGameRows());
    s.setValue("Arcade/CustomGame/Columns", board.getCustomGameColumns());
    s.setValue("Arcade/CustomGame/Mines", board.getCustomGameMines());
    s.setValue("Arcade/Username", username);
}

void Arcade::startTiming()
{
    timingUpdater.start(remainingTimeWhenPaused >= 0 ? remainingTimeWhenPaused : 1000);
    setRunning(true);
    setPaused(false);
}

void Arcade::advanceTiming()
{
    if (timingUpdater.interval() != 1000) {
        timingUpdater.setInterval(1000);
        remainingTimeWhenPaused = -1;
    }
    timing = timing.addSecs(1);
    QString format = "mm:ss";
    if (timing.hour() > 0) {
        format.prepend("hh:");
    }
    QString string = timing.toString(format);
    setStringTiming(string);
}

Board *Arcade::getBoard()
{
    return &board;
}

void Arcade::setBoard(Board *boardPtr)
{
    board = *boardPtr;
    emit boardChanged(&board);
}

QString Arcade::getStringTiming() const
{
    return stringTiming;
}

void Arcade::setStringTiming(const QString &value)
{
    if (stringTiming != value) {
        stringTiming = value;
        emit stringTimingChanged(stringTiming);
    }
}

bool Arcade::getRunning() const
{
    return running;
}

void Arcade::setRunning(bool value)
{
    if (running != value) {
        running = value;
        emit runningChanged(running);
    }
}

bool Arcade::getPaused() const
{
    return paused;
}

void Arcade::setPaused(bool value)
{
    if (paused != value) {
        paused = value;
        emit pausedChanged(paused);
    }
}


//! SLOTS
void Arcade::stopTimer()
{
    timingUpdater.stop();
}

void Arcade::resetTiming()
{
    timing = QTime(0, 0, 0, 0);
    setStringTiming(timing.toString("mm:ss"));
    setRecordMs(0);
    setPaused(false);
}

QString Arcade::getUsername() const
{
    return username;
}

void Arcade::setUsername(const QString &value)
{
    if (username != value) {
        username = value;
        emit usernameChanged(username);
    }
}

int Arcade::getRecordMs() const
{
    return recordMs;
}

void Arcade::setRecordMs(int value)
{
    if (recordMs != value) {
        recordMs = value;
        emit recordMsChanged(recordMs);
        emit recordStringChanged(getRecordString());
    }
}

QString Arcade::getRecordString() const
{

    return QTime(0, 0, 0, 0).addMSecs(recordMs).toString("mm:ss.zzz");
}

bool Arcade::getWaitingForOrder() const
{
    return waitingForOrder;
}

void Arcade::setWaitingForOrder(bool value)
{
    if (waitingForOrder != value) {
        waitingForOrder = value;
        emit waitingForOrderChanged(waitingForOrder);
    }
}
