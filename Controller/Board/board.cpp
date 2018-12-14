#include "board.h"

#include <QDebug>

Board::Board(QObject *parent) : QObject(parent)
{
    // in-class initialization
}

Board::Board(const Board &other) : QObject(other.parent())
{
    *this = other;
}

Board &Board::operator=(const Board &other)
{
    if (&other != this) {
        mode = other.mode;
        numberOfRows    = other.beginnerRows;
        numberOfColumns = other.beginnerColumns;
        numberOfMines   = other.beginnerMines;

        for (int r = 0; r < maxRows; ++r) {
            for (int c = 0; c < maxRows; ++c) {
                cells[r][c] = other.cells[r][c];
            }
        }

        activatedCells   = other.activatedCells;

        firstClick       = other.firstClick;
        firstClickRow    = other.firstClickRow;
        firstClickColumn = other.firstClickColumn;
    }

    return *this;
}

Board::BOARDMODE Board::getMode() const
{
    return mode;
}

void Board::setMode(const BOARDMODE &value, int customRows, int customColumns, int customMines)
{
    if (mode != value || value == Board::CUSTOM) {
        mode = value;
        switch (mode) {
        case BEGINNER:
            setNumberOfRows(beginnerRows);
            setNumberOfColumns(beginnerColumns);
            setNumberOfMines(beginnerMines);
            break;
        case MEDIUM:
            setNumberOfRows(mediumRows);
            setNumberOfColumns(mediumColumns);
            setNumberOfMines(mediumMines);
            break;
        case EXPERT:
            setNumberOfRows(expertRows);
            setNumberOfColumns(expertColumns);
            setNumberOfMines(expertMines);
            break;
        case CUSTOM:
            if (customRows != -1) {
                setCustomGameRows(customRows);
            }
            if (customColumns != -1) {
                setCustomGameColumns(customColumns);
            }
            if (customMines != -1) {
                setCustomGameMines(customMines);
            }
            setNumberOfRows(customRows == -1 ? customGameRows : customRows);
            setNumberOfColumns(customColumns == -1 ? customGameColumns : customColumns);
            setNumberOfMines(customMines == -1 ? customGameMines : customMines);
            break;
        }
        emit modeChanged(int(mode));
    }
    newGame();
}

int Board::getNumberOfRows() const
{
    return numberOfRows;
}

void Board::setNumberOfRows(int value)
{
    if (numberOfRows != value) {
        if (value < 5 || value > maxCols) {
            qCritical("Cannot set that amounts of rows. Allowed values are from 5 to %d.", maxRows);
        } else {
            numberOfRows = value;
            emit numberOfRowsChanged(numberOfRows);
        }
    }
}

int Board::getNumberOfColumns() const
{
    return numberOfColumns;
}

void Board::setNumberOfColumns(int value)
{
    if (numberOfColumns != value) {
        if (value < 5 || value > maxCols) {
            qCritical("Cannot set that amounts of columns. Allowed values are from 5 to %d.", maxCols);
        } else {
            numberOfColumns = value;
            emit numberOfColumnsChanged(numberOfColumns);
        }
    }
}

int Board::getNumberOfMines() const
{
    return numberOfMines;
}

void Board::setNumberOfMines(int value)
{
    if (numberOfMines != value) {
        if (value > (numberOfRows*numberOfColumns - 10)) {
            qCritical("Cannot set that amounts of mines. Maximum allowed is %d.", numberOfRows*numberOfColumns - 10);
        } else {
            numberOfMines = value;
            emit numberOfMinesChanged(numberOfMines);
        }
    }
}

Cell& Board::operator[](int index)
{
    int row = index / maxCols;
    int col = index % maxCols;
    return cells[row][col];
}

Cell Board::operator[](int index) const
{
    int row = index / numberOfColumns;
    int col = index % numberOfColumns;
    return cells[row][col];
}

Cell* Board::itemAt(int index)
{
    Cell *item = &(*this)[index];
    return item;
}

