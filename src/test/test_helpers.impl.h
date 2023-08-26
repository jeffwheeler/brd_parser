#include "lib/structure/types.h"

template <AllegroVersion version>
void check_header_values(File<version>& fs) {
    if (fs.hdr->ll_x04.head != 0) {
        // Iterating will crash if the pointers are incorrect
        for (auto& i : fs.iter_x04()) {
        }
    }

    if (fs.hdr->ll_x06.head != 0) {
        for (auto& i : fs.iter_x06()) {
        }
    }

    if (fs.hdr->ll_x0C_2.head != 0) {
        for (auto& i : fs.iter_x0C_2()) {
        }
    }
    if (fs.hdr->ll_x0E_x28.head != 0) {
        EXPECT_TRUE(fs.is_type(fs.hdr->ll_x0E_x28.head, 0x0E) ||
                    fs.is_type(fs.hdr->ll_x0E_x28.head, 0x28));
    }
    if (fs.hdr->ll_x14.head != 0) {
        for (auto& i : fs.iter_x14()) {
        }
    }
    if (fs.hdr->ll_x1B.head != 0) {
        for (auto& i : fs.iter_x1B()) {
        }
    }
    if (fs.hdr->ll_x1C.head != 0) {
        for (auto& i : fs.iter_x1C()) {
        }
    }
    if (fs.hdr->ll_x24_x28.head != 0) {
        EXPECT_TRUE(fs.is_type(fs.hdr->ll_x24_x28.head, 0x24) ||
                    fs.is_type(fs.hdr->ll_x24_x28.head, 0x28));
    }
    for (auto& i_x2B : fs.iter_x2B()) {
        for (auto& i_x2D : fs.iter_x2D(i_x2B.k)) {
            for (auto& i_x30 : fs.iter_x30(i_x2D.k)) {
            }
            for (auto& i_x32 : fs.iter_x32(i_x2D.k)) {
            }
        }
    }
    if (fs.hdr->ll_x03_x30.head != 0) {
        // EXPECT_TRUE(fs.x03_map.count(fs.hdr->ll_x03.head) > 0);
    }
    if (fs.hdr->ll_x0A_2.head != 0) {
        for (auto& i : fs.iter_x0A_2()) {
        }
    }
    if (fs.hdr->ll_x1D_x1E_x1F.head != 0) {
        EXPECT_TRUE(fs.is_type(fs.hdr->ll_x1D_x1E_x1F.head, 0x1D) ||
                    fs.x1E_map.count(fs.hdr->ll_x1D_x1E_x1F.head) > 0 ||
                    fs.is_type(fs.hdr->ll_x1D_x1E_x1F.head, 0x1F));
    }
    if (fs.hdr->ll_x38.head != 0) {
        for (auto& i : fs.iter_x38()) {
        }
    }
    if (fs.hdr->ll_x2C.head != 0) {
        for (auto& i : fs.iter_x2C()) {
        }
    }
    if (fs.hdr->ll_x0C.head != 0) {
        for (auto& i : fs.iter_x0C()) {
        }
    }
    /*
    if (fs.hdr->x35_head != 0) {
        EXPECT_TRUE(fs.x35_map.count(fs.hdr->x35_head) > 0);
    }
    */
    if (fs.hdr->ll_x36.head != 0) {
        auto& x = fs.x36_map.at(fs.hdr->ll_x36.head);
        while (x.next != fs.hdr->ll_x36.tail) {
            EXPECT_TRUE(fs.x36_map.count(x.next) > 0);
            x = fs.x36_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x0A.head != 0) {
        for (auto& i : fs.iter_x0A()) {
        }
    }
}

