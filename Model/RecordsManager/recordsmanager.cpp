#include "recordsmanager.h"

#include <QDebug>
#define DEBUG_OUTPUT

#define SHA3_512(X) QCryptographicHash::hash((X), QCryptographicHash::Sha3_512).toHex()

RecordsManager::RecordsManager(const QString &filePath, QObject *parent) : QThread(parent)
{
    setupRecordTemplate();
    setupDatabase(filePath);
}

RecordsManager::~RecordsManager()
{
    delete table;
}

void RecordsManager::run()
{
    setBusy(true);

    // to avoid 'jump to case label [-fpermissive]' warning
    QSqlQuery q(db);
    QSqlRecord record(recordTemplate);

    switch (action) {
    case RESET:
        for (int t = 0; t < allTables.count(); ++t) {
            QString sql = "DROP TABLE \'" + allTables[t] + '\'';
            if (!q.exec(sql)) {
#ifdef DEBUG_OUTPUT
                qCritical() << "Cannot reset table" << allTables[t] << ":" << q.lastError().text();
#endif
            }
        }
        setEmpty(true);
        setupDatabaseTables();
        setCurrentTable(boardMode);
        updateChecksum();
        updateNValidator();
        updateRanking();
        break;
    case ADD:
        record.setValue("NAME", playerToAdd);
        record.setValue("TIME", msToAdd);
        record.setValue("STEPS", stepsToAdd);
        record.setValue("SCORE", recordToAdd);
        static bool ok;
        ok = table->insertRecord(-1, record); // -1 means 'append'
        if (!ok) {
#ifdef DEBUG_OUTPUT
            qCritical() << "CRITICAL ERROR:" << table->lastError().text();
#endif
        }
        updateRanking();
        updateChecksum();
        updateNValidator();
        break;
    default:
        break;
    }

    setBusy(false);
}

bool RecordsManager::setupDatabase(const QString &filePath)
{
    if (QSqlDatabase::contains("recordsDatabase")) {
        db = QSqlDatabase::database("recordsDatabase", true); // retrieves and opens a db
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "recordsDatabase");
    }

    db.setDatabaseName(filePath);
    if (!db.open()) {
#ifdef DEBUG_OUTPUT
        qCritical() << "Cannot open the database";
#endif
        return false;
    } else {
        setupDatabaseTables();
        setupTable();
        if (!alreadyHasCryptographicTable/* || !alreadyHasStatisticsTable*/) {
            alreadyHasCryptographicTable = true;
//            alreadyHasStatisticsTable    = true;
            updateChecksum();
            updateNValidator();
        }
    }
//    updateStatistics();
    updateRanking();

    return true;
}

bool RecordsManager::setupDatabaseTables()
{
    QSqlQuery query(db);
    // score tables
    for (int i = 0; i < gameTables.count(); ++i) {
        bool ok = query.exec(QString("CREATE TABLE IF NOT EXISTS '%1' ("
                                     "NAME   TEXT NOT NULL,"
                                     "TIME   INTEGER NOT NULL,"
                                     "STEPS  INTEGER NOT NULL,"
                                     "SCORE  INTEGER NOT NULL)").arg(gameTables[i]));
        if (!ok) {
#ifdef DEBUG_OUTPUT
            qCritical() << "CRITICAL ERROR WHILE CREATING TABLE" << gameTables[i] + ':' << query.lastError().text();
#endif
            return false;
        }
    }

    // cryptographic table
    alreadyHasCryptographicTable = db.tables().contains("CRYPTOGRAPHIC");
    bool ok = query.exec("CREATE TABLE IF NOT EXISTS 'CRYPTOGRAPHIC' (CHECKSUM BLOB)");
    if (!ok) {
#ifdef DEBUG_OUTPUT
        qCritical() << "CRITICAL ERROR WHILE CREATING TABLE 'CRYPTOGRAPHIC':"<< query.lastError().text();
#endif
    }

    // statistics table
    /*
    alreadyHasStatisticsTable = db.tables().contains("STATISTICS");
    ok = query.exec("CREATE TABLE IF NOT EXISTS 'STATISTICS' ("
                    "TOTALBEGINNERGAMES INTEGER NOT NULL,"
                    "TOTALMEDIUMGAMES   INTEGER NOT NULL,"
                    "TOTALEXPERTGAMES   INTEGER NOT NULL,"
                    "TOTALCUSTOMGAMES   INTEGER NOT NULL,"
                    "BEGINNERGAMESWIN   INTEGER NOT NULL,"
                    "MEDIUMGAMESWIN     INTEGER NOT NULL,"
                    "EXPERTGAMESWIN     INTEGER NOT NULL,"
                    "CUSTOMGAMESWIN     INTEGER NOT NULL)");
    if (!ok) {
#ifdef DEBUG_OUTPUT
        qCritical() << "CRITICAL ERROR WHILE CREATING TABLE 'STATISTICS':"<< query.lastError().text();
#endif
    }

    if (!alreadyHasStatisticsTable) {
        QSqlQuery query(db);
        if (!query.exec("INSERT INTO 'STATISTICS' VALUES (0,0,0,0,0,0,0,0)")) {
#ifdef DEBUG_OUTPUT
            qCritical() << "CRITICAL ERROR. CANNOT INSERT THE STATISTIC ROW :"<< query.lastError().text();
#endif
        }
    }
    */

    return ok;
}