void Board::setup()
{
#ifdef DEBUG_OUTPUT
    qDebug("Setting up the board...");
#endif
    // reset all cells
    for (int row = 0; row < maxRows; ++row) {
        for (int col = 0; col < maxCols; ++col) {
            cells[row][col].reset();
            cells[row][col].disconnect();
        }
    }

    for (int row = 0; row < numberOfRows; ++row) {
        for (int col = 0; col < numberOfColumns; ++col) {
            cells[row][col].setRow(row);
            cells[row][col].setColumn(col);
            connect( &cells[row][col], &Cell::leftClicked, this, &Board::leftClick );
            connect( &cells[row][col], &Cell::rightClicked, this, &Board::rightClick );
            connect( &cells[row][col], &Cell::leftRightClicked, this, &Board::revealSiblings );
            connect( &cells[row][col], &Cell::leftRightUnclicked, this, &Board::unrevealSiblings );
            connect( &cells[row][col], &Cell::revealSiblingsTriggered, this, &Board::tryToRevealSiblings );
        }
    }
    firstClick = false;
    setRunning(false);
    firstClickRow = firstClickColumn = -1;
    activatedCells = 0;
    setMinesDiscovered(0);
#ifdef DEBUG_OUTPUT
    qDebug("Finished board setup.");
    qDebug("ROWS: %d | COLS: %d | MINES: %d", numberOfRows, numberOfColumns, numberOfMines);
#endif
    // update the UI
    setUiRows(numberOfRows);
    setUiCols(numberOfColumns);
    updateProgress();
}

void Board::newGame()
{
    setup();
}

void Board::leftClick(int row, int column)
{
    if (!running && firstClick) { // disable except when not click done yet
#ifdef DEBUG_OUTPUT
        qDebug("Not running, left-click ignored");
#endif
        return; // give up
    } else if (cells[row][column].getActivated()) {
#ifdef DEBUG_OUTPUT
        qDebug("Cell at (%d, %d) already activated, left-click ignored", row, column);
#endif
    } else if (cells[row][column].getFlagged()) {
#ifdef DEBUG_OUTPUT
        qDebug("Cell at (%d, %d) flagged, left-click ignored", row, column);
#endif
        return; // give up
    } else if (cells[row][column].getFlaggedUnknown()) {
#ifdef DEBUG_OUTPUT
        qDebug("Cell at (%d, %d) has flagged as unknown [?], left-click ignored", row, column);
#endif
        return; // give up
    }

#ifdef DEBUG_OUTPUT
    qDebug("Left-Clicked at (%d, %d)", row, column);
#endif
    if (row < 0 || row > numberOfRows) {
        qCritical("Critical error: The specified row in ([%d], %d) is out of range (maximum allowed is %d).", row, column, numberOfRows);
    } else if (column < 0 || column > numberOfColumns) {
        qCritical("Critical error: The specified column in (%d, [%d]) is out of range (maximum allowed is %d).", row, column, numberOfColumns);
    } else if (cells[row][column].getActivated()) {
        qInfo("The cell in (%d, %d) was already clicked before.", row, column);
    } else { // no input errors
        if (!firstClick) {
            firstClick = true;
            firstClickRow = row;
            firstClickColumn = column;
#ifdef DEBUG_OUTPUT
            qDebug("First click at (%d, %d)", row, column);
#endif
            fillBoard();
            emit start();
            setRunning(true);
            emit bigUnlock();
        }

        cells[row][column].setActivated(true);
        if (cells[row][column].getContent() == Cell::MINE) { // lose by hitting a mine!!!
            lose(row, column);
            cells[row][column].setExploted(true);
        } else if (cells[row][column].getContent() == Cell::BLANK) {
#ifdef DEBUG_OUTPUT
            qDebug("Unlocking cells adjacents to (%d, %d)", row, column);
#endif
            int unlockedCells = unlockAdjacentCells(row, column);
            activatedCells += unlockedCells;
            if (unlockedCells > 1) {
                emit bigUnlock();
            }
#ifdef DEBUG_OUTPUT
            qDebug("Finished unlocking.");
#endif
        } else {
            ++activatedCells; // just activate the cell
        }
        if (activatedCells + numberOfMines >= numberOfRows*numberOfColumns) { //! WIN!!!
            win();
        }
    }
    updateProgress();
#ifdef DEBUG_OUTPUT
    qDebug("Finished click handling.");
#endif
}

