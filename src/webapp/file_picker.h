#pragma once

#include <string>

class BrdViewerApp;

class FilePickerWidget {
 public:
  explicit FilePickerWidget(BrdViewerApp* app) : app_(app) {};
  void Draw();

 private:
  static void UploadFile(std::string const& filename,
                         std::string const& /* unused */,
                         std::string_view buffer, void* /* unused */);

  BrdViewerApp* app_;
};