void RecordsManager::setupTable()
{
    table = new QSqlTableModel(this, db);
    setCurrentTable(boardMode);
}

void RecordsManager::setupRecordTemplate()
{
    recordTemplate.append(QSqlField("NAME",  QVariant::String));
    recordTemplate.append(QSqlField("TIME",  QVariant::Int));
    recordTemplate.append(QSqlField("STEPS", QVariant::Int));
    recordTemplate.append(QSqlField("SCORE", QVariant::Int));
}

//! ADD GAME
/*  I can find a fair way to implement the statistics backend
 *  because I cant' know who user just lose, so I can't count
 *  the losed games for a particular user.
 */
/*bool RecordsManager::addGame(int mode, bool win)
{
    table->setTable("STATISTICS");
    table->select();
    if (table->rowCount() == 0) {
        table->insertRow(0);
    }
    bool ok = true;
    QSqlRecord stat(table->record(0));
    switch (Board::BOARDMODE(mode)) {
    case Board::BEGINNER:
        stat.setValue("TOTALBEGINNERGAMES", totalBeginnerGames + 1);
        if (win) {
            stat.setValue("BEGINNERGAMESWIN", beginnerGamesWin + 1);
        }
        ok = table->setRecord(0, stat);
        if (ok) {
            setTotalBeginnerGames(totalBeginnerGames + 1);
            if (win) {
                setBeginnerGamesWin(beginnerGamesWin + 1);
            }
        }
        break;
    case Board::MEDIUM:
        stat.setValue("TOTALMEDIUMGAMES", totalMediumGames + 1);
        if (win) {
            stat.setValue("MEDIUMGAMESWIN", mediumGamesWin + 1);
        }
        ok = table->setRecord(0, stat);
        if (ok) {
            setTotalMediumGames(totalMediumGames + 1);
            if (win) {
                setMediumGamesWin(mediumGamesWin + 1);
            }
        }
        break;
    case Board::EXPERT:
        stat.setValue("TOTALEXPERTGAMES", totalExpertGames + 1);
        if (win) {
            stat.setValue("EXPERTGAMESWIN", expertGamesWin + 1);
        }
        ok = table->setRecord(0, stat);
        if (ok) {
            setTotalExpertGames(totalExpertGames + 1);
            if (win) {
                setExpertGamesWin(expertGamesWin + 1);
            }
        }
        break;
    case Board::CUSTOM:
        stat.setValue("TOTALCUSTOMGAMES", totalCustomGames + 1);
        if (win) {
            stat.setValue("CUSTOMGAMESWIN", customGamesWin + 1);
        }
        ok = table->setRecord(0, stat);
        if (ok) {
            setTotalCustomGames(totalCustomGames + 1);
            if (win) {
                setCustomGamesWin(customGamesWin + 1);
            }
        }
        break;
    }

    setCurrentTable(boardMode);
    updateChecksum();
    updateNValidator();

    if (!ok) {
#ifdef DEBUG_OUTPUT
        qCritical() << "CRITICAL ERROR: CANNOT SAVE RECORD IN TABLE 'STATISTICS':"<< table->lastError().text();
#endif
    }

    return ok;
}
*/

//! ADD RECORD
bool RecordsManager::addRecord(const QString &playerName, int ms, int steps, int record)
{
    action = ADD;
    playerToAdd = playerName;
    msToAdd = ms;
    stepsToAdd = steps;
    recordToAdd = record;
    start();
    return true;
}

