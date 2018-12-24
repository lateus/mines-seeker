#include "Controller/GeneralSettings/generalsettings.h"
#include "Controller/StartupProgress/startupprogress.h"
#include "Controller/Arcade/arcade.h"
#include "Controller/Board/board.h"
#include "Controller/Cell/cell.h"
#include "Model/RecordsManager/recordsmanager.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QLocale>
#include <QVersionNumber>
#include <QFont>
#include <QFontDatabase>
#include <QTranslator>
#include <QLibraryInfo>
#include <QQuickView>
#include <QSplashScreen>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QLockFile>
#include <QIcon>

int main(int argc, char *argv[])
{
    // To bulk data in RecordsManager
    //    freopen("err.txt", "a+", stderr);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QLocale::setDefault(QLocale::English);

    QApplication app(argc, argv);
    app.setOrganizationName("TheCrowporation");
    app.setApplicationName("MinesSeeker");
    app.setApplicationVersion("1.0");
    app.setWindowIcon(QIcon(":/images/appIcon.png"));

    QSplashScreen splash(QPixmap(":/images/Logo512_BG.png").scaledToHeight(350, Qt::SmoothTransformation), Qt::WindowFlags() | Qt::WindowStaysOnTopHint);
    splash.show();
    app.processEvents();

    // SETTINGS
    GeneralSettings settings;

    // INTERNACIONALIZATION
    QTranslator qtTranslator;
    qtTranslator.load(QLocale(settings.getLanguage()), QStringLiteral("qt"), QStringLiteral("_"), ":/translations");
    app.installTranslator(&qtTranslator);

    QTranslator myAppTranslator;
    myAppTranslator.load(QLocale(settings.getLanguage()), QStringLiteral("mines"), QStringLiteral("_"), ":/translations");
    app.installTranslator(&myAppTranslator);

    // ALLOW JUST ONE RUNING INSTANCE
    QString lockFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QString("/m%1.lock").arg(qApp->applicationVersion().replace('.', '_'));
    QLockFile lockFile(lockFilePath);
    if (!lockFile.tryLock()) {
        QMessageBox::information(nullptr, QObject::tr("MinesSeeker"), QObject::tr("An instance of MinesSeeker is already running.\n"
                                                                                  "Close it in order to run a new one."));
        return 0;
    }

    // DATABASE PATH CHECKING
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir(appDataPath);
    if (!appDataDir.exists()) {
        if (!appDataDir.mkpath(appDataPath)) {
            splash.close();
            QMessageBox::critical(nullptr, QObject::tr("MinesSeeker"), QObject::tr("Cannot create the folder:\n"
                                                                                   "%1.\n"
                                                                                   "Execute this application as root or create the folder manually.").arg(appDataPath));
            app.processEvents();
            return 1;
        }
    }

#ifdef Q_OS_WIN
    QFont applicationFont("Sans Serif", -1, QFont::Normal);
    applicationFont.setStyleHint(QFont::SansSerif);
    app.setFont(applicationFont);
#endif

    // SPLASH
#ifndef Q_OS_ANDROID
    QQuickView qv;
    qv.setFlags(Qt::SplashScreen | Qt::WindowStaysOnTopHint);
    qv.setColor(Qt::transparent);
#endif
    QQmlApplicationEngine engine;
    qmlRegisterType<Cell>  ("Minesweeper", 1, 0, "Cell");
    qmlRegisterType<Board> ("Minesweeper", 1, 0, "Board");
    qmlRegisterType<Arcade>("Minesweeper", 1, 0, "Arcade");
    qmlRegisterUncreatableType<GeneralSettings>("Minesweeper", 1, 0, "GeneralSettings", "Cannot create object of this type.");
    settings.loadSettings();

    StartupProgress sp;
#ifndef Q_OS_ANDROID
    qv.engine()->rootContext()->setContextProperty("startupManager", &sp);
    qv.setSource(QUrl(QStringLiteral("qrc:/src/Loading.qml")));
    qv.show();
#endif

    QObject::connect( &sp, &StartupProgress::progressChanged, [&](double value) {
        splash.showMessage(sp.getProgressMessage() + QString(" ...%1%").arg(value * 100.0, 0, 'f', 2), Qt::AlignBottom | Qt::AlignHCenter, Qt::white);
        app.processEvents();
    } );
    QObject::connect( &sp, &StartupProgress::progressMessageChanged, [&](const QString &newMsg) {
        QString msg = newMsg;
        double progress = sp.getProgress();
        if (progress > 0.0) {
            msg += QString(" ...%1%%").arg(progress, 0, 'f', 2);
        }
        splash.showMessage(msg);
        app.processEvents();
    } );

    QObject::connect( &sp, &StartupProgress::readyToLoadMainQMLFile, [&]()
    {
#ifndef Q_OS_ANDROID
        splash.close();
        app.processEvents();
#endif
        // FONTS
#ifndef Q_OS_ANDROID
        sp.setProgressMessage(QObject::tr("Loading fonts..."));
#endif
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-Black.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-BlackIt.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-Bold.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-BoldIt.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-ExtraLight.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-ExtraLightIt.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-It.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-Light.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-LightIt.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-Medium.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-MediumIt.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-Regular.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-Semibold.otf");
        QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-SemiboldIt.otf");

        // DATABASE
#ifndef Q_OS_ANDROID
        sp.setProgressMessage(QObject::tr("Loading scores..."));
#endif
        QString databaseFile = appDataPath + "/records.db";
        static RecordsManager recordManager(databaseFile);

        // QML ENGINE
#ifndef Q_OS_ANDROID
        sp.setProgressMessage(QObject::tr("Registering C++ types into QML engine..."));
#endif
        engine.rootContext()->setContextProperty("settings", &settings);
        engine.rootContext()->setContextProperty("recordManager", &recordManager);
        engine.rootContext()->setContextProperty("startupManager", &sp);

        QObject::connect(&settings, &GeneralSettings::languageChanged, [&](const QString &newLanguage) {
            QLocale::setDefault(QLocale(newLanguage));
            qtTranslator.load(QLocale(), QStringLiteral("qt"), QStringLiteral("_"), ":/translations");
            myAppTranslator.load(QLocale(), QStringLiteral("mines"), QStringLiteral("_"), ":/translations");
            engine.retranslate();
        });

#ifndef Q_OS_ANDROID
        sp.setProgressMessage(QObject::tr("Loading UI..."));
#endif
        engine.load(QUrl(QStringLiteral("qrc:/src/mainqml.qml")));
        if (engine.rootObjects().isEmpty()) {
            QGuiApplication::exit(-1);
        } else {
#ifndef Q_OS_ANDROID
            qv.close();
#endif
        }
    });

#ifdef Q_OS_ANDROID
    sp.emitReadySignal();
#endif

    int result = app.exec();
    lockFile.unlock();
    return result;
}
