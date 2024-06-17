#include "brdview.h"

#include "lib/printing/printers.h"
#include "lib/structure/utils.h"
#include "moc_brdview.cpp"

BrdView::BrdView(MainWindow *parent)
    : QGraphicsView((QWidget *)parent), factor(1000) {
  qDebug() << "BrdView::BrdView";
  // This hides some annoying warnings printed to the console
  viewport()->setAttribute(Qt::WA_AcceptTouchEvents, false);

  scene = new QGraphicsScene();
  scene->setBackgroundBrush(QColor(20, 20, 20));

  scale(1, -1);

  setScene(scene);

  centerX = 0;
  centerY = 0;
  centerOn(centerX, centerY);

  // drawFile();

  if (parent != nullptr) {
    ((MainWindow *)parent)->updatePosition(QPointF(0, 0));
  }
};

void BrdView::loadFile(File<kAMax> *fs) {
  qDebug() << "BrdView::loadFile";
  this->fs = fs;
  drawFile();
}

void BrdView::zoomIn() { scale(1.2, 1.2); }

void BrdView::zoomOut() { scale(1 / 1.2, 1 / 1.2); }

void BrdView::zoomFit() { fitInView(scene->sceneRect(), Qt::KeepAspectRatio); }

bool BrdView::drewKey(const uint32_t ptr) {
  return already_drawn.count(ptr) > 0;
}

void BrdView::keyPressEvent(QKeyEvent *event) {
  qint32 k = event->key();

  if (k == Qt::Key_I) {
    zoomIn();
  } else if (k == Qt::Key_O) {
    zoomOut();
  } else if (k == Qt::Key_F) {
    zoomFit();
  }
};

void BrdView::mouseMoveEvent(QMouseEvent *event) {
  const QPointF pos = mapToScene(event->pos());
  ((MainWindow *)parent())->updatePosition(pos * factor);
}

void BrdView::mouseReleaseEvent(QMouseEvent *event) {
  const QPointF pos = mapToScene(event->pos());
  const qreal r = 0.1;
  QList<QGraphicsItem *> clickedItems =
      scene->items(QRectF(pos.x() - r, pos.y() - r, 2 * r, 2 * r),
                   Qt::IntersectsItemShape, Qt::AscendingOrder);
  std::printf(
      "\x1b[35m-----------------------------------------------\x1b[0m\n");
  for (auto &item : clickedItems) {
    const int ptr = item->data(0).toInt();
    if (fs->is_type(ptr, 0x32)) {
      // const x32<A_MAX> &inst = fs->get_x32(ptr);
      print_struct((const uint32_t)ptr, *fs, 0);
      // qDebug("Printing x32->ptr5");
      // print_struct((const uint32_t)inst->ptr5, fs, 0);
    } else {
      print_struct((const uint32_t)ptr, *fs, 0);
    }
    std::printf("\n\n");
  }
};

void BrdView::drawX01(const T01ArcSegment<kAMax> *inst, QPainterPath *path) {
  std::pair<int32_t, int32_t> center = x01_center(inst);
  double r = cfp_to_double(inst->r);

  /*
  scene->addEllipse(
      (center.first-r)/factor,
      (center.second-r)/factor,
      r/factor*2,
      r/factor*2,
      QPen(
          inst->subtype == 0x40
              ? QColorConstants::Svg::deeppink
              : QColorConstants::Svg::orange,
          0, Qt::DotLine
      )
  );
  */

  QLineF start_line =
      QLineF(center.first, center.second, inst->coords[0], inst->coords[1]);
  QLineF end_line =
      QLineF(center.first, center.second, inst->coords[2], inst->coords[3]);

  path->arcTo((center.first - r) / factor, (center.second - r) / factor,
              r / factor * 2, r / factor * 2, start_line.angle(),
              inst->subtype == 0x00 ? -end_line.angleTo(start_line)
                                    : start_line.angleTo(end_line));
};

void BrdView::updatePathWidth(QPainterPath *path, QPen **pen, QPen *base_pen,
                              uint32_t *prev_width, uint32_t new_width) {
  if (*pen == nullptr) {
    // This is the first time creating the pen, so use this
    // instance's width.
    *pen = new QPen(base_pen->color(), new_width / factor, Qt::SolidLine,
                    Qt::RoundCap);
    *prev_width = new_width;
  } else if (*prev_width != new_width) {
    scene->addPath(*path, **pen);
    QPointF p = path->currentPosition();
    *path = QPainterPath();
    path->moveTo(p);
    *pen = new QPen(base_pen->color(), new_width / factor, Qt::SolidLine,
                    Qt::RoundCap);
    *prev_width = new_width;
  }
}

void BrdView::drawX05(const T05Line<kAMax> *inst, QPen *pen_) {
  if (!onSelectedLayer(inst->subtype, inst->layer)) {
    return;
  }

  uint32_t k = inst->first_segment_ptr;

  QPen *pen = new QPen();
  uint32_t prev_width = -1;
  QPainterPath path;
  path.moveTo(*startingPoint(k) * 1.0 / factor);

  while (isLineSegment(k)) {
    if (fs->is_type(k, 0x01)) {
      const T01ArcSegment<kAMax> segment_inst = fs->get_x01(k);
      updatePathWidth(&path, &pen, pen_, &prev_width, segment_inst.width);
      drawX01(&segment_inst, &path);
      k = segment_inst.next;
    } else if (fs->is_type(k, 0x15)) {
      const T15LineSegment<kAMax> segment_inst = fs->get_x15(k);
      updatePathWidth(&path, &pen, pen_, &prev_width, segment_inst.width);
      drawX15(&segment_inst, &path);
      k = segment_inst.next;
    } else if (fs->is_type(k, 0x16)) {
      const T16LineSegment<kAMax> segment_inst = fs->get_x16(k);
      updatePathWidth(&path, &pen, pen_, &prev_width, segment_inst.width);
      drawX16(&segment_inst, &path);
      k = segment_inst.next;
    } else if (fs->is_type(k, 0x17)) {
      const T17LineSegment<kAMax> segment_inst = fs->get_x17(k);
      updatePathWidth(&path, &pen, pen_, &prev_width, segment_inst.width);
      drawX17(&segment_inst, &path);
      k = segment_inst.next;
    } else {
      return;
    }
  }

  QGraphicsItem *item = scene->addPath(path, *pen);
  item->setData(0, inst->k);
}

