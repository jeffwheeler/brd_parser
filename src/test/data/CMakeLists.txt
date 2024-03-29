include(FetchContent)

function(download_brd name)
    set(oneValueArgs URL URL_HASH)
    cmake_parse_arguments(DOWNLOAD_BRD "${options}" "${oneValueArgs}"
                      "${multiValueArgs}" ${ARGN} )

    FetchContent_Declare(
        ${name}
        URL ${DOWNLOAD_BRD_URL}
        URL_HASH ${DOWNLOAD_BRD_URL_HASH}
        DOWNLOAD_NO_EXTRACT TRUE
        SOURCE_DIR ${name}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
    )
endfunction()

# License: MIT (https://github.com/rifaasyari/1-16_RF_PWR_DIV/blob/5316812a7f701bc9aec36a8e75d7528a98e09efb/LICENSE)
download_brd(rf_pwr_div
    URL https://github.com/rifaasyari/1-16_RF_PWR_DIV/raw/5316812a7f701bc9aec36a8e75d7528a98e09efb/1-16_Power_Splitter.brd
    URL_HASH MD5=880273a2a0bc3d0185175a5cfc3ff0a3
)

# License: CC-BY-4.0 (https://github.com/beagleboard/beaglebone-ai/blob/8e5f107ce0716a79e03dc2a995788488bd99c57f/LICENSE)
download_brd(beaglebone_ai
    URL https://github.com/beagleboard/beaglebone-ai/raw/8e5f107ce0716a79e03dc2a995788488bd99c57f/HW/BeagleBone-AI.brd
    URL_HASH MD5=ec0a401266c8e8bf94f6dbf19eb5fafe
)

# License: CC-BY-4.0 (https://openbeagle.org/beagley-ai/beagley-ai/-/blob/c4c30e13adfb64c1feea3d927340288f7a261591/LICENSE)
download_brd(beagle_y
    URL https://openbeagle.org/beagley-ai/beagley-ai/-/raw/main/design/BeagleY-AI_PCB_V1.0_240105.brd
    URL_HASH MD5=adf96848a307076dca3ef09063179fc1
)

# License: CC BY-SA 3.0 US (https://github.com/parallella/parallella-hw/blob/a6bcdf844c56366525bef73d3355c75bba19e3f5/LICENSE)
download_brd(parallella
    URL https://github.com/parallella/parallella-hw/raw/a6bcdf844c56366525bef73d3355c75bba19e3f5/parallella/src/parallella_layout.brd
    URL_HASH MD5=ed1b43d589c25d37572a9768a9c60172
)

# License: GPL 3 (https://github.com/5H3LL3H5/MBED-TOOLS/blob/e043ff43e65f6e347aad80c0fe905da2e212300d/LICENSE)
download_brd(jtag_to_dc3
    URL https://github.com/5H3LL3H5/MBED-TOOLS/raw/e043ff43e65f6e347aad80c0fe905da2e212300d/HARDWARE/JTAG_TO_DC3-10P_V10_220511/PCB/JTAG_TO_DC3-10P_V10_220511.brd
    URL_HASH MD5=9157ae1659657cc44b04e571615fe1ad
)

# License: CC BY 4.0 (https://www.imx6rex.com/open-rex/)
download_brd(openrex
    URL https://github.com/FEDEVEL/board-openrex-in-cadence/raw/c414badae165ed2c09646e117ce1e75943b1257e/OpenRex_V1I1_PCB_V172.brd
    URL_HASH MD5=d4263aa03335a29d70337e05ed5cad4a
)

# License: CERN-OHL-S v2 (https://ohwr.org/openbreath/lungventilator/blob/981682d022c34674210e6027b1b03a9fd64bbaa2/Hardware/Encoder%20Board/allegro/motor_encoder_brd.brd.license)
download_brd(motor_encoder
    URL https://ohwr.org/openbreath/lungventilator/raw/981682d022c34674210e6027b1b03a9fd64bbaa2/Hardware/Encoder%20Board/allegro/motor_encoder_brd.brd
    URL_HASH MD5=a6a2478825ec013681d00ee0cefd7435
)

