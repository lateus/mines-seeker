#ifndef RECORDSMANAGER_H
#define RECORDSMANAGER_H

#include <QThread>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QSqlQuery>
#include <QTime> // to format time record
#include <QSettings>
#include <QCryptographicHash>
#include <QRandomGenerator>

#include "../../Controller/Board/board.h"

class RecordsManager : public QThread
{
    Q_OBJECT

    Q_PROPERTY(int totalBeginnerGames READ getTotalBeginnerGames NOTIFY totalBeginnerGamesChanged)
    Q_PROPERTY(int totalMediumGames   READ getTotalMediumGames   NOTIFY totalMediumGamesChanged)
    Q_PROPERTY(int totalExpertGames   READ getTotalExpertGames   NOTIFY totalExpertGamesChanged)
    Q_PROPERTY(int totalCustomGames   READ getTotalCustomGames   NOTIFY totalCustomGamesChanged)

    Q_PROPERTY(int beginnerGamesWin READ getBeginnerGamesWin NOTIFY beginnerGamesWinChanged)
    Q_PROPERTY(int mediumGamesWin   READ getMediumGamesWin   NOTIFY mediumGamesWinChanged)
    Q_PROPERTY(int expertGamesWin   READ getExpertGamesWin   NOTIFY expertGamesWinChanged)
    Q_PROPERTY(int customGamesWin   READ getCustomGamesWin   NOTIFY customGamesWinChanged)

    Q_PROPERTY(int     maxRecordsAllowed    READ getMaxRecordsAllowed WRITE setMaxRecordsAllowed NOTIFY maxRecordsAllowedChanged)
    Q_PROPERTY(int     maxRecordMs          READ getMaxRecordMs       NOTIFY maxRecordMsChanged)
    Q_PROPERTY(QString maxRecordString      READ getMaxRecordString   NOTIFY maxRecordStringChanged)
    Q_PROPERTY(Board::BOARDMODE currentMode READ getBoardMode         NOTIFY currentModeChanged)
    Q_PROPERTY(bool busy  READ getBusy  NOTIFY busyChanged)
    Q_PROPERTY(bool empty READ getEmpty NOTIFY emptyChanged)

    Q_PROPERTY(QString beginnerName1  READ getBeginnerName1  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerName2  READ getBeginnerName2  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerName3  READ getBeginnerName3  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerName4  READ getBeginnerName4  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerName5  READ getBeginnerName5  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerName6  READ getBeginnerName6  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerName7  READ getBeginnerName7  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerName8  READ getBeginnerName8  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerName9  READ getBeginnerName9  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerName10 READ getBeginnerName10 NOTIFY rankingUpdated)

    Q_PROPERTY(QString mediumName1  READ getMediumName1  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumName2  READ getMediumName2  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumName3  READ getMediumName3  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumName4  READ getMediumName4  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumName5  READ getMediumName5  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumName6  READ getMediumName6  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumName7  READ getMediumName7  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumName8  READ getMediumName8  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumName9  READ getMediumName9  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumName10 READ getMediumName10 NOTIFY rankingUpdated)

    Q_PROPERTY(QString expertName1  READ getExpertName1  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertName2  READ getExpertName2  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertName3  READ getExpertName3  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertName4  READ getExpertName4  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertName5  READ getExpertName5  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertName6  READ getExpertName6  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertName7  READ getExpertName7  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertName8  READ getExpertName8  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertName9  READ getExpertName9  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertName10 READ getExpertName10 NOTIFY rankingUpdated)

    Q_PROPERTY(QString customName1  READ getCustomName1  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customName2  READ getCustomName2  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customName3  READ getCustomName3  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customName4  READ getCustomName4  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customName5  READ getCustomName5  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customName6  READ getCustomName6  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customName7  READ getCustomName7  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customName8  READ getCustomName8  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customName9  READ getCustomName9  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customName10 READ getCustomName10 NOTIFY rankingUpdated)

