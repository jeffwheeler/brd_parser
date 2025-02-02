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

// Used so that etch can be given a group name
const constexpr Layer kG6Layers[] = {
    {0, 0, nullptr},
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

const std::map<uint16_t, const std::pair<const Layer *, const char *>>
    kFixedLayersMap = {
        {0x01, {kG1Layers, "Board geometry?"}}, {0x03, {kG3Layers, nullptr}},
        {0x04, {kG4Layers, "Drawing format?"}}, {0x06, {kG6Layers, "Etch"}},
        {0x07, {kG7Layers, "Manufacturing"}},   {0x09, {kG9Layers, nullptr}},
        {0x0C, {kGCLayers, nullptr}},           {0x0D, {kGDLayers, "Refdes?"}},
        {0x12, {kG12Layers, nullptr}},
};

class LayerModel : public QAbstractItemModel {
  Q_OBJECT

 public:
  Q_DISABLE_COPY_MOVE(LayerModel);

  explicit LayerModel(File<kAMax> &data, QObject *parent = nullptr);
  ~LayerModel() override = default;

  [[nodiscard]] auto flags(const QModelIndex &index) const
      -> Qt::ItemFlags override;
  [[nodiscard]] auto data(const QModelIndex &index, int role) const
      -> QVariant override;
  [[nodiscard]] auto layer_pair(const QModelIndex &index) const
      -> std::pair<uint16_t, uint16_t>;
  [[nodiscard]] auto headerData(int /* unused */, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const
      -> QVariant override;

  [[nodiscard]] auto index(int row, int col,
                           const QModelIndex &parent = {}) const
      -> QModelIndex override;
  [[nodiscard]] auto parent(const QModelIndex &parent) const
      -> QModelIndex override;

  [[nodiscard]] auto rowCount(const QModelIndex &parent = {}) const
      -> int override;
  [[nodiscard]] auto columnCount(const QModelIndex & /* unused */ = {}) const
      -> int override;

  [[nodiscard]] auto mapLayersToIndices(
      const std::set<std::pair<uint16_t, uint16_t>> &layer_pairs) const
      -> std::vector<QModelIndex>;

  void addLayerGroup(std::string label, const Layer layers[]);

 private:
  std::unique_ptr<LayerItem> root_item_;
};

#endif
