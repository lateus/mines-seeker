#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QSettings>
#include <QWindow>
#include <QSize>
#include <QVersionNumber>
#include <QCoreApplication>

class GeneralSettings : public QSettings
{
    Q_OBJECT

    Q_PROPERTY(QWindow::Visibility mainWindowVisibility READ getMainWindowVisibility WRITE setMainWindowVisibility NOTIFY mainWindowVisibilityChanged)
    Q_PROPERTY(QSize mainWindowSize READ getMainWindowSize WRITE setMainWindowSize NOTIFY mainWindowSizeChanged)
    Q_PROPERTY(QString language READ getLanguage WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(bool firstTime READ getFirstTime WRITE setFirstTime NOTIFY firstTimeChanged)
    Q_PROPERTY(bool firstTimeAfterUpgrade READ getFirstTimeAfterUpgrade WRITE setFirstTimeAfterUpgrade NOTIFY firstTimeAfterUpgradeChanged)

    Q_PROPERTY(double volume READ getVolume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool oneClickRevealSiblings READ getOneClickRevealSiblings WRITE setOneClickRevealSiblings NOTIFY oneClickRevealSiblingsChanged)
    Q_PROPERTY(BoardGeneration boardGeneration READ getBoardGeneration WRITE setBoardGeneration NOTIFY boardGenerationChanged)

public:
    enum BoardGeneration { FirstClickClear, TotallyRandom };
    Q_ENUM(BoardGeneration)

    explicit GeneralSettings(QObject *parent = nullptr);
    ~GeneralSettings();

    QWindow::Visibility getMainWindowVisibility() const;
    void setMainWindowVisibility(const QWindow::Visibility &value);

    QSize getMainWindowSize() const;
    void setMainWindowSize(const QSize &value);

    QString getLanguage() const;
    void setLanguage(const QString &value);

    bool getFirstTime() const;
    void setFirstTime(bool value);

    bool getFirstTimeAfterUpgrade() const;
    void setFirstTimeAfterUpgrade(bool value);

    double getVolume() const;
    void setVolume(double value);

    bool getOneClickRevealSiblings() const;
    void setOneClickRevealSiblings(bool value);

    BoardGeneration getBoardGeneration() const;
    void setBoardGeneration(BoardGeneration value);

signals:
    void mainWindowVisibilityChanged(QWindow::Visibility newState);
    void mainWindowSizeChanged(QSize newSize);
    void languageChanged(const QString &twoCharsLanguage);
    void firstTimeChanged(bool newState);
    void firstTimeAfterUpgradeChanged(bool newState);

    void volumeChanged(double vol);
    void oneClickRevealSiblingsChanged(bool newState);
    void boardGenerationChanged(BoardGeneration value);

public slots:
    void loadSettings();
    void saveSettings();

private:
    QWindow::Visibility mainWindowVisibility = QWindow::AutomaticVisibility;
    QSize mainWindowSize = QSize(600, 500);
    QString language = "en";
    bool firstTime = false;
    bool firstTimeAfterUpgrade = false;

    double volume = 1.0;
    bool oneClickRevealSiblings = false;
    BoardGeneration boardGeneration = FirstClickClear;
};

#endif // GENERALSETTINGS_H