    Q_PROPERTY(QString beginnerTime1  READ getBeginnerTime1  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerTime2  READ getBeginnerTime2  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerTime3  READ getBeginnerTime3  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerTime4  READ getBeginnerTime4  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerTime5  READ getBeginnerTime5  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerTime6  READ getBeginnerTime6  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerTime7  READ getBeginnerTime7  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerTime8  READ getBeginnerTime8  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerTime9  READ getBeginnerTime9  NOTIFY rankingUpdated)
    Q_PROPERTY(QString beginnerTime10 READ getBeginnerTime10 NOTIFY rankingUpdated)

    Q_PROPERTY(QString mediumTime1  READ getMediumTime1  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumTime2  READ getMediumTime2  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumTime3  READ getMediumTime3  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumTime4  READ getMediumTime4  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumTime5  READ getMediumTime5  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumTime6  READ getMediumTime6  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumTime7  READ getMediumTime7  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumTime8  READ getMediumTime8  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumTime9  READ getMediumTime9  NOTIFY rankingUpdated)
    Q_PROPERTY(QString mediumTime10 READ getMediumTime10 NOTIFY rankingUpdated)

    Q_PROPERTY(QString expertTime1  READ getExpertTime1  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertTime2  READ getExpertTime2  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertTime3  READ getExpertTime3  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertTime4  READ getExpertTime4  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertTime5  READ getExpertTime5  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertTime6  READ getExpertTime6  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertTime7  READ getExpertTime7  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertTime8  READ getExpertTime8  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertTime9  READ getExpertTime9  NOTIFY rankingUpdated)
    Q_PROPERTY(QString expertTime10 READ getExpertTime10 NOTIFY rankingUpdated)

    Q_PROPERTY(QString customTime1  READ getCustomTime1  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customTime2  READ getCustomTime2  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customTime3  READ getCustomTime3  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customTime4  READ getCustomTime4  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customTime5  READ getCustomTime5  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customTime6  READ getCustomTime6  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customTime7  READ getCustomTime7  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customTime8  READ getCustomTime8  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customTime9  READ getCustomTime9  NOTIFY rankingUpdated)
    Q_PROPERTY(QString customTime10 READ getCustomTime10 NOTIFY rankingUpdated)

public:
    enum ACTION {NONE, ADD, REMOVE, RESET};

    explicit RecordsManager(const QString &filePath = "./records.db", QObject *parent = nullptr);
    ~RecordsManager() override;

    /*  I can find a fair way to implement the statistics backend
     *  because I cant' know who user just lose, so I can't count
     *  the losed games for a particular user.
     */
//    Q_INVOKABLE bool addGame(int mode, bool win = false);
    Q_INVOKABLE bool addRecord(const QString &playerName, int ms, int steps, int record);
    Q_INVOKABLE void setCurrentTable(int mode);

    int getMaxRecordsAllowed() const;
    void setMaxRecordsAllowed(int value);

    int getMaxRecordMs() const;
    void setMaxRecordMs(int value);

    QString getMaxRecordString() const;

    Board::BOARDMODE getBoardMode() const;

    bool getBusy() const;
    void setBusy(bool value);

    bool getEmpty() const;
    void setEmpty(bool value);

    int getTotalBeginnerGames() const;
    void setTotalBeginnerGames(int value);

    int getTotalMediumGames() const;
    void setTotalMediumGames(int value);

    int getTotalExpertGames() const;
    void setTotalExpertGames(int value);

    int getTotalCustomGames() const;
    void setTotalCustomGames(int value);

    int getBeginnerGamesWin() const;
    void setBeginnerGamesWin(int value);

    int getMediumGamesWin() const;
    void setMediumGamesWin(int value);

    int getExpertGamesWin() const;
    void setExpertGamesWin(int value);

    int getCustomGamesWin() const;
    void setCustomGamesWin(int value);

    QString getBeginnerName1()  const;
    QString getBeginnerName2()  const;
    QString getBeginnerName3()  const;
    QString getBeginnerName4()  const;
    QString getBeginnerName5()  const;
    QString getBeginnerName6()  const;
    QString getBeginnerName7()  const;
    QString getBeginnerName8()  const;
    QString getBeginnerName9()  const;
    QString getBeginnerName10() const;

