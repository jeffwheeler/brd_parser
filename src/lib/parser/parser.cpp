#include "parser.h"

#include "lib/printing/printers.h"
#include "lib/printing/utils.h"
#include "lib/structure/utils.h"

void skip(std::ifstream* f, std::ifstream::pos_type n) {
    f->seekg(n, std::ios_base::cur);
}

void skip_and_pad(std::ifstream* f, std::ifstream::pos_type n) {
    f->seekg(n, std::ios_base::cur);
    if (f->tellg() % 4 != 0) {
        f->seekg(f->tellg() / 4 * 4 + 4);
    }
}

uint32_t round_to_word(uint32_t len) {
    if (len % 4 != 0) {
        return len / 4 * 4 + 4;
    } else {
        return len;
    }
}

template <AllegroVersion version>
uint8_t layer_count(File<version>* fs) {
    std::tuple<uint32_t, uint32_t> tup = (fs->layers)[4];
    uint32_t ptr = std::get<1>(tup);
    if (fs->x2A_map.count(ptr) > 0) {
        const x2A* x = &fs->x2A_map.at(ptr);
        if (x->references) {
            return x->reference_entries.size();
        } else {
            return x->local_entries.size();
        }
    } else {
        std::printf("Unable to determine layer count\n");
        exit(1);
    }
}

template <template <AllegroVersion> typename T, AllegroVersion version>
uint32_t default_parser(File<A_174>& fs, std::ifstream& f) {
    T<version>* inst = new T<version>;
    f.read((char*)inst, sizeof_until_tail<T<version>>());
    std::map<uint32_t, T<A_174>>* m = find_map<T<A_174>>(fs);
    if (m->count(inst->k) > 0) {
        log(&f, "- Already seen this key! = 0x %08X\n", ntohl(inst->k));
        exit(1);
    }
    (*m)[inst->k] = upgrade<version, A_174>(*inst);
    return inst->k;
}

template <AllegroVersion version>
uint32_t parse_x03(File<A_174>& fs, std::ifstream& f) {
    char* buf;
    x03* x03_inst = new x03;
    f.read((char*)&x03_inst->hdr, sizeof(x03_hdr));
    if (fs.x03_map.count(x03_inst->hdr.k) > 0) {
        log(&f, "- Already seen this key!\n");
        exit(1);
    }

    if (version >= A_172) {
        // log(&f, "- Next words:");
        // log_n_words(&f, 8);
        // skip(&f, -4*8);
        f.read((char*)&x03_inst->hdr.subtype, 4);
        skip(&f, 4);
    }

    // buf = new char[subtype.size];
    buf = (char*)malloc(x03_inst->hdr.subtype.size);
    x03_inst->s = std::string();
    x03_inst->has_str = false;

    // log(&f, "- Subtype.t = 0x%02X\n", x03_inst->hdr.subtype.t);

    switch (x03_inst->hdr.subtype.t & 0xFF) {
        case 0x65:
            break;
        case 0x64:
        case 0x66:
        case 0x67:
        case 0x6A:
            f.read((char*)&x03_inst->ptr, 4);
            break;
            /*
            std::getline(f, x03_inst.s, (char)0);
            skip_and_pad(&f, 0);
            break;
            */
        case 0x6D:
        case 0x6E:
        case 0x6F:
        case 0x68:
        case 0x6B:
        case 0x71:
        case 0x73:
        case 0x78:
            // log(&f, "- Expecting %d characters\n",
            // x03_inst->hdr.subtype.size);
            f.read(buf, round_to_word(x03_inst->hdr.subtype.size));
            x03_inst->s = std::string(buf);
            // log(&f, "- Read \"%s\"\n", buf);
            x03_inst->has_str = true;
            // log(&f, "- Read \"%s\"\n", x03_inst->s.c_str());
            break;
        case 0x69:
            skip(&f, 8);
            break;
        case 0x6C: {
            uint32_t size;
            f.read((char*)&size, 4);
            skip(&f, 4 * size);
        } break;
        case 0x70:
            uint16_t a, b;
            f.read((char*)&a, 2);
            skip(&f, 6);
            skip(&f, 4 * a);
            break;
        case 0x74:
            skip(&f, 16);
            break;
        case 0xF6:
            // log(&f, "- Doing f6\n");
            skip(&f, 80);
            break;
        default:
            log(&f, "- Unexpected value subtype=0x%02X\n",
                x03_inst->hdr.subtype.t);
            exit(1);
    };

    (fs.x03_map)[x03_inst->hdr.k] = *x03_inst;
    return 0;
}

