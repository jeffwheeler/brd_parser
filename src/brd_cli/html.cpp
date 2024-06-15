#include "html.h"

#include <iostream>

#include "lib/structure/utils.h"

char HTML_HEADER[] = R"A(
<!doctype html>
<html lang="en-US">
<head>
<title>.brd decoding</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-4bw+/aepP/YC94hEpVNVgiZdgIC5+VKNBQNGCHeKRQN+PtmoHDEXuppvnDJzQIu9" crossorigin="anonymous">
    <style>
    </style>
</head>
<body data-bs-theme="dark">
    <div class="container">
)A";

char HTML_FOOTER[] = R"A(
    </div>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.1/dist/js/bootstrap.min.js" integrity="sha384-Rx+T1VzGupg4BHQYs2gCW9It+akI2MM/mndMCy36UVfodzcJcF0GGLxZIzObiEfa" crossorigin="anonymous"></script>
</body>
</html>
)A";

void stream_header(std::string &fname, File<kAMax> &f) {
  std::cout << "<h1>Decoded .brd file</h1>";
  printf("<p>Filename: <code>%s</code></p>", fname.c_str());
  std::cout << R"A(
        <h2>Header</h2>
        <table class="table" style="max-width: 500px;">
            <thead>
                <tr>
                    <th>Parameter</th>
                    <th>Value</th>
                </tr>
            </thead>
            <tbody>
    )A";
  printf("<tr><td>Magic</td><td><code>0x%08X</code></td></tr>", f.hdr->magic);
  printf(
      "<tr><td>Allegro Version</td><td "
      "class='text-break'><code>%s</code></td></tr>",
      f.hdr->allegro_version);
  std::cout << "<tr><td>Units</td>";
  switch (f.hdr->units) {
    case Units::kMetric:
      std::cout << "<td>Metric</td>";
      break;
    case Units::kImperial:
      std::cout << "<td>Imperial</td>";
      break;
    default:
      printf("<td>Unrecognized! <code>%02hhX</code></td>",
             (uint8_t)f.hdr->units);
  };
  std::cout << R"A(
                </tr>
            </tbody>
        </table>
    )A";
}

void stream_layers(File<kAMax> &f) {
  std::cout << R"A(
        <h2>Layers</h2>
        <p>Not all fields shown</p>
        <div class="border overflow-auto mb-3" style="max-height: 300px; max-width: 900px">
            <table class="table">
                <thead>
                    <tr>
                        <th>Layer ID</th>
                        <th>Material</th>
                        <th>Kind</th>
                        <th>Thickness</th>
                        <th>Dk</th>
                        <th>Df</th>
                    </tr>
                </thead>
                <tbody>
    )A";
  const auto &stackup_materials = ordered_stackup_materials(f);
  if (stackup_materials.empty()) {
    std::cout
        << "<tr><td colspan=\"6\">No layers material details found</td></tr>";
  } else {
    for (const auto &sm : ordered_stackup_materials(f)) {
      printf(
          "<tr><td>%d</td><td><em>%s</em></td>"
          "<td><em>%s</em></td><td><em>%s</em></td>"
          "<td><em>%s</em></td><td><em>%s</em></td></tr>\n",
          sm.layer_id, sm.material, sm.kind, sm.thickness, sm.d_k, sm.d_f);
    }
  }
  std::cout << R"A(
                </tbody>
            </table>
        </div>
    )A";
}

void stream_films(File<kAMax> &f) {
  std::cout << R"A(
        <h2>Films</h2>
        <div class="border overflow-auto mb-3" style="max-height: 300px; max-width: 900px">
            <table class="table">
                <thead>
                    <tr>
                        <th>Film name</th>
                        <th>Layers</th>
                    </tr>
                </thead>
                <tbody>
    )A";
  for (const auto &x38_inst : f.iter_t38_film()) {
    printf("<tr><td><code>%s</code></td><td>",
           x38_layer_name(x38_inst, &f).c_str());
    const auto &layer_list = f.get_x39(x38_inst.layer_list);
    for (const auto &[a, b] : x39_layers(layer_list, f)) {
      printf("<code>%02X.%02X</code>, ", a, b);
    }
    printf("</td></tr>");
  }
  std::cout << R"A(
                </tbody>
            </table>
        </div>
    )A";
}

