#include "Scene/SimpleScene.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SimpleScene scene;
    scene.show();

    return a.exec();
}
