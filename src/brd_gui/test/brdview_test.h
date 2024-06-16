#include <QTest>

class TestBrdView : public QObject {
  Q_OBJECT
 private slots:
  void drawAllSymbolPaths();
  void handleBackdrills();
};