void RecordsManager::setCurrentTable(int mode)
{
    boardMode = Board::BOARDMODE(mode);
    switch (boardMode) {
    case Board::BEGINNER:
        table->setTable("BEGINNER");
        break;
    case Board::MEDIUM:
        table->setTable("MEDIUM");
        break;
    case Board::EXPERT:
        table->setTable("EXPERT");
        break;
    case Board::CUSTOM:
        table->setTable("CUSTOM");
        break;
    }
    table->select();
    updateCurrentMaxRecord();
    emit currentModeChanged(boardMode);
}


//! RANKING
void RecordsManager::updateCurrentMaxRecord()
{
    switch (boardMode) {
    case Board::BEGINNER:
        setMaxRecordMs(QTime::fromString(beginnerTime1, "mm:ss.zzz").msecsSinceStartOfDay());
        break;
    case Board::MEDIUM:
        setMaxRecordMs(QTime::fromString(mediumTime1, "mm:ss.zzz").msecsSinceStartOfDay());
        break;
    case Board::EXPERT:
        setMaxRecordMs(QTime::fromString(expertTime1, "mm:ss.zzz").msecsSinceStartOfDay());
        break;
    case Board::CUSTOM:
        setMaxRecordMs(QTime::fromString(customTime1, "mm:ss.zzz").msecsSinceStartOfDay());
        break;
    }
}

const QByteArray RecordsManager::checksum()
{
    QByteArray data;
    for (int t = 0; t < allTables.count(); ++t) {
        table->setTable(allTables[t]);
        table->select();
        int rows = table->rowCount();
        int cols = table->columnCount();
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                data = SHA3_512(data + table->record(r).value(c).toByteArray() + ".");
            }
        }
    }

    setCurrentTable(boardMode);
    return data;
}

bool RecordsManager::checkChecksum()
{
    table->setTable("CRYPTOGRAPHIC");
    table->select();
    const QByteArray storedChecksum  = table->record(0).value("CHECKSUM").toByteArray();
//    fprintf(stderr, "\n-------------------------CHECK CHECKSUM-------------------------\n"
//                    "[CHK] StoredDB: %s\n", storedChecksum.constData());
//    fflush(stderr);
    setCurrentTable(boardMode);
    const QByteArray currentChecksum = checksum();
//    fprintf(stderr, "[CHK] Calculated: %s\n", currentChecksum.constData());
//    fflush(stderr);
    if (storedChecksum != currentChecksum) {
//        fprintf(stderr, "[CHK] NO Passed\n\n\n");
//        fflush(stderr);
#ifdef DEBUG_OUTPUT
        qCritical() << "Checksum not match: " << storedChecksum << " != " << currentChecksum;
#endif
        return false;
    }
//    fprintf(stderr, "[CHK] Passed\n\n\n");
//    fflush(stderr);
    return true;
}

void RecordsManager::updateChecksum()
{
    const QByteArray chk = checksum();
//    fprintf(stderr, "\n-------------------------UPDATE CHECKSUM-------------------------\n"
//                    "[CHK] CalculatedToSave: %s\n\n\n", chk.constData());
//    fflush(stderr);

    table->setTable("CRYPTOGRAPHIC");
    table->select();
    int rows = table->rowCount();

    QSqlRecord record(table->record());
    record.setValue("CHECKSUM", chk);
    if (rows > 0) {
        table->setRecord(0, record);
    } else {
        bool ok;
        ok = table->insertRecord(0, record);
        if (!ok) {
#ifdef DEBUG_OUTPUT
            qCritical() << "CRITICAL ERROR: [UPDATE CHECKSUM]" << table->lastError().text();
#endif
        }
    }
    setCurrentTable(boardMode);
}

const QByteArray RecordsManager::nValidator()
{
    table->setTable("CRYPTOGRAPHIC");
    table->select();
    QByteArray stored = table->record(1).value("CHECKSUM").toByteArray();
    setCurrentTable(boardMode);
    return stored;
}

