#include <QtWidgets>
#include <iostream>

#include "brdview.h"
#include "lib/parser/parser.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setApplicationName("brd_parser");
  app.setApplicationVersion("0.0.0");

  QCommandLineParser parser;
  parser.addPositionalArgument("input", ".brd file to open");
  parser.addHelpOption();
  parser.addVersionOption();

  parser.process(app);
  const QStringList args = parser.positionalArguments();

  if (args.length() > 1) {
    parser.showHelp();
  }

  MainWindow window;
  window.resize(1000, 700);
  window.setWindowTitle(QApplication::translate("toplevel", "PCB viewer"));

  if (args.length() == 1) {
    window.loadFile(args.at(0).toStdString());
  }

  window.show();
  return app.exec();
}