# License: Public domain/GPL 3 (https://github.com/GeorgeHahn/Avalon/blob/f57052b959ce447afe530130450fa4b140093cba/LICENSE)
download_brd(avalon
    URL https://github.com/GeorgeHahn/Avalon/raw/f57052b959ce447afe530130450fa4b140093cba/hardware/allegro/AVALON.brd
    URL_HASH MD5=3752ff56f401edbb79ead7f7fb35495d
)

# License: Apache 2.0 (https://github.com/makerdiary/Smart-Plug/blob/74a78fb2830ac39d17cca9dabd4c7972945ab7b9/LICENSE)
download_brd(smart_plug
    URL https://github.com/makerdiary/Smart-Plug/raw/74a78fb2830ac39d17cca9dabd4c7972945ab7b9/Hardware/Smart%20Plug%20V2.0/pcb/wireless_plugmeter_v2_0.brd
    URL_HASH MD5=96e0253b4b9d26fe1a87faeb337fe347
)

# License: CC BY-SA 3.0 (https://github.com/greyltc/m1k-cad/blob/70d228a30fc22287ca20a5ae80d324df688d99a9/LICENSE)
download_brd(m1k
    URL https://github.com/greyltc/m1k-cad/raw/70d228a30fc22287ca20a5ae80d324df688d99a9/cadence/08_037760f.brd
    URL_HASH MD5=72e283730a6d29dc6ced6f5df34120af
)

# License: CC BY-SA 3.0 (https://github.com/AteamVentures/CREATABLE_D3/blob/e88341446109f16e9c14426649b1389b68e461ea/Hardware/LICENSE)
download_brd(creatable_main
    URL https://github.com/AteamVentures/CREATABLE_D3/raw/e88341446109f16e9c14426649b1389b68e461ea/Hardware/MainBoard/2nd/MAINBOARD_02_20181023.brd
    URL_HASH MD5=74dc4ba1c81f73ec563c0f13875b3296
)

# License: GPL 3 (https://github.com/ShellAlbert/PLL_FM_HW/blob/675df57f0d35e7e1f66c642d685645b6b2b9daa9/LICENSE)
download_brd(pll_fm_hw
    URL https://github.com/ShellAlbert/PLL_FM_HW/raw/675df57f0d35e7e1f66c642d685645b6b2b9daa9/PCB/FMDEMOD-Long.brd
    URL_HASH MD5=a5bdd25de51943e79d60462fbc8da183
)

# License: CERN-OHL-S v2 (https://github.com/uob-hep-cad/uob-hep-pc069/blob/fac621bf2b0896f75e671eaaf00d4d6714c3e6c6/LICENSE)
download_brd(pc069a_toplevel
    URL https://github.com/uob-hep-cad/uob-hep-pc069/raw/fac621bf2b0896f75e671eaaf00d4d6714c3e6c6/hardware/Cadence/worklib/pc069a_toplevel/physical/pc069a_toplevel_13_glossed.brd
    URL_HASH MD5=414c4168535a572fec7ee22d0c29b87d
)

# License: CERN-OHL-S v2 (https://github.com/uob-hep-cad/uob-hep-pc069/blob/fac621bf2b0896f75e671eaaf00d4d6714c3e6c6/LICENSE)
download_brd(eda_v3
    URL https://github.com/uob-hep-cad/uob-hep-pc069/raw/fac621bf2b0896f75e671eaaf00d4d6714c3e6c6/hardware/Cadence/worklib/pc069a_toplevel/physical/EDA-02319-V3-3.brd
    URL_HASH MD5=3c4983e207a9946307c65d5cf6d87153
)

# License: CERN-OHL-S v2 (https://github.com/uob-hep-cad/uob-hep-pc072/blob/b57c417695d5b9d1d8467a6b60b36175450ccba2/LICENSE)
download_brd(mtca_interface_board
    URL https://github.com/uob-hep-cad/uob-hep-pc072/raw/b57c417695d5b9d1d8467a6b60b36175450ccba2/hardware/Cadence/top/mtca_interface_board_reocc/top/physical/mib_rev2_v206.brd
    URL_HASH MD5=87351b4cf880f4f84f9763837e4e517d
)

