#ifndef BOARD_H
#define BOARD_H

#include "../Cell/cell.h"

#include <QObject>
#include <QTimer>
#include <QRandomGenerator>

class Board : public QObject
{
    Q_OBJECT

    //! Properties
    Q_PROPERTY(Board::BOARDMODE mode READ getMode WRITE setMode NOTIFY modeChanged)

    Q_PROPERTY(int    rows READ getNumberOfRows NOTIFY numberOfRowsChanged)
    Q_PROPERTY(int    columns READ getNumberOfColumns NOTIFY numberOfColumnsChanged)
    Q_PROPERTY(int    mines READ getNumberOfMines NOTIFY numberOfMinesChanged)
    Q_PROPERTY(double progress READ getProgress NOTIFY progressChanged)
    Q_PROPERTY(double uiBoardLoadingProgress READ getUiBoardLoadingProgress WRITE setUiBoardLoadingProgress NOTIFY progressChanged)
    Q_PROPERTY(int    minesDiscovered READ getMinesDiscovered NOTIFY minesDiscoveredChanged)
    Q_PROPERTY(bool   running READ getRunning NOTIFY runningChanged)

    Q_PROPERTY(int maxRows READ getMaxRows CONSTANT)
    Q_PROPERTY(int maxColumns READ getMaxColumns CONSTANT)

    Q_PROPERTY(int uiRows READ getUiRows WRITE setUiRows NOTIFY uiRowsChanged)
    Q_PROPERTY(int uiCols READ getUiCols WRITE setUiCols NOTIFY uiColsChanged)

    Q_PROPERTY(int customGameRows    READ getCustomGameRows    WRITE setCustomGameRows    NOTIFY customGameRowsChanged)
    Q_PROPERTY(int customGameColumns READ getCustomGameColumns WRITE setCustomGameColumns NOTIFY customGameColumnsChanged)
    Q_PROPERTY(int customGameMines   READ getCustomGameMines   WRITE setCustomGameMines   NOTIFY customGameMinesChanged)

    Q_PROPERTY(bool firstClickClear READ getFirstClickClear WRITE setFirstClickClear NOTIFY firstClickClearChanged)

public:
    enum BOARDMODE {BEGINNER, MEDIUM, EXPERT, CUSTOM}; // Add (Superhuman: 50, 50, 500) and Extraterrestrial (100, 100, 2000)
    enum REVEAL_SIBLINGS_RESULT {HIGHLIGHT_INACTIVE, HIGHLIGHT_VALUE, REVEAL, Lose};
    Q_ENUM(BOARDMODE)
    Q_ENUM(REVEAL_SIBLINGS_RESULT)

    explicit Board(QObject *parent = nullptr);
    Board(const Board &other);
    Board& operator=(const Board &other);
    bool operator==(const Board &other);

    BOARDMODE getMode() const;
    void setMode(const BOARDMODE &value, int customRows = -1, int customColumns = -1, int customMines = -1);

    int getNumberOfRows() const;
    void setNumberOfRows(int value);

    int getNumberOfColumns() const;
    void setNumberOfColumns(int value);

    int getNumberOfMines() const;
    void setNumberOfMines(int value);

    double getProgress() const;

    double getUiBoardLoadingProgress() const;
    void setUiBoardLoadingProgress(double value);

    int getUiRows() const;
    void setUiRows(int value);

    int getUiCols() const;
    void setUiCols(int value);

    int getMinesDiscovered() const;
    void setMinesDiscovered(int value);

    bool getRunning() const;
    void setRunning(bool value);

    int getCustomGameRows() const;
    void setCustomGameRows(int value);

    int getCustomGameColumns() const;
    void setCustomGameColumns(int value);

    int getCustomGameMines() const;
    void setCustomGameMines(int value);

    bool getFirstClickClear() const;
    void setFirstClickClear(bool value);

    Q_INVOKABLE Cell& operator[](int index);
    Q_INVOKABLE Cell operator[](int index) const;
    Q_INVOKABLE Cell* itemAt(int index);

    // public access
    static const int beginnerRows    = 8; // try 9, 9, 10
    static const int beginnerColumns = 8;
    static const int beginnerMines   = 10;

    static const int mediumRows      = 16;
    static const int mediumColumns   = 16;
    static const int mediumMines     = 40;

    static const int expertRows      = 16;
    static const int expertColumns   = 30;
    static const int expertMines     = 99;

    static const int maxRows         = 16;
    static const int maxCols         = 30;

    inline static int getMaxRows() { return maxRows; }
    inline static int getMaxColumns() { return maxCols; }

signals:
    void start();
    void modeChanged(int newMode);
    void numberOfRowsChanged(int newValue);
    void numberOfColumnsChanged(int newValue);
    void numberOfMinesChanged(int newValue);
    void minesDiscoveredChanged(int m);
    void runningChanged(bool newState);
    void progressChanged(double value);
    void uiBoardLoadingProgressChanged(double value);

    void uiRowsChanged(int r);
    void uiColsChanged(int c);

    void customGameRowsChanged(int r);
    void customGameColumnsChanged(int c);
    void customGameMinesChanged(int m);

    void firstClickClearChanged(bool newState);

    void bigUnlock();
    void flaggingDenied();

    void gameOverByMineActivation(int row, int column);
    void gameCleared();
    void finished();

public slots:
    void setup();
    void newGame();
    void leftClick(int row, int column);
    void rightClick(int row, int column);
    void revealSiblings(int row, int column);
    void unrevealSiblings(int row, int column);
    void tryToRevealSiblings(int row, int column);
    void revealBoard(bool keepFlags = false);
    void updateProgress();

private:
    BOARDMODE mode      = BEGINNER;
    int numberOfRows    = beginnerRows;
    int numberOfColumns = beginnerColumns;
    int numberOfMines   = beginnerMines;
    double progress     = 0;
    double uiBoardLoadingProgress = 0.0;

    // To populate the UIBoard. Wait to update this until the game logic is ready
    int uiRows = beginnerRows;
    int uiCols = beginnerColumns;

    // Cell handling
    Cell cells[maxRows][maxCols];
    int activatedCells = 0;
    int minesDiscovered = 0;
    bool running = false;

    // Custom game settings
    int customGameRows    = beginnerRows;
    int customGameColumns = beginnerColumns;
    int customGameMines   = beginnerMines;

    // settings
    bool firstClickClear = true;

    // for private usage (not exposed to QML engine)
    bool firstClick = false;
    int firstClickRow = -1;
    int firstClickColumn = -1;

    // some private flags
    bool revealingSiblings = false;

    // private functions
    void fillBoard();
    void solveBoard();
    int countSurroundingMines(int row, int column);
    int unlockAdjacentCells(int row, int column);
    void triggerSibblings(int row, int column, bool reveal);
    void win();
    void lose(int row = -1, int column = -1);
    REVEAL_SIBLINGS_RESULT checkFlaggedMinesAround(int row, int column);
    QList< QPair<int, int> > surroundingMines(int row, int column);
    QList< QPair<int, int> > surroundingFlags(int row, int column);
    void flashInactiveAround(int row, int column);
    void revealUnflaggedMinesAround(int row, int column);
    void flagRemainingMines();
};

#endif // BOARD_H