void Board::rightClick(int row, int column)
{
    if (!firstClick) {
#ifdef DEBUG_OUTPUT
        qDebug("No first left-click done yet, right-click ignored");
#endif
        return; // give up
    } else if (!running) {
#ifdef DEBUG_OUTPUT
        qDebug("Not running, left-click ignored");
#endif
        return; // give up
    } else if (cells[row][column].getActivated()) {
#ifdef DEBUG_OUTPUT
        qDebug("Cell at (%d, %d) active, right-click ignored", row, column);
#endif
        return; // give up
    }

#ifdef DEBUG_OUTPUT
    qDebug("Right-Clicked at (%d, %d)", row, column);
#endif
    if (!cells[row][column].getFlagged() && !cells[row][column].getFlaggedUnknown()) {
        if (minesDiscovered < numberOfMines) {
            cells[row][column].setFlagged(true);
            setMinesDiscovered(minesDiscovered + 1);
        } else {
            emit flaggingDenied();
            cells[row][column].setFlaggedUnknown(true);
        }
    } else if (cells[row][column].getFlagged()) {
        cells[row][column].setFlagged(false);
        setMinesDiscovered(minesDiscovered - 1);
        cells[row][column].setFlaggedUnknown(true);
    } else {
        cells[row][column].setFlaggedUnknown(false);
    }
    updateProgress();
}

void Board::revealBoard(bool keepFlags)
{
    for (int r = 0; r < numberOfRows; ++r) {
        for (int c = 0; c < numberOfColumns; ++c) {
            if (!cells[r][c].getActivated() && (!keepFlags || !cells[r][c].getFlagged())) {
                cells[r][c].setActivated(true);
            }
        }
    }
}

double Board::getProgress() const
{
    return progress;
}

void Board::updateProgress()
{
    double total = numberOfRows*numberOfColumns;
    double discovered = activatedCells + minesDiscovered;
    progress = discovered/total;
    emit progressChanged(progress);
}

bool Board::getFirstClickClear() const
{
    return firstClickClear;
}

void Board::setFirstClickClear(bool value)
{
    if (firstClickClear != value) {
        firstClickClear = value;
        emit firstClickClearChanged(firstClickClear);
    }
}

double Board::getUiBoardLoadingProgress() const
{
    return uiBoardLoadingProgress;
}

void Board::setUiBoardLoadingProgress(double value)
{
    if (!(uiBoardLoadingProgress < value) && !(uiBoardLoadingProgress > value)) {
        uiBoardLoadingProgress = value;
        emit uiBoardLoadingProgressChanged(uiBoardLoadingProgress);
    }
}

int Board::getMinesDiscovered() const
{
    return minesDiscovered;
}

void Board::setMinesDiscovered(int value)
{
    if (minesDiscovered != value) {
        minesDiscovered = value;
        emit minesDiscoveredChanged(minesDiscovered);
    }
}

int Board::getCustomGameMines() const
{
    return customGameMines;
}

void Board::setCustomGameMines(int value)
{
    if (customGameMines != value) {
        customGameMines = value;
        emit customGameMinesChanged(customGameMines);
    }
}

int Board::getCustomGameColumns() const
{
    return customGameColumns;
}

void Board::setCustomGameColumns(int value)
{
    if (customGameColumns != value) {
        customGameColumns = value;
        emit customGameColumnsChanged(customGameColumns);
    }
}

int Board::getCustomGameRows() const
{
    return customGameRows;
}

void Board::setCustomGameRows(int value)
{
    if (customGameRows != value) {
        customGameRows = value;
        emit customGameRowsChanged(customGameRows);
    }
}

int Board::getUiCols() const
{
    return uiCols;
}

void Board::setUiCols(int value)
{
    if (uiCols != value) {
        uiCols = value;
        emit uiColsChanged(uiCols);
    }
}

