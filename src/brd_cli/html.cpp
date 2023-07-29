#include "html.h"

#include <iostream>

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

void stream_header(std::string& fname, File<A_174>& f) {
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
    std::cout << R"A(
            </tbody>
        </table>
    )A";
}

void stream_strings(File<A_174>& f) {
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
    for (auto& [k, s] : f.strings) {
        printf(
            "<tr><td><code>0x%08X</code></td><td "
            "class='text-break'><code>%s</code></tr>\n",
            k, s.c_str());
    }
    std::cout << R"A(
                </tbody>
            </table>
        </div>
    )A";
}

void stream_x30(File<A_174>& f) {
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
    for (auto& [k, inst] : f.x30_map) {
        const x31* str_graphic =
            (const x31*)&f.x31_map.at(inst.str_graphic_ptr);

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
    std::cout << R"A(
                </tbody>
            </table>
        </div>
    )A";
}

void stream_file(std::string& fname, File<A_174>& f) {
    std::cout << HTML_HEADER;
    stream_header(fname, f);
    stream_strings(f);
    stream_x30(f);
    std::cout << HTML_FOOTER;
}
