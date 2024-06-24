#include "parser.h"

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <filesystem>

#include "lib/printing/printers.h"
#include "lib/printing/utils.h"
#include "lib/structure/utils.h"

void skip(void*& address, std::size_t n) {
  address = static_cast<void*>(static_cast<char*>(address) + n);
}

auto round_to_word(uint32_t len) -> uint32_t {
  if (len % 4 != 0) {
    return len / 4 * 4 + 4;
  }
  return len;
}

template <AllegroVersion version>
auto layer_count(File<version>* fs) -> uint8_t {
  std::tuple<uint32_t, uint32_t> tup = (fs->layers)[4];
  uint32_t ptr = std::get<1>(tup);
  if (fs->x2A_map.count(ptr) > 0) {
    const T2ACustomLayer* x = &fs->x2A_map.at(ptr);
    if (x->references) {
      return x->reference_entries.size();
    }
    return x->local_entries.size();
  }
  std::printf("Unable to determine layer count\n");
  exit(1);
}

template <template <AllegroVersion> typename T, AllegroVersion version>
uint32_t default_parser(File<kAMax>& fs, void*& address) {
  auto* inst = static_cast<T<version>*>(address);
  // fs.ptrs[inst->k] = address;
  // new_find_map<T<A_MAX>>(fs)[inst->k] = address;
  fs.ptrs[inst->k] = address;
  size_t size = sizeof_until_tail<T<version>>();
  skip(address, size);
  return inst->k;
}

template <AllegroVersion version>
auto parse_x03(File<kAMax>& fs, void*& address) -> uint32_t {
  x03<version>* i = static_cast<x03<version>*>(address);
  default_parser<x03, version>(fs, address);

  // inst.has_str = false;
  uint32_t size;

  // log(base_addr_glb, address, "- Subtype.t = 0x%02X\n", i->subtype.t);
  switch (i->subtype.t & 0xFF) {
    case 0x65:
      break;
    case 0x64:
    case 0x66:
    case 0x67:
    case 0x6A:
      skip(address, 4);
      break;
    case 0x6D:
    case 0x6E:
    case 0x6F:
    case 0x68:
    case 0x6B:
    case 0x71:
    case 0x73:
    case 0x78:
      skip(address, round_to_word(i->subtype.size));
      break;
    case 0x69:
      skip(address, 8);
      break;
    case 0x6C:
      size = *static_cast<uint32_t*>(static_cast<void*>(address));
      skip(address, 4 + 4 * size);
      break;
    case 0x70:
    case 0x74:
      uint16_t x[2];
      x[0] = *static_cast<uint16_t*>(static_cast<void*>(address));
      skip(address, 2);
      x[1] = *static_cast<uint16_t*>(static_cast<void*>(address));
      skip(address, 2);
      skip(address, x[1] + 4 * x[0]);
      break;
    case 0xF6:
      skip(address, 80);
      break;
    default:
      log(fs.region.get_address(), address,
          "- Unexpected value subtype=0x%02X\n", i->subtype.t);
      exit(1);
  };

  return 0;
}

template <AllegroVersion version>
uint32_t parse_x1C(File<kAMax>& fs, void*& address) {
  T1CPad<version>* i = static_cast<T1CPad<version>*>(address);
  uint32_t k = default_parser<T1CPad, version>(fs, address);

  uint16_t size;
  if constexpr (version < kA172) {
    // printf("layer count 1 %d\n", i->layer_count);
    size = 10 + i->layer_count * 3;
  } else {
    // printf("layer count 2 %d\n", i->layer_count);
    size = 21 + i->layer_count * 4;
  }

  for (uint32_t i = 0; i < size; i++) {
    // log(fs.region.get_address(), address, "- Skipping t13 %d\n", i);
    // log_n_words(address, 4);
    skip(address, sizeof_until_tail<t13<version>>());
  }
  // skip(address, size * sizeof_until_tail<t13<version>>());

  if constexpr (version >= kA172) {
    skip(address, 4);
  }

  if constexpr (version >= kA172) {
    skip(address, i->n * 40);
  } else {
    skip(address, i->n * 32);
  }

  return k;
}

