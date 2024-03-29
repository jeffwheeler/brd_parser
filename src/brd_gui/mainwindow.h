#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <string>
#include <unordered_set>
#include <utility>

// #include "brdview.h"
#include "lib/parser/parser.h"

class BrdView;

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = nullptr);

    void updatePosition(QPointF pos);
    void loadFile(std::string path);

   protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void openFile();
    void zoomIn();
    void zoomOut();
    void zoomFit();

   private slots:
    void selectLayer();
    void selectFilm();

   private:
    std::optional<File<A_175>> fs;
    BrdView *brdView;
    QTreeWidget *tree;
    std::map<std::string, uint32_t> layer_cache;

    void createToolBar();
    void createDockWidget();
    void createFilmSelectWidget();
    void loadFilms();
    QTreeWidgetItem *createTopLevelItem(QString s);
    void addLayer(QTreeWidgetItem *parent, QString s);
    std::pair<uint16_t, uint16_t> layerPair(uint16_t x, uint16_t y);
};

#endif
