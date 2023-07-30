#include "printers.h"

#include "lib/printing/utils.h"
#include "lib/structure/utils.h"

/***********
 * Helpers *
 ***********/

template <AllegroVersion version>
void print_struct(const void *untyped_inst, File<version> *fs, const int d) {
    uint8_t t = *(uint8_t *)untyped_inst;
    if (t >= sizeof(SFS_TABLE<version>) / sizeof(sfs<version>)) {
        std::printf("Do not know how to print item of type t=0x%02X\n", t);
    } else if (SFS_TABLE<version>[t].print == NULL) {
        printf_d(d, "Null printer for type t=0x%02X\n", t);
    } else if (d > MAX_DEPTH) {
        printf_d(d, "Max depth\n");
    } else {
        // std::printf("Printing item of type t=0x%02X\n", t);
        SFS_TABLE<version>[t].print(untyped_inst, fs, d);
    }
};

template <AllegroVersion version>
void print_struct(const uint32_t k, File<version> &fs, const int d) {
    if (fs.x01_map.contains(k)) {
        print_struct((const void *)&fs.x01_map.at(k), &fs, d);
    } else if (fs.x04_map.contains(k)) {
        print_struct((const void *)&fs.x04_map.at(k), &fs, d);
    } else if (fs.x05_map.contains(k)) {
        print_struct((const void *)&fs.x05_map.at(k), &fs, d);
    } else if (fs.x0A_map.contains(k)) {
        print_struct((const void *)&fs.x0A_map.at(k), &fs, d);
    } else if (fs.x0C_map.contains(k)) {
        print_struct((const void *)&fs.x0C_map.at(k), &fs, d);
    } else if (fs.x0D_map.contains(k)) {
        print_struct((const void *)&fs.x0D_map.at(k), &fs, d);
    } else if (fs.x14_map.contains(k)) {
        print_struct((const void *)&fs.x14_map.at(k), &fs, d);
    } else if (fs.x15_map.contains(k)) {
        print_struct((const void *)&fs.x15_map.at(k), &fs, d);
    } else if (fs.x16_map.contains(k)) {
        print_struct((const void *)&fs.x16_map.at(k), &fs, d);
    } else if (fs.x17_map.contains(k)) {
        print_struct((const void *)&fs.x17_map.at(k), &fs, d);
    } else if (fs.x1B_map.contains(k)) {
        print_struct((const void *)&fs.x1B_map.at(k), &fs, d);
    } else if (fs.x23_map.contains(k)) {
        print_struct((const void *)&fs.x23_map.at(k), &fs, d);
    } else if (fs.x28_map.contains(k)) {
        print_struct((const void *)&fs.x28_map.at(k), &fs, d);
    } else if (fs.x2C_map.contains(k)) {
        print_struct((const void *)&fs.x2C_map.at(k), &fs, d);
    } else if (fs.x2D_map.contains(k)) {
        print_struct((const void *)&fs.x2D_map.at(k), &fs, d);
    } else if (fs.x30_map.contains(k)) {
        print_struct((const void *)&fs.x30_map.at(k), &fs, d);
    } else if (fs.x31_map.contains(k)) {
        print_struct((const void *)&fs.x31_map.at(k), &fs, d);
    } else if (fs.x32_map.contains(k)) {
        print_struct((const void *)&fs.x32_map.at(k), &fs, d);
    } else if (fs.x33_map.contains(k)) {
        print_struct((const void *)&fs.x33_map.at(k), &fs, d);
    } else if (fs.x37_map.contains(k)) {
        print_struct((const void *)&fs.x37_map.at(k), &fs, d);
    } else if (fs.x38_map.contains(k)) {
        print_struct((const void *)&fs.x38_map.at(k), &fs, d);
    } else if (fs.x39_map.contains(k)) {
        print_struct((const void *)&fs.x39_map.at(k), &fs, d);
    } else if (fs.x3C_map.contains(k)) {
        print_struct((const void *)&fs.x3A_map.at(k), &fs, d);
    } else if (fs.x3A_map.contains(k)) {
        print_struct((const void *)&fs.x3C_map.at(k), &fs, d);
    } else {
        printf_d(d, "Did not find key 0x%08X\n", ntohl(k));
    }
}

template void print_struct<A_174>(const uint32_t k, File<A_174> &fs,
                                  const int d);

/*******************
 * Object printers *
 *******************/

template <AllegroVersion version>
void default_printer(const void *untyped_inst, File<version> *fs, const int d) {
    const x05<A_174> *inst = (const x05<A_174> *)untyped_inst;
    printf_d(d, "\x1b[33mDefault printer:\x1b[0m t=0x%08X k=0x%08X\n",
             ntohl(inst->t), ntohl(inst->k));
}

template <AllegroVersion version>
void print_x01(const void *untyped_inst, File<version> *fs, const int d) {
    const x01<A_174> *inst = (const x01<A_174> *)untyped_inst;
    std::pair<double, double> center = x01_center(inst);
    printf_d(d,
             "x01: t=0x%04X un0=%02X subtype=%02X k=0x%08X un1=%08X"
             " start=\x1b[2m(%d, %d)\x1b[0m"
             " end=\x1b[2m(%d, %d)\x1b[0m\n",
             ntohl(inst->t), inst->un0, inst->subtype, ntohl(inst->k),
             ntohl(inst->un1), inst->coords[0], inst->coords[1],
             inst->coords[2], inst->coords[3]);
    printf_d(d + 1,
             "bbox=\x1b[2m(%d, %d, %d, %d)\x1b[0m"
             " center=\x1b[2m(%.0f, %.0f)\x1b[0m"
             " r=%.0f width=%d\n",
             inst->bbox[0], inst->bbox[1], inst->bbox[2], inst->bbox[3],
             center.first, center.second, cfp_to_double(inst->r), inst->width);

    printf_d(d + 1, "next:\n");
    if (fs->x34_map.contains(inst->next)) {
        print_struct((const void *)&fs->x34_map.at(inst->next), fs, d + 2);
    } else if (fs->x28_map.contains(inst->next)) {
        print_struct((const void *)&fs->x28_map.at(inst->next), fs, d + 2);
    } else if (fs->x17_map.contains(inst->next)) {
        print_struct((const void *)&fs->x17_map.at(inst->next), fs, d + 2);
    } else if (fs->x16_map.contains(inst->next)) {
        print_struct((const void *)&fs->x16_map.at(inst->next), fs, d + 2);
    } else if (fs->x15_map.contains(inst->next)) {
        print_struct((const void *)&fs->x15_map.at(inst->next), fs, d + 2);
    } else if (fs->x14_map.contains(inst->next)) {
        print_struct((const void *)&fs->x14_map.at(inst->next), fs, d + 2);
    } else if (fs->x05_map.contains(inst->next)) {
        print_struct((const void *)&fs->x05_map.at(inst->next), fs, d + 2);
    } else if (fs->x01_map.contains(inst->next)) {
        print_struct((const void *)&fs->x01_map.at(inst->next), fs, d + 2);
    } else {
        printf_d(d + 2, "next unrecognized: 0x%08X\n", ntohl(inst->next));
        exit(0);
    }

    printf_d(d + 1, "parent:\n");
    if (fs->x34_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x34_map.at(inst->parent), fs, d + 2);
    } else if (fs->x28_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x28_map.at(inst->parent), fs, d + 2);
    } else if (fs->x17_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x17_map.at(inst->parent), fs, d + 2);
    } else if (fs->x16_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x16_map.at(inst->parent), fs, d + 2);
    } else if (fs->x15_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x15_map.at(inst->parent), fs, d + 2);
    } else if (fs->x14_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x14_map.at(inst->parent), fs, d + 2);
    } else if (fs->x05_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x05_map.at(inst->parent), fs, d + 2);
    } else if (fs->x01_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x01_map.at(inst->parent), fs, d + 2);
    } else {
        printf_d(d + 2, "parent unrecognized: 0x%08X\n", ntohl(inst->parent));
        exit(0);
    }

    /*
    printf_d(d+1, "ptr3:");
    if (inst->ptr3 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x10_map->contains(inst->ptr3)) {
            print_struct((const void*)&fs->x10_map->at(inst->ptr3), fs, d+2);
        } else {
            printf_d(d+2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
            exit(0);
        }
    }
    */
}

template <AllegroVersion version>
void print_x03(const void *untyped_inst, File<version> *fs, const int d) {
    const x03 *inst = (const x03 *)untyped_inst;
    if (inst->hdr.subtype.t == 0x6A) {
        printf_d(d,
                 "x03: \x1b[36;3mString?\x1b[0m type=0x%08X k=0x%08X %02X:"
                 " ptr=0x%08X \x1b[34m\"%.70s\"\x1b[0m\n",
                 ntohl(inst->hdr.type), ntohl(inst->hdr.k), inst->hdr.subtype.t,
                 ntohl(inst->hdr.ptr), fs->strings.at(inst->ptr).c_str());
    } else {
        printf_d(d,
                 "x03: \x1b[36;3mString?\x1b[0m type=0x%08X k=0x%08X %02X:"
                 " ptr=0x%08X \x1b[34m\"%.70s\"\x1b[0m\n",
                 ntohl(inst->hdr.type), ntohl(inst->hdr.k), inst->hdr.subtype.t,
                 ntohl(inst->hdr.ptr), inst->s.c_str());
    }

    if (!inst->has_str && inst->hdr.subtype.t != 0x6A) {
        printf_d(d + 1, "subtype = %02X, un3 = %02X\n", inst->hdr.subtype.t,
                 inst->hdr.subtype.un3);
        printf_d(d + 1, "value: %d = 0x %08X\n", inst->ptr, ntohl(inst->ptr));
    }
}

template <AllegroVersion version>
void print_x04(const void *untyped_inst, File<version> *fs, const int d) {
    const x04<version> *inst = (const x04<version> *)untyped_inst;
    printf_d(d,
             "x04: \x1b[36;3mNet/Shape Pair\x1b[0m t=0x%08X k=0x%08X"
             " \x1b[34m\"%s\"\x1b[0m ↔ \x1b[34m\"%s\"\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k),
             x1B_net_name(inst->ptr1, fs)->c_str(),
             x32_pin_name(inst->ptr2, fs)->c_str());

    printf_d(d + 1, "ptr1:\n");
    if (fs->x1B_map.contains(inst->ptr1)) {
        print_struct((const void *)&fs->x1B_map.at(inst->ptr1), fs, d + 2);
    } else if (inst->ptr1 == fs->hdr->ll_x04.tail) {
        printf_d(d + 2, "\x1b[33mptr1 is in header, 0x%08x?\x1b[0m\n",
                 ntohl(inst->ptr1));
    } else {
        printf_d(d + 2, "ptr1 unrecognized: 0x%08x\n", ntohl(inst->ptr1));
        exit(0);
    }

    printf_d(d + 1, "ptr2:\n");
    if (fs->x33_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x33_map.at(inst->ptr2), fs, d + 2);
    } else if (fs->x32_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x32_map.at(inst->ptr2), fs, d + 2);
    } else if (fs->x2E_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x2E_map.at(inst->ptr2), fs, d + 2);
    } else if (fs->x28_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x28_map.at(inst->ptr2), fs, d + 2);
    } else if (fs->x05_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x05_map.at(inst->ptr2), fs, d + 2);
    } else if (fs->x04_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x04_map.at(inst->ptr2), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
        exit(0);
    }
}