void BrdView::drawX0C(const T0CDrillIndicator<kAMax> *inst, QPen *pen) {
  if (!onSelectedLayer(inst->subtype, inst->layer)) {
    return;
  }

  int32_t w = inst->coords[2], h = inst->coords[3];
  QPointF center = QPointF(inst->coords[0] / factor, inst->coords[1] / factor);

  if (inst->drill_chart_symbol != DrillSymbol::NoSymbol) {
    QTransform t = QTransform()
                       .translate(center.x(), center.y())
                       .rotate(inst->rotation / 1000.);
    QGraphicsItem *item = nullptr;
    if (inst->drill_chart_symbol == DrillSymbol::Circle ||
        inst->drill_chart_symbol == DrillSymbol::RoundedRect) {
      item = scene->addEllipse((-w / 2.) / factor, (-h / 2.) / factor,
                               w / factor, h / factor, *pen);
    } else {
      item = scene->addRect((-w / 2.) / factor, (-h / 2.) / factor, w / factor,
                            h / factor, *pen);
    }
    item->setTransform(t);
    item->setData(0, inst->k);
  }

  if (strlen(inst->label) > 0) {
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setFixedPitch(true);
    font.setKerning(false);
    font.setPixelSize(h / factor);
    QGraphicsTextItem *text =
        scene->addText(QString::fromStdString(inst->label), font);
    QRectF boundingBox = text->boundingRect();
    QTransform t =
        QTransform()
            .translate(center.x(), center.y())
            .scale(1, -1)
            .translate(boundingBox.width() * -0.5, boundingBox.height() * -0.5);

    text->setDefaultTextColor(pen->color());
    text->setTransform(t);
    text->setData(0, inst->k);
  }
}

void BrdView::drawX14(const T14Path<kAMax> *inst, QPen *pen_) {
  if (!onSelectedLayer(inst->subtype, inst->layer) ||
      fs->is_type(inst->ptr1, 0x2B)) {
    return;
  }

  uint32_t k = inst->ptr2;

  QPen *pen = nullptr;
  uint32_t prev_width = -1;
  QPainterPath path;
  path.moveTo(*startingPoint(k) * 1.0 / factor);

  while (isLineSegment(k)) {
    if (fs->is_type(k, 0x01)) {
      const T01ArcSegment<kAMax> segment_inst = fs->get_x01(k);
      updatePathWidth(&path, &pen, pen_, &prev_width, segment_inst.width);
      drawX01(&segment_inst, &path);
      k = segment_inst.next;
    } else if (fs->is_type(k, 0x15)) {
      const T15LineSegment<kAMax> segment_inst = fs->get_x15(k);
      updatePathWidth(&path, &pen, pen_, &prev_width, segment_inst.width);
      drawX15(&segment_inst, &path);
      k = segment_inst.next;
    } else if (fs->is_type(k, 0x16)) {
      const T16LineSegment<kAMax> segment_inst = fs->get_x16(k);
      updatePathWidth(&path, &pen, pen_, &prev_width, segment_inst.width);
      drawX16(&segment_inst, &path);
      k = segment_inst.next;
    } else if (fs->is_type(k, 0x17)) {
      const T17LineSegment<kAMax> segment_inst = fs->get_x17(k);
      updatePathWidth(&path, &pen, pen_, &prev_width, segment_inst.width);
      drawX17(&segment_inst, &path);
      k = segment_inst.next;
    } else {
      return;
    }
  }

  QGraphicsItem *item = scene->addPath(path, *pen);
  item->setData(0, inst->k);
}

void BrdView::drawX15(const T15LineSegment<kAMax> *inst, QPainterPath *path) {
  path->lineTo(inst->coords[2] / factor, inst->coords[3] / factor);

  // QGraphicsItem *item = scene->addLine(
  //     inst->coords[0]/factor, inst->coords[1]/factor,
  //     inst->coords[2]/factor, inst->coords[3]/factor,
  //     QPen(Qt::red)
  // );
  // item->setData(0, inst->k);
};

void BrdView::drawX16(const T16LineSegment<kAMax> *inst, QPainterPath *path) {
  path->lineTo(inst->coords[2] / factor, inst->coords[3] / factor);

  // QGraphicsItem *item = scene->addLine(
  //     inst->coords[0]/factor, inst->coords[1]/factor,
  //     inst->coords[2]/factor, inst->coords[3]/factor,
  //     QPen(Qt::green)
  // );
  // item->setData(0, inst->k);
};

void BrdView::drawX17(const T17LineSegment<kAMax> *inst, QPainterPath *path) {
  path->lineTo(inst->coords[2] / factor, inst->coords[3] / factor);

  // QGraphicsItem *item = scene->addLine(
  //     inst->coords[0]/factor, inst->coords[1]/factor,
  //     inst->coords[2]/factor, inst->coords[3]/factor,
  //     QPen(Qt::blue)
  // );
  // item->setData(0, inst->k);
}