template <AllegroVersion version>
auto parse_x1D(File<kAMax>& fs, void*& address) -> uint32_t {
  x1D<version>* i = static_cast<x1D<version>*>(address);
  default_parser<x1D, version>(fs, address);

  // log(&f, "size_a = %d, size_b = %d\n", size_a, size_b);
  skip(address, i->size_b * (version >= kA162 ? 56 : 48));
  skip(address, i->size_a * 256);
  if (version >= kA172) {
    skip(address, 4);
  }
  return 0;
}

template <AllegroVersion version>
uint32_t parse_x1E(File<kAMax>& fs, void*& address) {
  x1E* x1E_inst = new x1E;
  memcpy(x1E_inst, address, sizeof(x1E_hdr));
  skip(address, sizeof(x1E_hdr));
  // f.read((char*)x1E_inst, sizeof(x1E_hdr));
  x1E_inst->s = static_cast<char*>(address);
  skip(address, round_to_word(x1E_inst->hdr.size));
  // f.read(x1E_inst->s, round_to_word(x1E_inst->hdr.size));
  (fs.x1E_map)[x1E_inst->hdr.k] = *x1E_inst;
  if (version >= kA172) {
    skip(address, 4);
  }
  return 0;
}

template <AllegroVersion version>
uint32_t parse_x1F(File<kAMax>& fs, void*& address) {
  x1F<version>* i = static_cast<x1F<version>*>(address);
  uint32_t k = default_parser<x1F, version>(fs, address);

  if constexpr (version >= kA175) {
    skip(address, i->size * 384 + 8);
  } else if constexpr (version >= kA172) {
    skip(address, i->size * 280 + 8);
  } else if constexpr (version >= kA162) {
    skip(address, i->size * 280 + 4);
  } else {
    skip(address, i->size * 240 + 4);
  }

  return k;
}

template <AllegroVersion version>
uint32_t parse_x21(File<kAMax>& fs, void*& address) {
  // x21_header i;
  // f.read((char*)&i, sizeof(x21_header));
  x21_header* i = static_cast<x21_header*>(address);
  // skip(address, -sizeof(x21_header));
  // log(&f, "- Next words:");
  // log_n_words(&f, 8);
  // skip(&f, -4 * 8);
  if (i->r == 1304) {
    stackup_material ps;
    // f.read((char*)&ps, sizeof(ps));
    memcpy(&ps, address, sizeof(stackup_material));
    skip(address, sizeof(stackup_material));
    fs.stackup_materials[ps.hdr.k] = ps;
    // log(&f, "- - Stackup material... %02d->%s\n", ps.layer_id,
    // ps.material);
  } else if (i->r == 14093) {
    // log(&f, "- - i->r=\x1b[31m14093\x1b[0m...?\n");
    skip(address, i->size);
    // } else if (i->r == 2050) {
  } else if (i->r == 0x0407) {
    meta_netlist_path r{};
    skip(address, sizeof_until_tail<meta_netlist_path>());
    skip(address, 1028);
    // char s[1028];
    // f.read((char*)&r, sizeof_until_tail<meta_netlist_path>());
    // f.read(s, sizeof(s));
    // r.path = std::string(s);
    fs.netlist_path = r;
  } else {
    skip(address, i->size);
  }
  return 0;
}

template <AllegroVersion version>
uint32_t parse_x27(File<kAMax>& fs, void*& address) {
  address =
      static_cast<char*>(fs.region.get_address()) + fs.hdr->x27_end_offset - 1;
  return 0;
}

