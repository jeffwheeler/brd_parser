#include "layer_item.h"

void LayerItem::appendChild(std::unique_ptr<LayerItem>&& child) {
  child->parent_item_ = this;
  child_items_.push_back(std::move(child));
}

auto LayerItem::child(int row) -> LayerItem* {
  return row >= 0 && row < childCount() ? child_items_.at(row).get() : nullptr;
}

auto LayerItem::data() const -> QVariant {
  if (is_leaf_ && label_) {
    return QString("%1 - %2 (%3)")
        .arg(x_)
        .arg(y_)
        .arg(QString::fromStdString(*label_));
  }
  if (is_leaf_) {
    return QString("%1 - %2").arg(x_).arg(y_);
  }
  return QString::fromStdString(*label_);
}

auto LayerItem::row() const -> int {
  if (parent_item_ == nullptr) {
    return 0;
  }
  const auto it = std::find_if(
      parent_item_->child_items_.cbegin(), parent_item_->child_items_.cend(),
      [this](const std::unique_ptr<LayerItem>& treeItem) {
        return treeItem.get() == this;
      });

  if (it != parent_item_->child_items_.cend()) {
    return std::distance(parent_item_->child_items_.cbegin(), it);
  }

  Q_ASSERT(false);  // should not happen
  return -1;
}

auto LayerItem::flags() const -> Qt::ItemFlags {
  if (is_leaf_) {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
  }
  return Qt::ItemIsEnabled;
}

auto LayerItem::layer_pair() const -> std::pair<uint16_t, uint16_t> {
  return {x_, y_};
}
