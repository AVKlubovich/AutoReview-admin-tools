#pragma once

#include "utils/BaseClasses/Singleton.h"


namespace network
{
    class RequestsManager;
    typedef QSharedPointer<RequestsManager> RequestsManagerShp;
}

namespace utils
{
    class Logger;
    typedef QSharedPointer<Logger> LoggerShp;
}

namespace test_client
{

    class MainWindow;
    typedef QSharedPointer<MainWindow> MainWindowShp;

    class Core
        : public utils::Singleton<Core>
    {
    public:
        Core();
        ~Core() = default;

        bool init();
        void run();
        void done();

    private:
        bool initLogger();
        bool initWindow();

    private:
        utils::LoggerShp _logger;
        MainWindowShp _window;
    };

}
