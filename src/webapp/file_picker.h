#pragma once

#include <string>

class FilePickerWidget {
 public:
  FilePickerWidget() = default;
  void Draw();

 private:
  static void UploadFile(std::string const &filename,
                         std::string const &mime_type, std::string_view buffer,
                         void *);
};