template <AllegroVersion version>
void print_x05(const void *untyped_inst, File<version> *fs, const int d) {
    const x05<A_174> *inst = (const x05<A_174> *)untyped_inst;
    printf_d(d, "x05: t=0x%04X subtype=%02X layer=%d k=0x%08X\n",
             ntohl(inst->t), inst->subtype, inst->layer, ntohl(inst->k));

    printf_d(d + 1, "ptr0:\n");
    if (fs->x34_map.contains(inst->ptr0)) {
        print_struct((const void *)&fs->x34_map.at(inst->ptr0), fs, d + 2);
    } else if (fs->x33_map.contains(inst->ptr0)) {
        print_struct((const void *)&fs->x33_map.at(inst->ptr0), fs, d + 2);
    } else if (fs->x32_map.contains(inst->ptr0)) {
        print_struct((const void *)&fs->x32_map.at(inst->ptr0), fs, d + 2);
    } else if (fs->x2E_map.contains(inst->ptr0)) {
        print_struct((const void *)&fs->x2E_map.at(inst->ptr0), fs, d + 2);
    } else if (fs->x28_map.contains(inst->ptr0)) {
        print_struct((const void *)&fs->x28_map.at(inst->ptr0), fs, d + 2);
    } else if (fs->x17_map.contains(inst->ptr0)) {
        print_struct((const void *)&fs->x17_map.at(inst->ptr0), fs, d + 2);
    } else if (fs->x16_map.contains(inst->ptr0)) {
        print_struct((const void *)&fs->x16_map.at(inst->ptr0), fs, d + 2);
    } else if (fs->x15_map.contains(inst->ptr0)) {
        print_struct((const void *)&fs->x15_map.at(inst->ptr0), fs, d + 2);
    } else if (fs->x14_map.contains(inst->ptr0)) {
        print_struct((const void *)&fs->x14_map.at(inst->ptr0), fs, d + 2);
    } else if (fs->x05_map.contains(inst->ptr0)) {
        print_struct((const void *)&fs->x05_map.at(inst->ptr0), fs, d + 2);
    } else if (fs->x04_map.contains(inst->ptr0)) {
        print_struct((const void *)&fs->x04_map.at(inst->ptr0), fs, d + 2);
    } else if (fs->x01_map.contains(inst->ptr0)) {
        print_struct((const void *)&fs->x01_map.at(inst->ptr0), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr0 unrecognized: 0x%08X\n", ntohl(inst->ptr0));
        exit(0);
    }

    printf_d(d + 1, "ptr1 \x1b[3m(net/shape pair)\x1b[0m:\n");
    if (fs->x04_map.contains(inst->ptr1)) {
        print_struct((const void *)&fs->x04_map.at(inst->ptr1), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
        exit(0);
    }

    if (inst->un1[0] != 0 || inst->un1[1] != 0) {
        printf_d(d + 1, "un1: %08X %08X\n", ntohl(inst->un1[0]),
                 ntohl(inst->un1[1]));
    }

    for (int i = 0; i < 2; i++) {
        printf_d(d + 1, "ptr2[%d]:", i);
        if (inst->ptr2[i] == 0) {
            printf(" \x1b[2mnull\x1b[0m\n");
        } else {
            std::printf("\n");
            if (fs->x33_map.contains(inst->ptr2[i])) {
                print_struct((const void *)&fs->x33_map.at(inst->ptr2[i]), fs,
                             d + 2);
            } else if (fs->x32_map.contains(inst->ptr2[i])) {
                print_struct((const void *)&fs->x32_map.at(inst->ptr2[i]), fs,
                             d + 2);
            } else if (fs->x2E_map.contains(inst->ptr2[i])) {
                print_struct((const void *)&fs->x2E_map.at(inst->ptr2[i]), fs,
                             d + 2);
            } else if (fs->x2D_map.contains(inst->ptr2[i])) {
                print_struct((const void *)&fs->x2D_map.at(inst->ptr2[i]), fs,
                             d + 2);
            } else if (fs->x2D_map.contains(inst->ptr2[i])) {
                print_struct((const void *)&fs->x2D_map.at(inst->ptr2[i]), fs,
                             d + 2);
            } else if (fs->x28_map.contains(inst->ptr2[i])) {
                print_struct((const void *)&fs->x28_map.at(inst->ptr2[i]), fs,
                             d + 2);
            } else if (fs->x09_map.contains(inst->ptr2[i])) {
                print_struct((const void *)&fs->x09_map.at(inst->ptr2[i]), fs,
                             d + 2);
            } else if (fs->x05_map.contains(inst->ptr2[i])) {
                print_struct((const void *)&fs->x05_map.at(inst->ptr2[i]), fs,
                             d + 2);
            } else if (fs->x04_map.contains(inst->ptr2[i])) {
                print_struct((const void *)&fs->x04_map.at(inst->ptr2[i]), fs,
                             d + 2);
            } else {
                printf_d(d + 2, "ptr2[%d] unrecognized: 0x%08X\n", i,
                         ntohl(inst->ptr2[i]));
                exit(0);
            }
        }
    }

    if (inst->un2 != 0) {
        printf_d(d + 1, "un2: %08X\n", ntohl(inst->un2));
    }

    for (int i = 0; i < 2; i++) {
        printf_d(d + 1, "ptr3[%d]:", i);
        if (inst->ptr3[i] == 0) {
            printf(" \x1b[2mnull\x1b[0m\n");
        } else {
            std::printf("\n");
            if (fs->x33_map.contains(inst->ptr3[i])) {
                print_struct((const void *)&fs->x33_map.at(inst->ptr3[i]), fs,
                             d + 2);
            } else if (fs->x32_map.contains(inst->ptr3[i])) {
                print_struct((const void *)&fs->x32_map.at(inst->ptr3[i]), fs,
                             d + 2);
            } else if (fs->x2E_map.contains(inst->ptr3[i])) {
                print_struct((const void *)&fs->x2E_map.at(inst->ptr3[i]), fs,
                             d + 2);
            } else if (fs->x28_map.contains(inst->ptr3[i])) {
                print_struct((const void *)&fs->x28_map.at(inst->ptr3[i]), fs,
                             d + 2);
            } else if (fs->x09_map.contains(inst->ptr3[i])) {
                print_struct((const void *)&fs->x09_map.at(inst->ptr3[i]), fs,
                             d + 2);
            } else if (fs->x05_map.contains(inst->ptr3[i])) {
                print_struct((const void *)&fs->x05_map.at(inst->ptr3[i]), fs,
                             d + 2);
            } else if (fs->x04_map.contains(inst->ptr3[i])) {
                print_struct((const void *)&fs->x04_map.at(inst->ptr3[i]), fs,
                             d + 2);
            } else {
                printf_d(d + 2, "ptr3[%d] unrecognized: 0x%08X\n", i,
                         ntohl(inst->ptr3[i]));
                exit(0);
            }
        }
    }

    printf_d(d + 1, "first_segment_ptr:\n");
    if (fs->x17_map.contains(inst->first_segment_ptr)) {
        print_struct((const void *)&fs->x17_map.at(inst->first_segment_ptr), fs,
                     d + 2);
    } else if (fs->x16_map.contains(inst->first_segment_ptr)) {
        print_struct((const void *)&fs->x16_map.at(inst->first_segment_ptr), fs,
                     d + 2);
    } else if (fs->x15_map.contains(inst->first_segment_ptr)) {
        print_struct((const void *)&fs->x15_map.at(inst->first_segment_ptr), fs,
                     d + 2);
    } else if (fs->x01_map.contains(inst->first_segment_ptr)) {
        print_struct((const void *)&fs->x01_map.at(inst->first_segment_ptr), fs,
                     d + 2);
    } else {
        printf_d(d + 2, "first_segment_ptr unrecognized: 0x%08X\n",
                 ntohl(inst->first_segment_ptr));
        exit(0);
    }

    printf_d(d + 1, "ptr5:");
    if (inst->ptr5 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x03_map.contains(inst->ptr5)) {
            print_struct((const void *)&fs->x03_map.at(inst->ptr5), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr5 unrecognized: 0x%08X\n", ntohl(inst->ptr5));
            exit(0);
        }
    }

    if (inst->un3 != 0) {
        printf_d(d + 1, "un3: %08X\n", ntohl(inst->un3));
    }
}

template <AllegroVersion version>
void print_x06(const void *untyped_inst, File<version> *fs, const int d) {
    const x06<A_174> *inst = (const x06<A_174> *)untyped_inst;
    printf_d(d, "x06: t=0x%08X k=0x%08X \x1b[34m\"%s\" \"%s\"\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k), fs->strings.at(inst->ptr1).c_str(),
             fs->strings.at(inst->ptr2).c_str());

    printf_d(d + 1, "ptr3:\n");
    if (fs->x07_map.contains(inst->ptr3)) {
        print_struct((const void *)&fs->x07_map.at(inst->ptr3), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
        exit(0);
    }

    printf_d(d + 1, "ptr4:\n");
    if (fs->x0F_map.contains(inst->ptr4)) {
        print_struct((const void *)&fs->x0F_map.at(inst->ptr4), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr4 unrecognized: 0x%08X\n", ntohl(inst->ptr4));
        exit(0);
    }

    printf_d(d + 1, "ptr5 \x1b[39;3m(footprint pin list)\x1b[0m:");
    if (inst->ptr5 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x08_map.contains(inst->ptr5)) {
            print_struct((const void *)&fs->x08_map.at(inst->ptr5), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr5 unrecognized: 0x%08X\n", ntohl(inst->ptr5));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr6 \x1b[39;3m(schematic symbol name)\x1b[0m:");
    if (inst->ptr6 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x03_map.contains(inst->ptr6)) {
            print_struct((const void *)&fs->x03_map.at(inst->ptr6), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr6 unrecognized: 0x%08X\n", ntohl(inst->ptr6));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x07(const void *untyped_inst, File<version> *fs, const int d) {
    const x07<version> *inst = (const x07<version> *)untyped_inst;
    printf_d(d,
             "x07: \x1b[36;3mInstance\x1b[0m t=0x%08X k=0x%08X"
             " \x1b[34m\"%s\"\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k), inst_refdes(inst, fs).c_str());

    // printf_d(d + 1, "un2=%08X un4=%08X\n", ntohl(inst->un2),
    // ntohl(inst->un4));

    if constexpr (!std::is_same_v<decltype(inst->ptr0), std::monostate>) {
        printf_d(d + 1, "ptr0:\n");
        if (HAS_ENTRY2(x06_map, inst->ptr0)) {
            PRINT2(x06_map, inst->ptr0, d + 2);
        } else {
            printf_d(d + 2, "ptr0 unrecognized: 0x%08X\n", ntohl(inst->ptr0));
            // exit(0);
        }
    }

    printf_d(d + 1, "ptr1:\n");
    if (fs->x2D_map.contains(inst->ptr1)) {
        print_struct((const void *)&fs->x2D_map.at(inst->ptr1), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
        exit(0);
    }

    if constexpr (!std::is_same_v<decltype(inst->un2), std::monostate>) {
        if (inst->un2 != 0) {
            printf_d(d + 1, "un2: %08X\n", ntohl(inst->un2));
        }
    }

    printf_d(d + 1, "ptr2: 0x%08X\n", ntohl(inst->ptr2));
    if (HAS_ENTRY2(x10_map, inst->ptr2)) {
        PRINT2(x10_map, inst->ptr2, d + 2);
    } else {
        printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
        exit(0);
    }

    printf_d(d + 1, "ptr3:");
    if (inst->ptr3 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x03_map.contains(inst->ptr3)) {
            print_struct((const void *)&fs->x03_map.at(inst->ptr3), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
            exit(0);
        }
    }

    if (inst->un3 != 0) {
        printf_d(d + 1, "un3 expected to be 0, but is actually 0x%08X\n",
                 ntohl(inst->un3));
        exit(0);
    }

    printf_d(d + 1, "ptr4:");
    if (inst->ptr4 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x32_map.contains(inst->ptr4)) {
            print_struct((const void *)&fs->x32_map.at(inst->ptr4), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr4 unrecognized: 0x%08X\n", ntohl(inst->ptr4));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x08(const void *untyped_inst, File<version> *fs, const int d) {
    const x08<version> *inst = (const x08<version> *)untyped_inst;
    uint32_t str_ptr;
    if constexpr (std::is_same_v<decltype(inst->str_ptr), std::monostate>) {
        str_ptr = inst->str_ptr_16x;
    } else {
        str_ptr = inst->str_ptr;
    }
    printf_d(d,
             "x08: \x1b[36;3mFootprint Pin\x1b[0m t=0x%08X k=0x%08X"
             " un1=%08X"
             " \x1b[34m\"%s\"\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k), inst->un1,
             fs->strings.at(str_ptr).c_str());

    printf_d(
        d + 1,
        "ptr1 \x1b[3m(prev footprint pin?, or footprint if at end)\x1b[0m:");
    if constexpr (std::is_same_v<decltype(inst->ptr1), std::monostate>) {
        printf(" \x1b[2mstd::monostate\1b[0m\n");
    } else {
        if (inst->ptr1 == 0) {
            printf(" \x1b[2mnull\x1b[0m\n");
        } else {
            std::printf("\n");
            if (fs->x08_map.contains(inst->ptr1)) {
                print_struct((const void *)&fs->x08_map.at(inst->ptr1), fs,
                             d + 2);
            } else if (fs->x06_map.contains(inst->ptr1)) {
                print_struct((const void *)&fs->x06_map.at(inst->ptr1), fs,
                             d + 2);
            } else {
                printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n",
                         ntohl(inst->ptr1));
                exit(0);
            }
        }
    }

    printf_d(
        d + 1,
        "ptr2 \x1b[3m(next footprint pin, or footprint if at end)\x1b[0m:\n");
    if (fs->x08_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x08_map.at(inst->ptr2), fs, d + 2);
    } else if (fs->x06_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x06_map.at(inst->ptr2), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
        exit(0);
    }

    printf_d(d + 1, "ptr3 \x1b[3m(matching schematic pin)\x1b[0m:");
    if (inst->ptr3 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x11_map.contains(inst->ptr3)) {
            print_struct((const void *)&fs->x11_map.at(inst->ptr3), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr4:");
    if (inst->ptr4 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x03_map.contains(inst->ptr4)) {
            print_struct((const void *)&fs->x03_map.at(inst->ptr4), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr4 unrecognized: 0x%08X\n", ntohl(inst->ptr4));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x09(const void *untyped_inst, File<version> *fs, const int d) {
    const x09<version> *inst = (const x09<version> *)untyped_inst;
    printf_d(d, "x09: t=0x%08X k=0x%08X\n", ntohl(inst->t), ntohl(inst->k));

    printf_d(d + 1, "ptr1:\n");
    if (fs->x33_map.contains(inst->ptr1)) {
        print_struct((const void *)&fs->x33_map.at(inst->ptr1), fs, d + 2);
    } else if (fs->x32_map.contains(inst->ptr1)) {
        print_struct((const void *)&fs->x32_map.at(inst->ptr1), fs, d + 2);
    } else if (fs->x28_map.contains(inst->ptr1)) {
        print_struct((const void *)&fs->x28_map.at(inst->ptr1), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
        exit(0);
    }

    printf_d(d + 1, "ptr2:");
    if (inst->ptr2 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x05_map.contains(inst->ptr2)) {
            print_struct((const void *)&fs->x05_map.at(inst->ptr2), fs, d + 2);
        } else if (fs->x09_map.contains(inst->ptr2)) {
            print_struct((const void *)&fs->x09_map.at(inst->ptr2), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr3:\n");
    if (fs->x33_map.contains(inst->ptr3)) {
        print_struct((const void *)&fs->x33_map.at(inst->ptr3), fs, d + 2);
    } else if (fs->x32_map.contains(inst->ptr3)) {
        print_struct((const void *)&fs->x32_map.at(inst->ptr3), fs, d + 2);
    } else if (fs->x28_map.contains(inst->ptr3)) {
        print_struct((const void *)&fs->x28_map.at(inst->ptr3), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
        exit(0);
    }

    printf_d(d + 1, "ptr4:");
    if (inst->ptr4 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x05_map.contains(inst->ptr4)) {
            print_struct((const void *)&fs->x05_map.at(inst->ptr4), fs, d + 2);
        } else if (fs->x09_map.contains(inst->ptr4)) {
            print_struct((const void *)&fs->x09_map.at(inst->ptr4), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr4 unrecognized: 0x%08X\n", ntohl(inst->ptr4));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x0A(const void *untyped_inst, File<version> *fs, const int d) {
    const x0A<version> *inst = (const x0A<version> *)untyped_inst;
    printf_d(d,
             "x0A: t=0x%04X subtype=%02X layer=%d k=0x%08X un1={%08X}"
             " \x1b[2m(%d, %d, %d, %d)\x1b[0m\n",
             ntohl(inst->t), inst->subtype, inst->layer, ntohl(inst->k),
             ntohl(inst->un1), inst->coords[0], inst->coords[1],
             inst->coords[2], inst->coords[3]);
    printf_d(d + 1, "un4={%08X %08X %08X %08X}\n", ntohl(inst->un4[0]),
             ntohl(inst->un4[1]), ntohl(inst->un4[2]), ntohl(inst->un4[3]));
    printf_d(d + 1, "un5={%08X %08X %08X %08X}\n", ntohl(inst->un5[0]),
             ntohl(inst->un5[1]), ntohl(inst->un5[2]), ntohl(inst->un5[3]));

    if (HAS_ENTRY2(x01_map, inst->un4[3])) {
        PRINT2(x01_map, inst->un4[3], d + 1);
    }
}

template <AllegroVersion version>
void print_x0C(const void *untyped_inst, File<version> *fs, const int d) {
    const x0C<version> *inst = (const x0C<version> *)untyped_inst;
    printf_d(d, "x0C: t=0x%04X subtype=%02X layer=%d k=0x%08X kind=%d\n",
             ntohl(inst->t), inst->subtype, inst->layer, ntohl(inst->k),
             inst->kind);
    printf_d(d + 1,
             "un=[%08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X]\n",
             ntohl(inst->un[0]), ntohl(inst->un[1]), ntohl(inst->un[2]),
             ntohl(inst->un[3]), ntohl(inst->un[4]), ntohl(inst->un[5]),
             ntohl(inst->un[6]), ntohl(inst->un[7]), ntohl(inst->un[8]),
             ntohl(inst->un[9]), ntohl(inst->un[10]));
    printf_d(d + 1, "\x1b[2m(%d, %d, %d, %d)\x1b[0m\n", inst->coords[0],
             inst->coords[1], inst->coords[2], inst->coords[3]);
}

template <AllegroVersion version>
void print_x0D(const void *untyped_inst, File<version> *fs, const int d) {
    const x0D<A_174> *inst = (const x0D<A_174> *)untyped_inst;
    printf_d(d,
             "x0D: t=0x%08X k=0x%08X"
             " bm2=%08X"
             " \x1b[2m(%d, %d rel sym origin, %0.1f deg)\x1b[0m"
             " \x1b[34m\"%s\"\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k), ntohl(inst->bitmask),
             inst->coords[0], inst->coords[1], inst->rotation / 1000.,
             fs->strings.at(inst->str_ptr).c_str());

    /* Not interesting
    printf_d(d+1, "ptr2:");
    if (inst->ptr2 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x2B_map->contains(inst->ptr2)) {
            print_struct((const void*)&fs->x2B_map->at(inst->ptr2), fs, d+2);
        } else if (fs->x0D_map->contains(inst->ptr2)) {
            print_struct((const void*)&fs->x0D_map->at(inst->ptr2), fs, d+2);
        } else {
            printf_d(d+2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
            exit(0);
        }
    }
    */

    printf_d(d + 1, "pad_ptr:\n");
    if (fs->x1C_map.contains(inst->pad_ptr)) {
        print_struct((const void *)&fs->x1C_map.at(inst->pad_ptr), fs, d + 2);
    } else {
        printf_d(d + 2, "pad_ptr unrecognized: 0x%08X\n", ntohl(inst->pad_ptr));
        exit(0);
    }

    if (inst->un1 != 0) {
        printf_d(d + 1, "expected un1 to be 0, but = %08X\n", ntohl(inst->un1));
        exit(1);
    }
}

template <AllegroVersion version>
void print_x0F(const void *untyped_inst, File<version> *fs, const int d) {
    const x0F<version> *inst = (const x0F<version> *)untyped_inst;
    printf_d(d,
             "x0F: \x1b[36;3mFootprint Instance\x1b[0m t=0x%08X k=0x%08X"
             " \x1b[34m\"%s\" \"%s\"\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k), fs->strings.at(inst->ptr1).c_str(),
             inst->s);

    printf_d(d + 1, "ptr2:\n");
    if (fs->x0F_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x0F_map.at(inst->ptr2), fs, d + 2);
    } else if (fs->x06_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x06_map.at(inst->ptr2), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
        exit(0);
    }

    printf_d(d + 1, "ptr3 \x1b[3m(schematic pin list)\x1b[0m:");
    if (inst->ptr3 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x11_map.contains(inst->ptr3)) {
            print_struct((const void *)&fs->x11_map.at(inst->ptr3), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
            exit(0);
        }
    }

    if (inst->un != 0) {
        printf_d(d + 1, "expected un to be 0, but = %08X\n", ntohl(inst->un));
        exit(1);
    }
}

template <AllegroVersion version>
void print_x10(const void *untyped_inst, File<version> *fs, const int d) {
    const x10<A_174> *inst = (const x10<A_174> *)untyped_inst;
    printf_d(d, "x10: t=0x%08X k=0x%08X\n", ntohl(inst->t), ntohl(inst->k));

    printf_d(d + 1, "ptr1:\n");
    if (fs->x07_map.contains(inst->ptr1)) {
        print_struct((const void *)&fs->x07_map.at(inst->ptr1), fs, d + 2);
    } else if (fs->x10_map.contains(inst->ptr1)) {
        print_struct((const void *)&fs->x10_map.at(inst->ptr1), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
        exit(0);
    }

    printf_d(d + 1, "ptr2:");
    if (inst->ptr2 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x12_map.contains(inst->ptr2)) {
            print_struct((const void *)&fs->x12_map.at(inst->ptr2), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
            exit(0);
        }
    }

    /*
    printf_d(d + 1, "ptr3: 0x%08X\n", ntohl(inst->ptr3));
    printf_d(d + 1, "ptr3: \x1b[34m\"%s\"\x1b[0m\n",
             fs->strings.at(inst->ptr3).c_str());
    */

    if (inst->un1 != 0) {
        printf_d(d + 1, "expected un1 = 0, but actually %08X\n",
                 ntohl(inst->un1));
    }

    printf_d(d + 1, "ptr4:\n");
    if (fs->x0F_map.contains(inst->ptr4)) {
        print_struct((const void *)&fs->x0F_map.at(inst->ptr4), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr4 unrecognized: 0x%08X\n", ntohl(inst->ptr4));
        exit(0);
    }

    printf_d(d + 1, "path_str:");
    if (inst->path_str == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x03_map.contains(inst->path_str)) {
            print_struct((const void *)&fs->x03_map.at(inst->path_str), fs,
                         d + 2);
        } else {
            printf_d(d + 2, "path_str unrecognized: 0x%08X\n",
                     ntohl(inst->path_str));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x11(const void *untyped_inst, File<version> *fs, const int d) {
    const x11<version> *inst = (const x11<version> *)untyped_inst;
    printf_d(d,
             "x11: \x1b[36;3mSchematic Pin\x1b[0m t=0x%08X k=0x%08X"
             " \x1b[34m\"%s\"\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k),
             fs->strings.at(inst->ptr1).c_str());

    printf_d(d + 1,
             "ptr2 \x1b[3m(next pin, or footprint if last pin)\x1b[0m:\n");
    if (fs->x11_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x11_map.at(inst->ptr2), fs, d + 2);
    } else if (fs->x0F_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x0F_map.at(inst->ptr2), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
        exit(0);
    }

    printf_d(d + 1, "ptr3 \x1b[3m(matching footprint pin)\x1b[0m:\n");
    if (fs->x08_map.contains(inst->ptr3)) {
        print_struct((const void *)&fs->x08_map.at(inst->ptr3), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
        exit(0);
    }

    if (inst->un != 0) {
        printf_d(d + 1, "expected un to be 0, but = %08X\n", ntohl(inst->un));
        exit(1);
    }
}

template <AllegroVersion version>
void print_x12(const void *untyped_inst, File<version> *fs, const int d) {
    const x12 *inst = (const x12 *)untyped_inst;
    printf_d(d, "x12: t=0x%08X k=0x%08X\n", ntohl(inst->t), ntohl(inst->k));

    printf_d(d + 1, "ptr1:\n");
    if (HAS_ENTRY2(x12_map, inst->ptr1)) {
        PRINT2(x12_map, inst->ptr1, d + 2);
    } else if (HAS_ENTRY2(x10_map, inst->ptr1)) {
        PRINT2(x10_map, inst->ptr1, d + 2);
    } else {
        printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
        exit(0);
    }

    printf_d(d + 1, "ptr2:\n");
    if (HAS_ENTRY2(x11_map, inst->ptr2)) {
        PRINT2(x11_map, inst->ptr2, d + 2);
    } else {
        printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
        exit(0);
    }

    printf_d(d + 1, "ptr3:\n");
    if (HAS_ENTRY2(x32_map, inst->ptr3)) {
        PRINT2(x32_map, inst->ptr3, d + 2);
    } else {
        printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
        exit(0);
    }

    for (int i = 0; i < 2; i++) {
        if (inst->un[i] != 0) {
            printf_d(d + 1, "un[%d] expected to be 0, but is actually 0x%08X\n",
                     i, ntohl(inst->un[i]));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x14(const void *untyped_inst, File<version> *fs, const int d) {
    const x14<A_174> *inst = (const x14<A_174> *)untyped_inst;
    printf_d(d, "x14: t=0x%04X subtype=%02X layer=%d k=0x%08X %08X\n",
             ntohl(inst->type), inst->subtype, inst->layer, ntohl(inst->k),
             ntohl(inst->un2));

    printf_d(d + 1, "ptr1:\n");
    if (fs->x2D_map.contains(inst->ptr1)) {
        print_struct((const void *)&fs->x2D_map.at(inst->ptr1), fs, d + 2);
    } else if (fs->x2B_map.contains(inst->ptr1)) {
        print_struct((const void *)&fs->x2B_map.at(inst->ptr1), fs, d + 2);
    } else if (inst->ptr1 == fs->hdr->ll_x14.tail) {
        printf_d(d + 2, "\x1b[33mptr1 is in header, 0x%08X\x1b[0m\n",
                 ntohl(inst->ptr1));
    } else {
        printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
        exit(0);
    }

    printf_d(d + 1, "ptr2:\n");
    if (fs->x17_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x17_map.at(inst->ptr2), fs, d + 2);
    } else if (fs->x16_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x16_map.at(inst->ptr2), fs, d + 2);
    } else if (fs->x15_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x15_map.at(inst->ptr2), fs, d + 2);
    } else if (fs->x01_map.contains(inst->ptr2)) {
        print_struct((const void *)&fs->x01_map.at(inst->ptr2), fs, d + 2);
    } else {
        printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
        exit(0);
    }

    printf_d(d + 1, "ptr3:");
    if (inst->ptr3 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x03_map.contains(inst->ptr3)) {
            print_struct((const void *)&fs->x03_map.at(inst->ptr3), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr4:");
    if (inst->ptr4 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x26_map.contains(inst->ptr4)) {
            print_struct((const void *)&fs->x26_map.at(inst->ptr4), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr4 unrecognized: 0x%08X\n", ntohl(inst->ptr4));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x15(const void *untyped_inst, File<version> *fs, const int d) {
    const x15<version> *inst = (const x15<version> *)untyped_inst;
    uint32_t un4;
    if constexpr (std::is_same_v<decltype(inst->un4), std::monostate>) {
        un4 = 0;
    } else {
        un4 = inst->un4;
    }
    printf_d(d,
             "x15: t=0x%08X k=0x%08X un3={%08X %08X}"
             " \x1b[2m(%d, %d, %d, %d, width=%d)\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k), ntohl(inst->un3), ntohl(un4),
             inst->coords[0], inst->coords[1], inst->coords[2], inst->coords[3],
             inst->width);
    print_type_hint(d, "Some graphical object? (Has dimensions)");

    printf_d(d + 1, "next:\n");
    if (fs->x34_map.contains(inst->next)) {
        print_struct((const void *)&fs->x34_map.at(inst->next), fs, d + 2);
    } else if (fs->x28_map.contains(inst->next)) {
        print_struct((const void *)&fs->x28_map.at(inst->next), fs, d + 2);
    } else if (fs->x17_map.contains(inst->next)) {
        print_struct((const void *)&fs->x17_map.at(inst->next), fs, d + 2);
    } else if (fs->x16_map.contains(inst->next)) {
        print_struct((const void *)&fs->x16_map.at(inst->next), fs, d + 2);
    } else if (fs->x15_map.contains(inst->next)) {
        print_struct((const void *)&fs->x15_map.at(inst->next), fs, d + 2);
    } else if (fs->x14_map.contains(inst->next)) {
        print_struct((const void *)&fs->x14_map.at(inst->next), fs, d + 2);
    } else if (fs->x05_map.contains(inst->next)) {
        print_struct((const void *)&fs->x05_map.at(inst->next), fs, d + 2);
    } else if (fs->x01_map.contains(inst->next)) {
        print_struct((const void *)&fs->x01_map.at(inst->next), fs, d + 2);
    } else {
        printf_d(d + 2, "next unrecognized: 0x%08X\n", ntohl(inst->next));
        exit(0);
    }

    printf_d(d + 1, "parent:\n");
    if (fs->x34_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x34_map.at(inst->parent), fs, d + 2);
    } else if (fs->x28_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x28_map.at(inst->parent), fs, d + 2);
    } else if (fs->x17_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x17_map.at(inst->parent), fs, d + 2);
    } else if (fs->x16_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x16_map.at(inst->parent), fs, d + 2);
    } else if (fs->x15_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x15_map.at(inst->parent), fs, d + 2);
    } else if (fs->x14_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x14_map.at(inst->parent), fs, d + 2);
    } else if (fs->x05_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x05_map.at(inst->parent), fs, d + 2);
    } else if (fs->x01_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x01_map.at(inst->parent), fs, d + 2);
    } else {
        printf_d(d + 2, "parent unrecognized: 0x%08X\n", ntohl(inst->parent));
        exit(0);
    }
}

template <AllegroVersion version>
void print_x16(const void *untyped_inst, File<version> *fs, const int d) {
    const x16<version> *inst = (const x16<version> *)untyped_inst;
    uint32_t un;
    if constexpr (std::is_same_v<decltype(inst->un), std::monostate>) {
        un = 0;
    } else {
        un = inst->un;
    }
    printf_d(d,
             "x16: t=0x%08X k=0x%08X bitmask=%08X un=%08X"
             " \x1b[2m(%d, %d, %d, %d, width=%d)\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k), ntohl(inst->bitmask), ntohl(un),
             inst->coords[0], inst->coords[1], inst->coords[2], inst->coords[3],
             inst->width);
    print_type_hint(d, "Line");

    printf_d(d + 1, "next:\n");
    if (fs->x34_map.contains(inst->next)) {
        print_struct((const void *)&fs->x34_map.at(inst->next), fs, d + 2);
    } else if (fs->x28_map.contains(inst->next)) {
        print_struct((const void *)&fs->x28_map.at(inst->next), fs, d + 2);
    } else if (fs->x17_map.contains(inst->next)) {
        print_struct((const void *)&fs->x17_map.at(inst->next), fs, d + 2);
    } else if (fs->x16_map.contains(inst->next)) {
        print_struct((const void *)&fs->x16_map.at(inst->next), fs, d + 2);
    } else if (fs->x15_map.contains(inst->next)) {
        print_struct((const void *)&fs->x15_map.at(inst->next), fs, d + 2);
    } else if (fs->x14_map.contains(inst->next)) {
        print_struct((const void *)&fs->x14_map.at(inst->next), fs, d + 2);
    } else if (fs->x05_map.contains(inst->next)) {
        print_struct((const void *)&fs->x05_map.at(inst->next), fs, d + 2);
    } else if (fs->x01_map.contains(inst->next)) {
        print_struct((const void *)&fs->x01_map.at(inst->next), fs, d + 2);
    } else {
        printf_d(d + 2, "next unrecognized: 0x%08X\n", ntohl(inst->next));
        exit(0);
    }

    printf_d(d + 1, "parent:\n");
    if (fs->x05_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x05_map.at(inst->parent), fs, d + 2);
    } else if (fs->x14_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x14_map.at(inst->parent), fs, d + 2);
    } else if (fs->x28_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x28_map.at(inst->parent), fs, d + 2);
    } else if (fs->x34_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x34_map.at(inst->parent), fs, d + 2);
    } else {
        printf_d(d + 2, "parent unrecognized: 0x%08X\n", ntohl(inst->parent));
        exit(0);
    }
}

template <AllegroVersion version>
void print_x17(const void *untyped_inst, File<version> *fs, const int d) {
    const x17<version> *inst = (const x17<version> *)untyped_inst;
    uint32_t un4;
    if constexpr (std::is_same_v<decltype(inst->un4), std::monostate>) {
        un4 = 0;
    } else {
        un4 = inst->un4;
    }
    printf_d(d,
             "x17: t=0x%08X k=0x%08X un3={%08X %08X}"
             " \x1b[2m(%d, %d, %d, %d, width=%d)\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k), ntohl(inst->un3), ntohl(un4),
             inst->coords[0], inst->coords[1], inst->coords[2], inst->coords[3],
             inst->width);
    print_type_hint(d,
                    "Also a graphical object of some kind? (Has dimensions)");

    printf_d(d + 1, "next:\n");
    if (fs->x34_map.contains(inst->next)) {
        print_struct((const void *)&fs->x34_map.at(inst->next), fs, d + 2);
    } else if (fs->x28_map.contains(inst->next)) {
        print_struct((const void *)&fs->x28_map.at(inst->next), fs, d + 2);
    } else if (fs->x17_map.contains(inst->next)) {
        print_struct((const void *)&fs->x17_map.at(inst->next), fs, d + 2);
    } else if (fs->x16_map.contains(inst->next)) {
        print_struct((const void *)&fs->x16_map.at(inst->next), fs, d + 2);
    } else if (fs->x15_map.contains(inst->next)) {
        print_struct((const void *)&fs->x15_map.at(inst->next), fs, d + 2);
    } else if (fs->x14_map.contains(inst->next)) {
        print_struct((const void *)&fs->x14_map.at(inst->next), fs, d + 2);
    } else if (fs->x05_map.contains(inst->next)) {
        print_struct((const void *)&fs->x05_map.at(inst->next), fs, d + 2);
    } else if (fs->x01_map.contains(inst->next)) {
        print_struct((const void *)&fs->x01_map.at(inst->next), fs, d + 2);
    } else {
        printf_d(d + 2, "next unrecognized: 0x%08X\n", ntohl(inst->next));
        exit(0);
    }

    printf_d(d + 1, "parent:\n");
    if (fs->x05_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x05_map.at(inst->parent), fs, d + 2);
    } else if (fs->x14_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x14_map.at(inst->parent), fs, d + 2);
    } else if (fs->x28_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x28_map.at(inst->parent), fs, d + 2);
    } else if (fs->x34_map.contains(inst->parent)) {
        print_struct((const void *)&fs->x34_map.at(inst->parent), fs, d + 2);
    } else {
        printf_d(d + 2, "parent unrecognized: 0x%08X\n", ntohl(inst->parent));
        exit(0);
    }
}

template <AllegroVersion version>
void print_x1B(const void *untyped_inst, File<version> *fs, const int d) {
    const x1B<version> *inst = (const x1B<version> *)untyped_inst;
    printf_d(d,
             "x1B: \x1b[36;3mNet\x1b[0m t=0x%08X k=0x%08X type=%08X %08X "
             "\x1b[34m\"%s\"\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k), ntohl(inst->type),
             ntohl(inst->un2), fs->strings.at(inst->net_name).c_str());

    printf_d(d + 1, "ptr1:");
    if (inst->ptr2 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x23_map.contains(inst->ptr1)) {
            print_struct((const void *)&fs->x23_map.at(inst->ptr1), fs, d + 2);
        } else if (fs->x04_map.contains(inst->ptr1)) {
            print_struct((const void *)&fs->x04_map.at(inst->ptr1), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr2:");
    if (inst->ptr2 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x23_map.contains(inst->ptr2)) {
            print_struct((const void *)&fs->x23_map.at(inst->ptr2), fs, d + 2);
        } else if (fs->x04_map.contains(inst->ptr2)) {
            print_struct((const void *)&fs->x04_map.at(inst->ptr2), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
            exit(0);
        }
    }

    printf_d(d + 1, "path_str_ptr:");
    if (inst->path_str_ptr == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x03_map.contains(inst->path_str_ptr)) {
            print_struct((const void *)&fs->x03_map.at(inst->path_str_ptr), fs,
                         d + 2);
        } else {
            printf_d(d + 2, "path_str_ptr unrecognized: 0x%08X\n",
                     ntohl(inst->path_str_ptr));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr4:");
    if (inst->ptr4 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x2C_map.contains(inst->ptr4)) {
            print_struct((const void *)&fs->x2C_map.at(inst->ptr4), fs, d + 2);
        } else if (fs->x26_map.contains(inst->ptr4)) {
            print_struct((const void *)&fs->x26_map.at(inst->ptr4), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr4 unrecognized: 0x%08X\n", ntohl(inst->ptr4));
            exit(0);
        }
    }

    printf_d(d + 1, "model_ptr:");
    if (inst->model_ptr == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x1E_map.contains(inst->model_ptr)) {
            print_struct((const void *)&fs->x1E_map.at(inst->model_ptr), fs,
                         d + 2);
        } else {
            printf_d(d + 2, "model_ptr unrecognized: 0x%08X\n",
                     ntohl(inst->model_ptr));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr6:");
    if (inst->ptr6 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x22_map.contains(inst->ptr6)) {
            print_struct((const void *)&fs->x22_map.at(inst->ptr6), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr6 unrecognized: 0x%08X\n", ntohl(inst->ptr6));
            exit(0);
        }
    }

    for (int i = 0; i < 2; i++) {
        if (inst->un3[i] != 0) {
            printf_d(d + 1,
                     "un3[%d] expected to be 0, but is actually 0x%08X\n", i,
                     ntohl(inst->un3[i]));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_t13(const t13<version> &t13_inst, File<version> *fs, const int d,
               const int i) {
    printf_d(d, "\x1b[2mparts[\x1b[0m%2d\x1b[2m] =\x1b[0m", i);
    switch (t13_inst.t) {
        case 2:
        case 5:
        case 6:
        case 11:
        case 12:
            printf("%2d: (%d, %d) \x1b[2m(%d, %d, %d)\x1b[0m\n", t13_inst.t,
                   t13_inst.w, t13_inst.h, t13_inst.x2, t13_inst.x3,
                   t13_inst.x4);
            return;
        case 22:
        case 23:
            // printf("%2d: %08X\n", t13_inst.t, ntohl(t13_inst.str_ptr));
            printf("%2d:  (%d, %d),  (%d, %d, %d) %08X\n", t13_inst.t,
                   t13_inst.w, t13_inst.h, t13_inst.x2, t13_inst.x3,
                   t13_inst.x4, ntohl(t13_inst.str_ptr));
            if (fs->strings.contains(t13_inst.str_ptr)) {
                printf_d(d + 1, "\"%s\"",
                         fs->strings.at(t13_inst.str_ptr).c_str());
            } else if (fs->x28_map.contains(t13_inst.str_ptr)) {
                print_struct((const void *)&fs->x28_map.at(t13_inst.str_ptr),
                             fs, d + 1);
            }
            return;
        case 0:
            printf("\x1b[2m%2d: %d, %d, %d, %d, %d\x1b[0m\n", t13_inst.t,
                   t13_inst.w, t13_inst.h, t13_inst.x2, t13_inst.x3,
                   t13_inst.x4);
            return;
        default:
            printf("%2d: ??? %d, %d, %d, %d, %d\n", t13_inst.t, t13_inst.w,
                   t13_inst.h, t13_inst.x2, t13_inst.x3, t13_inst.x4);
    }
}

template <AllegroVersion version>
void print_x1C(const void *untyped_inst, File<version> *fs, const int d) {
    const x1C<version> *inst = (const x1C<version> *)untyped_inst;
    printf_d(d,
             "x1C: \x1b[36;3mPad\x1b[0m t=0x%08X k=0x%08X"
             " \x1b[34m\"%s\"\x1b[0m"
             " \x1b[2m(%d, %d, %d, %d, %d, %d)\x1b[0m"
             " \x1b[2m(%d, %d, %d, %d)\x1b[0m\n",
             ntohl(inst->hdr.t), ntohl(inst->hdr.k),
             fs->strings.at(inst->hdr.pad_str).c_str(), inst->hdr.un0_0,
             inst->hdr.un0_1, inst->hdr.pad_path, inst->hdr.un0_3,
             inst->hdr.un0_4, inst->hdr.un0_5, inst->hdr.coords2[0],
             inst->hdr.coords2[1], inst->hdr.coords2[2], inst->hdr.coords2[3]);
    uint32_t un7;
    if constexpr (std::is_same_v<decltype(inst->hdr.un7), std::monostate>) {
        un7 = 0;
    } else {
        un7 = inst->hdr.un7;
    }
    printf_d(d + 1,
             "un1=%08X un2=%04X %08X un3=%04X size_hint=%d un4=%08X "
             "un5=%08X un6=%08X un7=%08X len(parts)=%d\n",
             ntohl(inst->hdr.un1), ntohs(inst->hdr.un2_0),
             ntohs(inst->hdr.un2_1), ntohl(inst->hdr.un3), inst->hdr.size_hint,
             ntohl(inst->hdr.un4), ntohl(inst->hdr.un5), ntohl(inst->hdr.un6),
             ntohl(un7), inst->parts.size());

    uint8_t i = 0;
    for (const t13<version> &t13_inst : inst->parts) {
        print_t13(t13_inst, fs, d + 1, i++);
    }
}

template <AllegroVersion version>
void print_x1E(const void *untyped_inst, File<version> *fs, const int d) {
    const x1E *inst = (const x1E *)untyped_inst;
    printf_d(d,
             "x1E: \x1b[36;3mModel\x1b[0m"
             " t=0x%08X k=0x%08X %08X %d %08X"
             " \x1b[34m\"%.70s\"\x1b[0m\n",
             ntohl(inst->hdr.t), ntohl(inst->hdr.k), ntohl(inst->hdr.un1),
             inst->hdr.un2, ntohl(inst->hdr.un3), inst->s);

    printf_d(d + 1, "str_ptr:");
    if (inst->hdr.str_ptr == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x03_map, inst->hdr.str_ptr)) {
            PRINT2(x03_map, inst->hdr.str_ptr, d + 2);
        } else {
            printf_d(d + 2, "str_ptr unrecognized: 0x%08X\n",
                     ntohl(inst->hdr.str_ptr));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x22(const void *untyped_inst, File<version> *fs, const int d) {
    const x22<version> *inst = (const x22<version> *)untyped_inst;
    printf_d(
        d, "x22: t=0x%08X k=0x%08X %08X %08X %08X %08X, %08X %08X %08X %08X\n",
        ntohl(inst->t), ntohl(inst->k), ntohl(inst->un[0]), ntohl(inst->un[1]),
        ntohl(inst->un[2]), ntohl(inst->un[3]), ntohl(inst->un[4]),
        ntohl(inst->un[5]), ntohl(inst->un[6]), ntohl(inst->un[7]));
}

template <AllegroVersion version>
void print_x23(const void *untyped_inst, File<version> *fs, const int d) {
    const x23<version> *inst = (const x23<version> *)untyped_inst;
    printf_d(d,
             "x23: \x1b[36;3mConnection (Rat)\x1b[0m t=0x%04X subtype=%02X "
             "layer=%02X k=0x%08X"
             " bm={%08X %08X}"
             " \x1b[2m(%d, %d, %d, %d, %d)\x1b[0m\n",
             ntohs(inst->type), inst->subtype, inst->layer, ntohl(inst->k),
             ntohl(inst->bitmask[0]), ntohl(inst->bitmask[1]), inst->coords[0],
             inst->coords[1], inst->coords[2], inst->coords[3],
             inst->coords[4]);

    printf_d(d + 1, "ptr0:\n");
    if (HAS_ENTRY2(x23_map, inst->ptr0)) {
        PRINT2(x23_map, inst->ptr0, d + 2);
    } else if (HAS_ENTRY2(x1B_map, inst->ptr0)) {
        PRINT2(x1B_map, inst->ptr0, d + 2);
    } else {
        printf_d(d + 2, "ptr0 unrecognized: 0x%08X\n", ntohl(inst->ptr0));
        exit(0);
    }

    printf_d(d + 1, "ptr1:\n");
    if (HAS_ENTRY2(x32_map, inst->ptr1)) {
        PRINT2(x32_map, inst->ptr1, d + 2);
    } else {
        printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
        exit(0);
    }

    printf_d(d + 1, "ptr2:");
    if (inst->ptr2 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x23_map, inst->ptr2)) {
            PRINT2(x23_map, inst->ptr2, d + 2);
        } else {
            printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr3:\n");
    if (HAS_ENTRY2(x32_map, inst->ptr3)) {
        PRINT2(x32_map, inst->ptr3, d + 2);
    } else {
        printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
        exit(0);
    }

    for (int i = 0; i < 8; i++) {
        if (inst->un[i] != 0) {
            printf_d(d + 1, "un[%d] expected to be 0, but is actually 0x%08X\n",
                     i, ntohl(inst->un[i]));
            exit(1);
        }
    }
}

template <AllegroVersion version>
void print_x26(const void *untyped_inst, File<version> *fs, const int d) {
    const x26<version> *inst = (const x26<version> *)untyped_inst;
    printf_d(d,
             "x26: \x1b[36;3mMembership Assignment?\x1b[0m"
             " t=0x%08X k=0x%08X\n",
             ntohl(inst->t), ntohl(inst->k));

    printf_d(d + 1, "member_ptr:\n");
    if (HAS_ENTRY2(x33_map, inst->member_ptr)) {
        PRINT2(x33_map, inst->member_ptr, d + 2);
    } else if (HAS_ENTRY2(x32_map, inst->member_ptr)) {
        PRINT2(x32_map, inst->member_ptr, d + 2);
    } else if (HAS_ENTRY2(x30_map, inst->member_ptr)) {
        PRINT2(x30_map, inst->member_ptr, d + 2);
    } else if (HAS_ENTRY2(x2D_map, inst->member_ptr)) {
        PRINT2(x2D_map, inst->member_ptr, d + 2);
    } else if (HAS_ENTRY2(x2C_map, inst->member_ptr)) {
        PRINT2(x2C_map, inst->member_ptr, d + 2);
    } else if (HAS_ENTRY2(x28_map, inst->member_ptr)) {
        PRINT2(x28_map, inst->member_ptr, d + 2);
    } else if (HAS_ENTRY2(x26_map, inst->member_ptr)) {
        PRINT2(x26_map, inst->member_ptr, d + 2);
    } else if (HAS_ENTRY2(x24_map, inst->member_ptr)) {
        PRINT2(x24_map, inst->member_ptr, d + 2);
    } else if (HAS_ENTRY2(x22_map, inst->member_ptr)) {
        PRINT2(x22_map, inst->member_ptr, d + 2);
    } else if (HAS_ENTRY2(x1B_map, inst->member_ptr)) {
        PRINT2(x1B_map, inst->member_ptr, d + 2);
    } else if (HAS_ENTRY2(x14_map, inst->member_ptr)) {
        PRINT2(x14_map, inst->member_ptr, d + 2);
    } else if (HAS_ENTRY2(x0C_map, inst->member_ptr)) {
        PRINT2(x0C_map, inst->member_ptr, d + 2);
    } else {
        printf_d(d + 2, "member_ptr unrecognized: 0x%08X\n",
                 ntohl(inst->member_ptr));
        exit(0);
    }

    printf_d(d + 1, "group_ptr:\n");
    if (fs->x33_map.contains(inst->group_ptr)) {
        print_struct((const void *)&fs->x33_map.at(inst->group_ptr), fs, d + 2);
    } else if (fs->x2C_map.contains(inst->group_ptr)) {
        print_struct((const void *)&fs->x2C_map.at(inst->group_ptr), fs, d + 2);
    } else if (fs->x22_map.contains(inst->group_ptr)) {
        print_struct((const void *)&fs->x22_map.at(inst->group_ptr), fs, d + 2);
    } else {
        printf_d(d + 2, "group_ptr unrecognized: 0x%08X\n",
                 ntohl(inst->group_ptr));
        exit(0);
    }

    printf_d(d + 1, "const_ptr:");
    if (inst->const_ptr == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x03_map, inst->const_ptr)) {
            PRINT2(x03_map, inst->const_ptr, d + 2);
        } else {
            printf_d(d + 2, "const_ptr unrecognized: 0x%08X\n",
                     ntohl(inst->const_ptr));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x28(const void *untyped_inst, File<version> *fs, const int d) {
    const x28<version> *inst = (const x28<version> *)untyped_inst;
    printf_d(d,
             "x28: t=0x%04X subtype=%02X layer=%d k=0x%08X 0x%08X"
             " \x1b[2m(%d, %d, %d, %d)\x1b[0m\n",
             ntohl(inst->type), inst->subtype, inst->layer, ntohl(inst->k),
             ntohl(inst->un4), inst->coords[0], inst->coords[1],
             inst->coords[2], inst->coords[3]);
    print_type_hint(d, "Shape like rect or circle?");

    /*
    printf_d(d+1, "un1:\n");
    if (fs->x34_map->contains(inst->un1)) {
        print_struct((const void*)&fs->x34_map->at(inst->un1), fs, d+2);
    } else if (fs->x33_map->contains(inst->un1)) {
        print_struct((const void*)&fs->x33_map->at(inst->un1), fs, d+2);
    } else if (fs->x32_map->contains(inst->un1)) {
        print_struct((const void*)&fs->x32_map->at(inst->un1), fs, d+2);
    } else if (fs->x2D_map->contains(inst->un1)) {
        print_struct((const void*)&fs->x2D_map->at(inst->un1), fs, d+2);
    } else if (fs->x2B_map->contains(inst->un1)) {
        print_struct((const void*)&fs->x2B_map->at(inst->un1), fs, d+2);
    } else if (fs->x28_map->contains(inst->un1)) {
        print_struct((const void*)&fs->x28_map->at(inst->un1), fs, d+2);
    } else if (fs->x17_map->contains(inst->un1)) {
        print_struct((const void*)&fs->x17_map->at(inst->un1), fs, d+2);
    } else if (fs->x16_map->contains(inst->un1)) {
        print_struct((const void*)&fs->x16_map->at(inst->un1), fs, d+2);
    } else if (fs->x15_map->contains(inst->un1)) {
        print_struct((const void*)&fs->x15_map->at(inst->un1), fs, d+2);
    } else if (fs->x14_map->contains(inst->un1)) {
        print_struct((const void*)&fs->x14_map->at(inst->un1), fs, d+2);
    } else if (fs->x05_map.contains(inst->un1)) {
        print_struct((const void*)&fs->x05_map.at(inst->un1), fs, d+2);
    } else if (fs->x04_map.contains(inst->un1)) {
        print_struct((const void*)&fs->x04_map.at(inst->un1), fs, d+2);
    } else if (fs->x01_map->contains(inst->un1)) {
        print_struct((const void*)&fs->x01_map->at(inst->un1), fs, d+2);
    } else if (inst->un1 == fs->hdr->un1[28]) {
        printf_d(d+2, "\x1b[33mun1 is in header, 0x%08X?\x1b[0m\n",
            ntohl(inst->un1));
    } else if (inst->un1 == fs->hdr->un1[20]) {
        printf_d(d+2, "\x1b[33mun1 is in header, 0x%08X?\x1b[0m\n",
            ntohl(inst->un1));
    } else {
        printf_d(d+2, "un1 unrecognized: 0x%08X\n", ntohl(inst->un1));
        exit(0);
    }
    */

    if (inst->ptr1 != 0) {
        printf_d(d + 1, "ptr1:\n");
        if (fs->x2D_map.contains(inst->ptr1)) {
            print_struct((const void *)&fs->x2D_map.at(inst->ptr1), fs, d + 2);
        } else if (fs->x2B_map.contains(inst->ptr1)) {
            print_struct((const void *)&fs->x2B_map.at(inst->ptr1), fs, d + 2);
        } else if (fs->x04_map.contains(inst->ptr1)) {
            print_struct((const void *)&fs->x04_map.at(inst->ptr1), fs, d + 2);
        } else if (inst->ptr1 == fs->hdr->ll_x0E_x28.tail) {
            printf_d(d + 2, "\x1b[33mptr1 is in header, 0x%08X?\x1b[0m\n",
                     ntohl(inst->ptr1));
        } else if (inst->ptr1 == fs->hdr->ll_x24_x28.tail) {
            printf_d(d + 2, "\x1b[33mptr1 is in header, 0x%08X?\x1b[0m\n",
                     ntohl(inst->ptr1));
        } else if (inst->ptr1 == fs->hdr->ll_x14.tail) {
            printf_d(d + 2, "\x1b[33mptr1 is in header, 0x%08X?\x1b[0m\n",
                     ntohl(inst->ptr1));
        } else {
            printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
            printf_d(d + 2,
                     "compared against 0x%08X and 0x%08X"
                     " and 0x%08X",
                     ntohl(fs->hdr->ll_x0E_x28.tail),
                     ntohl(fs->hdr->ll_x24_x28.tail),
                     ntohl(fs->hdr->ll_x1C.tail));
            exit(0);
        }
    }

    if (inst->next != 0) {
        printf_d(d + 1, "next:\n");
        if (fs->x28_map.contains(inst->next)) {
            print_struct((const void *)&fs->x28_map.at(inst->next), fs, d + 2);
        } else if (fs->x33_map.contains(inst->next)) {
            print_struct((const void *)&fs->x33_map.at(inst->next), fs, d + 2);
        } else if (fs->x2D_map.contains(inst->next)) {
            print_struct((const void *)&fs->x2D_map.at(inst->next), fs, d + 2);
        } else {
            printf_d(d + 2, "next unrecognized: 0x%08X\n", ntohl(inst->next));
            exit(0);
        }
    }

    if (inst->ptr3 != 0) {
        printf_d(d + 1, "ptr3:\n");
        if (fs->x09_map.contains(inst->ptr3)) {
            print_struct((const void *)&fs->x09_map.at(inst->ptr3), fs, d + 2);
        } else if (fs->x05_map.contains(inst->ptr3)) {
            print_struct((const void *)&fs->x05_map.at(inst->ptr3), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
            exit(0);
        }
    }

    if (inst->ptr4 != 0) {
        printf_d(d + 1, "ptr4 \x1b[3m(keepout, keepin, etc. region)\x1b[0m:\n");
        if (fs->x34_map.contains(inst->ptr4)) {
            print_struct((const void *)&fs->x34_map.at(inst->ptr4), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr4 unrecognized: 0x%08X\n", ntohl(inst->ptr4));
            exit(0);
        }
    }

    if (inst->first_segment_ptr != 0) {
        printf_d(d + 1, "first_segment_ptr:\n");
        if (fs->x17_map.contains(inst->first_segment_ptr)) {
            print_struct((const void *)&fs->x17_map.at(inst->first_segment_ptr),
                         fs, d + 2);
        } else if (fs->x16_map.contains(inst->first_segment_ptr)) {
            print_struct((const void *)&fs->x16_map.at(inst->first_segment_ptr),
                         fs, d + 2);
        } else if (fs->x15_map.contains(inst->first_segment_ptr)) {
            print_struct((const void *)&fs->x15_map.at(inst->first_segment_ptr),
                         fs, d + 2);
        } else if (fs->x01_map.contains(inst->first_segment_ptr)) {
            print_struct((const void *)&fs->x01_map.at(inst->first_segment_ptr),
                         fs, d + 2);
        } else {
            printf_d(d + 2, "first_segment_ptr unrecognized: 0x%08X\n",
                     ntohl(inst->first_segment_ptr));
            exit(0);
        }
    }

    if (inst->ptr6 != 0) {
        printf_d(d + 1, "ptr6:\n");
        if (fs->x03_map.contains(inst->ptr6)) {
            print_struct((const void *)&fs->x03_map.at(inst->ptr6), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr6 unrecognized: 0x%08X\n", ntohl(inst->ptr6));
            exit(0);
        }
    }

    if constexpr (!std::is_same_v<decltype(inst->ptr7), std::monostate>) {
        if (inst->ptr7 != 0) {
            printf_d(d + 1, "ptr7:\n");
            if (fs->x2C_map.contains(inst->ptr7)) {
                print_struct((const void *)&fs->x2C_map.at(inst->ptr7), fs,
                             d + 2);
            } else if (fs->x26_map.contains(inst->ptr7)) {
                print_struct((const void *)&fs->x26_map.at(inst->ptr7), fs,
                             d + 2);
            } else {
                printf_d(d + 2, "ptr7 unrecognized: 0x%08X\n",
                         ntohl(inst->ptr7));
                exit(0);
            }
        }
    }
}

template <AllegroVersion version>
void print_x2A(const void *untyped_inst, File<version> *fs, const int d) {
    const x2A *inst = (const x2A *)untyped_inst;
    printf_d(d,
             "x2A: \x1b[36;3mLayer Name List\x1b[0m"
             " t=0x%04X size=0x%04X k=0x%08X\n",
             ntohs(inst->hdr.t), ntohs(inst->hdr.size), ntohl(inst->k));

    if (inst->references) {
        for (const auto &entry : inst->reference_entries) {
            printf_d(d + 1, "%08X \x1b[34m\"%s\"\x1b[0m\n", ntohl(entry.suffix),
                     fs->strings.at(entry.ptr).c_str());
        }
    } else {
        for (const auto &entry : inst->local_entries) {
            printf_d(d + 1, "%08X \x1b[34m\"%s\"\x1b[0m\n", ntohl(entry.suffix),
                     entry.s.c_str());
        }
    }
}

template <AllegroVersion version>
void print_x2B(const void *untyped_inst, File<version> *fs, const int d) {
    const x2B<version> *inst = (const x2B<version> *)untyped_inst;
    printf_d(d,
             "x2B: t=0x%08X k=0x%08X"
             " \x1b[34m\"%s\"\x1b[0m"
             " (→\x1b[34m\"%s.%s\"\x1b[0m)"
             " \x1b[2m(%d, %d, %d, %d)\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k),
             x2B_footprint(inst, fs).value_or(std::string("?")).c_str(),
             x2D_refdes(inst->ptr2, fs).value_or(std::string("?")).c_str(),
             x0D_pin_name(inst->ptr3, fs).value_or(std::string("?")).c_str(),
             inst->coords[0], inst->coords[1], inst->coords[2],
             inst->coords[3]);

    if (inst->un1 != 0) {
        printf_d(d + 1, "un1 expected to be 0, but is actually 0x%08X\n",
                 ntohl(inst->un1));
        exit(0);
    }

    /*
    printf_d(d+1, "ptr1:\n");
    if (HAS_ENTRY(x2B_map, inst->ptr1)) {
        PRINT(x2B_map, inst->ptr1, d+2);
    } else if (inst->ptr1 == fs->hdr->un1[32]) {
        printf_d(d+2, "\x1b[33mptr1 is in header, 0x%08x?\x1b[0m\n",
            ntohl(inst->ptr1));
    } else {
        printf_d(d+2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
        exit(0);
    }
    */

    printf_d(d + 1, "ptr2:");
    if (inst->ptr2 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x2D_map, inst->ptr2)) {
            PRINT2(x2D_map, inst->ptr2, d + 2);
        } else {
            printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr3:");
    if (inst->ptr3 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x0D_map, inst->ptr3)) {
            PRINT2(x0D_map, inst->ptr3, d + 2);
        } else {
            printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr4:");
    if (inst->ptr4 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x32_map, inst->ptr4)) {
            PRINT2(x32_map, inst->ptr4, d + 2);
        } else {
            printf_d(d + 2, "ptr4 unrecognized: 0x%08X\n", ntohl(inst->ptr4));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr5:");
    if (inst->ptr5 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x14_map, inst->ptr5)) {
            PRINT2(x14_map, inst->ptr5, d + 2);
        } else {
            printf_d(d + 2, "ptr5 unrecognized: 0x%08X\n", ntohl(inst->ptr5));
            exit(0);
        }
    }

    printf_d(d + 1, "str_ptr:");
    if (inst->str_ptr == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x03_map, inst->str_ptr)) {
            PRINT2(x03_map, inst->str_ptr, d + 2);
        } else {
            printf_d(d + 2, "str_ptr unrecognized: 0x%08X\n",
                     ntohl(inst->str_ptr));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr6:");
    if (inst->ptr6 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x28_map, inst->ptr6)) {
            PRINT2(x28_map, inst->ptr6, d + 2);
        } else if (HAS_ENTRY2(x0E_map, inst->ptr6)) {
            PRINT2(x0E_map, inst->ptr6, d + 2);
        } else {
            printf_d(d + 2, "ptr6 unrecognized: 0x%08X\n", ntohl(inst->ptr6));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr7:");
    if (inst->ptr7 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x28_map, inst->ptr7)) {
            PRINT2(x28_map, inst->ptr7, d + 2);
        } else if (HAS_ENTRY2(x24_map, inst->ptr7)) {
            PRINT2(x24_map, inst->ptr7, d + 2);
        } else {
            printf_d(d + 2, "ptr7 unrecognized: 0x%08X\n", ntohl(inst->ptr7));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr8:");
    if (inst->ptr8 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x33_map, inst->ptr8)) {
            PRINT2(x33_map, inst->ptr8, d + 2);
        } else if (HAS_ENTRY2(x28_map, inst->ptr8)) {
            PRINT2(x28_map, inst->ptr8, d + 2);
        } else {
            printf_d(d + 2, "ptr8 unrecognized: 0x%08X\n", ntohl(inst->ptr8));
            exit(0);
        }
    }

    /*
    if (inst->un2 != 0) {
        printf_d(d + 1, "un2 expected to be 0, but is actually 0x%08X\n",
                 ntohl(inst->un2));
        exit(0);
    }
    */
}

template <AllegroVersion version>
void print_x2C(const void *untyped_inst, File<version> *fs, const int d) {
    const x2C<version> *inst = (const x2C<version> *)untyped_inst;
    printf_d(d, "x2C: t=0x%08X k=0x%08X bitmask=%08X", ntohl(inst->t),
             ntohl(inst->k), ntohl(inst->bitmask));

    if (inst->string_ptr != 0) {
        std::printf(" \x1b[34m\"%s\"\x1b[0m",
                    fs->strings.at(inst->string_ptr).c_str());
    }
    std::cout << std::endl;

    printf_d(d + 1, "ptr1:");
    if (inst->ptr1 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x3C_map, inst->ptr1)) {
            PRINT2(x3C_map, inst->ptr1, d + 2);
        } else if (HAS_ENTRY2(x37_map, inst->ptr1)) {
            PRINT2(x37_map, inst->ptr1, d + 2);
        } else {
            printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr2:");
    if (inst->ptr2 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x03_map, inst->ptr2)) {
            PRINT2(x03_map, inst->ptr2, d + 2);
        } else {
            printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr3:");
    if (inst->ptr3 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x2C_map, inst->ptr3)) {
            PRINT2(x2C_map, inst->ptr3, d + 2);
        } else if (HAS_ENTRY2(x26_map, inst->ptr3)) {
            PRINT2(x26_map, inst->ptr3, d + 2);
        } else {
            printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x2D(const void *untyped_inst, File<version> *fs, const int d) {
    const x2D<version> *inst = (const x2D<version> *)untyped_inst;

    printf_d(d,
             "x2D: \x1b[36;3mPlaced Symbol\x1b[0m"
             " t=0x%04X layer=0x%02X un0=0x%02X k=0x%08X"
             " bitmask=%08X un2=%08X un3=%08X"
             " (→\x1b[34m\"%s\"\x1b[0m)\n",
             ntohs(inst->t), inst->layer, inst->un0, ntohl(inst->k),
             ntohl(inst->bitmask1), ntohl(inst->un2), ntohl(inst->un3),
             x2D_refdes(inst->k, fs).value_or(std::string()).c_str());
    printf_d(d + 1, "\x1b[2m(%0.1f deg, %d, %d, L=%d)\x1b[0m\n",
             inst->rotation / 1000., inst->coords[0], inst->coords[1],
             x2D_layer(inst->k, fs).value_or(-1));

    printf_d(d + 1, "inst_ref:");
    if constexpr (std::is_same_v<decltype(inst->inst_ref), std::monostate>) {
        printf(" \x1b[2mmonostate\x1b[0m\n");
    } else {
        if (inst->inst_ref == 0) {
            printf(" \x1b[2mnull\x1b[0m\n");
        } else {
            std::printf("\n");
            if (HAS_ENTRY2(x07_map, inst->inst_ref)) {
                print_struct((const void *)&fs->x07_map.at(inst->inst_ref), fs,
                             d + 2);
            } else {
                printf_d(d + 2, "inst_ref unrecognized: 0x%08X\n",
                         ntohl(inst->inst_ref));
                exit(0);
            }
        }
    }

    printf_d(d + 1, "ptr1:");
    if (inst->ptr1 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x14_map.contains(inst->ptr1)) {
            print_struct((const void *)&fs->x14_map.at(inst->ptr1), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
            exit(0);
        }
    }

    printf_d(d + 1, "first_pad_ptr:");
    if (inst->first_pad_ptr == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x32_map.contains(inst->first_pad_ptr)) {
            print_struct((const void *)&fs->x32_map.at(inst->first_pad_ptr), fs,
                         d + 2);
        } else {
            printf_d(d + 2, "first_pad_ptr unrecognized: 0x%08X\n",
                     ntohl(inst->first_pad_ptr));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr3:");
    if (inst->ptr3 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x30_map.contains(inst->ptr3)) {
            print_struct((const void *)&fs->x30_map.at(inst->ptr3), fs, d + 2);
        } else if (fs->x03_map.contains(inst->ptr3)) {
            print_struct((const void *)&fs->x03_map.at(inst->ptr3), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
            exit(0);
        }
    }

    for (int i = 0; i < 3; i++) {
        printf_d(d + 1, "ptr4[%d]:", i);
        if (inst->ptr4[i] == 0) {
            printf(" \x1b[2mnull\x1b[0m\n");
        } else {
            std::printf("\n");
            if (fs->x33_map.contains(inst->ptr4[i])) {
                print_struct((const void *)&fs->x33_map.at(inst->ptr4[i]), fs,
                             d + 2);
            } else if (fs->x28_map.contains(inst->ptr4[i])) {
                print_struct((const void *)&fs->x28_map.at(inst->ptr4[i]), fs,
                             d + 2);
            } else if (fs->x24_map.contains(inst->ptr4[i])) {
                print_struct((const void *)&fs->x24_map.at(inst->ptr4[i]), fs,
                             d + 2);
            } else if (fs->x0E_map.contains(inst->ptr4[i])) {
                print_struct((const void *)&fs->x0E_map.at(inst->ptr4[i]), fs,
                             d + 2);
            } else if (fs->x05_map.contains(inst->ptr4[i])) {
                print_struct((const void *)&fs->x05_map.at(inst->ptr4[i]), fs,
                             d + 2);
            } else {
                printf_d(d + 2, "ptr4[%d] unrecognized: 0x%08X\n", i,
                         ntohl(inst->ptr4[i]));
                exit(0);
            }
        }
    }

    printf_d(d + 1, "group_assignment_ptr:");
    if (inst->group_assignment_ptr == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x26_map.contains(inst->group_assignment_ptr)) {
            print_struct(
                (const void *)&fs->x26_map.at(inst->group_assignment_ptr), fs,
                d + 2);
        } else {
            printf_d(d + 2, "group_assignment_ptr unrecognized: 0x%08X\n",
                     ntohl(inst->group_assignment_ptr));
            exit(0);
        }
    }
};

template <AllegroVersion version>
void print_x30(const void *untyped_inst, File<version> *fs, const int d) {
    const x30<version> *inst = (const x30<version> *)untyped_inst;
    TextProperties font;
    if constexpr (!std::is_same_v<decltype(inst->font), std::monostate>) {
        font = inst->font;
    } else {
        font = inst->font_16x;
    }
    printf_d(d,
             "x30: \x1b[36;3mString Graphic Wrapper\x1b[0m"
             " t=0x%04X subtype=%02X layer=%d k=0x%08X un1=%08X font_key=%02X"
             " bm1=%02X just=%02X rev=%02X un3=%08X"
             " \x1b[2m(%d, %d, %0.1f deg)\x1b[0m\n",
             ntohs(inst->type), inst->subtype, inst->layer, ntohl(inst->k),
             ntohl(inst->un1), font.key, font.bm1, font.align, font.reversed,
             ntohl(inst->un3), inst->coords[0], inst->coords[1],
             inst->rotation / 1000.);

    printf_d(d + 1, "str_graphic_ptr:\n");
    if (fs->x31_map.contains(inst->str_graphic_ptr)) {
        print_struct((const void *)&fs->x31_map.at(inst->str_graphic_ptr), fs,
                     d + 2);
    } else {
        printf_d(d + 2, "str_graphic_ptr unrecognized: 0x%08X\n",
                 ntohl(inst->str_graphic_ptr));
        exit(0);
    }

    if constexpr (!std::is_same_v<decltype(inst->un4), std::monostate>) {
        printf_d(d + 1, "un4:");
        if (inst->un4 == 0) {
            printf(" \x1b[2mnull\x1b[0m\n");
        } else {
            std::printf(" 0x %08X\n", ntohl(inst->un4));
        }
    }

    if constexpr (!std::is_same_v<decltype(inst->un5), std::monostate>) {
        printf_d(d + 1, "un5:");
        if (inst->un5 == 0) {
            printf(" \x1b[2mnull\x1b[0m\n");
        } else {
            std::printf(" 0x %08X\n", ntohl(inst->un5));
        }
    }

    if constexpr (!std::is_same_v<decltype(inst->ptr3), std::monostate>) {
        printf_d(d + 1, "ptr3:");
        if (inst->ptr3 == 0) {
            printf(" \x1b[2mnull\x1b[0m\n");
        } else {
            std::printf(" 0x %08X\n", ntohl(inst->ptr3));
        }
    }

    if constexpr (!std::is_same_v<decltype(inst->un6), std::monostate>) {
        printf_d(d + 1, "un6:");
        if (inst->un6 == 0) {
            printf(" \x1b[2mnull\x1b[0m\n");
        } else {
            std::printf(" 0x %08X\n", ntohl(inst->un6));
        }
    }

    if constexpr (!std::is_same_v<decltype(inst->ptr3), std::monostate>) {
        if (inst->ptr3 != 0) {
            printf_d(d + 1, "ptr3:\n");
            if (fs->x01_map.contains(inst->ptr3)) {
                print_struct((const void *)&fs->x01_map.at(inst->ptr3), fs,
                             d + 2);
            } else if (fs->x04_map.contains(inst->ptr3)) {
                print_struct((const void *)&fs->x04_map.at(inst->ptr3), fs,
                             d + 2);
            } else if (fs->x09_map.contains(inst->ptr3)) {
                print_struct((const void *)&fs->x09_map.at(inst->ptr3), fs,
                             d + 2);
            } else if (fs->x26_map.contains(inst->ptr3)) {
                print_struct((const void *)&fs->x26_map.at(inst->ptr3), fs,
                             d + 2);
            } else {
                printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n",
                         ntohl(inst->ptr3));
                exit(0);
            }
        }
    }

    const x36_x08<version> *maybe_font;
    if constexpr (!std::is_same_v<decltype(inst->font), std::monostate>) {
        maybe_font = font_lookup(inst->font.key, *fs);
    } else {
        maybe_font = font_lookup(inst->font_16x.key, *fs);
    }
    if (maybe_font) {
        auto font = *maybe_font;
        printf_d(d + 1,
                 "Font info? %08x %08x"
                 " xs={h=% 7d w=% 7d % 7d % 7d % 7d %7d}\n",
                 ntohl(font.a), ntohl(font.b), font.char_height / 1000,
                 font.char_width / 1000, font.xs[0] / 1000, font.xs[1] / 1000,
                 font.xs[2] / 1000, font.xs[3] / 1000);
        if constexpr (!std::is_same_v<decltype(font.ys), std::monostate>) {
            printf_d(d + 2, "ys={%d %d %d %d %d %d %d %d %d %d}\n",
                     font.ys[0] / 1000, font.ys[1] / 1000, font.ys[2] / 1000,
                     font.ys[3] / 1000, font.ys[4] / 1000, font.ys[5] / 1000,
                     font.ys[6] / 1000, font.ys[7] / 1000);
        }
    } else {
        printf_d(d + 1, "Failed to find font information\n");
    }
}

template <AllegroVersion version>
void print_x31(const void *untyped_inst, File<version> *fs, const int d) {
    const x31 *inst = (const x31 *)untyped_inst;
    printf_d(d,
             "x31: \x1b[36;3mString Graphic\x1b[0m"
             " t=0x%04X subtype=%02X layer=%d k=0x%08X un=%08X "
             "\x1b[34m\"%s\"\x1b[0m"
             " \x1b[2m(%d, %d)\x1b[0m\n",
             ntohs(inst->hdr.t), inst->hdr.subtype, inst->hdr.layer,
             ntohl(inst->hdr.k), ntohl(inst->hdr.un), inst->s.c_str(),
             inst->hdr.coords[0], inst->hdr.coords[1]);

    printf_d(d + 1, "str_graphic_wrapper_ptr:\n");
    if (fs->x30_map.contains(inst->hdr.str_graphic_wrapper_ptr)) {
        print_struct(
            (const void *)&fs->x30_map.at(inst->hdr.str_graphic_wrapper_ptr),
            fs, d + 2);
    } else {
        printf_d(d + 2, "str_graphic_wrapper_ptr unrecognized: 0x%08X\n",
                 ntohl(inst->hdr.str_graphic_wrapper_ptr));
        exit(0);
    }
}

template <AllegroVersion version>
void print_x32(const void *untyped_inst, File<version> *fs, const int d) {
    const x32<version> *inst = (const x32<version> *)untyped_inst;
    printf_d(
        d,
        "x32: \x1b[36;3mSymbol Pad\x1b[0m"
        " t=0x%04X subtype=%02X layer=%d k=0x%08X bitmask1=%08X"
        " (→\x1b[34m\"%s.%s\"\x1b[0m)"
        " \x1b[2m(%d, %d, %d, %d)\x1b[0m\n",
        ntohs(inst->type), inst->subtype, inst->layer, ntohl(inst->k),
        ntohl(inst->bitmask1),
        x2B_or_x2D_refdes(inst->ptr3, fs).value_or(std::string("?")).c_str(),
        x0D_pin_name(inst->ptr5, fs).value_or(std::string("?")).c_str(),
        inst->coords[0], inst->coords[1], inst->coords[2], inst->coords[3]);

    printf_d(d + 1, "ptr1 \x1b[3m(net/shape pair)\x1b[0m:");
    if (inst->ptr1 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x04_map, inst->ptr1)) {
            PRINT2(x04_map, inst->ptr1, d + 2);
        } else {
            printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
            exit(0);
        }
    }

    printf_d(d + 1, "next:\n");
    if (HAS_ENTRY2(x32_map, inst->next)) {
        PRINT2(x32_map, inst->next, d + 2);
    } else if (HAS_ENTRY2(x2D_map, inst->next)) {
        PRINT2(x2D_map, inst->next, d + 2);
    } else if (HAS_ENTRY2(x2B_map, inst->next)) {
        PRINT2(x2B_map, inst->next, d + 2);
    } else {
        printf_d(d + 2, "next unrecognized: 0x%08X\n", ntohl(inst->next));
        exit(0);
    }

    printf_d(d + 1, "ptr3 \x1b[3m(placed symbol)\x1b[0m:\n");
    if (HAS_ENTRY2(x2D_map, inst->ptr3)) {
        PRINT2(x2D_map, inst->ptr3, d + 2);
    } else if (HAS_ENTRY2(x2B_map, inst->ptr3)) {
        PRINT2(x2B_map, inst->ptr3, d + 2);
    } else {
        printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
        exit(0);
    }

    printf_d(d + 1, "ptr4:");
    if (inst->ptr4 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x09_map, inst->ptr4)) {
            PRINT2(x09_map, inst->ptr4, d + 2);
        } else if (HAS_ENTRY2(x05_map, inst->ptr4)) {
            PRINT2(x05_map, inst->ptr4, d + 2);
        } else {
            printf_d(d + 2, "ptr4 unrecognized: 0x%08X\n", ntohl(inst->ptr4));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr5:\n");
    if (HAS_ENTRY2(x0D_map, inst->ptr5)) {
        PRINT2(x0D_map, inst->ptr5, d + 2);
    } else {
        printf_d(d + 2, "ptr5 unrecognized: 0x%08X\n", ntohl(inst->ptr5));
        exit(0);
    }

    printf_d(d + 1, "ptr6:");
    if (inst->ptr6 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x12_map, inst->ptr6)) {
            PRINT2(x12_map, inst->ptr6, d + 2);
        } else {
            printf_d(d + 2, "ptr6 unrecognized: 0x%08X\n", ntohl(inst->ptr6));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr7 \x1b[3m(connection [rat])\x1b[0m:");
    if (inst->ptr7 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x23_map, inst->ptr7)) {
            PRINT2(x23_map, inst->ptr7, d + 2);
        } else {
            printf_d(d + 2, "ptr7 unrecognized: 0x%08X\n", ntohl(inst->ptr7));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr8 \x1b[3m(footprint pin)\x1b[0m:");
    if (inst->ptr8 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x08_map, inst->ptr8)) {
            PRINT2(x08_map, inst->ptr8, d + 2);
        } else {
            printf_d(d + 2, "ptr8 unrecognized: 0x%08X\n", ntohl(inst->ptr8));
            exit(0);
        }
    }

    /*
    printf_d(d+1, "previous:");
    if (inst->previous == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY(x32_map, inst->previous)) {
            PRINT(x32_map, inst->previous, d+2);
        } else if (HAS_ENTRY(x07_map, inst->previous)) {
            PRINT(x07_map, inst->previous, d+2);
        } else {
            printf_d(d+2, "previous unrecognized: 0x%08X\n",
                ntohl(inst->previous));
            exit(0);
        }
    }
    */

    printf_d(d + 1, "ptr10:");
    if (inst->ptr10 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x30_map, inst->ptr10)) {
            PRINT2(x30_map, inst->ptr10, d + 2);
        } else if (HAS_ENTRY2(x03_map, inst->ptr10)) {
            PRINT2(x03_map, inst->ptr10, d + 2);
        } else {
            printf_d(d + 2, "ptr10 unrecognized: 0x%08X\n", ntohl(inst->ptr10));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr11:");
    if (inst->ptr11 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x26_map, inst->ptr11)) {
            PRINT2(x26_map, inst->ptr11, d + 2);
        } else {
            printf_d(d + 2, "ptr11 unrecognized: 0x%08X\n", ntohl(inst->ptr11));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x33(const void *untyped_inst, File<version> *fs, const int d) {
    const x33<version> *inst = (const x33<version> *)untyped_inst;
    uint32_t un4 = 0;
    if constexpr (!std::is_same_v<decltype(inst->un4), std::monostate>) {
        un4 = inst->un4;
    }
    printf_d(d,
             "x33: t=0x%04X subtype=0x%02X layer=0x%02X k=0x%08X un2=%08X"
             " un4=%08X"
             " \x1b[2m(%d, %d)\x1b[0m un3=(%08X, %08X)"
             " bb_coords=\x1b[2m(%d, %d, %d, %d)\x1b[0m\n",
             ntohl(inst->t), inst->subtype, inst->layer, ntohl(inst->k),
             ntohl(inst->un2), ntohl(un4), inst->coords[0], inst->coords[1],
             ntohl(inst->un3[0]), ntohl(inst->un3[0]), inst->bb_coords[0],
             inst->bb_coords[1], inst->bb_coords[2], inst->bb_coords[3]);
    print_type_hint(d, "Drill or via?");

    printf_d(d + 1, "ptr1:");
    if (inst->ptr1 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x2B_map.contains(inst->ptr1)) {
            print_struct((const void *)&fs->x2B_map.at(inst->ptr1), fs, d + 2);
        } else if (fs->x04_map.contains(inst->ptr1)) {
            print_struct((const void *)&fs->x04_map.at(inst->ptr1), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr2:");
    if (inst->ptr2 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x33_map.contains(inst->ptr2)) {
            print_struct((const void *)&fs->x33_map.at(inst->ptr2), fs, d + 2);
        } else if (fs->x2D_map.contains(inst->ptr2)) {
            print_struct((const void *)&fs->x2D_map.at(inst->ptr2), fs, d + 2);
        } else if (fs->x05_map.contains(inst->ptr2)) {
            print_struct((const void *)&fs->x05_map.at(inst->ptr2), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr7:");
    if constexpr (std::is_same_v<decltype(inst->ptr7), std::monostate>) {
        printf(" \x1b[2mmonostate\x1b[0m\n");
    } else {
        if (inst->ptr7 == 0) {
            printf(" \x1b[2mnull\x1b[0m\n");
        } else {
            std::printf(" \n");
            if (fs->x2D_map.contains(inst->ptr7)) {
                print_struct((const void *)&fs->x2D_map.at(inst->ptr7), fs,
                             d + 2);
            } else {
                printf_d(d + 2, "ptr7 unrecognized: 0x%08X\n",
                         ntohl(inst->ptr7));
                exit(0);
            }
        }
    }

    printf_d(d + 1, "ptr3:");
    if (inst->ptr3 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x05_map.contains(inst->ptr3)) {
            print_struct((const void *)&fs->x05_map.at(inst->ptr3), fs, d + 2);
        } else if (fs->x09_map.contains(inst->ptr3)) {
            print_struct((const void *)&fs->x09_map.at(inst->ptr3), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr4:");
    if (inst->ptr4 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x1C_map.contains(inst->ptr4)) {
            print_struct((const void *)&fs->x1C_map.at(inst->ptr4), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr4 unrecognized: 0x%08X\n", ntohl(inst->ptr4));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr5:");
    if (inst->ptr5 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (fs->x03_map.contains(inst->ptr5)) {
            print_struct((const void *)&fs->x03_map.at(inst->ptr5), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr5 unrecognized: 0x%08X\n", ntohl(inst->ptr5));
            exit(0);
        }
    }

    printf_d(d + 1, "ptr6:");
    if (inst->ptr6 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf(" -\n");
        if (fs->x26_map.contains(inst->ptr6)) {
            print_struct((const void *)&fs->x26_map.at(inst->ptr6), fs, d + 2);
        } else {
            printf_d(d + 2, "ptr6 unrecognized: 0x%08X\n", ntohl(inst->ptr6));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x34(const void *untyped_inst, File<version> *fs, const int d) {
    const x34<version> *inst = (const x34<version> *)untyped_inst;
    uint32_t un2;
    if constexpr (!std::is_same_v<decltype(inst->un2), std::monostate>) {
        un2 = inst->un2;
    }
    printf_d(d,
             "x34: \x1b[36;3mKeepout/keepin/etc. Region\x1b[0m t=0x%04X "
             "subtype=%02X layer=%d k=0x%08X"
             " bitmask1=%08X un2=%08X %08X\n",
             ntohl(inst->t), inst->subtype, inst->layer, ntohl(inst->k),
             ntohl(inst->bitmask1), ntohl(un2), ntohl(inst->un));

    printf_d(d + 1, "ptr1:\n");
    if (HAS_ENTRY2(x28_map, inst->ptr1)) {
        PRINT2(x28_map, inst->ptr1, d + 2);
    } else {
        printf_d(d + 2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
        exit(0);
    }

    printf_d(d + 1, "ptr2:\n");
    if (HAS_ENTRY2(x17_map, inst->ptr2)) {
        PRINT2(x17_map, inst->ptr2, d + 2);
    } else if (HAS_ENTRY2(x16_map, inst->ptr2)) {
        PRINT2(x16_map, inst->ptr2, d + 2);
    } else if (HAS_ENTRY2(x15_map, inst->ptr2)) {
        PRINT2(x15_map, inst->ptr2, d + 2);
    } else if (HAS_ENTRY2(x01_map, inst->ptr2)) {
        PRINT2(x01_map, inst->ptr2, d + 2);
    } else {
        printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(inst->ptr2));
        exit(0);
    }

    printf_d(d + 1, "ptr3:");
    if (inst->ptr3 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x03_map, inst->ptr3)) {
            PRINT2(x03_map, inst->ptr3, d + 2);
        } else {
            printf_d(d + 2, "ptr3 unrecognized: 0x%08X\n", ntohl(inst->ptr3));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x36_x0F(const x36_x0F<version> &x, File<version> *fs, const int d) {
    printf_d(d, "(x0F): k=0x%08X\n", ntohl(x.k));

    for (uint8_t i = 0; i < 3; i++) {
        printf_d(d + 1, "ptrs[%d]:", i);
        if (x.ptrs[i] == 0) {
            printf(" \x1b[2mnull\x1b[0m\n");
        } else {
            std::printf("\n");
            if (HAS_ENTRY2(x2C_map, x.ptrs[i])) {
                PRINT2(x2C_map, x.ptrs[i], d + 2);
            } else {
                printf_d(d + 2, "ptrs[%d] unrecognized: 0x%08X\n", i,
                         ntohl(x.ptrs[i]));
                exit(0);
            }
        }
    }

    printf_d(d + 1, "ptr2:");
    if (x.ptr2 == 0) {
        printf(" \x1b[2mnull\x1b[0m\n");
    } else {
        std::printf("\n");
        if (HAS_ENTRY2(x03_map, x.ptr2)) {
            PRINT2(x03_map, x.ptr2, d + 2);
        } else {
            printf_d(d + 2, "ptr2 unrecognized: 0x%08X\n", ntohl(x.ptr2));
            exit(0);
        }
    }
}

template <AllegroVersion version>
void print_x36(const void *untyped_inst, File<version> *fs, const int d) {
    const x36<version> *inst = (const x36<version> *)untyped_inst;
    printf_d(d, "x36: t=0x%04X c=0x%04X k=0x%08X next=0x%08X size=%d\n",
             ntohs(inst->t), ntohs(inst->c), ntohl(inst->k), ntohl(inst->next),
             inst->size);
    if (false && inst->x0Fs.size() > 0) {
        for (const auto &x : inst->x0Fs) {
            if (x.k != 0) print_x36_x0F(x, fs, d + 1);
        }
    }
}

template <AllegroVersion version>
void print_x37(const void *untyped_inst, File<version> *fs, const int d) {
    const x37<version> *inst = (const x37<version> *)untyped_inst;
    const int MAX_TO_PRINT = 5;
    printf_d(d, "x37: t=0x%08X k=0x%08X %08X \x1b[2m(%d/%d)\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k), ntohl(inst->un2), inst->count,
             inst->capacity);

    /*
    printf_d(d+1, "ptr1:\n");
    if (fs->x37_map->contains(inst->ptr1)) {
        print_struct((const void*)&fs->x37_map->at(inst->ptr1), fs, d+2);
    } else if (fs->x2C_map->contains(inst->ptr1)) {
        print_struct((const void*)&fs->x2C_map->at(inst->ptr1), fs, d+2);
    } else {
        printf_d(d+2, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
        exit(0);
    }
    */

    for (uint32_t i = 0; i < MAX_TO_PRINT && i < inst->count; i++) {
        printf_d(d + 1, "ptr[% 3d] = 0x%08X\n", i, ntohl(inst->ptrs[i]));
        if (fs->x30_map.contains(inst->ptrs[i])) {
            print_struct((const void *)&fs->x30_map.at(inst->ptrs[i]), fs,
                         d + 2);
        } else if (fs->x2D_map.contains(inst->ptrs[i])) {
            print_struct((const void *)&fs->x2D_map.at(inst->ptrs[i]), fs,
                         d + 2);
        } else if (fs->x2C_map.contains(inst->ptrs[i])) {
            print_struct((const void *)&fs->x2C_map.at(inst->ptrs[i]), fs,
                         d + 2);
        } else if (fs->x28_map.contains(inst->ptrs[i])) {
            print_struct((const void *)&fs->x28_map.at(inst->ptrs[i]), fs,
                         d + 2);
        } else if (fs->x24_map.contains(inst->ptrs[i])) {
            print_struct((const void *)&fs->x24_map.at(inst->ptrs[i]), fs,
                         d + 2);
        } else if (fs->x1B_map.contains(inst->ptrs[i])) {
            print_struct((const void *)&fs->x1B_map.at(inst->ptrs[i]), fs,
                         d + 2);
        } else if (fs->x14_map.contains(inst->ptrs[i])) {
            print_struct((const void *)&fs->x14_map.at(inst->ptrs[i]), fs,
                         d + 2);
        } else if (fs->x0C_map.contains(inst->ptrs[i])) {
            print_struct((const void *)&fs->x0C_map.at(inst->ptrs[i]), fs,
                         d + 2);
        }
    }

    if (inst->count > MAX_TO_PRINT) {
        printf_d(d + 1, "⋮\n");
    }
}

template <AllegroVersion version>
void print_x38(const void *untyped_inst, File<version> *fs, const int d) {
    const x38<version> *inst = (const x38<version> *)untyped_inst;
    printf_d(d,
             "x38: \x1b[36;3mFilm\x1b[0m"
             " t=0x%08X k=0x%08X un1=%3d"
             " \x1b[34m\"%s\"\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k), inst->un1,
             x38_layer_name(*inst, fs).c_str());

    // `next` is not interesting
    /*
    if (HAS_ENTRY(x38_map, inst->hdr.next)) {
        PRINT(x38_map, inst->hdr.next, d+1);
    } else if (inst->hdr.next == fs->hdr->un1[42]) {
        printf_d(d+1, "\x1b[33mnext is in header, 0x%08X?\x1b[0m\n",
            ntohl(inst->hdr.next));
    } else {
        printf_d(d+1, "next unrecognized: 0x%08X\n", ntohl(inst->hdr.next));
        exit(0);
    }
    */

    if (HAS_ENTRY2(x39_map, inst->ptr1)) {
        PRINT2(x39_map, inst->ptr1, d + 1);
    } else {
        printf_d(d + 1, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
        exit(0);
    }
}

template <AllegroVersion version>
void print_x39(const void *untyped_inst, File<version> *fs, const int d) {
    const x39<version> *inst = (const x39<version> *)untyped_inst;
    printf_d(d,
             "x39: t=0x%08X k=0x%08X"
             " x=\x1b[2m{%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d"
             " %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d}\x1b[0m\n",
             ntohl(inst->t), ntohl(inst->k), inst->x[0], inst->x[1], inst->x[2],
             inst->x[3], inst->x[4], inst->x[5], inst->x[6], inst->x[7],
             inst->x[8], inst->x[9], inst->x[10], inst->x[11], inst->x[12],
             inst->x[13], inst->x[14], inst->x[15], inst->x[16], inst->x[17],
             inst->x[18], inst->x[19], inst->x[20], inst->x[21]);

    /*
    if (HAS_ENTRY(x38_map, inst->parent)) {
        PRINT(x38_map, inst->parent, d+1);
    } else {
        printf_d(d+1, "parent unrecognized: 0x%08X\n", ntohl(inst->parent));
        exit(0);
    }
    */

    printf_d(d + 1, "[");
    for (auto layer_pair : x39_layers(*inst, fs)) {
        std::printf("(%02X.%3d), ", layer_pair.first, layer_pair.second);
    }
    std::printf("]\n");

    /*
    if (HAS_ENTRY(x3A_map, inst->ptr1)) {
        PRINT(x3A_map, inst->ptr1, d+1);
    } else {
        printf_d(d+1, "ptr1 unrecognized: 0x%08X\n", ntohl(inst->ptr1));
        exit(0);
    }
    */
}

template <AllegroVersion version>
void print_x3A(const void *untyped_inst, File<version> *fs, const int d) {
    const x3A<version> *inst = (const x3A<version> *)untyped_inst;
    printf_d(d, "x3A: t=0x%04X subtype=%02X layer=%d k=0x%08X un=%d\n",
             ntohl(inst->t), inst->subtype, inst->layer, ntohl(inst->k),
             inst->un);

    if (HAS_ENTRY2(x3A_map, inst->next)) {
        PRINT2(x3A_map, inst->next, d + 1);
    } else if (HAS_ENTRY2(x39_map, inst->next)) {
        PRINT2(x39_map, inst->next, d + 1);
    } else {
        printf_d(d + 1, "next unrecognized: 0x%08X\n", ntohl(inst->next));
        exit(0);
    }
}

template <AllegroVersion version>
void print_x3C(const void *untyped_inst, File<version> *fs, const int d) {
    const x3C<version> *inst = (const x3C<version> *)untyped_inst;
    printf_d(d, "x3C: t=0x%08X k=0x%08X\n", ntohl(inst->t), ntohl(inst->k));

    if (HAS_ENTRY2(x32_map, inst->ptrs[0]) &&
        HAS_ENTRY2(x32_map, inst->ptrs[1])) {
        const x32<version> *a = &fs->x32_map.at(inst->ptrs[0]);
        const x32<version> *b = &fs->x32_map.at(inst->ptrs[1]);
        printf_d(d + 1, "\x1b[33mPad connectivity pair\x1b[0m\n");
    } else if (HAS_ENTRY2(x1B_map, inst->ptrs[0]) &&
               HAS_ENTRY2(x1B_map, inst->ptrs[1])) {
        const x1B<version> *a = &fs->x1B_map.at(inst->ptrs[0]);
        const x1B<version> *b = &fs->x1B_map.at(inst->ptrs[1]);
        printf_d(d + 1, "\x1b[33mNet pair\x1b[0m\n");
    }

    int i = 0;
    int non_null = 0;
    for (const auto &ptr : inst->ptrs) {
        printf_d(d + 1, "ptr[% 2d]: 0x%08X\n", i++, ntohl(ptr));
        if (ptr != 0x0) {
            non_null++;
        }
        if (HAS_ENTRY2(x32_map, ptr)) {
            PRINT2(x32_map, ptr, d + 2);
        } else if (HAS_ENTRY2(x2D_map, ptr)) {
            PRINT2(x2D_map, ptr, d + 2);
        } else if (HAS_ENTRY2(x28_map, ptr)) {
            PRINT2(x28_map, ptr, d + 2);
        } else if (HAS_ENTRY2(x22_map, ptr)) {
            PRINT2(x22_map, ptr, d + 2);
        } else if (HAS_ENTRY2(x1B_map, ptr)) {
            PRINT2(x1B_map, ptr, d + 2);
        }
    }
}