int Board::getUiRows() const
{
    return uiRows;
}

void Board::setUiRows(int value)
{
    if (uiRows != value) {
        uiRows = value;
        emit uiRowsChanged(uiRows);
    }
}

bool Board::getRunning() const
{
    return running;
}

void Board::setRunning(bool value)
{
    if (running != value) {
        running = value;
        emit runningChanged(running);
    }
}

//! PRIVATE USAGE (NOT EXPOSE TO QML)
// when the user made his first click, we fill the board to ensure that click is not a mine
void Board::fillBoard()
{
#ifdef DEBUG_OUTPUT
    qDebug("Filling board...");
#endif
    int randomRow = 0, randomColumn = 0;
    const int adjacentRows[9]    = { -1, -1, -1,  0,  0,  0,  1,  1,  1 };
    const int adjacentColumns[9] = { -1,  0,  1, -1,  0,  1, -1,  0,  1 };
    bool adjacentCell = true;

    for (int iteratorMines = 0; iteratorMines < numberOfMines; ++iteratorMines) {
        do {
            randomRow    = (QRandomGenerator::system()->generate() & 0x7FFFFFFF) % numberOfRows;
            randomColumn = (QRandomGenerator::system()->generate() & 0x7FFFFFFF) % numberOfColumns;
            for (int iteratorFirstClickCheck = 0; iteratorFirstClickCheck < 9; ++iteratorFirstClickCheck) {
                int rowCheck    = firstClickRow + adjacentRows[iteratorFirstClickCheck];
                int columnCheck = firstClickColumn + adjacentColumns[iteratorFirstClickCheck];
                adjacentCell = (randomRow == rowCheck && randomColumn == columnCheck);
                if (adjacentCell) {
                    break;
                }
            }
        } while ((adjacentCell && firstClickClear) || cells[randomRow][randomColumn].getContent() == Cell::MINE);

        // mine this spot
        cells[randomRow][randomColumn].setContent(Cell::MINE);
    }

    solveBoard(); // how many mines are around each cell?
#ifdef DEBUG_OUTPUT
    qDebug("Finished board filling.");
#endif
}

// set the values of each cell
void Board::solveBoard()
{
#ifdef DEBUG_OUTPUT
    qDebug("Solving board...");
#endif
    for (int r = 0; r < numberOfRows; ++r) {
        for (int c = 0; c < numberOfColumns; ++c) {
            if (cells[r][c].getContent() != Cell::MINE) {
                int count = countSurroundingMines(r, c);
                if (count) {
                    cells[r][c].setValue(count);
                    cells[r][c].setContent(Cell::VALUE);
                }
            }
        }
    }
#ifdef DEBUG_OUTPUT
    qDebug("Finished board solving.");
#endif
}

int Board::countSurroundingMines(int row, int column)
{
    const int adjacentRows[8]    = { -1, -1, -1,  0,  0,  1,  1,  1 };
    const int adjacentColumns[8] = { -1,  0,  1, -1,  1, -1,  0,  1 };

    int minesCount = 0;

    int i;
    for (i = 0; i < 8; ++i) {
        int rowToCheck    = row + adjacentRows[i];
        int columnToCheck = column + adjacentColumns[i];
        bool isValid = (rowToCheck >= 0 && rowToCheck < numberOfRows) && (columnToCheck >= 0 && columnToCheck < numberOfColumns);
        if (isValid && cells[rowToCheck][columnToCheck].getContent() == Cell::MINE) {
            ++minesCount;
        }
    }
    return minesCount;
}

// unlock recursively all adjacent cells and return the number of unlocked cells
int Board::unlockAdjacentCells(int row, int column)
{
    int totalUnlocked = 1;
    const int adjacentRows[8]    = { -1, -1, -1,  0,  0,  1,  1,  1 };
    const int adjacentColumns[8] = { -1,  0,  1, -1,  1, -1,  0,  1 };

    cells[row][column].setActivated(true);

    for (int i = 0; i < 8; ++i) {
        int newRow = row + adjacentRows[i];
        int newColumn = column + adjacentColumns[i];
        bool isValid = (newRow >= 0 && newRow < numberOfRows) && (newColumn >= 0 && newColumn < numberOfColumns);
        if (isValid && cells[row][column].getContent() == Cell::BLANK && cells[newRow][newColumn].getContent() != Cell::MINE && !cells[newRow][newColumn].getActivated() && !cells[newRow][newColumn].getFlagged() && !cells[newRow][newColumn].getFlaggedUnknown()) {
            totalUnlocked += unlockAdjacentCells(newRow, newColumn);
        }
    }
    return totalUnlocked;
}

