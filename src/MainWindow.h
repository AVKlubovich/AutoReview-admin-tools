#pragma once


namespace Ui
{
    class MainWindow;
}

namespace test_client
{

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow();
        ~MainWindow();

    private:
        Ui::MainWindow *_ui;

        const QString WINDOW_TITLE = "AutoReview. Administration tools.";
    };

}
