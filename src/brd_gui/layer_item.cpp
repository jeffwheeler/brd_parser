#include "layer_item.h"

void LayerItem::appendChild(std::unique_ptr<LayerItem>&& child) {
  child->parent_item_ = this;
  child_items_.push_back(std::move(child));
}

LayerItem* LayerItem::child(int row) {
  return row >= 0 && row < childCount() ? child_items_.at(row).get() : nullptr;
}

QVariant LayerItem::data() const {
  if (is_leaf_ && label_) {
    return QString("%1 - %2 (%3)")
        .arg(x_)
        .arg(y_)
        .arg(QString::fromStdString(*label_));
  } else if (is_leaf_) {
    return QString("%1 - %2").arg(x_).arg(y_);
  } else {
    return QString::fromStdString(*label_);
  }
}

int LayerItem::row() const {
  if (parent_item_ == nullptr) return 0;
  const auto it = std::find_if(
      parent_item_->child_items_.cbegin(), parent_item_->child_items_.cend(),
      [this](const std::unique_ptr<LayerItem>& treeItem) {
        return treeItem.get() == this;
      });

  if (it != parent_item_->child_items_.cend())
    return std::distance(parent_item_->child_items_.cbegin(), it);

  Q_ASSERT(false);  // should not happen
  return -1;
}

Qt::ItemFlags LayerItem::flags() const {
  if (is_leaf_) {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
  } else {
    return Qt::ItemIsEnabled;
  }
}

std::pair<uint16_t, uint16_t> LayerItem::layer_pair() const {
  return std::pair<uint16_t, uint16_t>(x_, y_);
}