void Board::revealSiblings(int row, int column)
{
    triggerSibblings(row, column, true);
}

void Board::unrevealSiblings(int row, int column)
{
    triggerSibblings(row, column, false);
}

void Board::triggerSibblings(int row, int column, bool reveal)
{
    if (!firstClick || !running || !cells[row][column].getActivated()) {
        return; // give up
    }

    const int adjacentRows[8]    = { -1, -1, -1,  0,  0,  1,  1,  1 };
    const int adjacentColumns[8] = { -1,  0,  1, -1,  1, -1,  0,  1 };

    for (int i = 0; i < 8; ++i) {
        int newRow = row + adjacentRows[i];
        int newColumn = column + adjacentColumns[i];
        bool isValid = (newRow >= 0 && newRow < numberOfRows) && (newColumn >= 0 && newColumn < numberOfColumns);
        if (isValid && !cells[newRow][newColumn].getActivated()) {
            cells[newRow][newColumn].setHighlightToReveal(reveal);
        }
    }
}

void Board::tryToRevealSiblings(int row, int column)
{
    if (!running) {
#ifdef DEBUG_OUTPUT
        qDebug("Not running, left-click ignored");
#endif
        return; // give up
    }

#ifdef DEBUG_OUTPUT
    qDebug("Try to reveal siblings at (%d, %d)", row, column);
#endif
    switch (checkFlaggedMinesAround(row, column)) {
    case HIGHLIGHT_INACTIVE:
#ifdef DEBUG_OUTPUT
        qDebug("C++: Inactive");
#endif
//        flashInactiveAround(row, column);
        break;
    case HIGHLIGHT_VALUE:
#ifdef DEBUG_OUTPUT
        qDebug("C++: Value");
#endif
        cells[row][column].activateflash();
        break;
    case Lose:
#ifdef DEBUG_OUTPUT
        qDebug("C++: Lose");
#endif
        // already lose by hitting the mine
        break;
    case REVEAL:
#ifdef DEBUG_OUTPUT
        qDebug("C++: Reveal");
#endif
        revealingSiblings = true;
        revealUnflaggedMinesAround(row, column);
        revealingSiblings = false;
        break;
    }
}

Board::REVEAL_SIBLINGS_RESULT Board::checkFlaggedMinesAround(int row, int column)
{
    QList< QPair<int, int> > minesCoords = surroundingMines(row, column);
    QList< QPair<int, int> > flagsCoords = surroundingFlags(row, column);

    bool lose = false;

    if (minesCoords.count() > flagsCoords.count()) { // if are more mines than flags
        return HIGHLIGHT_INACTIVE;
    } else if (minesCoords.count() < flagsCoords.count()) { // if are less
        return HIGHLIGHT_VALUE;
    } else { // equal
        const int n = minesCoords.count();
        for (int i = 0; i < n; ++i) {
            int r = minesCoords[i].first;
            int c = minesCoords[i].second;
            bool isflagged = cells[r][c].getFlagged();
            bool isUnknown = cells[r][c].getFlaggedUnknown();
            bool alreadyClicked = cells[r][c].getActivated();
            if (minesCoords[i] != flagsCoords[i] && !isflagged && !isUnknown && !alreadyClicked) {
                leftClick(minesCoords[i].first, minesCoords[i].second);
                if (!lose) {
                    lose = true;
                }
            }
        }
        return lose ? Lose : REVEAL;
    }
}