template <AllegroVersion version>
uint32_t parse_x12(File<A_174>& fs, std::ifstream& f) {
    uint8_t s;
    if (version <= A_164) {
        s = 24;
    } else {
        s = 28;
    }
    x12* x12_inst = new x12;
    f.read((char*)x12_inst, s);
    if (version >= A_174) {
        skip(&f, 4);
    }
    if (fs.x12_map.count(x12_inst->k) > 0) {
        log(&f, "- Already seen this key!\n");
        exit(1);
    }
    (fs.x12_map)[x12_inst->k] = *x12_inst;
    return 0;
}

template <AllegroVersion version>
uint32_t parse_x1C(File<A_174>& fs, std::ifstream& f) {
    uint16_t size;
    x1C<version>* x1C_inst = new x1C<version>;
    f.read((char*)&x1C_inst->hdr, sizeof_until_tail<x1C_header<version>>());
    if (version < A_172) {
        size = 10 + x1C_inst->hdr.size_hint * 3;
    } else {
        size = 21 + x1C_inst->hdr.un2_0 * 4;
        skip(&f, 104);
    }

    for (int i = 0; i < size; i++) {
        // log(&f, "- - Next words:");
        // log_n_words(&f, 8);
        // skip(&f, -4 * 8);

        t13<version> t13_inst;
        f.read((char*)&t13_inst, sizeof(t13<version>));
        x1C_inst->parts.push_back(t13_inst);
    }

    if (version >= A_172) {
        skip(&f, 4);
    }

    skip(&f, x1C_inst->hdr.n * 40);

    (fs.x1C_map)[x1C_inst->hdr.k] = upgrade<version, A_174>(*x1C_inst);
    return 0;
}

template <AllegroVersion version>
uint32_t parse_x1D(File<A_174>& fs, std::ifstream& f) {
    uint32_t k = default_parser<x1D, version>(fs, f);
    auto& inst = fs.x1D_map[k];

    // log(&f, "size_a = %d, size_b = %d\n", size_a, size_b);
    skip(&f, inst.size_b * (version >= A_162 ? 56 : 48));
    skip(&f, inst.size_a * 256);
    if (version >= A_172) {
        skip(&f, 4);
    }
    return 0;
}

template <AllegroVersion version>
uint32_t parse_x1E(File<A_174>& fs, std::ifstream& f) {
    x1E* x1E_inst = new x1E;
    f.read((char*)x1E_inst, sizeof(x1E_hdr));
    x1E_inst->s = new char[x1E_inst->hdr.size];
    f.read(x1E_inst->s, round_to_word(x1E_inst->hdr.size));
    (fs.x1E_map)[x1E_inst->hdr.k] = *x1E_inst;
    if (version >= A_172) {
        skip(&f, 4);
    }
    return 0;
}

template <AllegroVersion version>
uint32_t parse_x1F(File<A_174>& fs, std::ifstream& f) {
    uint32_t k = default_parser<x1F, version>(fs, f);
    auto& inst = fs.x1F_map[k];

    if constexpr (version >= A_172) {
        skip(&f, inst.size * 280 + 8);
    } else if constexpr (version >= A_162) {
        skip(&f, inst.size * 280 + 4);
    } else {
        skip(&f, inst.size * 240 + 4);
    }

    return k;
}