void stream_strings(File<kAMax> &f) {
  std::cout << R"A(
        <h2>Strings</h2>
        <div class="border overflow-auto mb-3" style="max-height: 300px; max-width: 500px">
            <table class="table">
                <thead>
                    <tr>
                        <th>Key</th>
                        <th>Contents</th>
                    </tr>
                </thead>
                <tbody>
    )A";
  for (auto &[k, s] : f.strings) {
    printf(
        "<tr><td><code>0x%08X</code></td><td "
        "class='text-break'><code>%s</code></tr>\n",
        k, s);
  }
  std::cout << R"A(
                </tbody>
            </table>
        </div>
    )A";
}

void stream_nets(File<kAMax> &f) {
  std::cout << R"A(
        <h2>Nets</h2>
        <div class="border overflow-auto mb-3" style="max-height: 300px; max-width: 900px">
            <table class="table">
                <thead>
                    <tr>
                        <th>Key</th>
                        <th>Net name</th>
                        <th>Path</th>
                    </tr>
                </thead>
                <tbody>
    )A";
  for (auto &net : f.iter_t1B_net()) {
    printf("<tr><td><code>0x%08X</code></td><td><code>%s</code></td>", net.k,
           x1B_net_name(net.k, &f));

    if (net.path_str_ptr == 0) {
      std::cout << "<td>Blank path</td></tr>";
    } else {
      printf("<td><code>%s</code></td></tr>",
             x03_str_lookup(net.path_str_ptr, f).c_str());
    }
  }
  std::cout << R"A(
                </tbody>
            </table>
        </div>
    )A";
}

void stream_x30([[maybe_unused]] File<kAMax> &f) {
  std::cout << R"A(
        <h2>Placed string objects</h2>
        <div class="border overflow-auto mb-3" style="max-height: 300px; max-width: 900px">
            <table class="table" style="max-width: 900px;">
                <thead>
                    <tr>
                        <th>Key</th>
                        <th>Subtype/layer</th>
                        <th style="width: 200px;">Coordinates</th>
                        <th>Contents</th>
                    </tr>
                </thead>
                <tbody>
    )A";
  /*
  for (auto& [k, inst] : f.x30_map) {
      const x31<A_MAX>* str_graphic =
          (const x31<A_MAX>*)&f.x31_map.at(inst.str_graphic_ptr);

      printf(R"A(
              <tr>
                  <td><code>0x%08X</code></td>
                  <td><code>%d</code>/<code>%d</code></td>
                  <td>(%d, %d)</td>
                  <td><code class="text-wrap">%s</code></td>
              </tr>
      )A",
             k, inst.subtype, inst.layer, inst.coords[0], inst.coords[1],
             str_graphic->s.c_str());
  }
  */
  std::cout << R"A(
                </tbody>
            </table>
        </div>
    )A";
}

void stream_misc_metadata(File<kAMax> &f) {
  std::cout << R"A(
        <h2>Misc other metadata</h2>
        <div class="mb-3">
    )A";
  if (f.netlist_path) {
    printf("<p>Netlist path: <code>%s</code></p>",
           (*f.netlist_path).path.c_str());
  } else {
    std::cout << "<p>Netlist path <em>unspecified</em></p>";
  }
  std::cout << R"A(
        </div>
    )A";
}

void stream_file(std::string &fname, File<kAMax> &f) {
  std::cout << HTML_HEADER;
  stream_header(fname, f);
  stream_layers(f);
  stream_films(f);
  // stream_strings(f);
  stream_nets(f);
  // stream_x30(f);
  stream_misc_metadata(f);
  std::cout << HTML_FOOTER;
}
