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

  void loadFile(File<kAMax> *fs);

  void zoomIn();
  void zoomOut();
  void zoomFit();

  void drawX01(const T01ArcSegment<kAMax> *inst, QPainterPath *path) const;
  void drawX05(const T05Line<kAMax> *inst, QPen *pen);
  void drawX0C(const T0CDrillIndicator<kAMax> *inst, QPen *pen);
  void drawX14(const T14Path<kAMax> *inst, QPen *pen);
  void drawX15(const T15LineSegment<kAMax> *inst, QPainterPath *path) const;
  void drawX16(const T16LineSegment<kAMax> *inst, QPainterPath *path) const;
  void drawX17(const T17LineSegment<kAMax> *inst, QPainterPath *path) const;
  void drawX23(const T23Rat<kAMax> *inst, QPen *pen);
  void drawX24(const T24Rectangle<kAMax> *inst, QPen *pen);
  void drawX28(const T28Shape<kAMax> *inst, QPen *pen);
  void drawX2B(const T2BSymbol<kAMax> *inst, QPen *pen);
  void drawX2D(const T2DSymbolInstance<kAMax> *inst, QPen *pen);
  void drawX30(const T30StringGraphic<kAMax> *inst, QPen *pen);
  void drawX32(const T32SymbolPin<kAMax> *inst, QPen *pen,
               uint32_t sym_rotation);
  void drawX33(const x33<kAMax> *inst, QPen *pen);
  void drawX34(const x34<kAMax> *inst, QPen *pen);
  void drawShape(uint32_t ptr, QPen *pen);
  void drawFile();

  void selectLayer(const std::set<std::pair<uint16_t, uint16_t>> &layers);

  auto drewKey(uint32_t ptr) -> bool;

  enum layer_choice { ALL = -1, FAB = -2 };

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

 private:
  auto customPenColor(uint32_t x05_k, QColor default_) -> QColor;
  auto isLineSegment(uint32_t k) -> bool;
  auto startingPoint(uint32_t k) -> std::optional<QPointF>;
  auto endingPoint(uint32_t k) -> std::optional<QPointF>;
  auto onSelectedLayer(uint8_t subtype, uint16_t layer) -> bool;
  auto netName(uint32_t k) -> char *;

  void updatePathWidth(QPainterPath *path, QPen **pen, QPen *base_pen,
                       uint32_t *prev_width, uint32_t new_width);

  void drawDrillSymbol(uint32_t k, DrillSymbol symbol, const char *label,
                       int32_t w, int32_t h, QPointF center, uint32_t rotation,
                       QPen &pen);

  QGraphicsScene *scene;
  qreal centerX, centerY;
  std::unordered_set<uint32_t> already_drawn;

  File<kAMax> *fs;
  const double factor;
  std::set<std::pair<uint16_t, uint16_t>> selectedLayers;
};

#endif