template <AllegroVersion version>
uint32_t parse_x2A(File<kAMax>& fs, void*& address) {
  T2ACustomLayer x2A_inst;
  memcpy(static_cast<void*>(&x2A_inst), address, sizeof(x2A_hdr));

  // f.read((char*)&x2A_inst.hdr, sizeof(x2A_hdr));
  skip(address, sizeof(x2A_hdr));
  if (version >= kA174) {
    skip(address, 4);
  }

  if (version <= kA164) {
    x2A_inst.references = false;
    for (uint16_t i = 0; i < x2A_inst.hdr.size; i++) {
      char buf[36] = {0};
      // f.read((char*)buf, 36);
      memcpy(buf, address, 36);
      skip(address, 36);
      x2A_layer_properties suffix =
          *reinterpret_cast<x2A_layer_properties*>(&buf[32]);
      x2A_local_entry entry = x2A_local_entry{std::string(buf), suffix};
      x2A_inst.local_entries.push_back(entry);
    }
  } else {
    x2A_inst.references = true;
    for (uint16_t i = 0; i < x2A_inst.hdr.size; i++) {
      x2A_reference_entry entry;
      // f.read((char*)&entry, sizeof(x2A_reference_entry));
      memcpy(&entry, address, sizeof(x2A_reference_entry));
      skip(address, sizeof(x2A_reference_entry));
      x2A_inst.reference_entries.push_back(entry);
      if (PRINT_ALL_ITEMS) {
        // log(&f, "- - Entry: %08X %08X %08X\n", ntohl(entry.ptr),
        //     ntohl(entry.suffix), ntohl(entry.un1));
      }
    }
  }
  // f.read((char*)&x2A_inst.k, 4);
  x2A_inst.k = *static_cast<uint32_t*>(address);
  skip(address, 4);
  (fs.x2A_map)[x2A_inst.k] = x2A_inst;
  if (PRINT_ALL_ITEMS) {
    // log(&f, "- Inserting x2A with key %08X\n", ntohl(x2A_inst.k));
  }
  return 0;
}

template <AllegroVersion version>
uint32_t parse_x31(File<kAMax>& fs, void*& address) {
  T31String<version>* i = static_cast<T31String<version>*>(address);
  default_parser<T31String, version>(fs, address);

  if (i->len > 0) {
    uint32_t len = round_to_word(i->len);
    skip(address, len);
  }

  return 0;
}

template <AllegroVersion version>
uint32_t parse_x35([[maybe_unused]] File<kAMax>& fs, void*& address) {
  skip(address, 124);
  return 0;
}

