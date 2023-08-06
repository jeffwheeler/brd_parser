#ifndef STRUCTURE_UTILS_H
#define STRUCTURE_UTILS_H

#include <optional>

#include "types.h"

std::pair<double, double> x01_center(const x01<A_174> *inst);
const std::vector<stackup_material> ordered_stackup_materials(File<A_174> &f);

template <AllegroVersion version>
char *str_lookup(uint32_t id, File<version> &fs) {
    if (fs.strings.count(id) > 0) {
        return fs.strings[id];
    } else {
        return nullptr;
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
    if (!HAS_ENTRY(x2B_map, k)) {
        return std::optional<std::string>();
    }

    const x2B<version> *inst = (const x2B<version> *)&fs->x2B_map.at(k);
    return x2D_refdes(inst->ptr2, fs);
}

template <AllegroVersion version>
std::optional<std::string> x2D_refdes(const uint32_t k, File<version> *fs) {
    if (!HAS_ENTRY(x2D_map, k)) {
        return std::optional<std::string>();
    }

    const x2D<version> *inst = (const x2D<version> *)&fs->x2D_map.at(k);
    if constexpr (std::is_same_v<decltype(inst->inst_ref), std::monostate>) {
        return std::optional<std::string>();
    } else {
        if (inst == nullptr || inst->inst_ref == 0 ||
            !HAS_ENTRY(x07_map, inst->inst_ref)) {
            return std::optional<std::string>();
        }

        const x07<version> *x07_inst = &fs->x07_map.at(inst->inst_ref);
        return inst_refdes(x07_inst, fs);
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
    if (HAS_ENTRY(x32_map, k)) {
        const x32<version> *inst = &fs->x32_map.at(k);
        return x2B_or_x2D_refdes(inst->ptr3, fs).value_or(std::string("?")) +
               "." + x0D_pin_name(inst->ptr5, fs).value_or(std::string("?"));
    } else {
        return std::optional<std::string>();
    }
}

template <AllegroVersion version>
std::string x38_layer_name(const x38<version> &inst, File<version> *fs) {
    if constexpr (!std::is_same_v<decltype(inst.ptr), std::monostate>) {
        if (inst.ptr == 0) {
            return inst.s;
        } else {
            std::string s = fs->strings.at(inst.ptr);
            return s;
        }
    } else {
        return inst.s;
    }
}

template <AllegroVersion version>
std::vector<std::pair<uint8_t, uint8_t>> x39_layers(const x39<version> &inst,
                                                    File<version> &fs_x) {
    std::vector<std::pair<uint8_t, uint8_t>> layers;
    File<version> *fs = &fs_x;

    uint32_t next_key = inst.ptr1;
    while (HAS_ENTRY(x3A_map, next_key)) {
        const x3A<version> *x3A_inst =
            (const x3A<version> *)&fs->x3A_map.at(next_key);
        layers.push_back(std::make_pair(x3A_inst->subtype, x3A_inst->layer));
        next_key = x3A_inst->next;
    }

    return layers;
}

template <AllegroVersion version>
std::vector<std::pair<std::string, uint32_t>> layer_list(File<version> &fs_x) {
    std::vector<std::pair<std::string, uint32_t>> list;
    File<version> *fs = &fs_x;

    for (const auto &[k, x38_inst] : fs->x38_map) {
        list.push_back(std::make_pair(x38_layer_name(x38_inst, fs), k));
    }
    return list;
}

template <AllegroVersion version>
std::optional<std::string> x0D_pin_name(const uint32_t k, File<version> *fs) {
    if (!HAS_ENTRY(x0D_map, k)) {
        return std::optional<std::string>();
    }

    const x0D<version> *inst = (const x0D<version> *)&fs->x0D_map.at(k);
    if (inst == nullptr || inst->str_ptr == 0 ||
        !HAS_ENTRY(strings, inst->str_ptr)) {
        return std::optional<std::string>();
    }

    return fs->strings.at(inst->str_ptr);
}

template <AllegroVersion version>
std::optional<uint8_t> x14_layer(const uint32_t k, File<version> *fs) {
    if (HAS_ENTRY(x14_map, k)) {
        const x14<version> inst = fs->get_x14(k);
        return inst.layer;
    } else {
        return std::optional<uint8_t>();
    }
}

template <AllegroVersion version>
std::optional<std::string> x1B_net_name(const uint32_t k, File<version> *fs) {
    if (HAS_ENTRY(x1B_map, k)) {
        const x1B<version> *inst = &fs->x1B_map.at(k);
        return fs->strings.at(inst->net_name);
    } else {
        return std::optional<std::string>();
    }
}

template <AllegroVersion version>
std::optional<uint8_t> x2D_layer(const uint32_t k, File<version> *fs) {
    if (HAS_ENTRY(x2D_map, k)) {
        const x2D<version> *inst = &fs->x2D_map.at(k);
        return x14_layer(inst->ptr1, fs);
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

template <AllegroVersion version>
int8_t read_layer(File<version> &fs, uint32_t k) {
    if (fs.x05_map.count(k) > 0) {
        const x05<version> *inst = (const x05<version> *)&fs.x05_map.at(k);
        return inst->layer;
    } else if (fs.x28_map.count(k) > 0) {
        const x28<version> *inst = (const x28<version> *)&fs.x28_map.at(k);
        return inst->layer;
    } else if (fs.x34_map.count(k) > 0) {
        const x34<version> *inst = (const x34<version> *)&fs.x34_map.at(k);
        return inst->layer;
    }

    return -1;
}

template <typename T, AllegroVersion version>
constexpr std::map<uint32_t, T> *find_map(File<version> &fs) {
    /* if constexpr (std::is_same_v<T, x01<version>>) {
        return &fs.x01_map;
    } else */
    if constexpr (std::is_same_v<T, x03<version>>) {
        return &fs.x03_map;
    } else if constexpr (std::is_same_v<T, x04<version>>) {
        return &fs.x04_map;
    } else if constexpr (std::is_same_v<T, x05<version>>) {
        return &fs.x05_map;
    } else if constexpr (std::is_same_v<T, x06<version>>) {
        return &fs.x06_map;
    } else if constexpr (std::is_same_v<T, x07<version>>) {
        return &fs.x07_map;
    } else if constexpr (std::is_same_v<T, x08<version>>) {
        return &fs.x08_map;
    } else if constexpr (std::is_same_v<T, x09<version>>) {
        return &fs.x09_map;
    } else if constexpr (std::is_same_v<T, x0A<version>>) {
        return &fs.x0A_map;
    } else if constexpr (std::is_same_v<T, x0C<version>>) {
        return &fs.x0C_map;
    } else if constexpr (std::is_same_v<T, x0D<version>>) {
        return &fs.x0D_map;
    } else if constexpr (std::is_same_v<T, x0E<version>>) {
        return &fs.x0E_map;
    } else if constexpr (std::is_same_v<T, x0F<version>>) {
        return &fs.x0F_map;
    } else if constexpr (std::is_same_v<T, x10<version>>) {
        return &fs.x10_map;
    } else if constexpr (std::is_same_v<T, x11<version>>) {
        return &fs.x11_map;
    } else if constexpr (std::is_same_v<T, x15<version>>) {
        return &fs.x15_map;
    } else if constexpr (std::is_same_v<T, x16<version>>) {
        return &fs.x16_map;
    } else if constexpr (std::is_same_v<T, x17<version>>) {
        return &fs.x17_map;
    } else if constexpr (std::is_same_v<T, x1B<version>>) {
        return &fs.x1B_map;
    } else if constexpr (std::is_same_v<T, x1D<version>>) {
        return &fs.x1D_map;
    } else if constexpr (std::is_same_v<T, x1F<version>>) {
        return &fs.x1F_map;
    } else if constexpr (std::is_same_v<T, x20<version>>) {
        return &fs.x20_map;
    } else if constexpr (std::is_same_v<T, x22<version>>) {
        return &fs.x22_map;
    } else if constexpr (std::is_same_v<T, x23<version>>) {
        return &fs.x23_map;
    } else if constexpr (std::is_same_v<T, x24<version>>) {
        return &fs.x24_map;
    } else if constexpr (std::is_same_v<T, x26<version>>) {
        return &fs.x26_map;
    } else if constexpr (std::is_same_v<T, x28<version>>) {
        return &fs.x28_map;
    } else if constexpr (std::is_same_v<T, x2B<version>>) {
        return &fs.x2B_map;
    } else if constexpr (std::is_same_v<T, x2C<version>>) {
        return &fs.x2C_map;
    } else if constexpr (std::is_same_v<T, x2D<version>>) {
        return &fs.x2D_map;
    } else if constexpr (std::is_same_v<T, x2E<version>>) {
        return &fs.x2E_map;
    } else if constexpr (std::is_same_v<T, x2F<version>>) {
        return &fs.x2F_map;
    } else if constexpr (std::is_same_v<T, x30<version>>) {
        return &fs.x30_map;
    } else if constexpr (std::is_same_v<T, x31<version>>) {
        return &fs.x31_map;
    } else if constexpr (std::is_same_v<T, x32<version>>) {
        return &fs.x32_map;
    } else if constexpr (std::is_same_v<T, x33<version>>) {
        return &fs.x33_map;
    } else if constexpr (std::is_same_v<T, x34<version>>) {
        return &fs.x34_map;
    } else if constexpr (std::is_same_v<T, x37<version>>) {
        return &fs.x37_map;
    } else if constexpr (std::is_same_v<T, x38<version>>) {
        return &fs.x38_map;
    } else if constexpr (std::is_same_v<T, x39<version>>) {
        return &fs.x39_map;
    } else if constexpr (std::is_same_v<T, x3A<version>>) {
        return &fs.x3A_map;
    } else if constexpr (std::is_same_v<T, x3C<version>>) {
        return &fs.x3C_map;
    }
}

template <typename T, AllegroVersion version>
constexpr std::map<uint32_t, void *> &new_find_map(File<version> &fs) {
    if constexpr (std::is_same_v<T, x01<version>>) {
        return fs.x01_map;
    } else if constexpr (std::is_same_v<T, x14<version>>) {
        return fs.x14_map;
    } else if constexpr (std::is_same_v<T, x15<version>>) {
        return fs.x15_map;
    } else if constexpr (std::is_same_v<T, x16<version>>) {
        return fs.x16_map;
    } else if constexpr (std::is_same_v<T, x17<version>>) {
        return fs.x17_map;
    }
}

#endif