// Connectivity (rat)
void BrdView::drawX23(const T23Rat<kAMax> *inst, QPen *pen) {
  if (!onSelectedLayer(inst->subtype, inst->layer)) {
    return;
  }

  QGraphicsItem *item = scene->addLine(
      (inst->coords[1]) / factor, (inst->coords[2]) / factor,
      (inst->coords[3]) / factor, (inst->coords[4]) / factor,
      QPen(inst->bitmask[0] == 0x20 ? QColorConstants::Svg::green
                                    : QColorConstants::Svg::orange,
           0, Qt::DotLine));
  item->setData(0, inst->k);
}

void BrdView::drawX24(const T24Rectangle<kAMax> *inst, QPen *pen) {
  if (!onSelectedLayer(inst->subtype, inst->layer)) {
    return;
  }

  QGraphicsItem *item =
      scene->addRect((inst->coords[0]) / factor, (inst->coords[1]) / factor,
                     (inst->coords[2] - inst->coords[0]) / factor,
                     (inst->coords[3] - inst->coords[1]) / factor, *pen);
  item->setData(0, inst->k);
}

// Shapes
void BrdView::drawX28(const T28Shape<kAMax> *inst, QPen *pen) {
  if (!onSelectedLayer(inst->subtype, inst->layer) ||
      fs->is_type(inst->ptr1, 0x2B)) {
    return;
  }

  char *name = netName(inst->k);

  QPen *customPen = new QPen(name != nullptr && (strcmp(name, "GND") == 0)
                                 ? Qt::green
                                 : customPenColor(inst->k, pen->color()),
                             pen->width());

  uint32_t k = inst->first_segment_ptr;

  QPainterPath path;
  path.setFillRule(Qt::WindingFill);
  path.moveTo(*startingPoint(k) * 1.0 / factor);

  while (isLineSegment(k)) {
    if (fs->is_type(k, 0x01)) {
      const T01ArcSegment<kAMax> segment_inst = fs->get_x01(k);
      drawX01(&segment_inst, &path);
      k = segment_inst.next;
    } else if (fs->is_type(k, 0x15)) {
      const T15LineSegment<kAMax> segment_inst = fs->get_x15(k);
      drawX15(&segment_inst, &path);
      k = segment_inst.next;
    } else if (fs->is_type(k, 0x16)) {
      const T16LineSegment<kAMax> segment_inst = fs->get_x16(k);
      drawX16(&segment_inst, &path);
      k = segment_inst.next;
    } else if (fs->is_type(k, 0x17)) {
      const T17LineSegment<kAMax> segment_inst = fs->get_x17(k);
      drawX17(&segment_inst, &path);
      k = segment_inst.next;
    } else {
      return;
    }
  }

  QColor lighterColor = QColor(customPen->color());
  lighterColor.setAlpha(127);

  QPen *polyPen;
  QBrush brush;
  switch (inst->subtype) {
    case 0x01:
      polyPen = new QPen(QColorConstants::Svg::indianred, 0.5);
      brush = QBrush();
      break;
    case 0x06:
      polyPen = customPen;
      brush = QBrush(lighterColor, Qt::FDiagPattern);
      break;
    case 0x09:
      // Something about pad regions?
      polyPen = new QPen(QColorConstants::Svg::olive, 0.5);
      brush = QBrush();
      break;
    case 0x0F:
      // FIXME: What is this?
      // return;
    case 0x13:
      // Something about pad regions?
      polyPen = new QPen(QColor(200, 200, 200, 100), 0.5);
      brush = QBrush();
      break;
    case 0x15:
      // Keepin
      return;
      polyPen = new QPen(Qt::white, 0.5);
      brush = QBrush();
      break;
    case 0x16:
      // DRC allowed region
      polyPen =
          new QPen(QColorConstants::Svg::lightslategray, 0.4, Qt::DotLine);
      brush = QBrush();
      break;
    default:
      polyPen = new QPen(Qt::yellow, 3);
      brush = QBrush();
  }

  QGraphicsItem *item = scene->addPath(path, *polyPen, brush);
  item->setData(0, inst->k);

  QPen *pen6 = new QPen(QColor(237, 211, 130, 127), 0);
  for (auto &i_x34 : fs->iter_x34(inst->k)) {
    drawShape(i_x34.k, pen6);
  }

  // Recurse if type(ptr2) == x28?
  // if (fs->is_type(inst->next, 0x28)) {
  //     drawShape(inst->next, pen);
  // }
};

void BrdView::drawX2B(const T2BSymbol<kAMax> *inst, QPen *pen) {
  // drawShape(inst->ptr2, pen);
  // drawShape(inst->ptr8, pen);
}

void BrdView::drawX2D(const T2DSymbolInstance<kAMax> *inst, QPen *pen) {
  for (auto &inst_x14 : fs->iter_x14(*inst)) {
    drawShape(inst_x14.k, pen);
  }

  // if (selectedLayer == 0 && inst->un2 != 0) {
  //     qDebug("Skipping 0x%08X, selectedLayer=%d but inst->un2 = 0x%08X",
  //         inst->k, selectedLayer, ntohl(inst->un2));
  //     return;
  // } else if (selectedLayer == 4 && inst->un2 != 0x0C) {
  //     qDebug("Skipping 0x%08X, selectedLayer=%d but inst->un2 = 0x%08X",
  //         inst->k, selectedLayer, ntohl(inst->un2));
  //     return;
  // }

  /*
  QPen pen_red  = QPen(Qt::red, 1);
  QPen pen_blue = QPen(Qt::blue, 1);
  if (x14_layer(inst->ptr1, &fs).value_or(-1) % 2 == 0) {
      pen = &pen_red;
  } else {
      pen = &pen_blue;
  }
  */

  drawShape(inst->ptr4[0], pen);
  drawShape(inst->ptr4[1], pen);
  drawShape(inst->ptr4[2], pen);
  // return;

  // drawShape(inst->ptr4[2], pen);

  for (auto &i_x30 : fs->iter_x30(inst->k)) {
    drawX30(&i_x30, pen);
  }

  for (auto &i_x32 : fs->iter_x32(inst->k)) {
    drawX32(&i_x32, pen, inst->rotation);
  }
}