bool RecordsManager::checkNValidator()
{
    QByteArray hashStored = nValidator();
//    fprintf(stderr, "\n-------------------------CHECK N-------------------------\n"
//                    "[N] StoredDB: %s\n", hashStored.constData());
//    fflush(stderr);
    QSettings s;
    QByteArray n = s.value("n").toByteArray();
//    fprintf(stderr, "[N] StoredReg: %s\n", n.constData());
//    fflush(stderr);
    QByteArray hash = SHA3_512(n);
//    fprintf(stderr, "[N] HashReg: %s\n", hash.constData());
//    fflush(stderr);
    if (hashStored != hash) {
//        fprintf(stderr, "[N] NO Passed\n\n\n");
//        fflush(stderr);
#ifdef DEBUG_OUTPUT
        qCritical() << "NValidation not match: " << hashStored << " != " << hash;
#endif
        return false;
    }
//    fprintf(stderr, "[N] Passed\n\n\n");
//    fflush(stderr);
    return true;
}

void RecordsManager::updateNValidator()
{
    unsigned long long n = QRandomGenerator::system()->generate64();
//    fprintf(stderr, "\n-------------------------UPDATE N-------------------------\n"
//                    "[N] Generated: %llu\n", n);
//    fflush(stderr);
    QSettings s;
    QByteArray nStr = QByteArray::number(n);
//    fprintf(stderr, "[N] Converted: %s\n", nStr.constData());
//    fflush(stderr);
    s.setValue("n", nStr);
    QByteArray hash = SHA3_512(nStr);
//    fprintf(stderr, "[N] HashToBD: %s\n\n\n", hash.constData());
//    fflush(stderr);

    table->setTable("CRYPTOGRAPHIC");
    table->select();
    int rows = table->rowCount();

    QSqlRecord record(table->record());
    record.setValue("CHECKSUM", hash);
    if (rows > 1) {
        table->setRecord(1, record);
    } else {
        bool ok;
        ok = table->insertRecord(1, record);
        if (!ok) {
#ifdef DEBUG_OUTPUT
            qCritical() << "CRITICAL ERROR: [UPDATE NVALIDATOR]" << table->lastError().text();
#endif
        }
    }

    setCurrentTable(boardMode);
}

void RecordsManager::reset()
{
    action = RESET;
    start();
}

int RecordsManager::getCustomGamesWin() const
{
    return customGamesWin;
}

void RecordsManager::setCustomGamesWin(int value)
{
    if (customGamesWin != value) {
        customGamesWin = value;
        emit customGamesWinChanged();
    }
}

int RecordsManager::getExpertGamesWin() const
{
    return expertGamesWin;
}

void RecordsManager::setExpertGamesWin(int value)
{
    if (expertGamesWin != value) {
        expertGamesWin = value;
        emit expertGamesWinChanged();
    }
}

int RecordsManager::getMediumGamesWin() const
{
    return mediumGamesWin;
}

void RecordsManager::setMediumGamesWin(int value)
{
    if (mediumGamesWin != value) {
        mediumGamesWin = value;
        emit mediumGamesWinChanged();
    }
}

int RecordsManager::getBeginnerGamesWin() const
{
    return beginnerGamesWin;
}

void RecordsManager::setBeginnerGamesWin(int value)
{
    if (beginnerGamesWin != value) {
        beginnerGamesWin = value;
        emit beginnerGamesWinChanged();
    }
}

int RecordsManager::getTotalCustomGames() const
{
    return totalCustomGames;
}

void RecordsManager::setTotalCustomGames(int value)
{
    if (totalCustomGames != value) {
        totalCustomGames = value;
        emit totalCustomGamesChanged();
    }
}

int RecordsManager::getTotalExpertGames() const
{
    return totalExpertGames;
}

void RecordsManager::setTotalExpertGames(int value)
{
    if (totalExpertGames != value) {
        totalExpertGames = value;
        emit totalExpertGamesChanged();
    }
}

int RecordsManager::getTotalMediumGames() const
{
    return totalMediumGames;
}

void RecordsManager::setTotalMediumGames(int value)
{
    if (totalMediumGames != value) {
        totalMediumGames = value;
        emit totalMediumGamesChanged();
    }
}

int RecordsManager::getTotalBeginnerGames() const
{
    return totalBeginnerGames;
}

void RecordsManager::setTotalBeginnerGames(int value)
{
    if (totalBeginnerGames != value) {
        totalBeginnerGames = value;
        emit totalBeginnerGamesChanged();
    }
}

/*  I can find a fair way to implement the statistics backend
 *  because I cant' know who user just lose, so I can't count
 *  the losed games for a particular user.
 */
