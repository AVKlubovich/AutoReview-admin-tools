#include "Common.h"
#include "Core.h"

#include "utils/Settings/Settings.h"
#include "utils/Settings/SettingsFactory.h"

#include "utils/Logging/Logger.h"
#include "utils/Logging/LoggerMessages.h"
#include "utils/Logging/Devices/FileDevice.h"
#include "utils/Logging/Devices/DebuggerDevice.h"
#include "utils/Logging/Devices/ConsoleDevice.h"

#include "MainWindow.h"


using namespace test_client;

Core::Core()
{
    QDir::setCurrent( QCoreApplication::applicationDirPath() );

    utils::Settings::Options config = { "configuration/client.ini", true };
    utils::SettingsFactory::instance().registerSettings("client-core", config);

    auto settings = utils::SettingsFactory::instance().settings("client-core");
    settings =
    {
        // Connection
        { "Connection/Scheme", "http" },
        { "Connection/Host", "192.168.202.222" },
        { "Connection/Port", "6661" },

        // Logs
        { "Log/EnableLog", false },
        { "Log/FlushInterval", 1000 },
        { "Log/PrefixName", "test_client.log" },
        { "Log/Dir", "./logs/" },
        { "Log/MaxSize", 134217728 }, // 100 mb
    };
}

bool Core::init()
{
    qsrand(QTime(0, 0, 0).msecsTo(QTime::currentTime()));

    if (!initLogger())
    {
        qDebug() << "Logger does not initialize";
        return false;
    }

    if (!initWindow())
    {
        qWarning() << "Could not initialize server";
        return false;
    }

    return true;
}

void Core::run()
{
    _window->show();
}

bool Core::initLogger()
{
    auto settings = utils::SettingsFactory::instance().settings("client-core");
    settings.beginGroup("Log");

    if (!settings["EnableLog"].toBool())
    {
        qDebug() << "logger disabled";
        return true;
    }

    _logger = QSharedPointer<utils::Logger>::create();

    const auto loggerOptions = QSharedPointer<utils::LoggerMessages::Options>::create();
    loggerOptions->timerInterval = settings["FlushInterval"].toInt();
    if (!_logger->init(loggerOptions))
        return false;

    // FileDevice
    const auto fileOptions = QSharedPointer<utils::FileDevice::FileOptions>::create();
    fileOptions->maxSize = settings["MaxSize"].toLongLong();
    fileOptions->prefixName = settings["PrefixName"].toString(); //"okk_server.log";
    fileOptions->directory = settings["Dir"].toString();

    if (!_logger->addDevice(fileOptions))
        return false;

    // DebuggerDevice
    const auto debuggerDevice = QSharedPointer<utils::DebuggerDevice::DebuggerOptions>::create();

    if (!_logger->addDevice(debuggerDevice))
        return false;

    // ConsoleDevice
    const auto consoleDevice = QSharedPointer<utils::ConsoleDevice::ConsoleOptions>::create();

    if (!_logger->addDevice(consoleDevice))
        return false;

    qDebug() << "initLogger";
    return true;
}

bool Core::initWindow()
{
    _window = test_client::MainWindowShp::create();

    return true;
}

void Core::done()
{
    _window.clear();
    _logger.clear();
}