void BrdView::drawX30(const T30StringGraphic<kAMax> *inst, QPen *pen) {
  if (!onSelectedLayer(inst->subtype, inst->layer)) {
    return;
  }

  /*
  if (fs->x27_db.keys.count(inst->str_graphic_ptr) == 0) {
      return;
  }
  */

  if (!fs->is_type(inst->str_graphic_ptr, 0x31)) {
    return;
  }

  const T31String<kAMax> &str_graphic = fs->get_x31(inst->str_graphic_ptr);

  QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  font.setFixedPitch(true);
  font.setKerning(false);

  qreal factor = 1000;

  const x36_x08<kAMax> *x36_x08_inst = font_lookup(inst->font.key, *fs);
  if (x36_x08_inst != nullptr) {
    font.setPixelSize(x36_x08_inst->char_height / factor);

    QFontMetrics m = QFontMetrics(font);

    font.setLetterSpacing(
        QFont::AbsoluteSpacing,
        x36_x08_inst->char_width / factor - m.horizontalAdvance("W"));

  } else {
    qDebug() << "Warning, did not find font size";
    font.setPixelSize(factor);
  }

  if (!str_graphic.s.empty()) {
    qreal x = str_graphic.coords[0] / 1000.;
    qreal y = str_graphic.coords[1] / 1000.;
    QTransform t = QTransform()
                       .translate(x, y)
                       .rotate(inst->rotation / factor)
                       .scale(inst->font.reversed == TextReversed ? -1 : 1, -1);

    QGraphicsTextItem *text =
        scene->addText(QString::fromStdString(str_graphic.s), font);
    QRectF boundingBox = text->boundingRect();
    if (x36_x08_inst != nullptr) {
      if ((inst->font.align) == TextAlignRight) {
        t = t.translate(-1 * boundingBox.width(), 0);
      } else if ((inst->font.align) == TextAlignCenter) {
        t = t.translate(-1 * boundingBox.width() / 2, 0);
      }
    }
    text->setDefaultTextColor(customPenColor(inst->k, pen->color()));
    // text->setTransform(t);
    // text->setTransform(t.translate(0, boundingBox.height() * -0.5));
    text->setTransform(t.translate(0, boundingBox.height() * -1));
    text->setData(0, inst->k);

    scene->addRect(t.mapRect(boundingBox),
                   QPen(QColorConstants::Svg::lightslategray, 0, Qt::DotLine));
    text->setTransform(t);

    // QPen p1 = QPen(QColorConstants::Svg::green, 0, Qt::DotLine);
    // QPen p2 = QPen(QColorConstants::Svg::yellow, 0, Qt::DotLine);
    // for (int8_t i = -10; i <= 10; i++) {
    //     scene->addLine(x + i * (x36_x08_inst->xs[1] / factor),
    //                    y + (x36_x08_inst->xs[0] / factor),
    //                    x + i * (x36_x08_inst->xs[1] / factor), y,
    //                    i == 0 ? p2 : p1);
    // }
  }
}

// Pad
void BrdView::drawX32(const T32SymbolPin<kAMax> *inst, QPen *pen,
                      uint32_t sym_rotation) {
  if (fs->is_type(inst->ptr3, 0x2D)) {
    const T2DSymbolInstance<kAMax> x2D_inst = fs->get_x2D(inst->ptr3);
    if (!onSelectedLayer(inst->subtype,
                         x2D_inst.layer == 0 ? 0 : fs->layer_count - 1)) {
      return;
    }
  } else {
    return;
  }
  /*
  if (!onSelectedLayer(inst->subtype, inst->layer)) {
      return;
  }
  */

  // Try to draw pad shape
  const x0D<kAMax> &x0D_inst = fs->get_x0D(inst->ptr5);
  const T1CPad<kAMax> &x1C_inst = fs->get_x1C(x0D_inst.pad_ptr);

  QPointF center = QPointF((inst->coords[0] + inst->coords[2]) / 2. / factor,
                           (inst->coords[1] + inst->coords[3]) / 2. / factor);

  // Just draw the first part
  const t13<kAMax> *first_part = &x1C_inst.parts[0];
  QGraphicsItem *pad;
  QTransform t = QTransform()
                     .translate(center.x(), center.y())
                     .rotate((x0D_inst.rotation + sym_rotation) / 1000.);
  switch (first_part->t) {
    case 2:
      pad = scene->addEllipse(-0.5 * first_part->w / factor,
                              -0.5 * first_part->h / factor,
                              first_part->w / factor, first_part->h / factor,
                              QPen(Qt::NoPen), QBrush(pen->color()));
      pad->setTransform(t);
      break;
    case 6:
      pad = scene->addRect(-0.5 * first_part->w / factor,
                           -0.5 * first_part->h / factor,
                           first_part->w / factor, first_part->h / factor,
                           QPen(Qt::NoPen), QBrush(pen->color()));
      pad->setTransform(t);
      break;
    default:
      return;
  }

  pad->setData(0, inst->k);

  // drawShape(inst->ptr10, pen2);
}

