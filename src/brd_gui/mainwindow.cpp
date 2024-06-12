#include "mainwindow.h"

#include "brdview.h"
#include "lib/printing/printers.h"
#include "lib/structure/utils.h"
#include "moc_mainwindow.cpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), updating_layers(false) {
  qDebug() << "MainWindow constructor";
  brdView = new BrdView(this);
  setCentralWidget(brdView);

  setAcceptDrops(true);

  createToolBar();
  createDockWidget();
  createFilmSelectWidget();
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
  event->acceptProposedAction();
  const QMimeData* mimeData = event->mimeData();
  if (mimeData->hasUrls()) {
    QList<QUrl> urls = mimeData->urls();
    if (urls.length() == 1) {
      QUrl url = urls.first();
      if (url.isLocalFile()) {
        event->acceptProposedAction();
      }
    }
  }
}

void MainWindow::dropEvent(QDropEvent* event) {
  const QMimeData* mimeData = event->mimeData();
  QUrl url = mimeData->urls().first();
  qDebug() << "URL" << url.toLocalFile();

  loadFile(url.toLocalFile().toStdString());
}

void MainWindow::loadFile(std::string path) {
  auto parsed_file = parse_file(path);
  if (parsed_file) {
    fs = std::move(parsed_file);
    qDebug("File version: %08X -> \"%s\"", ntohl(fs->hdr->magic),
           fs->hdr->allegro_version);
    brdView->loadFile(&fs.value());
    loadFilms();
  } else {
    qDebug() << "Failed to parse file";
  }
}

void MainWindow::openFile() {
  QString filename = QFileDialog::getOpenFileName(this, "Open .brd file");
  loadFile(filename.toStdString());
}

void MainWindow::zoomIn() { brdView->zoomIn(); }

void MainWindow::zoomOut() { brdView->zoomOut(); }

void MainWindow::zoomFit() { brdView->zoomFit(); }

void MainWindow::createFilmSelectWidget() {
  QDockWidget* dock = new QDockWidget("Film Selector", this);
  dock->setAllowedAreas(Qt::RightDockWidgetArea);

  film_tree_widget_ = new QTreeWidget();
  film_tree_widget_->setColumnCount(1);
  film_tree_widget_->setHeaderLabels(QStringList(QString("Films")));
  film_tree_widget_->setSelectionMode(QAbstractItemView::MultiSelection);

  dock->setWidget(film_tree_widget_);

  addDockWidget(Qt::RightDockWidgetArea, dock);

  connect(film_tree_widget_, &QTreeWidget::itemSelectionChanged, this,
          &MainWindow::selectFilm);
}

void MainWindow::loadFilms() {
  layer_cache.clear();

  if (fs) {
    layer_model_ = new LayerModel(*fs);
    layer_tree_view_->setModel(layer_model_);

    connect(layer_tree_view_->selectionModel(),
            &QItemSelectionModel::selectionChanged, this,
            &MainWindow::selectionChanged);
  }

  film_tree_widget_->clear();

  std::vector<std::pair<std::string, uint32_t>> layers = film_list(fs.value());
  for (const auto& pair : layers) {
    layer_cache[pair.first] = pair.second;
    film_tree_widget_->insertTopLevelItem(
        0,
        new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr),
                            QStringList(QString::fromStdString(pair.first))));
  }
}

void MainWindow::createToolBar() {
  QToolBar* toolbar = new QToolBar("Main ToolBar");
  toolbar->setFloatable(false);
  toolbar->setMovable(false);

  const QIcon openIcon = QIcon::fromTheme("document-open");
  QAction* openAct = new QAction(openIcon, "Open", this);
  openAct->setShortcuts(QKeySequence::Open);
  connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
  toolbar->addAction(openAct);

  toolbar->addSeparator();

  QAction* zoomInAct = new QAction("Zoom In", this);
  connect(zoomInAct, &QAction::triggered, this, &MainWindow::zoomIn);
  toolbar->addAction(zoomInAct);

  QAction* zoomOutAct = new QAction("Zoom Out", this);
  connect(zoomOutAct, &QAction::triggered, this, &MainWindow::zoomOut);
  toolbar->addAction(zoomOutAct);

  QAction* zoomFitAct = new QAction("Zoom Fit", this);
  connect(zoomFitAct, &QAction::triggered, this, &MainWindow::zoomFit);
  toolbar->addAction(zoomFitAct);

  this->addToolBar(toolbar);
}

void MainWindow::createDockWidget() {
  QDockWidget* dock = new QDockWidget("Layer Selector", this);
  dock->setAllowedAreas(Qt::RightDockWidgetArea);

  // layer_model_ = new LayerModel("ABC");

  layer_tree_view_ = new QTreeView();
  layer_tree_view_->setSelectionMode(QAbstractItemView::MultiSelection);
  // layer_tree_view_->setModel(layer_model_);

  dock->setWidget(layer_tree_view_);

  addDockWidget(Qt::RightDockWidgetArea, dock);
}

void MainWindow::selectionChanged() {
  // Do not respond to selection change events while the other widget is being
  // updated
  if (updating_layers) {
    return;
  }
  updating_layers = true;

  const auto& selection_model = layer_tree_view_->selectionModel();
  std::set<std::pair<uint16_t, uint16_t>> layers{};

  // Deselect any films
  film_tree_widget_->selectionModel()->clearSelection();

  // We are done as soon as we've finished updating the other widget's selection
  updating_layers = false;

  for (const QModelIndex& index : selection_model->selectedIndexes()) {
    layers.insert(layer_model_->layer_pair(index));
  }

  qDebug("Redrawing");
  brdView->selectLayer(layers);
}

void MainWindow::selectFilm() {
  // Do not respond to selection change events while the other widget is being
  // updated
  if (updating_layers) {
    return;
  }
  updating_layers = true;

  std::set<std::pair<uint16_t, uint16_t>> layers{};

  for (QTreeWidgetItem* item : film_tree_widget_->selectedItems()) {
    QString t = item->text(0);

    // Lookup layers associated with this film
    uint32_t x38_k = layer_cache[t.toStdString()];
    const T38Film<kAMax>& film = fs->get_t38_film(x38_k);
    const T39FilmLayerList<kAMax>& layer_list = fs->get_x39(film.layer_list);
    for (const auto& layer : x39_layers(layer_list, fs.value())) {
      layers.insert(layer);
    }
  }

  // Select matching layers in layer selection
  QItemSelection selection;
  for (QModelIndex index : layer_model_->mapLayersToIndices(layers)) {
    selection.select(index, index);
  }
  layer_tree_view_->selectionModel()->select(
      selection, QItemSelectionModel::ClearAndSelect);

  // We are done as soon as we've finished updating the other widget's selection
  updating_layers = false;

  qDebug("Redrawing");
  brdView->selectLayer(layers);
}

void MainWindow::updatePosition(QPointF pos) {
  statusBar()->showMessage(
      QString("(%1, %2)").arg((int)pos.x()).arg((int)pos.y()));
}
