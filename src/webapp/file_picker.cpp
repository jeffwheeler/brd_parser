#include "file_picker.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "emscripten.h"
#include "emscripten_browser_file.h"
#include "imgui.h"
#include "webapp.h"

void FilePickerWidget::Draw() {
  ImGui::Begin("File Picker");
  if (ImGui::Button("Open file")) {
    emscripten_browser_file::upload(".brd", UploadFile);
  }
  ImGui::Text("You can also drag-and-drop anywhere");
  ImGui::End();
}

void FilePickerWidget::UploadFile(std::string const &filename,
                                  std::string const & /* unused */,
                                  std::string_view buffer, void * /*unused*/) {
  emscripten_log(EM_LOG_INFO, "Receiving %s!", filename.c_str());

  std::filesystem::path file_path("uploaded_file.brd");
  std::ofstream outfile(file_path, std::ios::binary | std::ios::trunc);

  if (!outfile.is_open()) {
    emscripten_log(EM_LOG_ERROR, "Unable to open file for writing");
    return;
  }

  outfile.write(buffer.data(), buffer.size());

  if (outfile.fail()) {
    outfile.close();
    emscripten_log(EM_LOG_ERROR, "Error writing to file");
  }
  outfile.close();

  BrdViewerApp::App().HandleFileUpload("uploaded_file.brd");
}