# License: CC BY 4.0 (https://github.com/Telecominfraproject/OpenCellular/blob/0d5d7b005327e4378bd5c7fd44d7b8dc5ab796f6/LICENSE-HARDWARE)
download_brd(opencellular_gbc
    URL https://github.com/Telecominfraproject/OpenCellular/raw/0d5d7b005327e4378bd5c7fd44d7b8dc5ab796f6/electronics/GBC/Rotundu_x86/board/OpenCellular_Connect-1_GBC_Life-3_Board_v1.1.brd
    URL_HASH MD5=79ec74387be7ad788326cf1a95174a80
)

# License: CC BY 4.0 (https://github.com/Telecominfraproject/OpenCellular/blob/0d5d7b005327e4378bd5c7fd44d7b8dc5ab796f6/LICENSE-HARDWARE)
download_brd(opencellular_sdr
    URL https://github.com/Telecominfraproject/OpenCellular/raw/0d5d7b005327e4378bd5c7fd44d7b8dc5ab796f6/electronics/radio/SDR/board/oc_connect_1_sdr_life-3_board/OC_CONNECT_1_SDR_LIFE-3.brd
    URL_HASH MD5=8c484559b4b8791bb9c77a4e7b9af045
)

# License: CC BY 4.0 (https://github.com/Telecominfraproject/OpenCellular/blob/0d5d7b005327e4378bd5c7fd44d7b8dc5ab796f6/LICENSE-HARDWARE)
download_brd(opencellular_rf
    URL https://github.com/Telecominfraproject/OpenCellular/raw/0d5d7b005327e4378bd5c7fd44d7b8dc5ab796f6/electronics/front-end/GSM-900-2W/V2/RF_Board/FEMA_RF_Board.brd
    URL_HASH MD5=ab9a23a6a44d514d413f61ac7e5a9669
)

# License: MIT (https://github.com/gottlieb/SLUGS/blob/f812d3ceb9c29a023088e55c0ef57c524110521c/LICENSE)
download_brd(slugs
    URL https://github.com/gottlieb/SLUGS/raw/f812d3ceb9c29a023088e55c0ef57c524110521c/hardware/full_slugs2_and_2_1_with_all_library_pads/SLUGS_V2/allegro_slugs2.1/slugs_v2_1.brd
    URL_HASH MD5=766e4ad0929d8e1b0085e0642249125b
)

# License: CERN-OHL-S v1.2 (https://ohwr.org/project/fmc-mtlu-hw/blob/a01510502f65e309988f4d0f7952dec70d4c4f51/LICENSE)
download_brd(fmc_tlu
    URL https://ohwr.org/project/fmc-mtlu-hw/raw/a01510502f65e309988f4d0f7952dec70d4c4f51/Cadence/worklib/fmc_tlu_toplevel_f/physical/fmc_tlu_v1f_38.brd
    URL_HASH MD5=c0b7bd9af942adbe19cb64ab4ae3d592
)

# License: CERN-HLS-S v1.2 (https://ohwr.org/project/euro-adc-65m-14b-40cha-hw/blob/92ea9f4df160c57dad1d7350ae59c5a1d0c5b970/LICENSE)
download_brd(euro_adc
    URL https://ohwr.org/project/euro-adc-65m-14b-40cha-hw/raw/92ea9f4df160c57dad1d7350ae59c5a1d0c5b970/design_files/worklib/pc051d_puma/physical/pc051c_toplevel_209.brd
    URL_HASH MD5=1df2df7791c730a41c16c1ab0c40ca40
)

FetchContent_MakeAvailable(
    avalon
    beaglebone_ai
    beagle_y
    creatable_main
    eda_v3
    euro_adc
    fmc_tlu
    jtag_to_dc3
    motor_encoder
    mtca_interface_board
    m1k
    opencellular_gbc
    opencellular_rf
    opencellular_sdr
    openrex
    parallella
    pc069a_toplevel
    pll_fm_hw
    rf_pwr_div
    smart_plug
    slugs
)
