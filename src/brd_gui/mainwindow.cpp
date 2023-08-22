#include "mainwindow.h"

#include "brdview.h"
#include "lib/printing/printers.h"
#include "lib/structure/utils.h"
#include "moc_mainwindow.cpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    qDebug() << "MainWindow constructor";
    brdView = new BrdView(this);
    setCentralWidget(brdView);

    setAcceptDrops(true);

    createToolBar();
    // createDockWidget();
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

    tree = new QTreeWidget();
    tree->setColumnCount(1);
    tree->setHeaderLabels(QStringList(QString("Films")));
    tree->setSelectionMode(QAbstractItemView::MultiSelection);

    dock->setWidget(tree);

    addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(tree, &QTreeWidget::itemSelectionChanged, this,
            &MainWindow::selectFilm);
}

void MainWindow::loadFilms() {
    layer_cache.clear();
    tree->clear();

    std::vector<std::pair<std::string, uint32_t>> layers =
        layer_list(fs.value());
    for (const auto& pair : layers) {
        layer_cache[pair.first] = pair.second;
        tree->insertTopLevelItem(
            0, new QTreeWidgetItem(
                   static_cast<QTreeWidget*>(nullptr),
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

    tree = new QTreeWidget();
    tree->setColumnCount(1);
    tree->setHeaderLabels(QStringList(QString("Layers")));
    tree->setSelectionMode(QAbstractItemView::MultiSelection);

    QTreeWidgetItem* st01 = createTopLevelItem(QString("1"));
    QTreeWidgetItem* st03 = createTopLevelItem(QString("3"));
    QTreeWidgetItem* st04 = createTopLevelItem(QString("4 Fab/Assy?"));
    QTreeWidgetItem* st06 = createTopLevelItem(QString("6 Etch?"));
    QTreeWidgetItem* st07 = createTopLevelItem(QString("7"));
    QTreeWidgetItem* st09 = createTopLevelItem(QString("9"));
    QTreeWidgetItem* st0C = createTopLevelItem(QString("C"));
    QTreeWidgetItem* st0D = createTopLevelItem(QString("D"));
    QTreeWidgetItem* st0F = createTopLevelItem(QString("F"));
    QTreeWidgetItem* st12 = createTopLevelItem(QString("12"));
    QTreeWidgetItem* st13 = createTopLevelItem(QString("13"));
    QTreeWidgetItem* st14 = createTopLevelItem(QString("14 Voids?"));
    QTreeWidgetItem* st17 = createTopLevelItem(QString("17"));
    QTreeWidgetItem* stFF = createTopLevelItem(QString("All"));
    stFF->setFlags(stFF->flags() | Qt::ItemIsSelectable);

    tree->insertTopLevelItem(0, st01);
    tree->insertTopLevelItem(1, st03);
    tree->insertTopLevelItem(2, st04);
    tree->insertTopLevelItem(3, st06);
    tree->insertTopLevelItem(4, st07);
    tree->insertTopLevelItem(5, st09);
    tree->insertTopLevelItem(6, st0C);
    tree->insertTopLevelItem(7, st0D);
    tree->insertTopLevelItem(8, st0F);
    tree->insertTopLevelItem(9, st12);
    tree->insertTopLevelItem(10, st13);
    tree->insertTopLevelItem(11, st14);
    tree->insertTopLevelItem(12, st17);
    tree->insertTopLevelItem(13, stFF);

    addLayer(st01, QString("1 - 1"));
    addLayer(st01, QString("1 - 3"));
    addLayer(st01, QString("1 - 5"));
    addLayer(st01, QString("1 - 15"));
    addLayer(st01, QString("1 - 20"));
    addLayer(st01, QString("1 - 22"));
    addLayer(st01, QString("1 - 238"));
    addLayer(st01, QString("1 - 240"));
    addLayer(st01, QString("1 - 241"));
    addLayer(st01, QString("1 - 249 (Fab dims)"));
    addLayer(st01, QString("1 - 251 (ASSY?)"));
    addLayer(st01, QString("1 - 253 (Design outline?)"));
    addLayer(st01, QString("1 - All"));
    addLayer(st03, QString("3 - 0"));
    addLayer(st03, QString("3 - 1"));
    addLayer(st03, QString("3 - 250"));
    addLayer(st03, QString("3 - 252"));
    addLayer(st03, QString("3 - 253"));
    addLayer(st03, QString("3 - 353"));
    addLayer(st03, QString("3 - All"));
    addLayer(st04, QString("4 - 0 (Fab notes?)"));
    addLayer(st04, QString("4 - 1"));
    addLayer(st04, QString("4 - 2"));
    addLayer(st04, QString("4 - 5"));
    addLayer(st04, QString("4 - 23 (Assy notes?)"));
    addLayer(st04, QString("4 - 27"));
    addLayer(st04, QString("4 - 28"));
    addLayer(st04, QString("4 - 29"));
    addLayer(st04, QString("4 - 250 (Rev history?)"));
    addLayer(st04, QString("4 - 251"));
    addLayer(st04, QString("4 - 252 (Title block?)"));
    addLayer(st04, QString("4 - 253 (Page frame?)"));
    addLayer(st04, QString("4 - All"));
    addLayer(st06, QString("6 - 0"));
    addLayer(st06, QString("6 - 1"));
    addLayer(st06, QString("6 - 2"));
    addLayer(st06, QString("6 - 3"));
    addLayer(st06, QString("6 - 4"));
    addLayer(st06, QString("6 - 5"));
    addLayer(st06, QString("6 - 6"));
    addLayer(st06, QString("6 - 7"));
    addLayer(st06, QString("6 - 8"));
    addLayer(st06, QString("6 - 9"));
    addLayer(st06, QString("6 - 10"));
    addLayer(st06, QString("6 - 11"));
    addLayer(st06, QString("6 - 12"));
    addLayer(st06, QString("6 - 13"));
    addLayer(st06, QString("6 - 14"));
    addLayer(st06, QString("6 - 15"));
    addLayer(st06, QString("6 - All"));
    addLayer(st07, QString("7 - 1"));
    addLayer(st07, QString("7 - 4"));
    addLayer(st07, QString("7 - 18"));
    addLayer(st07, QString("7 - 241"));
    addLayer(st07, QString("7 - 242"));
    addLayer(st07, QString("7 - 243"));
    addLayer(st07, QString("7 - 244 (ASSY?)"));
    addLayer(st07, QString("7 - All"));
    addLayer(st09, QString("9 - 237 (Pmask top?)"));
    addLayer(st09, QString("9 - 241 (Something bottom?)"));
    addLayer(st09, QString("9 - 242 (Something top?)"));
    addLayer(st09, QString("9 - 244"));
    addLayer(st09, QString("9 - 246 (SM bottom?)"));
    addLayer(st09, QString("9 - 247 (SM top?)"));
    addLayer(st09, QString("9 - 249"));
    addLayer(st09, QString("9 - 250 (Place KO bot?)"));
    addLayer(st09, QString("9 - 251 (Place KO top?)"));
    addLayer(st09, QString("9 - 252 (ASSY bottom?)"));
    addLayer(st09, QString("9 - 253 (ASSY top?)"));
    addLayer(st09, QString("9 - All"));
    addLayer(st0C, QString("C - 0"));
    addLayer(st0C, QString("C - 1"));
    addLayer(st0C, QString("C - 2"));
    addLayer(st0C, QString("C- 128 (Holes?)"));
    addLayer(st0C, QString("C - 251"));
    addLayer(st0C, QString("C - All"));
    addLayer(st0D, QString("D - 249"));
    addLayer(st0D, QString("D - 250 (SM bottom?)"));
    addLayer(st0D, QString("D - 250 (SM bottom?)"));
    addLayer(st0D, QString("D - 251 (SM top?)"));
    addLayer(st0D, QString("D - 252 (ASSY bottom?)"));
    addLayer(st0D, QString("D - 253 (ASSY top?)"));
    addLayer(st0D, QString("D - All"));
    addLayer(st0F, QString("F - 253"));
    addLayer(st0F, QString("F - All"));
    addLayer(st12, QString("12 - 0"));
    addLayer(st12, QString("12 - 2"));
    addLayer(st12, QString("12 - 4"));
    addLayer(st12, QString("12 - 8"));
    addLayer(st12, QString("12 - 16"));
    addLayer(st12, QString("12 - 17"));
    addLayer(st12, QString("12 - 20"));
    addLayer(st12, QString("12 - 24"));
    addLayer(st12, QString("12 - 48"));
    addLayer(st12, QString("12 - 128"));
    addLayer(st12, QString("12 - 144"));
    addLayer(st12, QString("12 - 253"));
    addLayer(st12, QString("12 - All"));
    addLayer(st13, QString("13 - 0"));
    addLayer(st13, QString("13 - 7"));
    addLayer(st13, QString("13 - 251"));
    addLayer(st13, QString("13 - 253"));
    addLayer(st13, QString("13 - All"));
    addLayer(st14, QString("14 - 0 (Fab notes?)"));
    addLayer(st14, QString("14 - 1"));
    addLayer(st14, QString("14 - 2"));
    addLayer(st14, QString("14 - 3"));
    addLayer(st14, QString("14 - 4"));
    addLayer(st14, QString("14 - 5"));
    addLayer(st14, QString("14 - 6"));
    addLayer(st14, QString("14 - All"));
    addLayer(st17, QString("17 - 253"));
    addLayer(st17, QString("17 - All"));

    st01->setExpanded(true);
    st03->setExpanded(true);
    st04->setExpanded(true);
    st06->setExpanded(true);
    st07->setExpanded(true);
    st09->setExpanded(true);
    st0C->setExpanded(true);
    st0D->setExpanded(true);
    st0F->setExpanded(true);
    st12->setExpanded(true);
    st13->setExpanded(true);
    st14->setExpanded(true);
    st17->setExpanded(true);

    dock->setWidget(tree);

    addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(tree, &QTreeWidget::itemSelectionChanged, this,
            &MainWindow::selectLayer);
}

QTreeWidgetItem* MainWindow::createTopLevelItem(QString s) {
    QTreeWidgetItem* item =
        new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr), QStringList(s));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    return item;
}

void MainWindow::addLayer(QTreeWidgetItem* parent, QString s) {
    QTreeWidgetItem* item = new QTreeWidgetItem(parent, QStringList(s));
    tree->insertTopLevelItem(0, item);
}

void MainWindow::selectLayer() {
    std::vector<std::pair<uint16_t, uint16_t>> layers{};
    for (QTreeWidgetItem* item : tree->selectedItems()) {
        QString t = item->text(0);
        if (t == "1 - 1") {
            layers.push_back(layerPair(1, 1));
        } else if (t == "1 - 3") {
            layers.push_back(layerPair(1, 3));
        } else if (t == "1 - 5") {
            layers.push_back(layerPair(1, 5));
        } else if (t == "1 - 15") {
            layers.push_back(layerPair(1, 15));
        } else if (t == "1 - 20") {
            layers.push_back(layerPair(1, 20));
        } else if (t == "1 - 22") {
            layers.push_back(layerPair(1, 22));
        } else if (t == "1 - 238") {
            layers.push_back(layerPair(1, 238));
        } else if (t == "1 - 240") {
            layers.push_back(layerPair(1, 240));
        } else if (t == "1 - 241") {
            layers.push_back(layerPair(1, 241));
        } else if (t == "1 - 249 (Fab dims)") {
            layers.push_back(layerPair(1, 249));
        } else if (t == "1 - 251 (ASSY?)") {
            layers.push_back(layerPair(1, 251));
        } else if (t == "1 - 253 (Design outline?)") {
            layers.push_back(layerPair(1, 253));
        } else if (t == "1 - All") {
            layers.push_back(layerPair(1, 0x100));
        } else if (t == "3 - 0") {
            layers.push_back(layerPair(3, 0));
        } else if (t == "3 - 1") {
            layers.push_back(layerPair(3, 1));
        } else if (t == "3 - 250") {
            layers.push_back(layerPair(3, 250));
        } else if (t == "3 - 252") {
            layers.push_back(layerPair(3, 252));
        } else if (t == "3 - 253") {
            layers.push_back(layerPair(3, 253));
        } else if (t == "3 - 253") {
            layers.push_back(layerPair(3, 253));
        } else if (t == "3 - All") {
            layers.push_back(layerPair(3, 0x100));
        } else if (t == "4 - 0 (Fab notes?)") {
            layers.push_back(layerPair(4, 0));
        } else if (t == "4 - 1") {
            layers.push_back(layerPair(4, 1));
        } else if (t == "4 - 2") {
            layers.push_back(layerPair(4, 2));
        } else if (t == "4 - 5") {
            layers.push_back(layerPair(4, 5));
        } else if (t == "4 - 23 (Assy notes?)") {
            layers.push_back(layerPair(4, 23));
        } else if (t == "4 - 27") {
            layers.push_back(layerPair(4, 27));
        } else if (t == "4 - 28") {
            layers.push_back(layerPair(4, 28));
        } else if (t == "4 - 29") {
            layers.push_back(layerPair(4, 29));
        } else if (t == "4 - 250 (Rev history?)") {
            layers.push_back(layerPair(4, 250));
        } else if (t == "4 - 251") {
            layers.push_back(layerPair(4, 251));
        } else if (t == "4 - 252 (Title block?)") {
            layers.push_back(layerPair(4, 252));
        } else if (t == "4 - 253 (Page frame?)") {
            layers.push_back(layerPair(4, 253));
        } else if (t == "4 - All") {
            layers.push_back(layerPair(4, 0x100));
        } else if (t == "6 - 0") {
            layers.push_back(layerPair(6, 0));
        } else if (t == "6 - 1") {
            layers.push_back(layerPair(6, 1));
        } else if (t == "6 - 2") {
            layers.push_back(layerPair(6, 2));
        } else if (t == "6 - 3") {
            layers.push_back(layerPair(6, 3));
        } else if (t == "6 - 4") {
            layers.push_back(layerPair(6, 4));
        } else if (t == "6 - 5") {
            layers.push_back(layerPair(6, 5));
        } else if (t == "6 - 6") {
            layers.push_back(layerPair(6, 6));
        } else if (t == "6 - 7") {
            layers.push_back(layerPair(6, 7));
        } else if (t == "6 - 8") {
            layers.push_back(layerPair(6, 8));
        } else if (t == "6 - 9") {
            layers.push_back(layerPair(6, 9));
        } else if (t == "6 - 10") {
            layers.push_back(layerPair(6, 10));
        } else if (t == "6 - 11") {
            layers.push_back(layerPair(6, 11));
        } else if (t == "6 - 12") {
            layers.push_back(layerPair(6, 12));
        } else if (t == "6 - 13") {
            layers.push_back(layerPair(6, 13));
        } else if (t == "6 - 14") {
            layers.push_back(layerPair(6, 14));
        } else if (t == "6 - 15") {
            layers.push_back(layerPair(6, 15));
        } else if (t == "6 - All") {
            layers.push_back(layerPair(6, 0x100));
        } else if (t == "7 - 1") {
            layers.push_back(layerPair(7, 1));
        } else if (t == "7 - 4") {
            layers.push_back(layerPair(7, 4));
        } else if (t == "7 - 18") {
            layers.push_back(layerPair(7, 18));
        } else if (t == "7 - 241") {
            layers.push_back(layerPair(7, 241));
        } else if (t == "7 - 242") {
            layers.push_back(layerPair(7, 242));
        } else if (t == "7 - 243") {
            layers.push_back(layerPair(7, 243));
        } else if (t == "7 - 244 (ASSY?)") {
            layers.push_back(layerPair(7, 244));
        } else if (t == "7 - All") {
            layers.push_back(layerPair(7, 0x100));
        } else if (t == "9 - 237 (Pmask top?)") {
            layers.push_back(layerPair(9, 237));
        } else if (t == "9 - 241 (Something bottom?)") {
            layers.push_back(layerPair(9, 241));
        } else if (t == "9 - 242 (Something top?)") {
            layers.push_back(layerPair(9, 242));
        } else if (t == "9 - 244") {
            layers.push_back(layerPair(9, 244));
        } else if (t == "9 - 246 (SM bottom?)") {
            layers.push_back(layerPair(9, 246));
        } else if (t == "9 - 247 (SM top?)") {
            layers.push_back(layerPair(9, 247));
        } else if (t == "9 - 249") {
            layers.push_back(layerPair(9, 249));
        } else if (t == "9 - 250 (Place KO bot?)") {
            layers.push_back(layerPair(9, 250));
        } else if (t == "9 - 251 (Place KO top?)") {
            layers.push_back(layerPair(9, 251));
        } else if (t == "9 - 252 (ASSY bottom?)") {
            layers.push_back(layerPair(9, 252));
        } else if (t == "9 - 253 (ASSY top?)") {
            layers.push_back(layerPair(9, 253));
        } else if (t == "9 - All") {
            layers.push_back(layerPair(9, 0x100));
        } else if (t == "C - 0") {
            layers.push_back(layerPair(0xC, 0));
        } else if (t == "C - 1") {
            layers.push_back(layerPair(0xC, 1));
        } else if (t == "C - 2") {
            layers.push_back(layerPair(0xC, 2));
        } else if (t == "C- 128 (Holes?)") {
            layers.push_back(layerPair(0xC, 128));
        } else if (t == "C - 251") {
            layers.push_back(layerPair(0xC, 251));
        } else if (t == "C - All") {
            layers.push_back(layerPair(0xC, 0x100));
        } else if (t == "D - 249") {
            layers.push_back(layerPair(0xD, 249));
        } else if (t == "D - 250 (SM bottom?)") {
            layers.push_back(layerPair(0xD, 250));
        } else if (t == "D - 251 (SM top?)") {
            layers.push_back(layerPair(0xD, 251));
        } else if (t == "D - 252 (ASSY bottom?)") {
            layers.push_back(layerPair(0xD, 252));
        } else if (t == "D - 253 (ASSY top?)") {
            layers.push_back(layerPair(0xD, 253));
        } else if (t == "D - All") {
            layers.push_back(layerPair(0xD, 0x100));
        } else if (t == "F - 253") {
            layers.push_back(layerPair(0xF, 253));
        } else if (t == "F - All") {
            layers.push_back(layerPair(0xF, 0x100));
        } else if (t == "12 - 0") {
            layers.push_back(layerPair(0x12, 0));
        } else if (t == "12 - 2") {
            layers.push_back(layerPair(0x12, 2));
        } else if (t == "12 - 4") {
            layers.push_back(layerPair(0x12, 4));
        } else if (t == "12 - 8") {
            layers.push_back(layerPair(0x12, 8));
        } else if (t == "12 - 16") {
            layers.push_back(layerPair(0x12, 16));
        } else if (t == "12 - 17") {
            layers.push_back(layerPair(0x12, 17));
        } else if (t == "12 - 20") {
            layers.push_back(layerPair(0x12, 20));
        } else if (t == "12 - 24") {
            layers.push_back(layerPair(0x12, 24));
        } else if (t == "12 - 48") {
            layers.push_back(layerPair(0x12, 48));
        } else if (t == "12 - 128") {
            layers.push_back(layerPair(0x12, 128));
        } else if (t == "12 - 144") {
            layers.push_back(layerPair(0x12, 144));
        } else if (t == "12 - 253") {
            layers.push_back(layerPair(0x12, 253));
        } else if (t == "12 - All") {
            layers.push_back(layerPair(0x12, 0x100));
        } else if (t == "13 - 0") {
            layers.push_back(layerPair(0x13, 0));
        } else if (t == "13 - 7") {
            layers.push_back(layerPair(0x13, 7));
        } else if (t == "13 - 251") {
            layers.push_back(layerPair(0x13, 253));
        } else if (t == "13 - 253") {
            layers.push_back(layerPair(0x13, 253));
        } else if (t == "13 - All") {
            layers.push_back(layerPair(0x13, 0x100));
        } else if (t == "14 - 0 (Fab notes?)") {
            layers.push_back(layerPair(0x14, 0));
        } else if (t == "14 - 1") {
            layers.push_back(layerPair(0x14, 1));
        } else if (t == "14 - 2") {
            layers.push_back(layerPair(0x14, 2));
        } else if (t == "14 - 3") {
            layers.push_back(layerPair(0x14, 3));
        } else if (t == "14 - 4") {
            layers.push_back(layerPair(0x14, 4));
        } else if (t == "14 - 5") {
            layers.push_back(layerPair(0x14, 5));
        } else if (t == "14 - 6") {
            layers.push_back(layerPair(0x14, 6));
        } else if (t == "14 - All") {
            layers.push_back(layerPair(0x14, 0x100));
        } else if (t == "17 - 253") {
            layers.push_back(layerPair(0x17, 253));
        } else if (t == "17 - All") {
            layers.push_back(layerPair(0x17, 0x100));
        } else if (t == "All") {
            layers.push_back(layerPair(0x100, 0x100));
        }
    }
    qDebug("Redrawing");
    brdView->selectLayer(layers);
}

void MainWindow::selectFilm() {
    std::vector<std::pair<uint16_t, uint16_t>> layers{};
    for (QTreeWidgetItem* item : tree->selectedItems()) {
        QString t = item->text(0);

        // Lookup layers associated with this film
        uint32_t x38_k = layer_cache[t.toStdString()];
        const x38<A_174>* x38_inst = (const x38<A_174>*)&fs->x38_map.at(x38_k);
        const x39<A_174>* x39_inst =
            (const x39<A_174>*)&fs->x39_map.at(x38_inst->ptr1);
        for (const auto& layer : x39_layers(*x39_inst, fs.value())) {
            layers.push_back(layerPair(layer.first, layer.second));
        }
    }
    qDebug("Redrawing");
    brdView->selectLayer(layers);
}

std::pair<uint16_t, uint16_t> MainWindow::layerPair(uint16_t x, uint16_t y) {
    return std::pair<uint16_t, uint16_t>(x, y);
}

void MainWindow::updatePosition(QPointF pos) {
    statusBar()->showMessage(
        QString("(%1, %2)").arg((int)pos.x()).arg((int)pos.y()));
}
