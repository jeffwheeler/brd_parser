#include <stdio.h>

#include "html.h"
#include "lib/parser/parser.h"
#include "lib/printing/printers.h"
#include "lib/structure/utils.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: brd_cli filename.brd\n");
        exit(1);
    }

    std::string fname = argv[1];
    auto parsed_file = parse_file(argv[1]);
    if (parsed_file.has_value()) {
        stream_file(fname, *parsed_file);
    }

    /*
    ll_ptrs pts = parsed_file->hdr->ll_x03_x30;
    uint32_t k = pts.head;
    // k = ntohl(0x2FA3AD00);
    printf("Chain started at key = 0x %08X, expecting to end at 0x %08X\n",
           ntohl(k), ntohl(pts.tail));
    uint32_t i = 0;
    uint32_t off = 0;
    while (1) {
        i++;
        if (parsed_file->x38_map.count(k) > 0) {
            auto& i = parsed_file->x38_map[k];
            printf("Found x38 w/ key = 0x %08X\n", ntohl(k));
            k = i.next;
        } else if (parsed_file->x36_map.count(k) > 0) {
            auto& i = parsed_file->x36_map[k];
            printf("Found x36 w/ key = 0x %08X\n", ntohl(k));
            k = i.next;
        } else if (parsed_file->x31_map.count(k) > 0) {
            auto& i = parsed_file->x31_map[k];
            printf("Found x31 w/ key = 0x %08X\n", ntohl(k));
            k = i.str_graphic_wrapper_ptr;
        } else if (parsed_file->is_type(k, 0x30)) {
            auto& i = parsed_file->get_x30(k);
            printf("Found x30 w/ key = 0x %08X\n", ntohl(k));
            // print_struct(k, *parsed_file, off);
            k = i.next;
        } else if (parsed_file->is_type(k, 0x32)) {
            auto& i = parsed_file->get_x32(k);
            printf("Found x32 w/ key = 0x %08X\n", ntohl(k));
            k = i.ptr4;
        } else if (parsed_file->is_type(k, 0x33)) {
            auto& i = parsed_file->get_x33(k);
            printf("Found x33 w/ key = 0x %08X\n", ntohl(k));
            k = i.un1;
        } else if (parsed_file->is_type(k, 0x2D)) {
            auto& i = parsed_file->get_x2D(k);
            printf("Found x2D w/ key = 0x %08X\n", ntohl(k));
            // print_struct(k, *parsed_file, off);
            k = i.next;
        } else if (parsed_file->is_type(k, 0x2C)) {
            auto& i = parsed_file->get_x2C(k);
            printf("Found x2C w/ key = 0x %08X\n", ntohl(k));
            k = i.next;
        } else if (parsed_file->is_type(k, 0x2B)) {
            auto& i = parsed_file->get_x2B(k);
            printf("Found x2B w/ key = 0x %08X\n", ntohl(k));
            k = i.next;
        } else if (parsed_file->is_type(k, 0x28)) {
            auto& i = parsed_file->get_x28(k);
            printf("Found x28 w/ key = 0x %08X\n", ntohl(k));
            k = i.ptr1;
        } else if (parsed_file->is_type(k, 0x24)) {
            auto& i = parsed_file->get_x24(k);
            printf("Found x24 w/ key = 0x %08X\n", ntohl(k));
            k = i.un[0];
        } else if (parsed_file->x1F_map.count(k) > 0) {
            auto& i = parsed_file->x1F_map[k];
            printf("Found x1F w/ key = 0x %08X\n", ntohl(k));
            k = i.un[0];
        } else if (parsed_file->x1E_map.count(k) > 0) {
            auto& i = parsed_file->x1E_map[k];
            printf("Found x1E w/ key = 0x %08X\n", ntohl(k));
            k = i.hdr.un1;
        } else if (parsed_file->x1D_map.count(k) > 0) {
            auto& i = parsed_file->x1D_map[k];
            printf("Found x1D w/ key = 0x %08X\n", ntohl(k));
            k = i.un[0];
        } else if (parsed_file->x1C_map.count(k) > 0) {
            auto& i = parsed_file->x1C_map[k];
            printf("Found x1C w/ key = 0x %08X\n", ntohl(k));
            k = i.hdr.next;
        } else if (parsed_file->is_type(k, 0x1B)) {
            auto& i = parsed_file->get_x1B(k);
            printf("Found x1B w/ key = 0x %08X (\"%s\")\n", ntohl(k),
                   x1B_net_name(k, &parsed_file.value()));
            k = i.next;
        } else if (parsed_file->is_type(k, 0x14)) {
            auto& i = parsed_file->get_x14(k);
            printf("Found x14 w/ key = 0x %08X\n", ntohl(k));
            k = i.next;
        } else if (parsed_file->is_type(k, 0x0C)) {
            auto& i = parsed_file->get_x0C(k);
            printf("Found x0C w/ key = 0x %08X\n", ntohl(k));
            k = i.next;
        } else if (parsed_file->is_type(k, 0x0A)) {
            auto& i = parsed_file->get_x0A(k);
            printf("Found x0A w/ key = 0x %08X\n", ntohl(k));
            k = i.next;
        } else if (parsed_file->is_type(k, 0x09)) {
            auto& i = parsed_file->get_x09(k);
            printf("Found x09 w/ key = 0x %08X\n", ntohl(k));
            k = i.ptr1;
        } else if (parsed_file->is_type(k, 0x06)) {
            auto& i = parsed_file->get_x06(k);
            printf("Found x06 w/ key = 0x %08X\n", ntohl(k));
            k = i.next;
        } else if (parsed_file->is_type(k, 0x05)) {
            auto& i = parsed_file->get_x05(k);
            printf("Found x05 w/ key = 0x %08X\n", ntohl(k));
            k = i.ptr0;
        } else if (parsed_file->is_type(k, 0x04)) {
            auto& i = parsed_file->get_x04(k);
            printf("Found x04 w/ key = 0x %08X\n", ntohl(k));
            k = i.next;
        } else if (parsed_file->is_type(k, 0x03)) {
            auto& i = parsed_file->get_x03(k);
            printf("Found x03 w/ key = 0x %08X\n", ntohl(k));
            // print_struct(k, *parsed_file, off);
            k = i.next;
        } else {
            printf("Chain ended at key = 0x %08X\n", ntohl(k));
            printf("Matched expected value? %d\n", pts.tail == k);
            printf("i = %d\n", i);
            break;
        }
    }
    */
}
