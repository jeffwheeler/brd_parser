#ifndef STRUCTURE_UTILS_H
#define STRUCTURE_UTILS_H

#include <iostream>
#include <optional>

#include "types.h"

std::pair<double, double> x01_center(const x01<kAMax> *inst);
const std::vector<stackup_material> ordered_stackup_materials(File<kAMax> &f);
std::string padtype(PadType padtype);

template <AllegroVersion version>
char *str_lookup(uint32_t id, File<version> &fs) {
  if (fs.strings.count(id) > 0) {
    return fs.strings[id];
  } else {
    return nullptr;
  }
}

template <AllegroVersion version>
const std::string x03_str_lookup(uint32_t id, File<version> &fs) {
  // const x03<version> *i = (const x03<version> *)fs.ptrs[id];
  auto& i = fs.get_x03(id);
  std::cout << "x03_str_lookup id=" << id << std::endl;
  if (i.subtype.size == 0) {
    std::cout << "size=0" << std::endl;
    return "";
  } else if (i.subtype.t == 0x6A) {
    std::cout << "0x6A" << std::endl;
    return std::string(str_lookup(i.ptr, fs));
  } else {
    std::cout << "not 0x6A" << std::endl;
    return i.s;
    // return i.s.c_str();
  }
}

template <AllegroVersion version>
std::optional<std::string> x2B_footprint(const x2B<version> *inst,
                                         File<version> *fs) {
  if (fs->strings.count(inst->footprint_string_ref) > 0) {
    return fs->strings.at(inst->footprint_string_ref);
  } else {
    return {};
  }
}

template <AllegroVersion version>
std::string inst_refdes(const x07<version> *inst, File<version> *fs) {
  return fs->strings.at(inst->refdes_string_ref);
}

template <AllegroVersion version>
std::optional<std::string> x2B_refdes(const uint32_t k, File<version> *fs) {
  if (!fs->is_type(k, 0x2B)) {
    return std::optional<std::string>();
  }

  const x2B<version> &inst = fs->get_x2B(k);
  return x2D_refdes(inst.ptr2, fs);
}

template <AllegroVersion version>
std::optional<std::string> x2D_refdes(const uint32_t k, File<version> *fs) {
  if (!fs->is_type(k, 0x2D)) {
    return std::optional<std::string>();
  }

  const x2D<version> &inst = fs->get_x2D(k);
  if constexpr (std::is_same_v<decltype(inst.inst_ref), std::monostate>) {
    return std::optional<std::string>();
  } else {
    if (inst.inst_ref == 0 || !fs->is_type(inst.inst_ref, 0x07)) {
      return std::optional<std::string>();
    }

    const x07<version> x07_inst = fs->get_x07(inst.inst_ref);
    return inst_refdes(&x07_inst, fs);
  }
}

template <AllegroVersion version>
std::optional<std::string> x2B_or_x2D_refdes(const uint32_t k,
                                             File<version> *fs) {
  std::optional<std::string> d_refdes = x2D_refdes(k, fs);
  if (d_refdes) {
    return d_refdes;
  } else {
    return x2B_refdes(k, fs);
  }
}

template <AllegroVersion version>
std::optional<std::string> x32_pin_name(const uint32_t k, File<version> *fs) {
  if (fs->is_type(k, 0x32)) {
    const x32<version> &inst = fs->get_x32(k);
    return x2B_or_x2D_refdes(inst.ptr3, fs).value_or(std::string("?")) + "." +
           x0D_pin_name(inst.ptr5, fs).value_or(std::string("?"));
  } else {
    return std::optional<std::string>();
  }
}

template <AllegroVersion version>
std::string x38_layer_name(const t38_film<version> &inst, File<version> *fs) {
  if constexpr (!std::is_same_v<decltype(inst.layer_name_str),
                                std::monostate>) {
    if (inst.layer_name_str == 0) {
      return inst.s;
    } else {
      std::string s = fs->strings.at(inst.layer_name_str);
      return s;
    }
  } else {
    return inst.s;
  }
}

template <AllegroVersion version>
std::vector<std::pair<uint8_t, uint8_t>> x39_layers(
    const t39_film_layer_list<version> &inst, File<version> &fs) {
  std::vector<std::pair<uint8_t, uint8_t>> layers;

  uint32_t next_key = inst.head;
  while (fs.is_type(next_key, 0x3A)) {
    const t3A_film_layer_list_node<version> &x3A_inst = fs.get_x3A(next_key);
    layers.push_back(std::make_pair(x3A_inst.subtype, x3A_inst.layer));
    next_key = x3A_inst.next;
  }

  return layers;
}

template <AllegroVersion version>
std::vector<std::pair<std::string, uint32_t>> film_list(File<version> &fs_x) {
  std::vector<std::pair<std::string, uint32_t>> list;
  File<version> *fs = &fs_x;

  for (auto &x38_inst : fs->iter_t38_film()) {
    list.push_back(std::make_pair(x38_layer_name(x38_inst, fs), x38_inst.k));
  }
  return list;
}

template <AllegroVersion version>
std::optional<std::string> x0D_pin_name(const uint32_t k, File<version> *fs) {
  if (!fs->is_type(k, 0x0D)) {
    return std::optional<std::string>();
  }

  const x0D<version> &inst = fs->get_x0D(k);
  if (inst.str_ptr == 0 || !HAS_ENTRY(strings, inst.str_ptr)) {
    return std::optional<std::string>();
  }

  return fs->strings.at(inst.str_ptr);
}

template <AllegroVersion version>
std::optional<uint8_t> x14_layer(const uint32_t k, File<version> *fs) {
  if (fs->is_type(k, 0x14)) {
    const x14<version> inst = fs->get_x14(k);
    return inst.layer;
  } else {
    return std::optional<uint8_t>();
  }
}

template <AllegroVersion version>
char *x1B_net_name(const uint32_t k, File<version> *fs) {
  if (fs->is_type(k, 0x1B)) {
    const t1B_net<version> inst = fs->get_x1B(k);
    return str_lookup(inst.net_name, *fs);
  } else {
    return nullptr;
  }
}

template <AllegroVersion version>
std::optional<uint8_t> x2D_layer(const uint32_t k, File<version> *fs) {
  if (fs->is_type(k, 0x2D)) {
    const x2D<version> &inst = fs->get_x2D(k);
    return x14_layer(inst.ptr1, fs);
  } else {
    return std::optional<uint8_t>();
  }
}

template <AllegroVersion version>
const x36_x08<version> *font_lookup(uint8_t k, File<version> &fs) {
  constexpr int8_t offset = -1;
  for (const auto &[x36_k, x36_inst] : fs.x36_map) {
    // FIXME: Throw an error if `k` is larger than the size of the list
    if (x36_inst.x08s.size() > 0) {
      // Unclear why there's an offset, but this seems to make things
      // work nicely?
      return &x36_inst.x08s[k + offset];
    }
  }
  return nullptr;
}

#endif
