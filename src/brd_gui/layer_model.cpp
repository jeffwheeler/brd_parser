#include "layer_model.h"

#include "lib/structure/utils.h"

LayerModel::LayerModel(File<kAMax>& data, QObject* parent)
    : QAbstractItemModel(parent),
      root_item_(std::make_unique<LayerItem>("Layer")) {
  uint16_t i = 1;
  for ([[maybe_unused]] const auto& [a, b] : data.layers) {
    if (b != 0x00 && data.x2A_map.count(b) > 0) {
      x2A* inst = &data.x2A_map[b];

      Layer* layers = new Layer[inst->hdr.size + 1];
      if (inst->references) {
        uint8_t j = 0;
        for (const auto& entry : inst->reference_entries) {
          layers[j].x = i;
          layers[j].y = j;
          layers[j].label = str_lookup(entry.ptr, data);
          j++;
        }
      } else {
        uint8_t j = 0;
        for (const auto& entry : inst->local_entries) {
          layers[j].x = i;
          layers[j].y = j;
          layers[j].label = entry.s.c_str();
          j++;
        }
      }

      addLayerGroup(QString("%1 - ?").arg(i).toStdString(), layers);

      delete[] layers;
    }
    i++;
  }
  /*
  addLayerGroup("1 - Board geometry?", kG1Layers);
  addLayerGroup("3 - ?", kG3Layers);
  addLayerGroup("4 - Drawing Format?", kG4Layers);
  addLayerGroup("6 - Etch?", kG6Layers);
  addLayerGroup("7 - Manufacturing?", kG7Layers);
  addLayerGroup("9 - ?", kG9Layers);
  addLayerGroup("C - ?", kGCLayers);
  addLayerGroup("D - Refdes?", kGDLayers);
  addLayerGroup("12 - ?", kG12Layers);
  */
};

Qt::ItemFlags LayerModel::flags(const QModelIndex& index) const {
  if (!index.isValid()) {
    return Qt::ItemFlags(Qt::NoItemFlags);
  }

  const LayerItem* item =
      static_cast<const LayerItem*>(index.internalPointer());
  return item->flags();
};

QVariant LayerModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid() || role != Qt::DisplayRole) return {};

  const LayerItem* item =
      static_cast<const LayerItem*>(index.internalPointer());
  return item->data();
};

std::pair<uint16_t, uint16_t> LayerModel::layer_pair(
    const QModelIndex& index) const {
  LayerItem* childItem = static_cast<LayerItem*>(index.internalPointer());
  return childItem->layer_pair();
}

QVariant LayerModel::headerData(int section, Qt::Orientation orientation,
                                int role) const {
  return orientation == Qt::Horizontal && role == Qt::DisplayRole
             ? root_item_->data()
             : QVariant{};
};

QModelIndex LayerModel::index(int row, int col,
                              const QModelIndex& parent) const {
  if (!hasIndex(row, col, parent)) return {};

  LayerItem* parentItem =
      parent.isValid() ? static_cast<LayerItem*>(parent.internalPointer())
                       : root_item_.get();

  if (auto* childItem = parentItem->child(row))
    return createIndex(row, col, childItem);
  return {};
}

QModelIndex LayerModel::parent(const QModelIndex& index) const {
  if (!index.isValid()) return {};

  LayerItem* childItem = static_cast<LayerItem*>(index.internalPointer());
  LayerItem* parentItem = childItem->parentItem();

  return parentItem != root_item_.get()
             ? createIndex(parentItem->row(), 0, parentItem)
             : QModelIndex{};
}

int LayerModel::rowCount(const QModelIndex& parent) const {
  if (parent.column() > 0) return 0;

  const LayerItem* parentItem =
      parent.isValid() ? static_cast<const LayerItem*>(parent.internalPointer())
                       : root_item_.get();

  return parentItem->childCount();
};

int LayerModel::columnCount(const QModelIndex& parent) const { return 1; };

std::vector<QModelIndex> LayerModel::mapLayersToIndices(
    const std::set<std::pair<uint16_t, uint16_t>>& layer_pairs) const {
  std::vector<QModelIndex> indices;

  for (uint16_t i = 0; i < root_item_->childCount(); i++) {
    QModelIndex groupIndex = index(i, 0);
    for (uint16_t j = 0; j < rowCount(groupIndex); j++) {
      const auto current_index = index(j, 0, groupIndex);
      const auto current_layer_pair = layer_pair(current_index);
      if (layer_pairs.find(current_layer_pair) != layer_pairs.end()) {
        indices.push_back(current_index);
      }
    }
  }

  return indices;
}

void LayerModel::addLayerGroup(const std::string label, const Layer layers[]) {
  auto group = std::make_unique<LayerItem>(label, root_item_.get());

  uint16_t i = 0;
  while (true) {
    const Layer* layer_info = &layers[i];
    if (layer_info->x == 0 && layer_info->y == 0) break;

    const auto label_str = layer_info->label == nullptr
                               ? std::optional<std::string>()
                               : std::string(layer_info->label);
    auto child =
        std::make_unique<LayerItem>(layer_info->x, layer_info->y, label_str);
    group->appendChild(std::move(child));

    i++;
  }

  root_item_->appendChild(std::move(group));
}
