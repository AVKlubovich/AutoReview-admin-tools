#include "Common.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"


using namespace test_client;

MainWindow::MainWindow()
    : QMainWindow(nullptr)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    setWindowTitle(WINDOW_TITLE);
}

MainWindow::~MainWindow()
{
    delete _ui;
}
