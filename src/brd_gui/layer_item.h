#include <QtWidgets>

// This class is based on the "Simple Tree Model Example" from
// https://doc.qt.io/qt-6/qtwidgets-itemviews-simpletreemodel-example.html
class LayerItem {
 public:
  explicit LayerItem(const std::string &root_label, LayerItem *parent = nullptr)
      : is_leaf_(false), label_(root_label), parent_item_(parent) {};
  explicit LayerItem(uint8_t x, uint8_t y, std::optional<std::string> label)
      : is_leaf_(true), x_(x), y_(y), label_(label), parent_item_(nullptr) {};

  void appendChild(std::unique_ptr<LayerItem> &&child);
  LayerItem *child(int row);
  QVariant data() const;
  int row() const;
  Qt::ItemFlags flags() const;
  std::pair<uint16_t, uint16_t> layer_pair() const;

  int childCount() const { return int(child_items_.size()); }
  LayerItem *parentItem() { return parent_item_; }

 private:
  std::vector<std::unique_ptr<LayerItem>> child_items_;
  bool is_leaf_;
  uint8_t x_, y_;
  std::optional<std::string> label_;
  LayerItem *parent_item_;
};