/*void RecordsManager::updateStatistics()
{
    table->setTable("STATISTICS");
    table->select();

    QSqlRecord stat(table->record(0));
    setTotalBeginnerGames(stat.value("TOTALBEGINNERGAMES").toInt());
    setTotalMediumGames(stat.value("TOTALMEDIUMGAMES").toInt());
    setTotalExpertGames(stat.value("TOTALEXPERTGAMES").toInt());
    setTotalCustomGames(stat.value("TOTALCUSTOMGAMES").toInt());

    setBeginnerGamesWin(stat.value("BEGINNERGAMESWIN").toInt());
    setBeginnerGamesWin(stat.value("MEDIUMGAMESWIN").toInt());
    setBeginnerGamesWin(stat.value("EXPERTGAMESWIN").toInt());
    setBeginnerGamesWin(stat.value("CUSTOMGAMESWIN").toInt());

    setCurrentTable(boardMode);
}
*/

void RecordsManager::updateRanking()
{
    updateRankingBeginner();
    updateRankingMedium();
    updateRankingExpert();
    updateRankingCustom();

    emit rankingUpdated();
    setCurrentTable(int(boardMode));
}

void RecordsManager::updateRankingBeginner()
{
    table->setTable("BEGINNER");
    table->select();
    table->sort(1, Qt::AscendingOrder);

    if (table->rowCount() > 0) {
        setEmpty(false);
    }

    if (table->rowCount() > maxRecordsAllowed) {
        table->removeRow(maxRecordsAllowed);
    }

    beginnerName1  = table->record(0).value(0).toString();
    beginnerName2  = table->record(1).value(0).toString();
    beginnerName3  = table->record(2).value(0).toString();
    beginnerName4  = table->record(3).value(0).toString();
    beginnerName5  = table->record(4).value(0).toString();
    beginnerName6  = table->record(5).value(0).toString();
    beginnerName7  = table->record(6).value(0).toString();
    beginnerName8  = table->record(7).value(0).toString();
    beginnerName9  = table->record(8).value(0).toString();
    beginnerName10 = table->record(9).value(0).toString();

    QTime t(0, 0, 0, 0);
    beginnerTime1  = t.addMSecs(table->record(0).value(1).toInt()).toString("mm:ss.zzz");
    beginnerTime2  = t.addMSecs(table->record(1).value(1).toInt()).toString("mm:ss.zzz");
    beginnerTime3  = t.addMSecs(table->record(2).value(1).toInt()).toString("mm:ss.zzz");
    beginnerTime4  = t.addMSecs(table->record(3).value(1).toInt()).toString("mm:ss.zzz");
    beginnerTime5  = t.addMSecs(table->record(4).value(1).toInt()).toString("mm:ss.zzz");
    beginnerTime6  = t.addMSecs(table->record(5).value(1).toInt()).toString("mm:ss.zzz");
    beginnerTime7  = t.addMSecs(table->record(6).value(1).toInt()).toString("mm:ss.zzz");
    beginnerTime8  = t.addMSecs(table->record(7).value(1).toInt()).toString("mm:ss.zzz");
    beginnerTime9  = t.addMSecs(table->record(8).value(1).toInt()).toString("mm:ss.zzz");
    beginnerTime10 = t.addMSecs(table->record(9).value(1).toInt()).toString("mm:ss.zzz");
}

