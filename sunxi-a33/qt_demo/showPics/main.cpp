#include "showpicsdlg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShowPicsDlg w;
    w.show();

    return a.exec();
}
