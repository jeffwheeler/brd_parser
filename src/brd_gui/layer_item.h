#include <QtWidgets>
#include <utility>

// This class is based on the "Simple Tree Model Example" from
// https://doc.qt.io/qt-6/qtwidgets-itemviews-simpletreemodel-example.html
class LayerItem {
 public:
  explicit LayerItem(const std::string &root_label, LayerItem *parent = nullptr)
      : is_leaf_(false), label_(root_label), parent_item_(parent){};
  explicit LayerItem(uint8_t x, uint8_t y, std::optional<std::string> label)
      : is_leaf_(true), x_(x), y_(y), label_(std::move(label)), parent_item_(nullptr){};

  void appendChild(std::unique_ptr<LayerItem> &&child);
  auto child(int row) -> LayerItem *;
  [[nodiscard]] auto data() const -> QVariant;
  [[nodiscard]] auto row() const -> int;
  [[nodiscard]] auto flags() const -> Qt::ItemFlags;
  [[nodiscard]] auto layer_pair() const -> std::pair<uint16_t, uint16_t>;

  [[nodiscard]] auto childCount() const -> int { return static_cast<int>(child_items_.size()); }
  auto parentItem() -> LayerItem * { return parent_item_; }

 private:
  std::vector<std::unique_ptr<LayerItem>> child_items_;
  bool is_leaf_;
  uint8_t x_{}, y_{};
  std::optional<std::string> label_;
  LayerItem *parent_item_;
};