void RecordsManager::updateRankingMedium()
{
    table->setTable("MEDIUM");
    table->select();
    table->sort(1, Qt::AscendingOrder);

    if (table->rowCount() > 0) {
        setEmpty(false);
    }

    if (table->rowCount() > maxRecordsAllowed) {
        table->removeRow(maxRecordsAllowed);
    }

    mediumName1  = table->record(0).value(0).toString();
    mediumName2  = table->record(1).value(0).toString();
    mediumName3  = table->record(2).value(0).toString();
    mediumName4  = table->record(3).value(0).toString();
    mediumName5  = table->record(4).value(0).toString();
    mediumName6  = table->record(5).value(0).toString();
    mediumName7  = table->record(6).value(0).toString();
    mediumName8  = table->record(7).value(0).toString();
    mediumName9  = table->record(8).value(0).toString();
    mediumName10 = table->record(9).value(0).toString();

    QTime t(0, 0, 0, 0);
    mediumTime1  = t.addMSecs(table->record(0).value(1).toInt()).toString("mm:ss.zzz");
    mediumTime2  = t.addMSecs(table->record(1).value(1).toInt()).toString("mm:ss.zzz");
    mediumTime3  = t.addMSecs(table->record(2).value(1).toInt()).toString("mm:ss.zzz");
    mediumTime4  = t.addMSecs(table->record(3).value(1).toInt()).toString("mm:ss.zzz");
    mediumTime5  = t.addMSecs(table->record(4).value(1).toInt()).toString("mm:ss.zzz");
    mediumTime6  = t.addMSecs(table->record(5).value(1).toInt()).toString("mm:ss.zzz");
    mediumTime7  = t.addMSecs(table->record(6).value(1).toInt()).toString("mm:ss.zzz");
    mediumTime8  = t.addMSecs(table->record(7).value(1).toInt()).toString("mm:ss.zzz");
    mediumTime9  = t.addMSecs(table->record(8).value(1).toInt()).toString("mm:ss.zzz");
    mediumTime10 = t.addMSecs(table->record(9).value(1).toInt()).toString("mm:ss.zzz");
}

void RecordsManager::updateRankingExpert()
{
    table->setTable("EXPERT");
    table->select();
    table->sort(1, Qt::AscendingOrder);

    if (table->rowCount() > 0) {
        setEmpty(false);
    }

    if (table->rowCount() > maxRecordsAllowed) {
        table->removeRow(maxRecordsAllowed);
    }

    expertName1  = table->record(0).value(0).toString();
    expertName2  = table->record(1).value(0).toString();
    expertName3  = table->record(2).value(0).toString();
    expertName4  = table->record(3).value(0).toString();
    expertName5  = table->record(4).value(0).toString();
    expertName6  = table->record(5).value(0).toString();
    expertName7  = table->record(6).value(0).toString();
    expertName8  = table->record(7).value(0).toString();
    expertName9  = table->record(8).value(0).toString();
    expertName10 = table->record(9).value(0).toString();

    QTime t(0, 0, 0, 0);
    expertTime1  = t.addMSecs(table->record(0).value(1).toInt()).toString("mm:ss.zzz");
    expertTime2  = t.addMSecs(table->record(1).value(1).toInt()).toString("mm:ss.zzz");
    expertTime3  = t.addMSecs(table->record(2).value(1).toInt()).toString("mm:ss.zzz");
    expertTime4  = t.addMSecs(table->record(3).value(1).toInt()).toString("mm:ss.zzz");
    expertTime5  = t.addMSecs(table->record(4).value(1).toInt()).toString("mm:ss.zzz");
    expertTime6  = t.addMSecs(table->record(5).value(1).toInt()).toString("mm:ss.zzz");
    expertTime7  = t.addMSecs(table->record(6).value(1).toInt()).toString("mm:ss.zzz");
    expertTime8  = t.addMSecs(table->record(7).value(1).toInt()).toString("mm:ss.zzz");
    expertTime9  = t.addMSecs(table->record(8).value(1).toInt()).toString("mm:ss.zzz");
    expertTime10 = t.addMSecs(table->record(9).value(1).toInt()).toString("mm:ss.zzz");
}

void RecordsManager::updateRankingCustom()
{
    table->setTable("CUSTOM");
    table->select();
    table->sort(1, Qt::AscendingOrder);

    if (table->rowCount() > 0) {
        setEmpty(false);
    }

    if (table->rowCount() > maxRecordsAllowed) {
        table->removeRow(maxRecordsAllowed);
    }

    customName1  = table->record(0).value(0).toString();
    customName2  = table->record(1).value(0).toString();
    customName3  = table->record(2).value(0).toString();
    customName4  = table->record(3).value(0).toString();
    customName5  = table->record(4).value(0).toString();
    customName6  = table->record(5).value(0).toString();
    customName7  = table->record(6).value(0).toString();
    customName8  = table->record(7).value(0).toString();
    customName9  = table->record(8).value(0).toString();
    customName10 = table->record(9).value(0).toString();

    QTime t(0, 0, 0, 0);
    customTime1  = t.addMSecs(table->record(0).value(1).toInt()).toString("mm:ss.zzz");
    customTime2  = t.addMSecs(table->record(1).value(1).toInt()).toString("mm:ss.zzz");
    customTime3  = t.addMSecs(table->record(2).value(1).toInt()).toString("mm:ss.zzz");
    customTime4  = t.addMSecs(table->record(3).value(1).toInt()).toString("mm:ss.zzz");
    customTime5  = t.addMSecs(table->record(4).value(1).toInt()).toString("mm:ss.zzz");
    customTime6  = t.addMSecs(table->record(5).value(1).toInt()).toString("mm:ss.zzz");
    customTime7  = t.addMSecs(table->record(6).value(1).toInt()).toString("mm:ss.zzz");
    customTime8  = t.addMSecs(table->record(7).value(1).toInt()).toString("mm:ss.zzz");
    customTime9  = t.addMSecs(table->record(8).value(1).toInt()).toString("mm:ss.zzz");
    customTime10 = t.addMSecs(table->record(9).value(1).toInt()).toString("mm:ss.zzz");
}