QList< QPair<int, int> > Board::surroundingMines(int row, int column)
{
    const int adjacentRows[8]    = { -1, -1, -1,  0,  0,  1,  1,  1 };
    const int adjacentColumns[8] = { -1,  0,  1, -1,  1, -1,  0,  1 };

    QList<QPair<int, int>> minesCoords;

    int i;
    for (i = 0; i < 8; ++i) {
        int rowToCheck    = row + adjacentRows[i];
        int columnToCheck = column + adjacentColumns[i];
        bool isValid = (rowToCheck >= 0 && rowToCheck < numberOfRows) && (columnToCheck >= 0 && columnToCheck < numberOfColumns);
        if (isValid && cells[rowToCheck][columnToCheck].getContent() == Cell::MINE) {
            minesCoords << QPair<int, int>(rowToCheck, columnToCheck);
        }
    }
    return minesCoords;
}

QList< QPair<int, int> > Board::surroundingFlags(int row, int column)
{
    const int adjacentRows[8]    = { -1, -1, -1,  0,  0,  1,  1,  1 };
    const int adjacentColumns[8] = { -1,  0,  1, -1,  1, -1,  0,  1 };

    QList<QPair<int, int>> flagsCoords;

    int i;
    for (i = 0; i < 8; ++i) {
        int rowToCheck    = row + adjacentRows[i];
        int columnToCheck = column + adjacentColumns[i];
        bool isValid = (rowToCheck >= 0 && rowToCheck < numberOfRows) && (columnToCheck >= 0 && columnToCheck < numberOfColumns);
        if (isValid && cells[rowToCheck][columnToCheck].getFlagged()) {
            flagsCoords << QPair<int, int>(rowToCheck, columnToCheck);
        }
    }
    return flagsCoords;
}

void Board::flashInactiveAround(int row, int column)
{
    const int adjacentRows[8]    = { -1, -1, -1,  0,  0,  1,  1,  1 };
    const int adjacentColumns[8] = { -1,  0,  1, -1,  1, -1,  0,  1 };

    for (int i = 0; i < 8; ++i) {
        int newRow = row + adjacentRows[i];
        int newColumn = column + adjacentColumns[i];
        bool isValid = (newRow >= 0 && newRow < numberOfRows) && (newColumn >= 0 && newColumn < numberOfColumns);
        if (isValid && !cells[newRow][newColumn].getActivated() && !cells[newRow][newColumn].getFlagged() && !cells[newRow][newColumn].getFlaggedUnknown()) {
            cells[newRow][newColumn].activateflash();
        }
    }
}

void Board::revealUnflaggedMinesAround(int row, int column)
{
    const int adjacentRows[8]    = { -1, -1, -1,  0,  0,  1,  1,  1 };
    const int adjacentColumns[8] = { -1,  0,  1, -1,  1, -1,  0,  1 };

    for (int i = 0; i < 8; ++i) {
        int newRow = row + adjacentRows[i];
        int newColumn = column + adjacentColumns[i];
        bool isValid = (newRow >= 0 && newRow < numberOfRows) && (newColumn >= 0 && newColumn < numberOfColumns);
        if (isValid && !cells[newRow][newColumn].getFlagged() && !cells[newRow][newColumn].getActivated()) {
            if (cells[newRow][newColumn].getFlaggedUnknown()) {
                cells[newRow][newColumn].setFlaggedUnknown(false);
            }
            leftClick(newRow, newColumn);
        }
    }
}

void Board::flagRemainingMines()
{
    for (int r = 0; r < numberOfRows; ++r) {
        for (int c = 0; c < numberOfColumns; ++c) {
            if (cells[r][c].getContent() == Cell::MINE && !cells[r][c].getFlagged()) {
                cells[r][c].setFlagged(true);
                setMinesDiscovered(minesDiscovered + 1);
            }
        }
    }
    updateProgress();
}

//! WIN OR Lose
void Board::win()
{
    setRunning(false);
    flagRemainingMines();
    revealBoard(true);
    emit gameCleared();
    emit finished();
}

void Board::lose(int row, int column)
{
    setRunning(false);
    revealBoard();
    emit gameOverByMineActivation(row, column);
    emit finished();
}