template <AllegroVersion version>
uint32_t parse_x21(File<A_174>& fs, std::ifstream& f) {
    x21_header i;
    f.read((char*)&i, sizeof(x21_header));
    skip(&f, -sizeof(x21_header));
    // log(&f, "- Next words:");
    // log_n_words(&f, 8);
    // skip(&f, -4 * 8);
    if (i.r == 1304) {
        stackup_material ps;
        f.read((char*)&ps, sizeof(ps));
        fs.stackup_materials[ps.hdr.k] = ps;
        // log(&f, "- - Stackup material... %02d->%s\n", ps.layer_id,
        // ps.material);
    } else if (i.r == 14093) {
        // log(&f, "- - i.r=\x1b[31m14093\x1b[0m...?\n");
        skip(&f, i.size);
        // } else if (i.r == 2050) {
    } else if (i.r == 0x0407) {
        meta_netlist_path r;
        char s[1028];
        f.read((char*)&r, sizeof_until_tail<meta_netlist_path>());
        f.read(s, sizeof(s));
        r.path = std::string(s);
        fs.netlist_path = r;
    } else {
        skip(&f, i.size);
    }
    return 0;
}

template <AllegroVersion version>
uint32_t parse_x27(File<A_174>& fs, std::ifstream& f) {
    if (PRINT_ALL_ITEMS) {
        log(&f, "- Expecting to read until 0x%08X\n", fs.x27_end_pos - 1);
    }
    f.read((char*)&fs.x27_db.t, 4);
    uint32_t k;
    uint32_t pos;
    pos = f.tellg();
    while (pos < fs.x27_end_pos - 1) {
        f.read((char*)&k, 4);
        fs.x27_db.keys.insert(k);
        pos += 4;
    };
    return 0;
}

template <AllegroVersion version>
uint32_t parse_x2A(File<A_174>& fs, std::ifstream& f) {
    x2A* x2A_inst = new x2A;
    f.read((char*)&x2A_inst->hdr, sizeof(x2A_hdr));
    if (version >= A_174) {
        skip(&f, 4);
    }

    if (version <= A_164) {
        x2A_inst->references = false;
        for (uint16_t i = 0; i < x2A_inst->hdr.size; i++) {
            char buf[36] = {0};
            f.read((char*)buf, 36);
            uint32_t suffix = *((uint32_t*)&buf[32]);
            x2A_local_entry entry = x2A_local_entry{std::string(buf), suffix};
            x2A_inst->local_entries.push_back(entry);
        }
    } else {
        x2A_inst->references = true;
        for (uint16_t i = 0; i < x2A_inst->hdr.size; i++) {
            x2A_reference_entry entry;
            f.read((char*)&entry, sizeof(x2A_reference_entry));
            x2A_inst->reference_entries.push_back(entry);
            if (PRINT_ALL_ITEMS) {
                log(&f, "- - Entry: %08X %08X %08X\n", ntohl(entry.ptr),
                    ntohl(entry.suffix), ntohl(entry.un1));
            }
        }
    }
    f.read((char*)&x2A_inst->k, 4);
    (fs.x2A_map)[x2A_inst->k] = *x2A_inst;
    if (PRINT_ALL_ITEMS) {
        log(&f, "- Inserting x2A with key %08X\n", ntohl(x2A_inst->k));
    }
    return 0;
}

template <AllegroVersion version>
uint32_t parse_x31(File<A_174>& fs, std::ifstream& f) {
    x31* x31_inst = new x31;
    f.read((char*)x31_inst, sizeof(x31_hdr));
    if (version >= A_174) {
        skip(&f, 4);
    }
    if (x31_inst->hdr.len > 0) {
        std::getline(f, x31_inst->s, (char)0);
    }
    (fs.x31_map)[x31_inst->hdr.k] = *x31_inst;
    skip_and_pad(&f, 0);
    return 0;
}

