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

  layer_model_ = new LayerModel("ABC");

  layer_tree_view_ = new QTreeView();
  layer_tree_view_->setSelectionMode(QAbstractItemView::MultiSelection);
  layer_tree_view_->setModel(layer_model_);

  connect(layer_tree_view_->selectionModel(),
          &QItemSelectionModel::selectionChanged, this,
          &MainWindow::selectionChanged);

  // tree->setColumnCount(1);
  // tree->setHeaderLabels(QStringList(QString("Layers")));

  // QTreeWidgetItem* st01 = createTopLevelItem(QString("1"));
  // QTreeWidgetItem* st03 = createTopLevelItem(QString("3"));
  // QTreeWidgetItem* st04 = createTopLevelItem(QString("4 Fab/Assy?"));
  // QTreeWidgetItem* st06 = createTopLevelItem(QString("6 Etch?"));
  // QTreeWidgetItem* st07 = createTopLevelItem(QString("7"));
  // QTreeWidgetItem* st09 = createTopLevelItem(QString("9"));
  // QTreeWidgetItem* st0C = createTopLevelItem(QString("C"));
  // QTreeWidgetItem* st0D = createTopLevelItem(QString("D"));
  // QTreeWidgetItem* st0F = createTopLevelItem(QString("F"));
  // QTreeWidgetItem* st12 = createTopLevelItem(QString("12"));
  // QTreeWidgetItem* st13 = createTopLevelItem(QString("13"));
  // QTreeWidgetItem* st14 = createTopLevelItem(QString("14 Voids?"));
  // QTreeWidgetItem* st17 = createTopLevelItem(QString("17"));
  // QTreeWidgetItem* stFF = createTopLevelItem(QString("All"));
  // stFF->setFlags(stFF->flags() | Qt::ItemIsSelectable);

  // tree->insertTopLevelItem(0, st01);
  // tree->insertTopLevelItem(1, st03);
  // tree->insertTopLevelItem(2, st04);
  // tree->insertTopLevelItem(3, st06);
  // tree->insertTopLevelItem(4, st07);
  // tree->insertTopLevelItem(5, st09);
  // tree->insertTopLevelItem(6, st0C);
  // tree->insertTopLevelItem(7, st0D);
  // tree->insertTopLevelItem(8, st0F);
  // tree->insertTopLevelItem(9, st12);
  // tree->insertTopLevelItem(10, st13);
  // tree->insertTopLevelItem(11, st14);
  // tree->insertTopLevelItem(12, st17);
  // tree->insertTopLevelItem(13, stFF);

  // addLayer(st01, QString("1 - 1"));
  // addLayer(st01, QString("1 - 3"));
  // addLayer(st01, QString("1 - 5"));
  // addLayer(st01, QString("1 - 15"));
  // addLayer(st01, QString("1 - 20"));
  // addLayer(st01, QString("1 - 22"));
  // addLayer(st01, QString("1 - 238"));
  // addLayer(st01, QString("1 - 240"));
  // addLayer(st01, QString("1 - 241"));
  // addLayer(st01, QString("1 - 249 (Fab dims)"));
  // addLayer(st01, QString("1 - 251 (ASSY?)"));
  // addLayer(st01, QString("1 - 253 (Design outline?)"));
  // addLayer(st01, QString("1 - All"));
  // addLayer(st03, QString("3 - 0"));
  // addLayer(st03, QString("3 - 1"));
  // addLayer(st03, QString("3 - 250"));
  // addLayer(st03, QString("3 - 252"));
  // addLayer(st03, QString("3 - 253"));
  // addLayer(st03, QString("3 - 353"));
  // addLayer(st03, QString("3 - All"));
  // addLayer(st04, QString("4 - 0 (Fab notes?)"));
  // addLayer(st04, QString("4 - 1"));
  // addLayer(st04, QString("4 - 2"));
  // addLayer(st04, QString("4 - 5"));
  // addLayer(st04, QString("4 - 23 (Assy notes?)"));
  // addLayer(st04, QString("4 - 27"));
  // addLayer(st04, QString("4 - 28"));
  // addLayer(st04, QString("4 - 29"));
  // addLayer(st04, QString("4 - 250 (Rev history?)"));
  // addLayer(st04, QString("4 - 251"));
  // addLayer(st04, QString("4 - 252 (Title block?)"));
  // addLayer(st04, QString("4 - 253 (Page frame?)"));
  // addLayer(st04, QString("4 - All"));
  // addLayer(st06, QString("6 - 0"));
  // addLayer(st06, QString("6 - 1"));
  // addLayer(st06, QString("6 - 2"));
  // addLayer(st06, QString("6 - 3"));
  // addLayer(st06, QString("6 - 4"));
  // addLayer(st06, QString("6 - 5"));
  // addLayer(st06, QString("6 - 6"));
  // addLayer(st06, QString("6 - 7"));
  // addLayer(st06, QString("6 - 8"));
  // addLayer(st06, QString("6 - 9"));
  // addLayer(st06, QString("6 - 10"));
  // addLayer(st06, QString("6 - 11"));
  // addLayer(st06, QString("6 - 12"));
  // addLayer(st06, QString("6 - 13"));
  // addLayer(st06, QString("6 - 14"));
  // addLayer(st06, QString("6 - 15"));
  // addLayer(st06, QString("6 - All"));
  // addLayer(st07, QString("7 - 1"));
  // addLayer(st07, QString("7 - 4"));
  // addLayer(st07, QString("7 - 5"));
  // addLayer(st07, QString("7 - 12"));
  // addLayer(st07, QString("7 - 18"));
  // addLayer(st07, QString("7 - 20"));
  // addLayer(st07, QString("7 - 21"));
  // addLayer(st07, QString("7 - 24"));
  // addLayer(st07, QString("7 - 26"));
  // addLayer(st07, QString("7 - 27"));
  // addLayer(st07, QString("7 - 241"));
  // addLayer(st07, QString("7 - 242"));
  // addLayer(st07, QString("7 - 243"));
  // addLayer(st07, QString("7 - 244 (ASSY?)"));
  // addLayer(st07, QString("7 - All"));
  // addLayer(st09, QString("9 - 237 (Pmask top?)"));
  // addLayer(st09, QString("9 - 241 (Something bottom?)"));
  // addLayer(st09, QString("9 - 242 (Something top?)"));
  // addLayer(st09, QString("9 - 244"));
  // addLayer(st09, QString("9 - 246 (SM bottom?)"));
  // addLayer(st09, QString("9 - 247 (SM top?)"));
  // addLayer(st09, QString("9 - 249"));
  // addLayer(st09, QString("9 - 250 (Place KO bot?)"));
  // addLayer(st09, QString("9 - 251 (Place KO top?)"));
  // addLayer(st09, QString("9 - 252 (ASSY bottom?)"));
  // addLayer(st09, QString("9 - 253 (ASSY top?)"));
  // addLayer(st09, QString("9 - All"));
  // addLayer(st0C, QString("C - 0"));
  // addLayer(st0C, QString("C - 1"));
  // addLayer(st0C, QString("C - 2"));
  // addLayer(st0C, QString("C- 128 (Holes?)"));
  // addLayer(st0C, QString("C - 251"));
  // addLayer(st0C, QString("C - All"));
  // addLayer(st0D, QString("D - 249"));
  // addLayer(st0D, QString("D - 250 (SM bottom?)"));
  // addLayer(st0D, QString("D - 250 (SM bottom?)"));
  // addLayer(st0D, QString("D - 251 (SM top?)"));
  // addLayer(st0D, QString("D - 252 (ASSY bottom?)"));
  // addLayer(st0D, QString("D - 253 (ASSY top?)"));
  // addLayer(st0D, QString("D - All"));
  // addLayer(st0F, QString("F - 253"));
  // addLayer(st0F, QString("F - All"));
  // addLayer(st12, QString("12 - 0"));
  // addLayer(st12, QString("12 - 2"));
  // addLayer(st12, QString("12 - 4"));
  // addLayer(st12, QString("12 - 8"));
  // addLayer(st12, QString("12 - 16"));
  // addLayer(st12, QString("12 - 17"));
  // addLayer(st12, QString("12 - 20"));
  // addLayer(st12, QString("12 - 24"));
  // addLayer(st12, QString("12 - 48"));
  // addLayer(st12, QString("12 - 128"));
  // addLayer(st12, QString("12 - 144"));
  // addLayer(st12, QString("12 - 253"));
  // addLayer(st12, QString("12 - All"));
  // addLayer(st13, QString("13 - 0"));
  // addLayer(st13, QString("13 - 7"));
  // addLayer(st13, QString("13 - 251"));
  // addLayer(st13, QString("13 - 253"));
  // addLayer(st13, QString("13 - All"));
  // addLayer(st14, QString("14 - 0 (Fab notes?)"));
  // addLayer(st14, QString("14 - 1"));
  // addLayer(st14, QString("14 - 2"));
  // addLayer(st14, QString("14 - 3"));
  // addLayer(st14, QString("14 - 4"));
  // addLayer(st14, QString("14 - 5"));
  // addLayer(st14, QString("14 - 6"));
  // addLayer(st14, QString("14 - All"));
  // addLayer(st17, QString("17 - 253"));
  // addLayer(st17, QString("17 - All"));

  // st01->setExpanded(true);
  // st03->setExpanded(true);
  // st04->setExpanded(true);
  // st06->setExpanded(true);
  // st07->setExpanded(true);
  // st09->setExpanded(true);
  // st0C->setExpanded(true);
  // st0D->setExpanded(true);
  // st0F->setExpanded(true);
  // st12->setExpanded(true);
  // st13->setExpanded(true);
  // st14->setExpanded(true);
  // st17->setExpanded(true);

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
    const t38_film<kAMax>& film = fs->get_t38_film(x38_k);
    const t39_film_layer_list<kAMax>& layer_list = fs->get_x39(film.layer_list);
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
