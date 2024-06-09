#ifndef FILM_MODEL_H
#define FILM_MODEL_H

#include <QtWidgets>
#include <set>

#include "layer_item.h"

struct Layer {
  const uint8_t x, y;
  const char *label;
};

const constexpr Layer kG1Layers[] = {
    {0x1, 0x01, nullptr},
    {0x1, 0x03, nullptr},
    {0x1, 0x05, nullptr},
    {0x1, 0x0F, nullptr},
    {0x1, 0x14, nullptr},
    {0x1, 0x16, nullptr},
    {0x1, 0xED, nullptr},
    {0x1, 0xEE, nullptr},
    {0x1, 0xF0, nullptr},
    {0x1, 0xF1, nullptr},
    {0x1, 0xF9, "Fab dims?"},
    {0x1, 0xFB, "Assy?"},
    {0x1, 0xFD, "Design outline?"},
    {0x1, 0xFF, "All"},
    {0, 0, nullptr},
};

const constexpr Layer kG3Layers[] = {
    {0x3, 0x00, nullptr}, {0x3, 0x01, nullptr}, {0x3, 0xFA, nullptr},
    {0x3, 0xFC, nullptr}, {0x3, 0xFD, nullptr}, {0x3, 0xFF, "All"},
    {0, 0, nullptr},
};

const constexpr Layer kG4Layers[] = {
    {0x04, 0x00, "Top something?"}, {0x04, 0x01, "Bottom something?"},
    {0x04, 0x02, nullptr},          {0x04, 0x03, nullptr},
    {0x04, 0x04, nullptr},          {0x04, 0x05, nullptr},
    {0x04, 0x06, nullptr},          {0x04, 0x07, nullptr},
    {0x04, 0x08, nullptr},          {0x04, 0x0B, nullptr},
    {0x04, 0x0C, nullptr},          {0x04, 0x11, nullptr},
    {0x04, 0x13, nullptr},          {0x04, 0x14, nullptr},
    {0x04, 0xFC, "Titleblock?"},    {0, 0, nullptr},
};

const constexpr Layer kG6Layers[] = {
    {0x06, 0x00, nullptr}, {0x06, 0x01, nullptr}, {0x06, 0x02, nullptr},
    {0x06, 0x03, nullptr}, {0x06, 0x04, nullptr}, {0x06, 0x05, nullptr},
    {0x06, 0x06, nullptr}, {0x06, 0x07, nullptr}, {0x06, 0x08, nullptr},
    {0x06, 0x09, nullptr}, {0x06, 0x0A, nullptr}, {0x06, 0x0B, nullptr},
    {0x06, 0x0C, nullptr}, {0x06, 0x0D, nullptr}, {0x06, 0x0E, nullptr},
    {0x06, 0x0F, nullptr}, {0, 0, nullptr},
};

const constexpr Layer kG7Layers[] = {
    {0x07, 0x00, nullptr}, {0x07, 0x01, nullptr}, {0x07, 0x02, nullptr},
    {0x07, 0x03, nullptr}, {0x07, 0x04, nullptr}, {0x07, 0xF7, nullptr},
    {0x07, 0xF8, nullptr}, {0x07, 0xFD, nullptr}, {0, 0, nullptr},
};

const constexpr Layer kG9Layers[] = {
    {0x09, 0xEC, nullptr},        {0x09, 0xED, nullptr},
    {0x09, 0xF3, nullptr},        {0x09, 0xF4, nullptr},
    {0x09, 0xF6, "Pin nums bot"}, {0x09, 0xF7, "Pin nums top"},
    {0x09, 0xFC, nullptr},        {0x09, 0xFC, nullptr},
    {0x09, 0xFD, nullptr},        {0, 0, nullptr},
};

const constexpr Layer kGCLayers[] = {
    {0x0C, 0x00, nullptr}, {0x0C, 0x01, "L2"},    {0x0C, 0x02, "L3"},
    {0x0C, 0x03, "L4"},    {0x0C, 0x04, "L5"},    {0x0C, 0x05, "L6"},
    {0x0C, 0x06, "L7"},    {0x0C, 0x07, "L8"},    {0x0C, 0x08, "L9"},
    {0x0C, 0x09, "L10"},   {0x0C, 0x0A, "L11"},   {0x0C, 0x0B, "L12"},
    {0x0C, 0xFA, nullptr}, {0x0C, 0xFB, nullptr}, {0x0C, 0xFC, nullptr},
    {0x0C, 0xFD, nullptr}, {0, 0, nullptr},
};

const constexpr Layer kGDLayers[] = {
    {0x0D, 0xFA, nullptr}, {0x0D, 0xFB, nullptr}, {0x0D, 0xFC, nullptr},
    {0x0D, 0xFD, nullptr}, {0, 0, nullptr},
};

const constexpr Layer kG12Layers[] = {
    {0x12, 0x00, "L1"},
    {0x12, 0x01, "L2"},
    {0x12, 0x02, "L3"},
    {0x12, 0x03, "L4"},
    {0x12, 0x04, "L5"},
    {0x12, 0x05, "L6"},
    {0x12, 0x06, "L7"},
    {0x12, 0x07, "L8"},
    {0x12, 0x08, "L9"},
    {0x12, 0x09, "L10"},
    {0x12, 0x0A, "L11"},
    {0x12, 0x0B, "L12"},
    {0x12, 0xFA, "Paste bottom?"},
    {0x12, 0xFB, "Paste top?"},
    {0x12, 0xFC, "Mask bottom?"},
    {0x12, 0xFD, "Mask top?"},
    {0, 0, nullptr},
};

class LayerModel : public QAbstractItemModel {
  Q_OBJECT

 public:
  Q_DISABLE_COPY_MOVE(LayerModel);

  explicit LayerModel(const QString &data, QObject *parent = nullptr);
  ~LayerModel() override = default;

  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  std::pair<uint16_t, uint16_t> layer_pair(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  QModelIndex index(int row, int col,
                    const QModelIndex &parent = {}) const override;
  QModelIndex parent(const QModelIndex &parent) const override;

  int rowCount(const QModelIndex &parent = {}) const override;
  int columnCount(const QModelIndex &parent = {}) const override;

  std::vector<QModelIndex> mapLayersToIndices(
      const std::set<std::pair<uint16_t, uint16_t>> &layer_pairs) const;

 private:
  std::unique_ptr<LayerItem> root_item_;

  void addLayerGroup(const std::string label, const Layer layers[]);
};

#endif