int RecordsManager::getMaxRecordsAllowed() const
{
    return maxRecordsAllowed;
}

void RecordsManager::setMaxRecordsAllowed(int value)
{
    if (maxRecordsAllowed != value) {
        maxRecordsAllowed = value;
        emit maxRecordsAllowedChanged(maxRecordsAllowed);
    }
}

QString RecordsManager::getMaxRecordString() const
{
    return QTime(0, 0, 0, 0).addMSecs(maxRecordMs).toString("mm:ss.zzz");
}

int RecordsManager::getMaxRecordMs() const
{
    return maxRecordMs;
}

void RecordsManager::setMaxRecordMs(int value)
{
    if (value == 0) {
        value = 3477519;
    }
    if (maxRecordMs != value) {
        maxRecordMs = value;
        emit maxRecordMsChanged(maxRecordMs);
        emit maxRecordStringChanged(getMaxRecordString());
    }
}

bool RecordsManager::getBusy() const
{
    return busy;
}

void RecordsManager::setBusy(bool value)
{
    if (busy != value) {
        busy = value;
        emit busyChanged(busy);
    }
}

bool RecordsManager::getEmpty() const
{
    return empty;
}

void RecordsManager::setEmpty(bool value)
{
    if (empty != value) {
        empty = value;
        emit emptyChanged(empty);
    }
}


//! ACCESS DESCRIPTORS
Board::BOARDMODE RecordsManager::getBoardMode() const
{
    return boardMode;
}

QString RecordsManager::getCustomTime10() const
{
    return customTime10;
}

QString RecordsManager::getCustomTime9() const
{
    return customTime9;
}

QString RecordsManager::getCustomTime8() const
{
    return customTime8;
}

QString RecordsManager::getCustomTime7() const
{
    return customTime7;
}

QString RecordsManager::getCustomTime6() const
{
    return customTime6;
}

QString RecordsManager::getCustomTime5() const
{
    return customTime5;
}

QString RecordsManager::getCustomTime4() const
{
    return customTime4;
}

QString RecordsManager::getCustomTime3() const
{
    return customTime3;
}

QString RecordsManager::getCustomTime2() const
{
    return customTime2;
}

QString RecordsManager::getCustomTime1() const
{
    return customTime1;
}

QString RecordsManager::getExpertTime10() const
{
    return expertTime10;
}

QString RecordsManager::getExpertTime9() const
{
    return expertTime9;
}

QString RecordsManager::getExpertTime8() const
{
    return expertTime8;
}

QString RecordsManager::getExpertTime7() const
{
    return expertTime7;
}

QString RecordsManager::getExpertTime6() const
{
    return expertTime6;
}

QString RecordsManager::getExpertTime5() const
{
    return expertTime5;
}

QString RecordsManager::getExpertTime4() const
{
    return expertTime4;
}

QString RecordsManager::getExpertTime3() const
{
    return expertTime3;
}

QString RecordsManager::getExpertTime2() const
{
    return expertTime2;
}

QString RecordsManager::getExpertTime1() const
{
    return expertTime1;
}

QString RecordsManager::getMediumTime10() const
{
    return mediumTime10;
}

QString RecordsManager::getMediumTime9() const
{
    return mediumTime9;
}

QString RecordsManager::getMediumTime8() const
{
    return mediumTime8;
}

QString RecordsManager::getMediumTime7() const
{
    return mediumTime7;
}

QString RecordsManager::getMediumTime6() const
{
    return mediumTime6;
}

QString RecordsManager::getMediumTime5() const
{
    return mediumTime5;
}