void BrdView::drawX33(const x33<kAMax> *inst, QPen *pen) {
  if (!onSelectedLayer(inst->subtype, 0x100)) {
    return;
  }

  /*
  scene->addRect(
      (inst->bb_coords[0])/factor,
      (inst->bb_coords[1])/factor,
      (inst->bb_coords[2]-inst->bb_coords[0])/factor,
      (inst->bb_coords[3]-inst->bb_coords[1])/factor,
      QPen(Qt::green, 0, Qt::DotLine)
  );
  */

  /*
  qint32 r = 1.2;
  QGraphicsItem *item = scene->addEllipse(
      (inst->coords[0]/factor)-r, (inst->coords[1]/factor)-r,
      2*r, 2*r, QPen(Qt::red, 0)
  );
  item->setData(0, inst->k);
  */

  if (!fs->is_type(inst->ptr4, 0x1C)) {
    printf("Drawing x33, but didn't find x1C; is it an x2F?\n");
    return;
  }

  const T1CPad<kAMax> &x1C_inst = fs->get_x1C(inst->ptr4);
  /*
  if (x1C_inst.pad_info.a == 0) {
      pen = new QPen(QColorConstants::Svg::mediumpurple);
  } else {
      pen = new QPen(QColorConstants::Svg::olive);
  }
  */

  QPointF center =
      QPointF((inst->coords[0]) / factor, (inst->coords[1]) / factor);

  // Just draw the first part
  const t13<kAMax> *first_part = &x1C_inst.parts[4];
  QGraphicsItem *pad;
  QTransform t = QTransform().translate(center.x(), center.y());
  switch (first_part->t) {
    case 2:
    case 6:
      pad = scene->addEllipse(-0.5 * first_part->w / factor,
                              -0.5 * first_part->h / factor,
                              first_part->w / factor, first_part->h / factor,
                              QPen(Qt::NoPen), QBrush(pen->color()));
      pad->setTransform(t);
      break;
    default:
      return;
  }

  pad->setData(0, inst->k);

  // drawShape(inst->ptr10, pen2);

  first_part = &x1C_inst.parts[0];
  switch (first_part->t) {
    case 2:
    case 6:
      pad = scene->addEllipse(-0.5 * first_part->w / factor,
                              -0.5 * first_part->h / factor,
                              first_part->w / factor, first_part->h / factor,
                              QPen(Qt::NoPen), QBrush(pen->color()));
      pad->setTransform(t);
      break;
    default:
      return;
  }

  pad->setData(0, inst->k);
}

void BrdView::drawX34(const x34<kAMax> *inst, QPen *pen) {
  if (!onSelectedLayer(inst->subtype, inst->layer)) {
    return;
  }

  uint32_t k = inst->ptr2;

  QPainterPath path;
  path.moveTo(*startingPoint(k) * 1.0 / factor);

  while (isLineSegment(k)) {
    if (fs->is_type(k, 0x01)) {
      const T01ArcSegment<kAMax> segment_inst = fs->get_x01(k);
      drawX01(&segment_inst, &path);
      k = segment_inst.next;
    } else if (fs->is_type(k, 0x15)) {
      const T15LineSegment<kAMax> segment_inst = fs->get_x15(k);
      drawX15(&segment_inst, &path);
      k = segment_inst.next;
    } else if (fs->is_type(k, 0x16)) {
      const T16LineSegment<kAMax> segment_inst = fs->get_x16(k);
      drawX16(&segment_inst, &path);
      k = segment_inst.next;
    } else if (fs->is_type(k, 0x17)) {
      const T17LineSegment<kAMax> segment_inst = fs->get_x17(k);
      drawX17(&segment_inst, &path);
      k = segment_inst.next;
    } else {
      return;
    }
  }

  QGraphicsItem *item = scene->addPath(path, *pen);
  item->setData(0, inst->k);
}

