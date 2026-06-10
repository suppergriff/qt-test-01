#include <QApplication>
#include "LoginWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 不调用 setStyle()，macOS 自动使用原生控件风格

    LoginWindow window;
    window.show();

    return app.exec();
}
