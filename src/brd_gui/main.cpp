#include <QtWidgets>
#include <iostream>

#include "brdview.h"
#include "lib/parser/parser.h"
#include "mainwindow.h"

const double FACTOR_MAP[] = {60000., 1000., 2000., 600.,  1100.,  600.,
                             1000.,  1200., 500.,  6000., 10000., 500.,
                             500.,   500.,  500.,  500.,  800.,   500.};

int main(int argc, char *argv[]) {
    uint8_t id = 10;

    QApplication app(argc, argv);
    MainWindow window;
    window.resize(1000, 700);
    window.setWindowTitle(QApplication::translate("toplevel", "PCB viewer"));

    window.show();
    return app.exec();
}