void BrdView::drawShape(const uint32_t ptr, QPen *pen) {
  if (already_drawn.count(ptr) > 0) {
    // std::printf("Already drawn 0x%08X\n", ntohl(ptr));
    return;
  } else {
    // std::printf("Drawing 0x%08X\n", ntohl(ptr));
    already_drawn.insert(ptr);
  }

  // QPen *darkerPen = new QPen(pen->color().lighter(101), 0.3);
  QPen *darkerPen = pen;

  if (fs->is_type(ptr, 0x05)) {
    // std::printf("Trying to draw x05\n");
    const T05Line<kAMax> inst = fs->get_x05(ptr);
    drawX05(&inst, pen);
  } else if (fs->is_type(ptr, 0x0C)) {
    const T0CDrillIndicator<kAMax> inst = fs->get_x0C(ptr);
    drawX0C(&inst, pen);
  } else if (fs->is_type(ptr, 0x10)) {
    const x10<kAMax> inst = fs->get_x10(ptr);
    drawShape(inst.ptr1, darkerPen);
    drawShape(inst.ptr2, darkerPen);
    drawShape(inst.ptr3, darkerPen);
  } else if (fs->is_type(ptr, 0x14)) {
    const T14Path<kAMax> inst = fs->get_x14(ptr);
    drawX14(&inst, pen);
    // } else if (fs->x15_map.count(ptr) > 0) {
    // const x15<A_MAX> *inst = (const x15<A_MAX> *)&fs->x15_map.at(ptr);
    // drawX15(inst, pen);
    // drawShape(inst->un1, darkerPen);
    // drawShape(inst->ptr, darkerPen);
    // } else if (fs->x16_map.count(ptr) > 0) {
    // const x16<A_MAX> *inst = (const x16<A_MAX> *)&fs->x16_map.at(ptr);
    // drawX16(inst, pen);
    // drawShape(inst->un1, darkerPen);
    // drawShape(inst->ptr, darkerPen);
    // } else if (fs->x17_map.count(ptr) > 0) {
    // const x17<A_MAX> *inst = (const x17<A_MAX> *)&fs->x17_map.at(ptr);
    // drawX17(inst, pen);
    // drawShape(inst->un1, darkerPen);
    // drawShape(inst->ptr, darkerPen);
  } else if (fs->is_type(ptr, 0x23)) {
    const T23Rat<kAMax> inst = fs->get_x23(ptr);
    drawX23(&inst, pen);
  } else if (fs->is_type(ptr, 0x24)) {
    const T24Rectangle<kAMax> inst = fs->get_x24(ptr);
    drawX24(&inst, pen);
  } else if (fs->is_type(ptr, 0x28)) {
    const T28Shape<kAMax> inst = fs->get_x28(ptr);
    drawX28(&inst, pen);
    // drawShape(inst->ptr5, darkerPen);
    // drawShape(inst->ptr1, darkerPen);
    // drawShape(inst->ptr2, darkerPen);
    // drawShape(inst->ptr5, darkerPen);
  } else if (fs->is_type(ptr, 0x2D)) {
    const T2DSymbolInstance<kAMax> inst = fs->get_x2D(ptr);
    drawX2D(&inst, pen);
  } else if (fs->is_type(ptr, 0x30)) {
    const T30StringGraphic<kAMax> &inst = fs->get_x30(ptr);
    drawX30(&inst, pen);
    // } else if (fs.x31_map->count(ptr) > 0) {
    //     const x31 *inst = (const x31*)&fs.x31_map->at(ptr);
    //     drawX31((const x31*)&fs.x31_map->at(ptr), pen);
  } else if (fs->is_type(ptr, 0x32)) {
    const T32SymbolPin<kAMax> &inst = fs->get_x32(ptr);
    drawX32(&inst, pen, 0);
  } else if (fs->is_type(ptr, 0x33)) {
    const x33<kAMax> &inst = fs->get_x33(ptr);
    drawX33(&inst, pen);
    // drawShape(inst->un1, darkerPen);
    // drawShape(inst->ptr1, darkerPen);
  } else if (fs->is_type(ptr, 0x34)) {
    const x34<kAMax> &inst = fs->get_x34(ptr);
    drawX34(&inst, pen);
  } else if (fs->is_type(ptr, 0x37)) {
    const x37<kAMax> &inst = fs->get_x37(ptr);
    drawShape(inst.ptr1, darkerPen);
    // for (uint8_t i; i<inst->count; i++) {
    // for (uint8_t i=0; i<inst->capacity; i++) {
    //    drawShape(inst->ptrs[i], pen);
    //}
  } else {
    // std::printf("Don't know how to draw 0x%08X\n", ntohl(ptr));
  }
  // uint8_t t = *(uint8_t*)untyped_inst;
  // switch (t) {
  //     case 0x16:
  //         drawX16((const x16*)untyped_inst, pen);
  //         break;
  //     default:
  //         std::printf("Do not know how to draw t=0x%X\n", t);
  // }
};