QString RecordsManager::getMediumTime4() const
{
    return mediumTime4;
}

QString RecordsManager::getMediumTime3() const
{
    return mediumTime3;
}

QString RecordsManager::getMediumTime2() const
{
    return mediumTime2;
}

QString RecordsManager::getMediumTime1() const
{
    return mediumTime1;
}

QString RecordsManager::getBeginnerTime10() const
{
    return beginnerTime10;
}

QString RecordsManager::getBeginnerTime9() const
{
    return beginnerTime9;
}

QString RecordsManager::getBeginnerTime8() const
{
    return beginnerTime8;
}

QString RecordsManager::getBeginnerTime7() const
{
    return beginnerTime7;
}

QString RecordsManager::getBeginnerTime6() const
{
    return beginnerTime6;
}

QString RecordsManager::getBeginnerTime5() const
{
    return beginnerTime5;
}

QString RecordsManager::getBeginnerTime4() const
{
    return beginnerTime4;
}

QString RecordsManager::getBeginnerTime3() const
{
    return beginnerTime3;
}

QString RecordsManager::getBeginnerTime2() const
{
    return beginnerTime2;
}

QString RecordsManager::getBeginnerTime1() const
{
    return beginnerTime1;
}

QString RecordsManager::getCustomName10() const
{
    return customName10;
}

QString RecordsManager::getCustomName9() const
{
    return customName9;
}

QString RecordsManager::getCustomName8() const
{
    return customName8;
}

QString RecordsManager::getCustomName7() const
{
    return customName7;
}

QString RecordsManager::getCustomName6() const
{
    return customName6;
}

QString RecordsManager::getCustomName5() const
{
    return customName5;
}

QString RecordsManager::getCustomName4() const
{
    return customName4;
}

QString RecordsManager::getCustomName3() const
{
    return customName3;
}

QString RecordsManager::getCustomName2() const
{
    return customName2;
}

QString RecordsManager::getCustomName1() const
{
    return customName1;
}

QString RecordsManager::getExpertName10() const
{
    return expertName10;
}

QString RecordsManager::getExpertName9() const
{
    return expertName9;
}

QString RecordsManager::getExpertName8() const
{
    return expertName8;
}

QString RecordsManager::getExpertName7() const
{
    return expertName7;
}

QString RecordsManager::getExpertName6() const
{
    return expertName6;
}

QString RecordsManager::getExpertName5() const
{
    return expertName5;
}

QString RecordsManager::getExpertName4() const
{
    return expertName4;
}

QString RecordsManager::getExpertName3() const
{
    return expertName3;
}

QString RecordsManager::getExpertName2() const
{
    return expertName2;
}

QString RecordsManager::getExpertName1() const
{
    return expertName1;
}

QString RecordsManager::getMediumName10() const
{
    return mediumName10;
}

QString RecordsManager::getMediumName9() const
{
    return mediumName9;
}

QString RecordsManager::getMediumName8() const
{
    return mediumName8;
}

QString RecordsManager::getMediumName7() const
{
    return mediumName7;
}

QString RecordsManager::getMediumName6() const
{
    return mediumName6;
}

QString RecordsManager::getMediumName5() const
{
    return mediumName5;
}

QString RecordsManager::getMediumName4() const
{
    return mediumName4;
}

QString RecordsManager::getMediumName3() const
{
    return mediumName3;
}

QString RecordsManager::getMediumName2() const
{
    return mediumName2;
}

QString RecordsManager::getMediumName1() const
{
    return mediumName1;
}

QString RecordsManager::getBeginnerName10() const
{
    return beginnerName10;
}

QString RecordsManager::getBeginnerName9() const
{
    return beginnerName9;
}

QString RecordsManager::getBeginnerName8() const
{
    return beginnerName8;
}

QString RecordsManager::getBeginnerName7() const
{
    return beginnerName7;
}

QString RecordsManager::getBeginnerName6() const
{
    return beginnerName6;
}

QString RecordsManager::getBeginnerName5() const
{
    return beginnerName5;
}

QString RecordsManager::getBeginnerName4() const
{
    return beginnerName4;
}

QString RecordsManager::getBeginnerName3() const
{
    return beginnerName3;
}

QString RecordsManager::getBeginnerName2() const
{
    return beginnerName2;
}

QString RecordsManager::getBeginnerName1() const
{
    return beginnerName1;
}