template <AllegroVersion version>
uint32_t parse_x36(File<kAMax>& fs, void*& address) {
  x36<version> inst;
  memcpy(static_cast<void*>(&inst), address, sizeof_until_tail<x36<version>>());
  skip(address, sizeof_until_tail<x36<version>>());
  // f.read((char*)inst, sizeof_until_tail<x36<version>>());

  // log(&f, "- x36\n");
  // log(&f, "- - inst->c    = 0x %04X\n", ntohs(inst->c));
  // log(&f, "- - inst->k    = 0x %08X\n", ntohl(inst->k));
  // log(&f, "- - inst->next = 0x %08X\n", ntohl(inst->next));
  // if constexpr (!std::is_same_v<decltype(inst->un1), std::monostate>) {
  //     log(&f, "- - inst->un1  = 0x %08X\n", ntohl(inst->un1));
  // } else {
  //     log(&f, "- - inst->un1  = n/a\n");
  // }
  // log(&f, "- - inst->size = %d\n", inst->size);
  // log(&f,
  //     "- - inst->count = %d, inst->last_idx = %d, inst->un3 = 0x"
  //     " % 08X\n ",
  //     inst->count, inst->last_idx, ntohl(inst->un3));
  // if constexpr (!std::is_same_v<decltype(inst->un2), std::monostate>) {
  //     log(&f, "- - inst->un2  = 0x %08X\n", ntohl(inst->un2));
  // } else {
  //     log(&f, "- - inst->un2  = n/a\n");
  // }

  switch (inst.c) {
    case 0x02:
      for (uint32_t i = 0; i < inst.size; i++) {
        // Discard
        // x36_x02<version> inst_x02 =
        //     *static_cast<x36_x02<version>*>(address);
        skip(address, sizeof_until_tail<x36_x02<version>>());
        // f.read((char*)&inst_x02,
        // sizeof_until_tail<x36_x02<version>>()); log(&f, "- x02.str =
        // %s\n", inst_x02.str);
      }
      break;
    case 0x06:
      for (uint32_t i = 0; i < inst.size; i++) {
        // Currently just read the object and immediately throw it
        // away, because we don't know of any use for it.
        // x36_x06<version> inst_x06 =
        //     *static_cast<x36_x06<version>*>(address);
        skip(address, sizeof_until_tail<x36_x06<version>>());
        // f.read((char*)&inst_x06,
        // sizeof_until_tail<x36_x06<version>>()); log(&f, "- x06.n =
        // %d\n", inst_x06.n); log(&f, "- x06.r = %d\n", inst_x06.r);
        // log(&f, "- x06.s = %d\n", inst_x06.s);
        // log(&f, "- x06.un1 = %d\n\n", inst_x06.un1);
      }
      break;
    case 0x03:
      for (uint32_t i = 0; i < inst.size; i++) {
        // Just throw this away after reading it.
        // x36_x03<version> inst_x03 =
        //     *static_cast<x36_x03<version>*>(address);
        skip(address, sizeof_until_tail<x36_x03<version>>());
        // f.read((char*)&inst_x03,
        // sizeof_until_tail<x36_x03<version>>()); if (version >= A_172)
        // {
        //     log(&f, "- - s = \"%s\"\n", inst_x03.str);
        // } else {
        //     log(&f, "- - s = \"%s\"\n", inst_x03.str_16x);
        // }
      }
      break;
    case 0x05:
      skip(address, inst.size * 28);
      break;
    case 0x08:
      for (uint32_t i = 0; i < inst.size; i++) {
        x36_x08<version> x = *static_cast<x36_x08<version>*>(address);
        skip(address, sizeof_until_tail<x36_x08<version>>());
        // f.read((char*)&x, sizeof_until_tail<x36_x08<version>>());
        inst.x08s.push_back(x);
        // log(&f,
        //     "- - 0x%02X %08x %08x"
        //     " xs={h=% 7d w=% 7d % 7d % 7d % 7d %7d}",
        //     i, ntohl(x.a), ntohl(x.b), x.char_height, x.char_width,
        //     x.xs[0], x.xs[1], x.xs[2], x.xs[3]);
        // if constexpr (!std::is_same_v<decltype(x.ys),
        // std::monostate>) {
        //     printf(" ys={% 7d % 7d % 7d}", x.ys[0], x.ys[1],
        //     x.ys[2]);
        // }
        // printf("\n");
      }
      break;
    case 0x0B:
      // No clue what this represents?
      skip(address, inst.size * 1016);
      // for (uint32_t i = 0; i < inst.size; i++) {
      //     log(&f, "- - Next words:");
      //     log_n_words(&f, 6);
      //     skip(&f, 1016 - 4 * 6);
      // }
      break;
    case 0x0C:
      skip(address, inst.size * 232);
      break;
    case 0x0D:
      skip(address, inst.size * 200);
      break;
    case 0x0F:
      for (uint32_t i = 0; i < inst.size; i++) {
        x36_x0F<version> x = *static_cast<x36_x0F<version>*>(address);
        skip(address, sizeof(x36_x0F<version>));
        // f.read((char*)&x, sizeof(x36_x0F<version>));
        inst.x0Fs.push_back(x);
      }
      break;
    case 0x10:
      skip(address, inst.size * 108);
      // for (uint32_t i = 0; i < inst.size; i++) {
      //     log(&f, "- - Next words:");
      //     log_n_words(&f, 6);
      //     skip(&f, 108 - 4 * 6);
      // }
      break;
    default:
      log(fs.region.get_address(), address,
          "- Don\'t know how to handle c=%X\n", inst.c);
      exit(1);
  }

  fs.x36_map[inst.k] = upgrade<version, kAMax>(inst);
  return 0;
}

template <AllegroVersion version>
uint32_t parse_x3B([[maybe_unused]] File<kAMax>& fs, void*& address) {
  x3B<version>* i = static_cast<x3B<version>*>(address);

  skip(address, sizeof_until_tail<x3B<version>>());
  skip(address, round_to_word(i->len));

  return 0;
}

template <AllegroVersion version>
uint32_t parse_x3C([[maybe_unused]] File<kAMax>& fs, void*& address) {
  x3C<version>* i = static_cast<x3C<version>*>(address);
  default_parser<x3C, version>(fs, address);
  skip(address, i->size * 4);
  return 0;
}