    QString getMediumName1()  const;
    QString getMediumName2()  const;
    QString getMediumName3()  const;
    QString getMediumName4()  const;
    QString getMediumName5()  const;
    QString getMediumName6()  const;
    QString getMediumName7()  const;
    QString getMediumName8()  const;
    QString getMediumName9()  const;
    QString getMediumName10() const;

    QString getExpertName1()  const;
    QString getExpertName2()  const;
    QString getExpertName3()  const;
    QString getExpertName4()  const;
    QString getExpertName5()  const;
    QString getExpertName6()  const;
    QString getExpertName7()  const;
    QString getExpertName8()  const;
    QString getExpertName9()  const;
    QString getExpertName10() const;

    QString getCustomName1()  const;
    QString getCustomName2()  const;
    QString getCustomName3()  const;
    QString getCustomName4()  const;
    QString getCustomName5()  const;
    QString getCustomName6()  const;
    QString getCustomName7()  const;
    QString getCustomName8()  const;
    QString getCustomName9()  const;
    QString getCustomName10() const;

    QString getBeginnerTime1()  const;
    QString getBeginnerTime2()  const;
    QString getBeginnerTime3()  const;
    QString getBeginnerTime4()  const;
    QString getBeginnerTime5()  const;
    QString getBeginnerTime6()  const;
    QString getBeginnerTime7()  const;
    QString getBeginnerTime8()  const;
    QString getBeginnerTime9()  const;
    QString getBeginnerTime10() const;

    QString getMediumTime1()  const;
    QString getMediumTime2()  const;
    QString getMediumTime3()  const;
    QString getMediumTime4()  const;
    QString getMediumTime5()  const;
    QString getMediumTime6()  const;
    QString getMediumTime7()  const;
    QString getMediumTime8()  const;
    QString getMediumTime9()  const;
    QString getMediumTime10() const;

    QString getExpertTime1()  const;
    QString getExpertTime2()  const;
    QString getExpertTime3()  const;
    QString getExpertTime4()  const;
    QString getExpertTime5()  const;
    QString getExpertTime6()  const;
    QString getExpertTime7()  const;
    QString getExpertTime8()  const;
    QString getExpertTime9()  const;
    QString getExpertTime10() const;

    QString getCustomTime1()  const;
    QString getCustomTime2()  const;
    QString getCustomTime3()  const;
    QString getCustomTime4()  const;
    QString getCustomTime5()  const;
    QString getCustomTime6()  const;
    QString getCustomTime7()  const;
    QString getCustomTime8()  const;
    QString getCustomTime9()  const;
    QString getCustomTime10() const;

protected:
    void run() override;

signals:
    void rankingUpdated();
    void maxRecordsAllowedChanged(int max);
    void maxRecordMsChanged(int ms);
    void maxRecordStringChanged(const QString &str);
    void currentModeChanged(Board::BOARDMODE newMode);
    void busyChanged(bool newState);
    void emptyChanged(bool newState);

    void totalBeginnerGamesChanged();
    void totalMediumGamesChanged();
    void totalExpertGamesChanged();
    void totalCustomGamesChanged();

    void beginnerGamesWinChanged();
    void mediumGamesWinChanged();
    void expertGamesWinChanged();
    void customGamesWinChanged();

public slots:
//    void updateStatistics();
    void updateRanking();
    void updateCurrentMaxRecord();
    void reset();

private:
    QSqlDatabase db;
    const QStringList gameTables = QStringList() << "BEGINNER" << "MEDIUM" << "EXPERT" << "CUSTOM";
    const QStringList allTables  = QStringList() << "BEGINNER" << "MEDIUM" << "EXPERT" << "CUSTOM"/* << "STATISTICS"*/;
    QSqlTableModel *table = nullptr;
    QSqlRecord recordTemplate;
    int maxRecordsAllowed = 10;
    Board::BOARDMODE boardMode = Board::BEGINNER;
    int maxRecordMs = 3477519; // 59:59.999

    ACTION action = NONE;
    bool busy = false;
    bool empty = true;

