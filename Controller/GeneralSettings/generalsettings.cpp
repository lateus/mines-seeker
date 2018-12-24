#include "generalsettings.h"

#include <QDebug>

#define DEBUG_OUTPUT

GeneralSettings::GeneralSettings(QObject *parent) : QSettings(parent)
{
    loadSettings();
}

GeneralSettings::~GeneralSettings()
{
    saveSettings();
//    fprintf(stderr, "\n\n-------------------------END-------------------------\n\n");
}

void GeneralSettings::loadSettings()
{
    if (contains("Application/MainWindowVisibility")) {
        mainWindowVisibility = QWindow::Visibility(value("Application/MainWindowVisibility").toInt());
    } else {
        mainWindowVisibility = QWindow::AutomaticVisibility;
    }

    if (contains("Application/MainWindowSize")) {
        mainWindowSize = value("Application/MainWindowSize").toSize();
    } else {
        mainWindowSize = QSize(600, 500);
    }

    if (contains("Application/Language")) {
        setLanguage(value("Application/Language").toString());
    } else {
        setLanguage(QLocale::system().name().left(2));
    }

    if (!contains("Application/Volume")) {
        setVolume(1.0);
    } else {
        setVolume(value("Application/Volume").toDouble());
    }

    if (!contains("Arcade/OneClickRevealSiblings")) {
        setOneClickRevealSiblings(false);
    } else {
        setOneClickRevealSiblings(value("Arcade/OneClickRevealSiblings").toBool());
    }

    if (!contains("Arcade/BoardGeneration")) {
        setBoardGeneration(GeneralSettings::FirstClickClear);
    } else {
        setBoardGeneration(GeneralSettings::BoardGeneration(value("Arcade/BoardGeneration").toInt()));
    }

    if (!contains("Version")) {
        setFirstTime(true);
    } else if (QVersionNumber::fromString(value("Version").toString()) < QVersionNumber::fromString(QCoreApplication::instance()->applicationVersion()) ) {
        setFirstTimeAfterUpgrade(true);
    }
}

void GeneralSettings::saveSettings()
{
    setValue("Application/MainWindowVisibility", mainWindowVisibility);
    setValue("Application/MainWindowSize", mainWindowSize);
    setValue("Application/Language", language);
    setValue("Application/Volume", volume);
    setValue("Arcade/OneClickRevealSiblings", oneClickRevealSiblings);
    setValue("Arcade/BoardGeneration", int(boardGeneration));
    setValue("Version", QCoreApplication::instance()->applicationVersion());
}

GeneralSettings::BoardGeneration GeneralSettings::getBoardGeneration() const
{
    return boardGeneration;
}

void GeneralSettings::setBoardGeneration(GeneralSettings::BoardGeneration value)
{
    if (boardGeneration != value) {
        boardGeneration = value;
        emit boardGenerationChanged(boardGeneration);
    }
}

bool GeneralSettings::getOneClickRevealSiblings() const
{
    return oneClickRevealSiblings;
}

void GeneralSettings::setOneClickRevealSiblings(bool value)
{
    if (oneClickRevealSiblings != value) {
        oneClickRevealSiblings = value;
        emit oneClickRevealSiblingsChanged(oneClickRevealSiblings);
    }
}

double GeneralSettings::getVolume() const
{
    return volume;
}

void GeneralSettings::setVolume(double value)
{
    if (value > 1.0) {
        value  = 1.0;
    }
    if (volume != value) {
        volume = value;
        emit volumeChanged(volume);
    }
}

bool GeneralSettings::getFirstTimeAfterUpgrade() const
{
    return firstTimeAfterUpgrade;
}

void GeneralSettings::setFirstTimeAfterUpgrade(bool value)
{
    if (firstTimeAfterUpgrade != value) {
        firstTimeAfterUpgrade = value;
        emit firstTimeAfterUpgradeChanged(firstTimeAfterUpgrade);
    }
}

bool GeneralSettings::getFirstTime() const
{
    return firstTime;
}

void GeneralSettings::setFirstTime(bool value)
{
    if (firstTime != value) {
        firstTime = value;
        firstTimeChanged(firstTime);
    }
}

QString GeneralSettings::getLanguage() const
{
    return language;
}

void GeneralSettings::setLanguage(const QString &value)
{
    if (language != value) {
        language = value;
        emit languageChanged(language);
    }
}

QWindow::Visibility GeneralSettings::getMainWindowVisibility() const
{
    return mainWindowVisibility;
}

void GeneralSettings::setMainWindowVisibility(const QWindow::Visibility &value)
{
    if (mainWindowVisibility != value) {
        mainWindowVisibility = value;
        emit mainWindowVisibilityChanged(mainWindowVisibility);
    }
}

QSize GeneralSettings::getMainWindowSize() const
{
    return mainWindowSize;
}

void GeneralSettings::setMainWindowSize(const QSize &value)
{
    if (mainWindowSize != value) {
        mainWindowSize = value;
        emit mainWindowSizeChanged(mainWindowSize);
    }
}
