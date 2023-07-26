#include "parser.h"

#include <gtest/gtest.h>

#include <iostream>
#include <map>
#include <set>

#include "lib/structure/utils.h"

template <AllegroVersion version>
bool check_overlapping_ids(File<version>& fs) {
    for (const auto& [k, x01_inst] : fs.x01_map) {
        if (fs.x03_map.contains(k)) return true;
        if (fs.x04_map.contains(k)) return true;
        if (fs.x05_map.contains(k)) return true;
        if (fs.x06_map.contains(k)) return true;
        if (fs.x08_map.contains(k)) return true;
    }
    return false;
}

template <AllegroVersion version>
void check_header_values(File<version>& fs) {
    if (fs.hdr->ll_x04.head != 0) {
        auto& x = fs.x04_map.at(fs.hdr->ll_x04.head);
        while (x.next != fs.hdr->ll_x04.tail) {
            EXPECT_TRUE(fs.x04_map.contains(x.next));
            x = fs.x04_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x06.head != 0) {
        auto& x = fs.x06_map.at(fs.hdr->ll_x06.head);
        while (x.next != fs.hdr->ll_x06.tail) {
            EXPECT_TRUE(fs.x06_map.contains(x.next));
            x = fs.x06_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x0C_2.head != 0) {
        auto& x = fs.x0C_map.at(fs.hdr->ll_x0C_2.head);
        while (x.next != fs.hdr->ll_x0C_2.tail) {
            EXPECT_TRUE(fs.x0C_map.contains(x.next));
            x = fs.x0C_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x0E_x28.head != 0) {
        EXPECT_TRUE(fs.x0E_map.contains(fs.hdr->ll_x0E_x28.head) ||
                    fs.x28_map.contains(fs.hdr->ll_x0E_x28.head));
    }
    if (fs.hdr->ll_x14.head != 0) {
        auto& x = fs.x14_map.at(fs.hdr->ll_x14.head);
        while (x.next != fs.hdr->ll_x14.tail) {
            EXPECT_TRUE(fs.x14_map.contains(x.next));
            x = fs.x14_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x1B.head != 0) {
        auto& x = fs.x1B_map.at(fs.hdr->ll_x1B.head);
        while (x.next != fs.hdr->ll_x1B.tail) {
            EXPECT_TRUE(fs.x1B_map.contains(x.next));
            x = fs.x1B_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x1C.head != 0) {
        auto& x = fs.x1C_map.at(fs.hdr->ll_x1C.head);
        while (x.hdr.next != fs.hdr->ll_x1C.tail) {
            EXPECT_TRUE(fs.x1C_map.contains(x.hdr.next));
            x = fs.x1C_map.at(x.hdr.next);
        }
    }
    if (fs.hdr->ll_x24_x28.head != 0) {
        EXPECT_TRUE(fs.x24_map.contains(fs.hdr->ll_x24_x28.head) ||
                    fs.x28_map.contains(fs.hdr->ll_x24_x28.head));
    }
    if (fs.hdr->ll_x2B.head != 0) {
        auto& x = fs.x2B_map.at(fs.hdr->ll_x2B.head);
        while (x.next != fs.hdr->ll_x2B.tail) {
            EXPECT_TRUE(fs.x2B_map.contains(x.next));
            x = fs.x2B_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x03.head != 0) {
        EXPECT_TRUE(fs.x03_map.contains(fs.hdr->ll_x03.head));
    }
    if (fs.hdr->ll_x0A_2.head != 0) {
        auto& x = fs.x0A_map.at(fs.hdr->ll_x0A_2.head);
        while (x.next != fs.hdr->ll_x0A_2.tail) {
            EXPECT_TRUE(fs.x0A_map.contains(x.next));
            x = fs.x0A_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x1D_x1E_x1F.head != 0) {
        EXPECT_TRUE(fs.x1D_map.contains(fs.hdr->ll_x1D_x1E_x1F.head) ||
                    fs.x1E_map.contains(fs.hdr->ll_x1D_x1E_x1F.head) ||
                    fs.x1F_map.contains(fs.hdr->ll_x1D_x1E_x1F.head));
    }
    if (fs.hdr->ll_x38.head != 0) {
        auto& x = fs.x38_map.at(fs.hdr->ll_x38.head);
        while (x.next != fs.hdr->ll_x38.tail) {
            EXPECT_TRUE(fs.x38_map.contains(x.next));
            x = fs.x38_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x2C.head != 0) {
        auto& x = fs.x2C_map.at(fs.hdr->ll_x2C.head);
        while (x.next != fs.hdr->ll_x2C.tail) {
            EXPECT_TRUE(fs.x2C_map.contains(x.next));
            x = fs.x2C_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x0C.head != 0) {
        auto& x = fs.x0C_map.at(fs.hdr->ll_x0C.head);
        while (x.next != fs.hdr->ll_x0C.tail) {
            EXPECT_TRUE(fs.x0C_map.contains(x.next));
            x = fs.x0C_map.at(x.next);
        }
    }
    /*
    if (fs.hdr->x35_head != 0) {
        EXPECT_TRUE(fs.x35_map.contains(fs.hdr->x35_head));
    }
    */
    if (fs.hdr->ll_x36.head != 0) {
        auto& x = fs.x36_map.at(fs.hdr->ll_x36.head);
        while (x.next != fs.hdr->ll_x36.tail) {
            EXPECT_TRUE(fs.x36_map.contains(x.next));
            x = fs.x36_map.at(x.next);
        }
    }
    if (fs.hdr->ll_x0A.head != 0) {
        auto& x = fs.x0A_map.at(fs.hdr->ll_x0A.head);
        while (x.next != fs.hdr->ll_x0A.tail) {
            EXPECT_TRUE(fs.x0A_map.contains(x.next));
            x = fs.x0A_map.at(x.next);
        }
    }
}

// Magic is 0x00130200
// Allegro 16.0 P006
TEST(ParseFile, Slugs) {
    File<A_174> fs = *parse_file("../../test/data/slugs/slugs_v2_1.brd");

    EXPECT_EQ(fs.layer_count, 4);
    EXPECT_EQ(fs.x17_map.size(), 2859);
    EXPECT_TRUE(fs.x23_map.contains(0x06634ED0));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);

    const auto x30_inst = &fs.x30_map.at(0x065BD3E8);
    const auto x36_x08_inst = font_lookup(x30_inst->font.key, fs);
    EXPECT_EQ(x36_x08_inst->char_height, 5000);
}

// Magic is 0x00130C03
// Allegro 16.3 S021
TEST(ParseFile, Parallella) {
    File<A_174> fs =
        *parse_file("../../test/data/parallella/parallella_layout.brd");

    EXPECT_EQ(fs.layer_count, 12);
    EXPECT_EQ(fs.x17_map.size(), 17053);
    EXPECT_TRUE(fs.x23_map.contains(0x0B63E710));

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
    EXPECT_EQ(fs.x17_map.size(), 2762);
    EXPECT_TRUE(fs.x23_map.contains(0x0EBB2200));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00131503
// Allegro 16.6 P004
TEST(ParseFile, AvalonParsed) {
    File<A_174> fs = *parse_file("../../test/data/avalon/AVALON.brd");

    EXPECT_EQ(fs.layer_count, 4);
    EXPECT_EQ(fs.x17_map.size(), 4079);
    EXPECT_TRUE(fs.x23_map.contains(0x13E64B30));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00131503
TEST(ParseFile, OpenCellularRf) {
    File<A_174> fs =
        *parse_file("../../test/data/opencellular_rf/FEMA_RF_Board.brd");

    EXPECT_EQ(fs.layer_count, 6);
    EXPECT_EQ(fs.x17_map.size(), 24624);
    EXPECT_TRUE(fs.x09_map.contains(0x035B0010));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00131504
// Allegro 17.2 S060 (?)
TEST(ParseFile, EdaV3) {
    File<A_174> fs = *parse_file("../../test/data/eda_v3/EDA-02319-V3-3.brd");

    EXPECT_EQ(fs.layer_count, 4);
    EXPECT_EQ(fs.x17_map.size(), 3106);
    EXPECT_TRUE(fs.x03_map.contains(0xF2692148));

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
    EXPECT_EQ(fs.x17_map.size(), 97199);
    EXPECT_TRUE(fs.x23_map.contains(0x163ECF78));

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
    EXPECT_EQ(fs.x17_map.size(), 31392);
    EXPECT_TRUE(fs.x23_map.contains(0x0FD96758));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00131504
TEST(ParseFile, EuroAdc) {
    File<A_174> fs = *parse_file(
        "../../test/data/euro_adc/"
        "pc051c_toplevel_209.brd");

    EXPECT_EQ(fs.layer_count, 6);
    EXPECT_EQ(fs.x17_map.size(), 19812);
    EXPECT_TRUE(fs.x03_map.contains(0xEC0FA1D0));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140400
// Allegro 17.2 S049
TEST(ParseFile, BeagleBoneAIParsed) {
    File<A_174> fs =
        *parse_file("../../test/data/beaglebone_ai/BeagleBone-AI.brd");

    EXPECT_EQ(fs.layer_count, 12);
    EXPECT_EQ(fs.x17_map.size(), 18405);
    EXPECT_TRUE(fs.x23_map.contains(0x0002814C));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);

    const x1C<A_174>* x1C_inst = &fs.x1C_map.at(0x00002010);
    EXPECT_EQ(x1C_inst->parts[0].w, 23000);
    EXPECT_EQ(x1C_inst->parts[0].h, 43000);

    const x01<A_174>* x01_inst = &fs.x01_map.at(0x000278DE);
    EXPECT_EQ(x01_inst->coords[0], -575000);
    EXPECT_EQ(x01_inst->coords[1], 125000);
}

// Magic is 0x00140400
// Allegro 17.2 S028
TEST(ParseFile, RFPowerDivider) {
    File<A_174> fs =
        *parse_file("../../test/data/rf_pwr_div/1-16_Power_Splitter.brd");

    EXPECT_EQ(fs.layer_count, 2);
    EXPECT_EQ(fs.x17_map.size(), 1900);
    EXPECT_TRUE(fs.x03_map.contains(0x0C190840));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140400
TEST(ParseFile, OpenRex) {
    File<A_174> fs =
        *parse_file("../../test/data/openrex/OpenRex_V1I1_PCB_V172.brd");

    EXPECT_EQ(fs.layer_count, 10);
    EXPECT_EQ(fs.x17_map.size(), 20174);
    EXPECT_TRUE(fs.x15_map.contains(0x08E10832));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140400
TEST(ParseFile, M1K) {
    File<A_174> fs = *parse_file("../../test/data/m1k/08_037760f.brd");

    EXPECT_EQ(fs.layer_count, 4);
    EXPECT_EQ(fs.x17_map.size(), 5607);
    EXPECT_TRUE(fs.x23_map.contains(0x084A1B4D));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140400
TEST(ParseFile, CreatableMain) {
    File<A_174> fs =
        *parse_file("../../test/data/creatable_main/MAINBOARD_02_20181023.brd");

    EXPECT_EQ(fs.layer_count, 2);
    EXPECT_EQ(fs.x17_map.size(), 3853);
    EXPECT_TRUE(fs.x16_map.contains(0x0076E830));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140400
// Allegro 17.2 S071
TEST(ParseFile, JtagToDc3) {
    File<A_174> fs = *parse_file(
        "../../test/data/jtag_to_dc3/JTAG_TO_DC3-10P_V10_220511.brd");

    EXPECT_EQ(fs.layer_count, 2);
    EXPECT_EQ(fs.x17_map.size(), 137);
    EXPECT_TRUE(fs.x03_map.contains(0x00000A6F));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140400
TEST(ParseFile, FmcTlu) {
    File<A_174> fs = *parse_file("../../test/data/fmc_tlu/fmc_tlu_v1f_38.brd");

    EXPECT_EQ(fs.layer_count, 6);
    EXPECT_EQ(fs.x17_map.size(), 11249);
    EXPECT_TRUE(fs.x23_map.contains(0x0001937A));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140900
// Allegro -unreleased* -- per the Gerbers...? 17.4 format?
TEST(ParseFile, MotorEncoderParsed) {
    File<A_174> fs =
        *parse_file("../../test/data/motor_encoder/motor_encoder_brd.brd");

    EXPECT_EQ(fs.layer_count, 2);
    EXPECT_EQ(fs.x17_map.size(), 410);
    EXPECT_TRUE(fs.x23_map.contains(0x00001197));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140902
// Allegro 17.4 S026
TEST(ParseFile, Pc069A) {
    File<A_174> fs = *parse_file(
        "../../test/data/pc069a_toplevel/pc069a_toplevel_13_glossed.brd");

    EXPECT_EQ(fs.layer_count, 6);
    EXPECT_EQ(fs.x17_map.size(), 4628);
    EXPECT_TRUE(fs.x23_map.contains(0x0000CDE7));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}

// Magic is 0x00140902
// Allegro 17.4 S033
TEST(ParseFile, MtcaInterfaceBoard) {
    File<A_174> fs =
        *parse_file("../../test/data/mtca_interface_board/mib_rev2_v206.brd");

    EXPECT_EQ(fs.layer_count, 10);
    EXPECT_EQ(fs.x17_map.size(), 16792);
    EXPECT_TRUE(fs.x23_map.contains(0x0002ABBC));

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
    EXPECT_EQ(fs.x17_map.size(), 20619);
    EXPECT_TRUE(fs.x03_map.contains(0x0003096A));

    EXPECT_FALSE(check_overlapping_ids(fs));
    check_header_values(fs);
}
