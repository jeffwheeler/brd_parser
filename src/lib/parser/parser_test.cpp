#include "parser.h"

#include <gtest/gtest.h>

#include <iostream>
#include <map>
#include <set>

#include "lib/structure/utils.h"

template <AllegroVersion version>
bool check_overlapping_ids(File<version>& fs) {
    /*
    for (const auto& [k, x01_inst] : fs.x01_map) {
        if (fs.x03_map.count(k) > 0) return true;
        if (fs.x04_map.count(k) > 0) return true;
        if (fs.x05_map.count(k) > 0) return true;
        if (fs.x06_map.count(k) > 0) return true;
        if (fs.x08_map.count(k) > 0) return true;
    }
    return false;
    */
    return false;
}

template <AllegroVersion version>
void check_header_values(File<version>& fs) {
    if (fs.hdr->ll_x04.head != 0) {
        auto x = fs.get_x04(fs.hdr->ll_x04.head);
        uint32_t j = 0, k = 0;
        while (x.next != fs.hdr->ll_x04.tail) {
            EXPECT_TRUE(fs.is_type(x.next, 0x04));
            x = fs.get_x04(x.next);
            j++;
        }
        printf("\n");

        for (auto& i : fs.iter_x04()) {
            k++;
        }

        EXPECT_EQ(j + 1, k);
    }

    if (fs.hdr->ll_x06.head != 0) {
        auto x = fs.get_x06(fs.hdr->ll_x06.head);
        while (x.next != fs.hdr->ll_x06.tail) {
            EXPECT_TRUE(fs.is_type(x.next, 0x06));
            x = fs.get_x06(x.next);
        }
    }
    if (fs.hdr->ll_x0C_2.head != 0) {
        auto& x = fs.x0C_map.at(fs.hdr->ll_x0C_2.head);
        while (x.next != fs.hdr->ll_x0C_2.tail) {
            EXPECT_TRUE(fs.x0C_map.count(x.next) > 0);
            x = fs.x0C_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x0E_x28.head != 0) {
        EXPECT_TRUE(fs.x0E_map.count(fs.hdr->ll_x0E_x28.head) > 0 ||
                    fs.x28_map.count(fs.hdr->ll_x0E_x28.head) > 0);
    }
    if (fs.hdr->ll_x14.head != 0) {
        auto x = fs.get_x14(fs.hdr->ll_x14.head);
        while (x.next != fs.hdr->ll_x14.tail) {
            EXPECT_TRUE(fs.is_type(x.next, 0x14));
            x = fs.get_x14(x.next);
        }
    }
    if (fs.hdr->ll_x1B.head != 0) {
        auto x = fs.get_x1B(fs.hdr->ll_x1B.head);
        // auto& x = fs.x1B_map.at(fs.hdr->ll_x1B.head);
        while (x.next != fs.hdr->ll_x1B.tail) {
            EXPECT_TRUE(fs.is_type(x.next, 0x1B));
            x = fs.get_x1B(x.next);
        }
    }
    if (fs.hdr->ll_x1C.head != 0) {
        auto& x = fs.x1C_map.at(fs.hdr->ll_x1C.head);
        while (x.hdr.next != fs.hdr->ll_x1C.tail) {
            EXPECT_TRUE(fs.x1C_map.count(x.hdr.next) > 0);
            x = fs.x1C_map.at(x.hdr.next);
        }
    }
    if (fs.hdr->ll_x24_x28.head != 0) {
        EXPECT_TRUE(fs.x24_map.count(fs.hdr->ll_x24_x28.head) > 0 ||
                    fs.x28_map.count(fs.hdr->ll_x24_x28.head) > 0);
    }
    if (fs.hdr->ll_x2B.head != 0) {
        auto& x = fs.x2B_map.at(fs.hdr->ll_x2B.head);
        while (x.next != fs.hdr->ll_x2B.tail) {
            EXPECT_TRUE(fs.x2B_map.count(x.next) > 0);
            x = fs.x2B_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x03_x30.head != 0) {
        // EXPECT_TRUE(fs.x03_map.count(fs.hdr->ll_x03.head) > 0);
    }
    if (fs.hdr->ll_x0A_2.head != 0) {
        auto& x = fs.x0A_map.at(fs.hdr->ll_x0A_2.head);
        while (x.next != fs.hdr->ll_x0A_2.tail) {
            EXPECT_TRUE(fs.x0A_map.count(x.next) > 0);
            x = fs.x0A_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x1D_x1E_x1F.head != 0) {
        EXPECT_TRUE(fs.x1D_map.count(fs.hdr->ll_x1D_x1E_x1F.head) > 0 ||
                    fs.x1E_map.count(fs.hdr->ll_x1D_x1E_x1F.head) > 0 ||
                    fs.x1F_map.count(fs.hdr->ll_x1D_x1E_x1F.head) > 0);
    }
    if (fs.hdr->ll_x38.head != 0) {
        auto& x = fs.x38_map.at(fs.hdr->ll_x38.head);
        while (x.next != fs.hdr->ll_x38.tail) {
            EXPECT_TRUE(fs.x38_map.count(x.next) > 0);
            x = fs.x38_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x2C.head != 0) {
        auto& x = fs.x2C_map.at(fs.hdr->ll_x2C.head);
        while (x.next != fs.hdr->ll_x2C.tail) {
            EXPECT_TRUE(fs.x2C_map.count(x.next) > 0);
            x = fs.x2C_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x0C.head != 0) {
        auto& x = fs.x0C_map.at(fs.hdr->ll_x0C.head);
        while (x.next != fs.hdr->ll_x0C.tail) {
            EXPECT_TRUE(fs.x0C_map.count(x.next) > 0);
            x = fs.x0C_map.at(x.next);
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
        auto& x = fs.x0A_map.at(fs.hdr->ll_x0A.head);
        while (x.next != fs.hdr->ll_x0A.tail) {
            EXPECT_TRUE(fs.x0A_map.count(x.next) > 0);
            x = fs.x0A_map.at(x.next);
        }
    }
}

// Magic is 0x00130200
// Allegro 16.0 P006
TEST(ParseFile, Slugs) {
    File<A_174> fs = *parse_file("../../test/data/slugs/slugs_v2_1.brd");

    EXPECT_EQ(fs.layer_count, 4);
    // EXPECT_EQ(fs.x17_map.size(), 2859);
    EXPECT_TRUE(fs.x23_map.count(0x06634ED0) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);

    const auto x30_inst = &fs.x30_map.at(0x065BD3E8);
    const auto x36_x08_inst = font_lookup(x30_inst->font.key, fs);
    EXPECT_EQ(x36_x08_inst->char_height, 5000);
}

// Magic is 0x00130C03
// Allegro 16.3 S021
TEST(ParseFile, Parallella) {
    printf("Parallella 0\n");
    File<A_174> fs =
        *parse_file("../../test/data/parallella/parallella_layout.brd");
    printf("Parallella 1\n");

    EXPECT_EQ(fs.layer_count, 12);
    // EXPECT_EQ(fs.x17_map.size(), 17053);
    EXPECT_TRUE(fs.x23_map.count(0x0B63E710) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);

    const x1C<A_174>* x1C_inst = &fs.x1C_map.at(0x0ACD3EE0);
    EXPECT_EQ(x1C_inst->parts[0].w, 2600);
    EXPECT_EQ(x1C_inst->parts[0].h, 2600);

    const x3C<A_174>* x3C_inst = &fs.x3C_map.at(0x0ACF9230);
    EXPECT_EQ(x3C_inst->ptrs.size(), 2);
    EXPECT_EQ(x3C_inst->ptrs.at(0), 0x0ACCDAC8);

    const auto x30_inst = &fs.x30_map.at(0x0AD336C0);
    const auto x36_x08_inst = font_lookup(x30_inst->font.key, fs);
    EXPECT_EQ(x36_x08_inst->char_height, 12500);
}

// Magic is 0x00131003
// Allegro 16.5-P002
TEST(ParseFile, SmartPlug) {
    File<A_174> fs =
        *parse_file("../../test/data/smart_plug/wireless_plugmeter_v2_0.brd");

    EXPECT_EQ(fs.layer_count, 2);
    // EXPECT_EQ(fs.x17_map.size(), 2762);
    EXPECT_TRUE(fs.x23_map.count(0x0EBB2200) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00131503
// Allegro 16.6 P004
TEST(ParseFile, AvalonParsed) {
    File<A_174> fs = *parse_file("../../test/data/avalon/AVALON.brd");

    EXPECT_EQ(fs.layer_count, 4);
    // EXPECT_EQ(fs.x17_map.size(), 4079);
    EXPECT_TRUE(fs.x23_map.count(0x13E64B30) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00131503
TEST(ParseFile, OpenCellularRf) {
    File<A_174> fs =
        *parse_file("../../test/data/opencellular_rf/FEMA_RF_Board.brd");

    EXPECT_EQ(fs.layer_count, 6);
    // EXPECT_EQ(fs.x17_map.size(), 24624);
    EXPECT_TRUE(fs.is_type(0x035B0010, 0x09));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00131504
// Allegro 17.2 S060 (?)
TEST(ParseFile, EdaV3) {
    File<A_174> fs = *parse_file("../../test/data/eda_v3/EDA-02319-V3-3.brd");

    EXPECT_EQ(fs.layer_count, 4);
    // EXPECT_EQ(fs.x17_map.size(), 3106);
    // EXPECT_TRUE(fs.x03_map.count(0xF2692148) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00131504
// Allegro 17.2 S015 (?)
TEST(ParseFile, OpenCellularGbc) {
    File<A_174> fs = *parse_file(
        "../../test/data/opencellular_gbc/"
        "OpenCellular_Connect-1_GBC_Life-3_Board_v1.1.brd");

    EXPECT_EQ(fs.layer_count, 10);
    // EXPECT_EQ(fs.x17_map.size(), 97199);
    EXPECT_TRUE(fs.x23_map.count(0x163ECF78) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);

    const auto x30_inst = &fs.x30_map.at(0x147E3120);
    const auto x36_x08_inst = font_lookup(x30_inst->font.key, fs);
    EXPECT_EQ(x36_x08_inst->char_height, 75000);
}

// Magic is 0x00131504
// Allegro 17.2 S015 (?)
TEST(ParseFile, OpenCellularSdr) {
    File<A_174> fs = *parse_file(
        "../../test/data/opencellular_sdr/"
        "OC_CONNECT_1_SDR_LIFE-3.brd");

    EXPECT_EQ(fs.layer_count, 10);
    // EXPECT_EQ(fs.x17_map.size(), 31392);
    EXPECT_TRUE(fs.x23_map.count(0x0FD96758) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00131504
TEST(ParseFile, EuroAdc) {
    File<A_174> fs = *parse_file(
        "../../test/data/euro_adc/"
        "pc051c_toplevel_209.brd");

    EXPECT_EQ(fs.layer_count, 6);
    // EXPECT_EQ(fs.x17_map.size(), 19812);
    // EXPECT_TRUE(fs.x03_map.count(0xEC0FA1D0) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140400
// Allegro 17.2 S049
TEST(ParseFile, BeagleBoneAIParsed) {
    File<A_174> fs =
        *parse_file("../../test/data/beaglebone_ai/BeagleBone-AI.brd");

    EXPECT_EQ(fs.layer_count, 12);
    // EXPECT_EQ(fs.x17_map.size(), 18405);
    EXPECT_TRUE(fs.x23_map.count(0x0002814C) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);

    const x1C<A_174>* x1C_inst = &fs.x1C_map.at(0x00002010);
    EXPECT_EQ(x1C_inst->parts[0].w, 23000);
    EXPECT_EQ(x1C_inst->parts[0].h, 43000);

    const x01<A_174> x01_inst = fs.get_x01(0x000278DE);
    EXPECT_EQ(x01_inst.coords[0], -575000);
    EXPECT_EQ(x01_inst.coords[1], 125000);
}

// Magic is 0x00140400
// Allegro 17.2 S028
TEST(ParseFile, RFPowerDivider) {
    File<A_174> fs =
        *parse_file("../../test/data/rf_pwr_div/1-16_Power_Splitter.brd");

    EXPECT_EQ(fs.layer_count, 2);
    // EXPECT_EQ(fs.x17_map.size(), 1900);
    // EXPECT_TRUE(fs.x03_map.count(0x0C190840) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140400
TEST(ParseFile, OpenRex) {
    File<A_174> fs =
        *parse_file("../../test/data/openrex/OpenRex_V1I1_PCB_V172.brd");

    EXPECT_EQ(fs.layer_count, 10);
    // EXPECT_EQ(fs.x17_map.size(), 20174);
    // EXPECT_TRUE(fs.x15_map.count(0x08E10832) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140400
TEST(ParseFile, M1K) {
    File<A_174> fs = *parse_file("../../test/data/m1k/08_037760f.brd");

    EXPECT_EQ(fs.layer_count, 4);
    // EXPECT_EQ(fs.x17_map.size(), 5607);
    EXPECT_TRUE(fs.x23_map.count(0x084A1B4D) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140400
TEST(ParseFile, CreatableMain) {
    File<A_174> fs =
        *parse_file("../../test/data/creatable_main/MAINBOARD_02_20181023.brd");

    EXPECT_EQ(fs.layer_count, 2);
    // EXPECT_EQ(fs.x17_map.size(), 3853);
    // EXPECT_TRUE(fs.x16_map.count(0x0076E830) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140400
// Allegro 17.2 S071
TEST(ParseFile, JtagToDc3) {
    File<A_174> fs = *parse_file(
        "../../test/data/jtag_to_dc3/JTAG_TO_DC3-10P_V10_220511.brd");

    EXPECT_EQ(fs.layer_count, 2);
    // EXPECT_EQ(fs.x17_map.size(), 137);
    // EXPECT_TRUE(fs.x03_map.count(0x00000A6F) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140400
TEST(ParseFile, FmcTlu) {
    File<A_174> fs = *parse_file("../../test/data/fmc_tlu/fmc_tlu_v1f_38.brd");

    EXPECT_EQ(fs.layer_count, 6);
    // EXPECT_EQ(fs.x17_map.size(), 11249);
    EXPECT_TRUE(fs.x23_map.count(0x0001937A) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140900
// Allegro -unreleased* -- per the Gerbers...? 17.4 format?
TEST(ParseFile, MotorEncoderParsed) {
    File<A_174> fs =
        *parse_file("../../test/data/motor_encoder/motor_encoder_brd.brd");

    EXPECT_EQ(fs.layer_count, 2);
    // EXPECT_EQ(fs.x17_map.size(), 410);
    EXPECT_TRUE(fs.x23_map.count(0x00001197) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140902
// Allegro 17.4 S026
TEST(ParseFile, Pc069A) {
    File<A_174> fs = *parse_file(
        "../../test/data/pc069a_toplevel/pc069a_toplevel_13_glossed.brd");

    EXPECT_EQ(fs.layer_count, 6);
    // EXPECT_EQ(fs.x17_map.size(), 4628);
    EXPECT_TRUE(fs.x23_map.count(0x0000CDE7) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140902
// Allegro 17.4 S033
TEST(ParseFile, MtcaInterfaceBoard) {
    File<A_174> fs =
        *parse_file("../../test/data/mtca_interface_board/mib_rev2_v206.brd");

    EXPECT_EQ(fs.layer_count, 10);
    // EXPECT_EQ(fs.x17_map.size(), 16792);
    EXPECT_TRUE(fs.x23_map.count(0x0002ABBC) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);

    const auto x30_inst = &fs.x30_map.at(0x00001CD4);
    const auto x36_x08_inst = font_lookup(x30_inst->font.key, fs);
    EXPECT_EQ(x36_x08_inst->char_height, 127000);
}

// Magic is 0x00140E00
// Allegro 17.4 S017
TEST(ParseFile, PllFmHw) {
    File<A_174> fs = *parse_file("../../test/data/pll_fm_hw/FMDEMOD-Long.brd");

    EXPECT_EQ(fs.layer_count, 10);
    // EXPECT_EQ(fs.x17_map.size(), 20619);
    // EXPECT_TRUE(fs.x03_map.count(0x0003096A) > 0);

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}
