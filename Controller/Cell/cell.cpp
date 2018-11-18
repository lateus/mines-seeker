#include "cell.h"

#include <QDebug>

Cell::Cell(QObject *parent) : QObject(parent)
{
    // nothing to do yet
}

Cell::Cell(int r, int c, QObject *parent) : QObject(parent)
{
    row = r;
    column = c;
}

Cell::Cell(const Cell &other) : QObject (other.parent())
{
    *this = other;
}

Cell::~Cell()
{
    // nothing to do yet
}

Cell& Cell::operator=(const Cell &other)
{
    row = other.row;
    column = other.column;
    activated = other.activated;
    flagged = other.flagged;
    content = other.content;
    value = other.value;
    highlightToReveal = other.highlightToReveal;
    return *this;
}

int Cell::getRow() const
{
    return row;
}

void Cell::setRow(int r)
{
    if (row != r) {
        row = r;
        emit rowChanged(row);
    }
}

int Cell::getColumn() const
{
    return column;
}

void Cell::setColumn(int c)
{
    if (column != c) {
        column = c;
        emit columnChanged(column);
    }
}

bool Cell::getActivated() const
{
    return activated;
}

void Cell::setActivated(bool act)
{
    if (activated != act) {
        activated = act;
        emit activatedChanged(activated);
    }
}

bool Cell::getFlagged() const
{
    return flagged;
}

void Cell::setFlagged(bool f)
{
    if (flagged != f) {
        flagged = f;
        emit flaggedChanged(flagged);
    }
}

bool Cell::getFlaggedUnknown() const
{
    return flaggedUnknown;
}

void Cell::setFlaggedUnknown(bool value)
{
    if (flaggedUnknown != value) {
        flaggedUnknown = value;
        emit flaggedUnknownChanged(flaggedUnknown);
    }
}

Cell::CONTENT Cell::getContent() const
{
    return content;
}

void Cell::setContent(const CONTENT &cont)
{
    if (content != cont) {
        content = cont;
        emit contentChanged(cont);
    }
}

int Cell::getValue() const
{
    return value;
}

void Cell::setValue(int n)
{
    if (value != n) {
        value = n;
        emit valueChanged(value);
    }
}

bool Cell::getExploted() const
{
    return exploted;
}

void Cell::setExploted(bool value)
{
    if (exploted != value) {
        exploted = value;
        emit explotedChanged(exploted);
    }
}

bool Cell::getHighlightToReveal() const
{
    return highlightToReveal;
}

void Cell::setHighlightToReveal(bool value)
{
    if (highlightToReveal != value) {
        highlightToReveal = value;
        emit highlightToRevealChanged(highlightToReveal);
    }
}

bool Cell::getFlash() const
{
    return flash;
}

void Cell::setFlash(bool value)
{
    if (flash != value) {
        flash = value;
        emit flashChanged(flash);
    }
}

bool Cell::getRipple() const
{
    return ripple;
}

void Cell::setRipple(bool value)
{
    if (ripple != value) {
        ripple = value;
        emit rippleChanged(ripple);
    }
}

bool Cell::getBothClick() const
{
    return bothClick;
}

void Cell::setBothClick(bool value)
{
    if (bothClick != value) {
        bothClick = value;
        emit bothClickChanged(bothClick);
    }
}

// SLOTS
void Cell::reset()
{
    setRow(-1);
    setColumn(-1);
    setActivated(false);
    setFlagged(false);
    setFlaggedUnknown(false);
    setContent(BLANK);
    setValue(0);
    setExploted(false);
    setHighlightToReveal(false);
    setFlash(false);
    setRipple(false);
}

void Cell::leftClick()
{
    emit leftClicked(row, column);
}

void Cell::rightClick()
{
    emit rightClicked(row, column);
}

void Cell::leftRightClick()
{
    emit leftRightClicked(row, column);
}

void Cell::leftRightUnclick()
{
    emit leftRightUnclicked(row, column);
}

void Cell::tryToRevealSiblings()
{
    emit revealSiblingsTriggered(row, column);
}

void Cell::activateflash()
{
    if (flash) {
        setFlash(false);
    }
    setFlash(true);
}

void Cell::unsetFlash()
{
    setFlash(false);
}
