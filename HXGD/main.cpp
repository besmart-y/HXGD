#include "HXGD.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
 /*   QTextCodec* codec = QTextCodec::codecForName("UTF8");
    if (codec) {
        QTextCodec::setCodecForLocale(codec);
    }*/
    QApplication a(argc, argv);
    HXGD w;
    w.show();
    return a.exec();
}