    // NOT expose to QML engine:
    // to add player
    QString playerToAdd;
    int msToAdd;
    int stepsToAdd;
    int recordToAdd;

//    bool alreadyHasStatisticsTable = false;

    int totalBeginnerGames = 0;
    int totalMediumGames   = 0;
    int totalExpertGames   = 0;
    int totalCustomGames   = 0;

    int beginnerGamesWin = 0;
    int mediumGamesWin   = 0;
    int expertGamesWin   = 0;
    int customGamesWin   = 0;

    // QML:
    QString beginnerName1  = "----------";
    QString beginnerName2  = "----------";
    QString beginnerName3  = "----------";
    QString beginnerName4  = "----------";
    QString beginnerName5  = "----------";
    QString beginnerName6  = "----------";
    QString beginnerName7  = "----------";
    QString beginnerName8  = "----------";
    QString beginnerName9  = "----------";
    QString beginnerName10 = "----------";

    QString mediumName1  = "----------";
    QString mediumName2  = "----------";
    QString mediumName3  = "----------";
    QString mediumName4  = "----------";
    QString mediumName5  = "----------";
    QString mediumName6  = "----------";
    QString mediumName7  = "----------";
    QString mediumName8  = "----------";
    QString mediumName9  = "----------";
    QString mediumName10 = "----------";

    QString expertName1  = "----------";
    QString expertName2  = "----------";
    QString expertName3  = "----------";
    QString expertName4  = "----------";
    QString expertName5  = "----------";
    QString expertName6  = "----------";
    QString expertName7  = "----------";
    QString expertName8  = "----------";
    QString expertName9  = "----------";
    QString expertName10 = "----------";

    QString customName1  = "----------";
    QString customName2  = "----------";
    QString customName3  = "----------";
    QString customName4  = "----------";
    QString customName5  = "----------";
    QString customName6  = "----------";
    QString customName7  = "----------";
    QString customName8  = "----------";
    QString customName9  = "----------";
    QString customName10 = "----------";

    QString beginnerTime1  = "59:59.999";
    QString beginnerTime2  = "59:59.999";
    QString beginnerTime3  = "59:59.999";
    QString beginnerTime4  = "59:59.999";
    QString beginnerTime5  = "59:59.999";
    QString beginnerTime6  = "59:59.999";
    QString beginnerTime7  = "59:59.999";
    QString beginnerTime8  = "59:59.999";
    QString beginnerTime9  = "59:59.999";
    QString beginnerTime10 = "59:59.999";

    QString mediumTime1    = "59:59.999";
    QString mediumTime2    = "59:59.999";
    QString mediumTime3    = "59:59.999";
    QString mediumTime4    = "59:59.999";
    QString mediumTime5    = "59:59.999";
    QString mediumTime6    = "59:59.999";
    QString mediumTime7    = "59:59.999";
    QString mediumTime8    = "59:59.999";
    QString mediumTime9    = "59:59.999";
    QString mediumTime10   = "59:59.999";

    QString expertTime1    = "59:59.999";
    QString expertTime2    = "59:59.999";
    QString expertTime3    = "59:59.999";
    QString expertTime4    = "59:59.999";
    QString expertTime5    = "59:59.999";
    QString expertTime6    = "59:59.999";
    QString expertTime7    = "59:59.999";
    QString expertTime8    = "59:59.999";
    QString expertTime9    = "59:59.999";
    QString expertTime10   = "59:59.999";

    QString customTime1    = "59:59.999";
    QString customTime2    = "59:59.999";
    QString customTime3    = "59:59.999";
    QString customTime4    = "59:59.999";
    QString customTime5    = "59:59.999";
    QString customTime6    = "59:59.999";
    QString customTime7    = "59:59.999";
    QString customTime8    = "59:59.999";
    QString customTime9    = "59:59.999";
    QString customTime10   = "59:59.999";

    bool setupDatabase(const QString &filePath);
    bool setupDatabaseTables();
    void setupTable();
    void setupRecordTemplate();

    void updateRankingBeginner();
    void updateRankingMedium();
    void updateRankingExpert();
    void updateRankingCustom();
};

#endif // RECORDSMANAGER_H
