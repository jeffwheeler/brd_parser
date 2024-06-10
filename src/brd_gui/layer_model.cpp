#include "layer_model.h"

#include "lib/structure/utils.h"

LayerModel::LayerModel(File<kAMax>& fs, QObject* parent)
    : QAbstractItemModel(parent),
      root_item_(std::make_unique<LayerItem>("Layer")) {
  uint16_t i = 1;
  for ([[maybe_unused]] const auto& [a, x2A_k] : fs.layers) {
    qDebug() << "Starting " << i;
    uint8_t fixedEntriesLength = 0;

    if (kFixedLayersMap.count(i) > 0) {
      const Layer* fixedEntries = kFixedLayersMap.at(i);
      while (fixedEntries[fixedEntriesLength].x != 0) fixedEntriesLength++;
    }

    uint8_t dynamicEntriesLength = 0;

    if (x2A_k != 0x00 && fs.x2A_map.count(x2A_k) > 0) {
      x2A* inst = &fs.x2A_map[x2A_k];
      dynamicEntriesLength = inst->hdr.size;
    }

    Layer* layers = new Layer[dynamicEntriesLength + fixedEntriesLength + 1];
    uint8_t currentLayer = 0;

    if (x2A_k != 0x00 && fs.x2A_map.count(x2A_k) > 0) {
      x2A* inst = &fs.x2A_map[x2A_k];
      if (inst->references) {
        for (const auto& entry : inst->reference_entries) {
          layers[currentLayer].x = i;
          layers[currentLayer].y = currentLayer;
          layers[currentLayer].label = str_lookup(entry.ptr, fs);
          currentLayer++;
        }
      } else {
        for (const auto& entry : inst->local_entries) {
          layers[currentLayer].x = i;
          layers[currentLayer].y = currentLayer;
          layers[currentLayer].label = entry.s.c_str();
          currentLayer++;
        }
      }
    }

    // Now add the fixed layers
    if (kFixedLayersMap.count(i) > 0) {
      const Layer* fixedEntries = kFixedLayersMap.at(i);
      for (uint8_t j = 0; j < fixedEntriesLength; j++) {
        const Layer* layer = &fixedEntries[j];
        layers[currentLayer].x = i;
        layers[currentLayer].y = layer->y;
        layers[currentLayer].label = layer->label;
        currentLayer++;
      }
    }

    // Add layer if we've iterated through _any_ layers, fixed or dynamic
    qDebug() << "looking to add?";
    if (currentLayer > 0) {
      qDebug() << "adding";
      addLayerGroup(QString("%1 - ?").arg(i).toStdString(), layers);
    }

    delete[] layers;

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
