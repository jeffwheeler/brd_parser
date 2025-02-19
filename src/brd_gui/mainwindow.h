#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <string>
#include <utility>

// #include "brdview.h"
#include "layer_model.h"
#include "lib/parser/parser.h"

class BrdView;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);

  void updatePosition(QPointF pos);
  void loadFile(const std::string &path);

 protected:
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;

  void openFile();
  void zoomIn();
  void zoomOut();
  void zoomFit();

 private slots:
  void selectionChanged();
  void selectFilm();

 private:
  std::unique_ptr<File<kAMax>> fs;
  BrdView *brdView;
  QTreeWidget *film_tree_widget_;
  QTreeView *layer_tree_view_;
  LayerModel *layer_model_;
  std::map<std::string, uint32_t> layer_cache;

  // This is used by the layer selection event handlers to avoid reacting to
  // each other.
  bool updating_layers;

  void createToolBar();
  void createDockWidget();
  void createFilmSelectWidget();
  void loadFilms();
  QTreeWidgetItem *createTopLevelItem(QString s);
  void addLayer(QTreeWidgetItem *parent, QString s);
};

#endif