void BrdView::drawFile() {
  scene->clear();
  already_drawn.clear();

  QPen *pen1 = new QPen(QColor(89, 195, 195, 50), 0);
  pen1->setCapStyle(Qt::RoundCap);
  QPen *pen2 = new QPen(QColor(82, 72, 156, 127), 0);
  pen2->setCapStyle(Qt::RoundCap);
  QPen *pen3 = new QPen(QColor(235, 235, 235, 127), 0);
  QPen *pen4 = new QPen(QColor(202, 210, 197, 127), 0);
  QPen *pen5 = new QPen(QColor(132, 169, 140, 127), 0);
  QPen *pen6 = new QPen(QColor(237, 211, 130, 127), 0);

  for (auto &i_x1B : fs->iter_t1B_net()) {
    for (auto &i_x04 : fs->iter_x04(i_x1B.k)) {
      uint32_t k = i_x04.ptr2;
      while (1) {
        if (fs->is_type(k, 0x33)) {
          auto &i = fs->get_x33(k);
          // printf("- - Found x33 w/ key = 0x %08X\n", ntohl(k));
          drawShape(k, pen3);
          k = i.un1;
        } else if (fs->is_type(k, 0x32)) {
          auto &i = fs->get_x32(k);
          // printf("- - Found x32 w/ key = 0x %08X\n", ntohl(k));
          k = i.un1;
        } else if (fs->is_type(k, 0x2E)) {
          auto &i = fs->get_x2E(k);
          // printf("- - Found x2E w/ key = 0x %08X\n", ntohl(k));
          k = i.un[0];
        } else if (fs->is_type(k, 0x28)) {
          auto &x = fs->get_x28(k);
          drawShape(x.k, pen2);
          k = x.next;
        } else if (fs->is_type(k, 0x1B)) {
          break;
        } else if (fs->is_type(k, 0x0E)) {
          auto &x = fs->get_x0E(k);
          k = x.un[0];
        } else if (fs->is_type(k, 0x05)) {
          auto &x = fs->get_x05(k);
          drawShape(x.k, pen3);
          k = x.ptr0;
        } else if (fs->is_type(k, 0x04)) {
          break;
        } else {
          printf("- - \x1b[31mUnexpected key\x1b[0m = 0x %08X :(\n", ntohl(k));
          break;
        }
      }
    }
  }

  for (auto &inst : fs->iter_x0C()) {
    drawShape(inst.k, pen5);
  }
  for (auto &inst : fs->iter_x0C_2()) {
    drawShape(inst.k, pen6);
  }

  // for (const auto& [k, x14_inst] : *fs.x14_map) {
  //     // QPen *p;
  //     // if (fs.x2D_map->count(x14_inst.ptr1) > 0) {
  //     //     p = pen1;
  //     // } else {
  //     //     p = pen2;
  //     // }

  //     drawShape(x14_inst.ptr2,
  //         fs.x2D_map->count(x14_inst.ptr1) > 0
  //         // ||
  //         // fs.x2B_map->count(x14_inst.ptr1) > 0
  //         ? pen1 : pen2);
  // }

  /*
  for (const auto &[k, x14_inst] : fs->x14_map) {
      drawShape(k, pen3);
  }
  */

  if (fs->hdr->ll_x14.head != 0) {
    auto x = fs->get_x14(fs->hdr->ll_x14.head);
    drawShape(x.k, pen3);
    while (x.next != fs->hdr->ll_x14.tail) {
      x = fs->get_x14(x.next);
      drawShape(x.k, pen3);
    }
  }

  // Connectivity (rats)
  // for (const auto& [k, x23_inst] : *fs.x23_map) {
  //     drawShape(k, pen4);
  // }

  for (auto &x2B_inst : fs->iter_x2B()) {
    for (auto &x2D_inst : fs->iter_x2D(x2B_inst.k)) {
      drawShape(x2D_inst.k, pen4);
    }
  }

  // Text
  /*
  ll_ptrs text_ll = fs->hdr->ll_x03_x30;
  uint32_t k = text_ll.head;
  while (1) {
      if (fs->is_type(k, 0x30)) {
          auto &i = fs->x30_map[k];
          printf("Found x30 w/ key = 0x %08X\n", ntohl(k));
          drawShape(k, pen4);
          k = i.next;
      } else if (fs->is_type(k, 0x03)) {
          auto &i = fs->get_x03(k);
          printf("Found x03 w/ key = 0x %08X\n", ntohl(k));
          k = i.next;
      } else {
          printf("Unexpected item k = 0x %08X in list\n", ntohl(k));
          break;
      }
  }
  */

  for (auto &i_x03_x30 : fs->iter_x30()) {
    if (fs->is_type(i_x03_x30.k, 0x30)) {
      drawShape(i_x03_x30.k, pen2);
    }
  }

  // Draw some additional shapes in an extra linked list
  LinkedListPtrs text_ll = fs->hdr->ll_x24_x28;
  uint32_t k = text_ll.head;
  if (k != 0) {
    while (k != text_ll.tail) {
      if (fs->is_type(k, 0x28)) {
        auto &i = fs->get_x28(k);
        drawShape(k, pen4);
        k = i.next;
      } else if (fs->is_type(k, 0x24)) {
        auto &i = fs->get_x24(k);
        drawShape(k, pen5);
        k = i.next;
      } else {
        printf("Unexpected end of x24/x28 list, found item k=0x %08X\n",
               ntohl(k));
        break;
      }
    }
  }

  // for (const auto &[k, x30_inst] : fs->x30_map) {
  //     drawShape(k, pen2);
  // }

  // Pads
  // for (const auto& [k, x32_inst] : *fs.x32_map) {
  //     drawShape(k, pen3);
  // }

  /*
      for (const auto& [k, x15_inst] : *fs.x15_map) {
      drawShape(k, pen5);
  }

  for (const auto& [k, x16_inst] : *fs.x16_map) {
      drawShape(k, pen5);
  }

  for (const auto& [k, x17_inst] : *fs.x17_map) {
      drawShape(k, pen5);
  }
  */

  // qint32 r = 1;
  // for (const auto& [k, x33_inst] : *fs.x33_map) {
  //     scene->addEllipse(
  //         (x33_inst.coords[0]/factor)-r, (x33_inst.coords[1]/factor)-r,
  //         2*r, 2*r, *pen3
  //     );

  //     scene->addEllipse(
  //         x33_inst.un4[0]/factor, x33_inst.un4[1]/factor,
  //         (x33_inst.un4[2]-x33_inst.un4[0])/factor,
  //         (x33_inst.un4[3]-x33_inst.un4[1])/factor,
  //         *pen2
  //     );
  // }
};

