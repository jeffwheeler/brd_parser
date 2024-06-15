#include "mainwindow_test.h"

#include "brd_gui/mainwindow.h"

void TestMainWindow::loadFile() {
  MainWindow window;
  window.loadFile("../../test/data/avalon/AVALON.brd");
}

QTEST_MAIN(TestMainWindow)
