#include "brdview_test.h"

#include "brd_gui/brdview.h"

void TestBrdView::drawAllSymbolPaths() {
  BrdView brdview;

  auto parsed_file = parse_file("../../test/data/fmc_tlu/fmc_tlu_v1f_38.brd");
  if (parsed_file) {
    brdview.loadFile(&parsed_file.value());
  }

  // Show the appropriate layer
  const std::set<std::pair<uint16_t, uint16_t>> layers = {{0x09, 0xFD}};
  brdview.selectLayer(layers);

  // If we don't iterate over all symbol paths, we miss this one.
  QVERIFY2(brdview.drewKey(ntohl(0xF6550000)),
           "Path unexpectedly missing after selecting its layer");
}

QTEST_MAIN(TestBrdView)
