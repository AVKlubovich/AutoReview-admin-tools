#include "Common.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "network-core/RequestsManager/RequestsManager.h"


using namespace test_client;

MainWindow::MainWindow()
    : QMainWindow(nullptr)
    , _ui(new Ui::MainWindow)
    , _requestsManager(network::RequestsManagerShp::create())
{
    _ui->setupUi(this);

    setWindowTitle(WINDOW_TITLE);
}

MainWindow::~MainWindow()
{
    delete _ui;
}
