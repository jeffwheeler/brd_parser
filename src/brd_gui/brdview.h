#ifndef BRDVIEW_H
#define BRDVIEW_H

#include <QtDebug>
#include <QtWidgets>
#include <iostream>
#include <optional>
#include <unordered_set>
#include <utility>

#include "lib/parser/parser.h"
#include "mainwindow.h"

class BrdView : public QGraphicsView {
    Q_OBJECT

   public:
    explicit BrdView(MainWindow *parent = nullptr);

    void loadFile(File<A_175> *fs);

    void zoomIn();
    void zoomOut();
    void zoomFit();

    void drawX01(const x01<A_175> *inst, QPainterPath *path);
    void drawX05(const x05<A_175> *inst, QPen *pen);
    void drawX14(const x14<A_175> *inst, QPen *pen);
    void drawX15(const x15<A_175> *inst, QPainterPath *path);
    void drawX16(const x16<A_175> *inst, QPainterPath *path);
    void drawX17(const x17<A_175> *inst, QPainterPath *path);
    void drawX23(const x23<A_175> *inst, QPen *pen);
    void drawX28(const x28<A_175> *inst, QPen *pen);
    void drawX2B(const x2B<A_175> *inst, QPen *pen);
    void drawX2D(const x2D<A_175> *inst, QPen *pen);
    void drawX30(const x30<A_175> *inst, QPen *pen);
    void drawX32(const x32<A_175> *inst, QPen *pen, uint32_t sym_rotation);
    void drawX33(const x33<A_175> *inst, QPen *pen);
    void drawX34(const x34<A_175> *inst, QPen *pen);
    void drawShape(const uint32_t ptr, QPen *pen);
    void drawFile();

    void selectLayer(std::vector<std::pair<uint16_t, uint16_t>> layers);

    enum layer_choice { ALL = -1, FAB = -2 };

   protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

   private:
    QColor customPenColor(uint32_t x05_k, QColor default_);
    bool isLineSegment(uint32_t k);
    std::optional<QPointF> startingPoint(uint32_t k);
    std::optional<QPointF> endingPoint(uint32_t k);
    bool onSelectedLayer(uint8_t subtype, uint16_t layer);
    char *netName(uint32_t k);

    void updatePathWidth(QPainterPath *path, QPen **pen, QPen *base_pen,
                         uint32_t *prev_width, uint32_t new_width);

    QGraphicsScene *scene;
    qreal centerX, centerY;
    std::unordered_set<uint32_t> already_drawn;

    File<A_175> *fs;
    const double factor;
    std::vector<std::pair<uint16_t, uint16_t>> selectedLayers;
};

#endif
