#include "Common.h"

#include "Core.h"


void setTextCodec(const QString& encodingName)
{
    QTextCodec *codec = QTextCodec::codecForName(encodingName.toStdString().c_str());
    QTextCodec::setCodecForLocale(codec);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    setTextCodec("UTF-8");

    auto& core = test_client::Core::instance();
    if (!core.init())
    {
        core.done();
        return -1;
    }

    core.run();

    a.exec();

    core.done();

    return 0;
}
