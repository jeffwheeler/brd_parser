#include "utils.h"

void print_hex(uint32_t n) { std::printf("0x%X\n", n); }

void log(std::ifstream *f, const char *fmt...) {
    va_list args;
    va_start(args, fmt);

    std::printf("\x1b[35m@ 0x%08X:\x1b[0m ", (uint32_t)f->tellg());
    std::vprintf(fmt, args);

    va_end(args);
}

void log_n_words(std::ifstream *f, uint8_t n) {
    uint32_t x;
    std::cout << " 0x";
    for (; n > 0; n--) {
        f->read((char *)&x, 4);
        if (x == 0) {
            std::printf(" \x1b[30m%02X %02X %02X %02X\x1b[0m", x & 0xFF,
                        (x >> 8) & 0xFF, (x >> 16) & 0xFF, (x >> 24) & 0xFF);
        } else {
            std::printf(" \x1b[4m%02X %02X %02X %02X\x1b[0m", x & 0xFF,
                        (x >> 8) & 0xFF, (x >> 16) & 0xFF, (x >> 24) & 0xFF);
        }
    }
    std::cout << std::endl;
}

void printf_d(const int d, const char *fmt...) {
    va_list args;
    va_start(args, fmt);

    if (d > 0) {
        std::printf("%*s. ", d * 2 - 2, "");
    }
    std::vprintf(fmt, args);

    va_end(args);
}

void print_type_hint(const int d, const char *type) {
    printf_d(d, "\x1b[36;3m%s\x1b[0m\n", type);
}
