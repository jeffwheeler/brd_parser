#pragma once

#include <string>

class FilePickerWidget {
 public:
  FilePickerWidget() = default;
  static void Draw();

 private:
  static void UploadFile(std::string const &filename,
                         std::string const & /* unused */,
                         std::string_view buffer, void * /* unused */);
};