QColor BrdView::customPenColor(uint32_t x05_k, QColor default_) {
  if (fs->is_type(x05_k, 0x05)) {
    const T05Line<kAMax> inst = fs->get_x05(x05_k);
    if (onSelectedLayer(inst.subtype, inst.layer)) {
      return QColorConstants::Svg::palevioletred;
    } else {
      return QColorConstants::Svg::darkseagreen;
    }
  } else if (fs->is_type(x05_k, 0x14)) {
    return QColorConstants::Svg::goldenrod;
  } else if (fs->is_type(x05_k, 0x28)) {
    const T28Shape<kAMax> &inst = fs->get_x28(x05_k);
    if (onSelectedLayer(inst.subtype, inst.layer)) {
      return QColorConstants::Svg::palevioletred;
    } else {
      return default_;
      // return QColorConstants::Svg::crimson;
    }
  } else if (fs->is_type(x05_k, 0x34)) {
    const x34<kAMax> &inst = fs->get_x34(x05_k);
    if (onSelectedLayer(inst.subtype, inst.layer)) {
      return QColorConstants::Svg::palevioletred;
    } else {
      return QColorConstants::Svg::blanchedalmond;
    }
  } else if (fs->is_type(x05_k, 0x30)) {
    const T30StringGraphic<kAMax> &inst = fs->get_x30(x05_k);
    if (onSelectedLayer(inst.subtype, inst.layer)) {
      return QColorConstants::Svg::lightsalmon;
    } else {
      return QColorConstants::Svg::darkviolet;
    }
  } else {
    return QColorConstants::Svg::lightsteelblue;
  }

  /*
  if (fs.x05_map->count(x05_k) > 0) {
      const x05 *inst = (const x05*)&fs.x05_map->at(x05_k);
      switch (inst->layer) {
          case 0x00:
              return Qt::blue;
          case 0x01:
              return QColorConstants::Svg::lime;
          case 0x02:
              return QColorConstants::Svg::khaki;
          case 0x03:
              return QColorConstants::Svg::violet;
          case 0x04:
              return Qt::yellow;
          case 0x05:
              return QColorConstants::Svg::palevioletred;
          case 0x06:
              return QColorConstants::Svg::darkseagreen;
          case 0x07:
              return QColorConstants::Svg::royalblue;
          case 0x09:
              return Qt::green;
          case 0x0B:
              return QColorConstants::Svg::coral;
          case 0x0C:
              return QColorConstants::Svg::peru;
          case 0x0D:
              return Qt::red;
          case 0x0F:
              return QColorConstants::Svg::darkmagenta;
          default:
              return default_;
      }
  } else {
      return Qt::yellow;
  }
  */
  /*
  switch (ntohl(x05_k)) {
      case 0xB020C927:
          return Qt::green;
      case 0xB0B05728:
          return Qt::red;
      case 0x90B3CC28:
          return Qt::blue;
      default:
          return default_;
  }
  */
}

void BrdView::selectLayer(std::set<std::pair<uint16_t, uint16_t>> layers) {
  selectedLayers = layers;
  drawFile();

  std::printf("Selected layers:");
  for (const auto &[subtype, layer] : layers) {
    std::printf(" %X.%d,", subtype, layer);
  }
  std::printf("\n");

  // std::printf("\x1b[35m-----------------------------------------------\x1b[0m\n");
  // for (const auto& [k, x28_inst] : *fs.x28_map) {
  //     if (x28_inst.layer == selectedLayer) {
  //         print_struct((const void*)&fs.x28_map->at(k), &fs, 0);
  //     }
  // }
}

bool BrdView::onSelectedLayer(uint8_t subtype, uint16_t layer) {
  for (auto lp : selectedLayers) {
    if (lp.first == 0x100) {
      return true;
    }

    if (subtype != lp.first) {
      continue;
    }

    if (lp.second == 0x100 || layer == 0x100 || layer == lp.second) {
      return true;
    }
  }

  return false;
}

bool BrdView::isLineSegment(uint32_t k) {
  bool r = (fs->is_type(k, 0x01)) || (fs->is_type(k, 0x15)) ||
           (fs->is_type(k, 0x16)) || (fs->is_type(k, 0x17));
  // std::printf("isLineSegment k = 0x%08X, r = %d\n", ntohl(k), r);
  return r;
}

std::optional<QPointF> BrdView::startingPoint(uint32_t k) {
  if (fs->is_type(k, 0x01)) {
    const T01ArcSegment<kAMax> segment_inst = fs->get_x01(k);
    return QPointF(segment_inst.coords[0], segment_inst.coords[1]);
  } else if (fs->is_type(k, 0x15)) {
    const T15LineSegment<kAMax> segment_inst = fs->get_x15(k);
    return QPointF(segment_inst.coords[0], segment_inst.coords[1]);
  } else if (fs->is_type(k, 0x16)) {
    const T16LineSegment<kAMax> segment_inst = fs->get_x16(k);
    return QPointF(segment_inst.coords[0], segment_inst.coords[1]);
  } else if (fs->is_type(k, 0x17)) {
    const T17LineSegment<kAMax> segment_inst = fs->get_x17(k);
    return QPointF(segment_inst.coords[0], segment_inst.coords[1]);
  } else {
    return std::optional<QPointF>();
  }
}

std::optional<QPointF> BrdView::endingPoint(uint32_t k) {
  if (fs->is_type(k, 0x01)) {
    const T01ArcSegment<kAMax> segment_inst = fs->get_x01(k);
    return QPointF(segment_inst.coords[2], segment_inst.coords[3]);
  } else if (fs->is_type(k, 0x15)) {
    const T15LineSegment<kAMax> segment_inst = fs->get_x15(k);
    return QPointF(segment_inst.coords[2], segment_inst.coords[3]);
  } else if (fs->is_type(k, 0x16)) {
    const T16LineSegment<kAMax> segment_inst = fs->get_x16(k);
    return QPointF(segment_inst.coords[2], segment_inst.coords[3]);
  } else if (fs->is_type(k, 0x17)) {
    const T17LineSegment<kAMax> segment_inst = fs->get_x17(k);
    return QPointF(segment_inst.coords[2], segment_inst.coords[3]);
  } else {
    return std::optional<QPointF>();
  }
}

char *BrdView::netName(uint32_t k) {
  if (fs->is_type(k, 0x28)) {
    const T28Shape<kAMax> &x28_inst = fs->get_x28(k);
    if (fs->is_type(x28_inst.ptr1, 0x04)) {
      const x04<kAMax> &x04_inst = fs->get_x04(x28_inst.ptr1);
      if (fs->is_type(x04_inst.ptr1, 0x1B)) {
        const T1BNet<kAMax> x1B_inst = fs->get_x1B(x04_inst.ptr1);
        // qDebug("Net name: 0x%08X", x1B_inst->net_name);
        return fs->strings.at(x1B_inst.net_name);
      }
    }
  }

  return nullptr;
}