template <AllegroVersion version>
auto parse_file_raw(boost::interprocess::mapped_region region) -> File<kAMax> {
  void* base_addr = region.get_address();
  void* cur_addr = base_addr;
  size_t size = region.get_size();

  File<kAMax> fs(std::move(region));

  // Read header
  fs.hdr = static_cast<Header*>(base_addr);
  // memcpy(fs.hdr, cur_addr, sizeof(header));
  if (PRINT_ALL_ITEMS) {
    printf("Magic: 0x%08X\n", fs.hdr->magic);
    printf("Logcated at %p\n", &fs.hdr->magic);
    std::cout << fs.hdr->allegro_version << std::endl;
  }
  skip(cur_addr, sizeof(Header));

  // This must be done after reading the header
  fs.prepare();

  // Layer map
  for (uint8_t i = 0; i < 25; i++) {
    uint32_t xs[2] = {*static_cast<uint32_t*>(cur_addr),
                      *(static_cast<uint32_t*>(cur_addr) + 1)};
    skip(cur_addr, sizeof(xs));
    fs.layers.emplace_back(xs[0], xs[1]);
  }

  // Strings map
  cur_addr = static_cast<char*>(base_addr) + 0x1200;
  for (uint32_t i = 0; i < fs.hdr->strings_count; i++) {
    uint32_t id = *static_cast<uint32_t*>(cur_addr);
    skip(cur_addr, 4);

    fs.strings[id] = static_cast<char*>(cur_addr);

    // Add one to include the NULL byte that might force the length to one
    // word longer.
    uint32_t len = strlen(static_cast<char*>(cur_addr));
    skip(cur_addr, round_to_word(len + 1));
  }

  // All other objects
  while (cur_addr < static_cast<char*>(base_addr) + size &&
         *static_cast<uint8_t*>(cur_addr) != 0x00) {
    uint8_t t = *static_cast<uint8_t*>(cur_addr);
    if (PRINT_ALL_ITEMS) {
      log(base_addr, cur_addr, "Handling t=0x%02X: ", t);
      log_n_words(cur_addr, 4);
    }

    const parser_t parser = PARSER_TABLE<version>[t];
    if (t < 0x3E && parser.parse != 0) {
      parser.parse(fs, cur_addr);
    } else {
      log(base_addr, cur_addr, "Stuck here at t=0x%02X\n", t);
      break;
    }
  }

  // log(base_addr_glb, cur_addr, "Magic = %08X\n", fs.hdr->magic);
  fs.layer_count = layer_count(&fs);

  return fs;
}

auto parse_file(const std::string& filepath) -> std::optional<File<kAMax>> {
  if (!std::filesystem::exists(filepath)) {
    printf("Unable to open file because it does not exist.\n");
    return {};
  }

  boost::interprocess::file_mapping mapped_file;
  try {
    mapped_file = boost::interprocess::file_mapping(
        filepath.c_str(), boost::interprocess::read_only);
  } catch (const boost::interprocess::interprocess_exception& e) {
    printf("Unable to open file. Exception: %s.\n", e.what());
    return {};
  }
  boost::interprocess::mapped_region region(mapped_file,
                                            boost::interprocess::read_only);

  /*
  if (!f.is_open()) {
      printf("Failed to open file \"%s\"\n", filepath.c_str());
      return {};
  }
  */

  void* address = region.get_address();

  uint32_t magic = *static_cast<uint32_t*>(address);
  switch (magic) {
    case 0x00130000:
    case 0x00130200:
      return parse_file_raw<kA160>(std::move(region));
    case 0x00130402:
      return parse_file_raw<kA162>(std::move(region));
    case 0x00130C03:
      return parse_file_raw<kA164>(std::move(region));
    case 0x00131003:
      return parse_file_raw<kA165>(std::move(region));
    case 0x00131503:
    case 0x00131504:
      return parse_file_raw<kA166>(std::move(region));
    case 0x00140400:
    case 0x00140500:
    case 0x00140501:
    case 0x00140502:
    case 0x00140600:
    case 0x00140700:
      return parse_file_raw<kA172>(std::move(region));
    case 0x00140900:
    case 0x00140901:
    case 0x00140902:
    case 0x00140E00:
      return parse_file_raw<kA174>(std::move(region));
    case 0x00141502:
      return parse_file_raw<kAMax>(std::move(region));
  }

  printf("Magic unrecognized! Magic = 0x%08X\n", magic);
  return {};
}