template <AllegroVersion version>
uint32_t parse_x35(File<A_174>& fs, std::ifstream& f) {
    skip(&f, 124);
    return 0;
}

template <AllegroVersion version>
uint32_t parse_x36(File<A_174>& fs, std::ifstream& f) {
    x36<version>* inst = new x36<version>;
    f.read((char*)inst, sizeof_until_tail<x36<version>>());

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

    switch (inst->c) {
        case 0x02:
            for (uint32_t i = 0; i < inst->size; i++) {
                // Discard
                x36_x02<version> inst_x02;
                f.read((char*)&inst_x02, sizeof_until_tail<x36_x02<version>>());
                // log(&f, "- x02.str = %s\n", inst_x02.str);
            }
            break;
        case 0x06:
            for (uint32_t i = 0; i < inst->size; i++) {
                // Currently just read the object and immediately throw it
                // away, because we don't know of any use for it.
                x36_x06<version> inst_x06;
                f.read((char*)&inst_x06, sizeof_until_tail<x36_x06<version>>());
                // log(&f, "- x06.n = %d\n", inst_x06.n);
                // log(&f, "- x06.r = %d\n", inst_x06.r);
                // log(&f, "- x06.s = %d\n", inst_x06.s);
                // log(&f, "- x06.un1 = %d\n\n", inst_x06.un1);
            }
            break;
        case 0x03:
            for (uint32_t i = 0; i < inst->size; i++) {
                // Just throw this away after reading it.
                x36_x03<version> inst_x03;
                f.read((char*)&inst_x03, sizeof_until_tail<x36_x03<version>>());
                // if (version >= A_172) {
                //     log(&f, "- - s = \"%s\"\n", inst_x03.str);
                // } else {
                //     log(&f, "- - s = \"%s\"\n", inst_x03.str_16x);
                // }
            }
            break;
        case 0x05:
            skip(&f, inst->size * 28);
            break;
        case 0x08:
            for (uint32_t i = 0; i < inst->size; i++) {
                x36_x08<version> x;
                f.read((char*)&x, sizeof_until_tail<x36_x08<version>>());
                inst->x08s.push_back(x);
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
            skip(&f, inst->size * 1016);
            // for (uint32_t i = 0; i < inst->size; i++) {
            //     log(&f, "- - Next words:");
            //     log_n_words(&f, 6);
            //     skip(&f, 1016 - 4 * 6);
            // }
            break;
        case 0x0C:
            skip(&f, inst->size * 232);
            break;
        case 0x0D:
            skip(&f, inst->size * 200);
            break;
        case 0x0F:
            for (uint32_t i = 0; i < inst->size; i++) {
                x36_x0F<version> x;
                f.read((char*)&x, sizeof(x36_x0F<version>));
                inst->x0Fs.push_back(x);
            }
            break;
        case 0x10:
            skip(&f, inst->size * 108);
            // for (uint32_t i = 0; i < inst->size; i++) {
            //     log(&f, "- - Next words:");
            //     log_n_words(&f, 6);
            //     skip(&f, 108 - 4 * 6);
            // }
            break;
        default:
            log(&f, "- Don\'t know how to handle c=%X\n", inst->c);
            exit(1);
    }

    fs.x36_map[inst->k] = upgrade<version, A_174>(*inst);
    return 0;
}

template <AllegroVersion version>
uint32_t parse_x3B(File<A_174>& fs, std::ifstream& f) {
    x3B<version>* x3B_inst = new x3B<version>;
    f.read((char*)x3B_inst, sizeof_until_tail<x3B<version>>());

    char* buf = (char*)malloc(x3B_inst->len);
    f.read(buf, x3B_inst->len);
    skip_and_pad(&f, 0);
    x3B_inst->model_str = std::string(buf);
    return 0;
}

template <AllegroVersion version>
uint32_t parse_x3C(File<A_174>& fs, std::ifstream& f) {
    uint32_t k = default_parser<x3C, version>(fs, f);
    auto& inst = fs.x3C_map[k];

    for (uint32_t i = 0; i < inst.size; i++) {
        uint32_t ptr;
        f.read((char*)&ptr, sizeof(ptr));
        inst.ptrs.push_back(ptr);
    }

    return 0;
}

template <AllegroVersion version>
File<A_174> parse_file_raw(const std::string& filepath) {
    File<A_174> fs;

    std::ifstream f(filepath, std::ios::binary);

    // Read header
    header* hdr = new header;
    fs.hdr = hdr;
    f.read((char*)hdr, sizeof(header));
    if (PRINT_ALL_ITEMS) {
        printf("Magic: 0x%08X\n", hdr->magic);
        std::cout << hdr->allegro_version << std::endl;
    }

    // Skip until `unknown19`
    f.seekg(60, std::ios_base::cur);

    if (PRINT_ALL_ITEMS) {
        log(&f, "Looking at un19\n");
    }
    uint32_t un19[11];
    f.read((char*)un19, sizeof(un19));
    fs.x27_end_pos = un19[7];

    // Skip to layers
    f.seekg(0x430);
    if (PRINT_ALL_ITEMS) {
        log(&f, "Looking at layer map\n");
    }
    for (int i = 0; i < 25; i++) {
        uint32_t xs[2];
        f.read((char*)xs, sizeof(xs));
        fs.layers.push_back(std::make_tuple(xs[0], xs[1]));
    }

    // Skip `unknown20`
    f.seekg(0x1200);

    if (PRINT_ALL_ITEMS) {
        log(&f, "Looking at strings\n");
    }
    for (uint32_t i = 0; i < un19[9]; i++) {
        uint32_t id;
        f.read((char*)&id, 4);

        std::string* s = new std::string;
        std::getline(f, *s, (char)0);

        (fs.strings)[id] = *s;

        skip_and_pad(&f, 0);
    }

    // If we peek a `0x00`, we may be at the end of the file, but have extra
    // empty bytes appended.
    while (f.peek() != EOF && f.peek() != 0x00) {
        uint8_t t = f.peek() & 0xFF;
        if (PRINT_ALL_ITEMS) {
            log(&f, "Handling t=0x%02X: ", t);
            log_n_words(&f, 4);
            skip(&f, -4 * 4);
        }

        const parser_t parser = PARSER_TABLE<version>[t];
        if (t < 0x3E && parser.parse != 0) {
            parser.parse(fs, f);
        } else {
            log(&f, "\x1b[31m- Unrecognized type\x1b[0m\n");
            log(&f, "- Next words:");
            log_n_words(&f, 6);
            exit(1);
        }
    }

    fs.layer_count = layer_count(&fs);

    return fs;
}

std::optional<File<A_174>> parse_file(const std::string& filepath) {
    std::ifstream f(filepath, std::ios::binary);

    if (!f.is_open()) {
        printf("Failed to open file \"%s\"\n", filepath.c_str());
        return {};
    }

    uint32_t magic;
    f.read((char*)&magic, sizeof(magic));
    f.close();

    // printf("Comparing magic 0x%08X\n", magic);
    switch (magic) {
        case 0x00130000:
        case 0x00130200:
            return parse_file_raw<A_160>(filepath);
        case 0x00130402:
            return parse_file_raw<A_162>(filepath);
        case 0x00130C03:
            return parse_file_raw<A_164>(filepath);
        case 0x00131003:
            return parse_file_raw<A_165>(filepath);
        case 0x00131503:
        case 0x00131504:
            return parse_file_raw<A_166>(filepath);
        case 0x00140400:
        case 0x00140500:
        case 0x00140600:
        case 0x00140700:
            return parse_file_raw<A_172>(filepath);
        case 0x00140900:
        case 0x00140901:
        case 0x00140902:
        case 0x00140E00:
            return parse_file_raw<A_174>(filepath);
    }

    printf("Magic unrecognized! Magic = 0x%08X\n", magic);
    return {};
}
