#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QTimer>

Q_DECLARE_METATYPE(class Cell);

class Cell : public QObject
{
    Q_OBJECT

    //! Properties
    Q_PROPERTY(int row READ getRow WRITE setRow NOTIFY rowChanged)
    Q_PROPERTY(int column READ getColumn WRITE setColumn NOTIFY columnChanged)
    Q_PROPERTY(bool activated READ getActivated WRITE setActivated NOTIFY activatedChanged)
    Q_PROPERTY(bool flagged READ getFlagged WRITE setFlagged NOTIFY flaggedChanged)
    Q_PROPERTY(bool flaggedUnknown READ getFlaggedUnknown WRITE setFlaggedUnknown NOTIFY flaggedUnknownChanged)
    Q_PROPERTY(Cell::CONTENT content READ getContent WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(int value READ getValue WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int exploted READ getExploted WRITE setExploted NOTIFY explotedChanged)

    Q_PROPERTY(bool highlightToReveal READ getHighlightToReveal WRITE setHighlightToReveal NOTIFY highlightToRevealChanged)
    Q_PROPERTY(bool flash READ getFlash WRITE setFlash NOTIFY flashChanged)
    Q_PROPERTY(bool ripple READ getRipple WRITE setRipple NOTIFY rippleChanged)

    Q_PROPERTY(bool bothClick READ getBothClick WRITE setBothClick NOTIFY bothClickChanged)

public:
    enum CONTENT {BLANK, VALUE, MINE};
    Q_ENUM(CONTENT)

    explicit Cell(QObject *parent = nullptr);
    Cell(int r, int c, QObject *parent = nullptr);
    Cell(const Cell &other);
    ~Cell();
    Cell& operator=(const Cell &other);

    int getRow() const;
    void setRow(int r);

    int getColumn() const;
    void setColumn(int c);

    bool getActivated() const;
    void setActivated(bool act);

    bool getFlagged() const;
    void setFlagged(bool f);

    bool getFlaggedUnknown() const;
    void setFlaggedUnknown(bool value);

    CONTENT getContent() const;
    void setContent(const CONTENT &cont);

    int getValue() const;
    void setValue(int n);

    bool getExploted() const;
    void setExploted(bool value);

    bool getHighlightToReveal() const;
    void setHighlightToReveal(bool value);

    bool getFlash() const;
    void setFlash(bool value);

    bool getBothClick() const;
    void setBothClick(bool value);

    bool getRipple() const;
    void setRipple(bool value);

signals:
    void rowChanged(int newRow);
    void columnChanged(int newColumn);
    void activatedChanged(bool newState);
    void flaggedChanged(bool newState);
    void flaggedUnknownChanged(bool newState);
    void contentChanged(CONTENT c);
    void valueChanged(int newValue);
    void explotedChanged(bool newState);

    void highlightToRevealChanged(bool newState);

    void leftClicked(int row, int column);
    void rightClicked(int row, int column);
    void leftRightClicked(int row, int column);
    void leftRightUnclicked(int row, int column);
    void revealSiblingsTriggered(int row, int column);

    void flashChanged(bool newState);
    void rippleChanged(bool newState);

    void bothClickChanged(bool newState);

public slots:
    void reset();
    void leftClick();
    void rightClick();
    void leftRightClick();
    void leftRightUnclick();
    void tryToRevealSiblings();
    void activateflash();
    void unsetFlash();

private:
    int row = -1, column = -1;
    bool activated = false;
    bool flagged = false;
    bool flaggedUnknown = false;
    CONTENT content = BLANK;
    int value = 0; // means blank. set to a positive value when the CONTENT is a VALUE
    bool exploted = false;
    bool highlightToReveal = false;
    bool flash = false;
    bool bothClick = false;
    bool ripple = false;

    void toggleRipple();
};

#endif // CELL_H
