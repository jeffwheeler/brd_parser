#ifndef FILM_MODEL_H
#define FILM_MODEL_H

#include <QtWidgets>
#include <map>
#include <set>

#include "layer_item.h"
#include "lib/structure/types.h"

struct Layer {
  uint8_t x = 0, y = 0;
  const char *label = nullptr;
};

const constexpr Layer kG1Layers[] = {
    {0x01, 0xEA, nullptr},    {0x01, 0xED, nullptr},
    {0x01, 0xEE, nullptr},    {0x01, 0xF0, "Silk bot"},
    {0x01, 0xF1, "Silk top"}, {0x01, 0xF9, "Fab dims?"},
    {0x01, 0xFB, "Assy?"},    {0x01, 0xFD, "Design outline?"},
    {0, 0, nullptr},
};

const constexpr Layer kG3Layers[] = {
    {0x3, 0xFA, nullptr},
    {0x3, 0xFC, nullptr},
    {0x3, 0xFD, nullptr},
    {0, 0, nullptr},
};

const constexpr Layer kG4Layers[] = {
    {0x04, 0xF9, nullptr},       {0x04, 0xFA, nullptr}, {0x04, 0xFB, nullptr},
    {0x04, 0xFC, "Title block"}, {0x04, 0xFD, nullptr}, {0, 0, nullptr},
};

const constexpr Layer kG7Layers[] = {
    {0x07, 0xF0, nullptr}, {0x07, 0x12, nullptr}, {0x07, 0x13, nullptr},
    {0x07, 0xF7, nullptr}, {0x07, 0xF8, nullptr}, {0x07, 0xFD, nullptr},
    {0, 0, nullptr},
};

const constexpr Layer kG9Layers[] = {
    {0x09, 0xEC, nullptr},
    {0x09, 0xED, nullptr},
    {0x09, 0xF3, nullptr},
    {0x09, 0xF4, nullptr},
    {0x09, 0xF6, "Pin nums bot"},
    {0x09, 0xF7, "Pin nums top"},
    {0x09, 0xFC, "Pkg outline bot?"},
    {0x09, 0xFD, "Pkg outline top?"},
    {0, 0, nullptr},
};

const constexpr Layer kGCLayers[] = {
    {0x0C, 0xF4, nullptr}, {0x0C, 0xF5, nullptr}, {0x0C, 0xFA, nullptr},
    {0x0C, 0xFB, nullptr}, {0x0C, 0xFC, nullptr}, {0x0C, 0xFD, nullptr},
    {0, 0, nullptr},
};

const constexpr Layer kGDLayers[] = {
    {0x0D, 0xF8, "Assy bot?"}, {0x0D, 0xF9, "Assy top?"},
    {0x0D, 0xFA, "Silk bot?"}, {0x0D, 0xFB, "Silk top?"},
    {0x0D, 0xFC, "Silk bot?"}, {0x0D, 0xFD, "Silk top?"},
    {0, 0, nullptr},
};

const constexpr Layer kG12Layers[] = {
    {0x12, 0xF4, nullptr},
    {0x12, 0xF5, nullptr},
    {0x12, 0xFA, "Paste bottom?"},
    {0x12, 0xFB, "Paste top?"},
    {0x12, 0xFC, "Mask bottom?"},
    {0x12, 0xFD, "Mask top?"},
    {0, 0, nullptr},
};

const std::map<uint16_t, const Layer *> kFixedLayersMap = {
    {1, kG1Layers}, {3, kG3Layers},    {4, kG4Layers},    {7, kG7Layers},
    {9, kG9Layers}, {0x0C, kGCLayers}, {0x0D, kGDLayers}, {0x12, kG12Layers},
};

class LayerModel : public QAbstractItemModel {
  Q_OBJECT

 public:
  Q_DISABLE_COPY_MOVE(LayerModel);

  explicit LayerModel(File<kAMax> &data, QObject *parent = nullptr);
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

  void addLayerGroup(const std::string label, const Layer layers[]);

 private:
  std::unique_ptr<LayerItem> root_item_;
};

#endif
