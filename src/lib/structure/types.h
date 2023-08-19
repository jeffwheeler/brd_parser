#ifndef TYPES_H
#define TYPES_H

#include <array>
#include <boost/interprocess/mapped_region.hpp>
#include <cstddef>
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "cadence_fp.h"

using namespace boost::interprocess;

#define HAS_ENTRY(MAP, KEY) (fs->MAP.count(KEY) > 0)

#if _MSC_VER
#define COND_FIELD(COND, T, NAME)                                             \
    [[msvc::no_unique_address]] std::conditional_t<(COND), T, std::monostate> \
        NAME;
#else
#define COND_FIELD(COND, T, NAME) \
    [[no_unique_address]] std::conditional_t<(COND), T, std::monostate> NAME;
#endif

enum AllegroVersion {
    A_160 = 0x00130000,
    A_162 = 0x00130400,
    A_164 = 0x00130C00,
    A_165 = 0x00131000,
    A_166 = 0x00131500,
    A_172 = 0x00140400,
    A_174 = 0x00140900
};

// This alternative to `sizeof` is used where conditional fields are at the end
// of a `struct`. Without a `uint32_t TAIL` at the end, the size is incorrect.
template <typename T>
constexpr size_t sizeof_until_tail() {
    return offsetof(T, TAIL);
}

template <AllegroVersion start, AllegroVersion end,
          template <AllegroVersion> class T>
constexpr T<end> upgrade(const T<start> &a) {
    constexpr uint8_t n = sizeof(T<start>::versions) / sizeof(AllegroVersion);
    if constexpr (n >= 1 && start < T<start>::versions[0]) {
        // Reinterpret the element as T<A_160> so we can use the explicitly-
        // defined conversion function.
        return *reinterpret_cast<const T<A_160> *>(&a);
    }
    if constexpr (n >= 2 && start < T<start>::versions[1]) {
        return *reinterpret_cast<const T<T<start>::versions[0]> *>(&a);
    }
    if constexpr (n >= 3 && start < T<start>::versions[2]) {
        return *reinterpret_cast<const T<T<start>::versions[1]> *>(&a);
    }
    return *reinterpret_cast<const T<end> *>(&a);
}

template <AllegroVersion start, AllegroVersion end,
          template <AllegroVersion> class T>
constexpr T<end> new_upgrade(void *x) {
    T<start> &a = *static_cast<T<start> *>(x);
    T<end> t;
    constexpr uint8_t n = sizeof(T<start>::versions) / sizeof(AllegroVersion);
    if constexpr (n >= 1 && start < T<start>::versions[0]) {
        // Reinterpret the element as T<A_160> so we can use the explicitly-
        // defined conversion function.
        t = *reinterpret_cast<const T<A_160> *>(&a);
        return t;
    }
    if constexpr (n >= 2 && start < T<start>::versions[1]) {
        t = *reinterpret_cast<const T<T<start>::versions[0]> *>(&a);
        return t;
    }
    if constexpr (n >= 3 && start < T<start>::versions[2]) {
        t = *reinterpret_cast<const T<T<start>::versions[1]> *>(&a);
        return t;
    }

    memcpy(&t, x, sizeof_until_tail<T<end>>());
    return t;
}

// Linked list
struct ll_ptrs {
    uint32_t tail;
    uint32_t head;
};

struct header {
    uint32_t magic;
    uint32_t un1[14];
    ll_ptrs ll_x04;
    ll_ptrs ll_x06;
    ll_ptrs ll_x0C_2;
    ll_ptrs ll_x0E_x28;
    ll_ptrs ll_x14;
    ll_ptrs ll_x1B;
    ll_ptrs ll_x1C;
    ll_ptrs ll_x24_x28;
    ll_ptrs ll_unused_1;
    ll_ptrs ll_x2B;
    ll_ptrs ll_x03_x30;
    ll_ptrs ll_x0A_2;
    ll_ptrs ll_x1D_x1E_x1F;
    ll_ptrs ll_unused_2;
    ll_ptrs ll_x38;
    ll_ptrs ll_x2C;
    ll_ptrs ll_x0C;
    ll_ptrs ll_unused_3;

    // Is there only ever one x35? This points to both the start and end?
    uint32_t x35_start;
    uint32_t x35_end;

    ll_ptrs ll_x36;
    ll_ptrs ll_x21;
    ll_ptrs ll_unused_4;
    ll_ptrs ll_x0A;
    uint32_t un5;
    char allegro_version[60];
    uint32_t un6;
    uint32_t max_key;
    uint32_t un7[13];
};

// BOOST_FUSION_ADAPT_STRUCT(header, magic, un1, allegro_version);

// Instance
template <AllegroVersion version>
struct x07 {
    uint32_t t;
    uint32_t k;
    uint32_t un1;  // Points to another instance

    COND_FIELD(version >= A_172, uint32_t, ptr0);
    COND_FIELD(version >= A_172, uint32_t, un4);
    COND_FIELD(version >= A_172, uint32_t, un2);

    uint32_t ptr1;  // 0x2D

    COND_FIELD(version < A_172, uint32_t, un5);

    uint32_t refdes_string_ref;

    uint32_t ptr2;

    uint32_t ptr3;  // x03 or null

    uint32_t un3;  // Always null?

    uint32_t ptr4;  // x32 or null

    uint32_t TAIL;
    operator x07<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

static_assert(sizeof_until_tail<x07<A_172>>() == 48);
static_assert(sizeof_until_tail<x07<A_174>>() == 48);

// Line segment (with some curve, I think)
template <AllegroVersion version>
struct x01 {
    uint16_t t;
    uint8_t un0;

    // Determines whether the shape extends outwards or cuts into the shape.
    uint8_t subtype;

    uint32_t k;

    uint32_t next;
    uint32_t parent;

    // Some bit mask?
    uint32_t un1;

    // New in 17.2
    COND_FIELD(version >= A_172, uint32_t, un6);

    uint32_t width;

    // Start and end coordinates
    int32_t coords[4];

    cadence_fp x;
    cadence_fp y;
    cadence_fp r;

    int32_t bbox[4];

    uint32_t TAIL;
    operator x01<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

static_assert(sizeof_until_tail<x01<A_160>>() == 80);
static_assert(sizeof_until_tail<x01<A_172>>() == 84);
static_assert(sizeof_until_tail<x01<A_174>>() == 84);

struct x02 {
    uint32_t t;
    uint32_t k;
    uint32_t un[7];
};

struct x03_hdr_subtype {
    uint8_t t;
    uint8_t un3;
    uint16_t size;
};

template <AllegroVersion version>
struct x03 {
    uint32_t t;
    uint32_t k;

    // May point to `x36_x0F` object, among other types?
    uint32_t next;

    COND_FIELD(version >= A_172, uint32_t, un1);
    x03_hdr_subtype subtype;
    COND_FIELD(version >= A_172, uint32_t, un2);

    uint32_t TAIL;
    operator x03<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};

    bool has_str;
    std::string s;
    uint32_t ptr;
};

template <AllegroVersion version>
struct x04 {
    uint32_t t;
    uint32_t k;
    uint32_t next;

    // Points to `x1B`
    uint32_t ptr1;

    // Points to `x05`
    uint32_t ptr2;

    COND_FIELD(version >= A_174, uint32_t, un1);

    uint32_t TAIL;
    operator x04<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_174};
};

// Line (composed of multiple line segments, x01, x15, x16, and x17)
template <AllegroVersion version>
struct x05 {
    uint16_t t;
    uint8_t subtype;
    uint8_t layer;

    uint32_t k;

    uint32_t ptr0;

    // Points to `x04` (net/shape pair)
    uint32_t ptr1;

    uint32_t un1[2];

    // Points to one of: `x04`, `x05`, `0x09`, `x28`, `x2E`, `x32`, `x33` or
    // null.
    uint32_t ptr2[2];

    uint32_t un2;

    // Points to one of: `x04`, `x05`, `0x09`, `x28`, `x2E`, `x32`, `x33`.
    uint32_t ptr3[2];

    COND_FIELD(version >= A_172, uint32_t, un4[3]);

    // Points to one of: `0x01`, `x15, `x16`, `x17`.
    uint32_t first_segment_ptr;

    // Points to instance of `0x03` (nullable).
    uint32_t ptr5;

    // Null
    uint32_t un3;

    uint32_t TAIL;
    operator x05<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

static_assert(sizeof_until_tail<x05<A_160>>() == 60);
static_assert(sizeof_until_tail<x05<A_172>>() == 68);

template <AllegroVersion version>
struct x06 {
    uint32_t t;
    uint32_t k;

    // Pointer to x06
    uint32_t next;

    // Pointer to string
    uint32_t ptr1;

    uint32_t ptr2;

    // Points to instance
    uint32_t ptr3;

    // Points to footprint
    uint32_t ptr4;

    // Points to x08
    uint32_t ptr5;

    // Points to x03, schematic symbol (e.g. RESISTOR)
    uint32_t ptr6;

    // Added in 17.x?
    COND_FIELD(version >= A_172, uint32_t, un2);

    uint32_t TAIL;
    operator x06<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

static_assert(sizeof_until_tail<x06<A_160>>() == 36);
static_assert(sizeof_until_tail<x06<A_172>>() == 40);

template <AllegroVersion version>
struct x08 {
    uint32_t t;
    uint32_t k;

    // x08?
    COND_FIELD(version >= A_172, uint32_t, ptr1);

    // String
    // Note: String pointers swap position around `ptr2` in different versions.
    COND_FIELD(version < A_172, uint32_t, str_ptr_16x);

    // x06
    uint32_t ptr2;

    // String
    COND_FIELD(version >= A_172, uint32_t, str_ptr);

    // x11
    uint32_t ptr3;

    COND_FIELD(version >= A_172, uint32_t, un1);

    // Can be string, usually null
    uint32_t ptr4;

    uint32_t TAIL;
    operator x08<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

template <AllegroVersion version>
struct x09 {
    uint32_t t;
    uint32_t k;

    // Always null?
    uint32_t un1[4];
    COND_FIELD(version >= A_172, uint32_t, un3);

    // Points to `x28`, `x32`, or `x33` (sometimes null)
    uint32_t ptr1;

    // Points to `x05` or `x09`
    uint32_t ptr2;

    // Always null?
    uint32_t un2;

    // Points to `x28`, `x32`, or `x33
    uint32_t ptr3;

    uint32_t ptr4;

    COND_FIELD(version >= A_174, uint32_t, un4);

    uint32_t TAIL;
    operator x09<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_172, A_174};
};

// DRC, not fully decoded
template <AllegroVersion version>
struct x0A {
    uint16_t t;
    uint8_t subtype;
    uint8_t layer;
    uint32_t k;
    uint32_t next;
    uint32_t un1;  // Always null?
    COND_FIELD(version >= A_172, uint32_t, un2);
    int32_t coords[4];
    uint32_t un4[4];
    uint32_t un5[5];
    COND_FIELD(version >= A_174, uint32_t, un3);

    uint32_t TAIL;
    operator x0A<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_172, A_174};
};

template <AllegroVersion version>
struct x0C {
    uint16_t t;
    uint8_t subtype;
    uint8_t layer;
    uint32_t k;
    uint32_t next;
    COND_FIELD(version >= A_172, uint32_t, un2);
    COND_FIELD(version >= A_172, uint32_t, un3);
    union {
        uint32_t un[11];
        struct {
            uint32_t un1[2];  // Typically (always?) null
            uint32_t kind;
            uint32_t un5;
            int32_t coords[4];
            uint32_t un6[3];
        };
    };
    COND_FIELD(version >= A_174, uint32_t, un4);

    uint32_t TAIL;
    operator x0C<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

template <AllegroVersion version>
struct x0D {
    uint32_t t;
    uint32_t k;
    uint32_t str_ptr;
    uint32_t ptr2;      // Points to a random different `x0D`?
    int32_t coords[2];  // Relative to symbol origin
    COND_FIELD(version >= A_174, uint32_t, un3);
    uint32_t pad_ptr;  // Points to `x1C`
    uint32_t un1;      // Always null?
    COND_FIELD(version >= A_172, uint32_t, un2);
    uint32_t bitmask;
    uint32_t rotation;

    uint32_t TAIL;
    operator x0D<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_172, A_174};
};

template <AllegroVersion version>
struct x0E {
    uint32_t t;
    uint32_t k;
    uint32_t un[13];

    COND_FIELD(version >= A_172, uint32_t[2], un1);

    uint32_t TAIL;
    operator x0E<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

// Footprint
// There's a copy for every instance, not just every kind of footprint.
template <AllegroVersion version>
struct x0F {
    uint32_t t;
    uint32_t k;
    uint32_t ptr1;  // Refers to `G1`, `G2`, `G3`, etc. string...?
    char s[32];
    uint32_t ptr2;  // Points to x06
    uint32_t ptr3;  // Points to x11
    uint32_t un;    // Always null?

    COND_FIELD(version >= A_172, uint32_t, un2);
    COND_FIELD(version >= A_174, uint32_t, un3);

    uint32_t TAIL;
    operator x0F<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_172, A_174};
};

template <AllegroVersion version>
struct x10 {
    uint32_t t;
    uint32_t k;
    COND_FIELD(version >= A_172, uint32_t, un2);

    // Instance
    uint32_t ptr1;

    COND_FIELD(version >= A_174, uint32_t, un3);

    uint32_t ptr2;

    // Always null?
    uint32_t un1;

    uint32_t ptr3;

    // Points to a footprint (x0F) or so that I can't print right now?
    uint32_t ptr4;

    // Something like:
    // "@beagle_xm_revcb.schematic(sch_1):ins21222351@beagle_d.\tfp410.normal\(chips)"
    // or occasionally like:
    // ".\pstchip.dat"
    // or even like:
    // "L:/hdllib2/library/mechanical/rpv64101#2f02r1/chips/chips.prt"
    uint32_t path_str;

    uint32_t TAIL;
    operator x10<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_172, A_174};
};

template <AllegroVersion version>
struct x11 {
    uint32_t t;
    uint32_t k;

    // String
    uint32_t ptr1;

    // Points to: x11, footprint
    uint32_t ptr2;

    // Points to: x08
    uint32_t ptr3;

    uint32_t un;

    COND_FIELD(version >= A_174, uint32_t, un1);

    uint32_t TAIL;
    operator x11<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_174};
};

struct x12 {
    uint32_t t;
    uint32_t k;

    // x10 or x12
    uint32_t ptr1;

    // x11
    uint32_t ptr2;

    // x32
    uint32_t ptr3;

    uint32_t un[2];
};

template <AllegroVersion version>
struct x14 {
    uint16_t type;
    uint8_t subtype;
    uint8_t layer;

    uint32_t k;
    uint32_t next;

    uint32_t ptr1;

    uint32_t un2;
    COND_FIELD(version >= A_172, uint32_t, un3);

    uint32_t ptr2;

    // Null or sometimes `x03`
    uint32_t ptr3;

    // `x26`
    uint32_t ptr4;

    uint32_t TAIL;
    operator x14<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

// Line segment
template <AllegroVersion version>
struct x15 {
    uint32_t t;
    uint32_t k;

    uint32_t next;
    uint32_t parent;

    // Usually null, sometimes one bit is set
    uint32_t un3;
    COND_FIELD(version >= A_172, uint32_t, un4);

    // Often 0
    uint32_t width;

    int32_t coords[4];

    uint32_t TAIL;
    operator x15<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

// Line segment
template <AllegroVersion version>
struct x16 {
    uint32_t t;
    uint32_t k;

    uint32_t next;
    uint32_t parent;

    // Some bit mask?
    uint32_t bitmask;

    COND_FIELD(version >= A_172, uint32_t, un);

    uint32_t width;
    int32_t coords[4];

    uint32_t TAIL;
    operator x16<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

// Line segment
template <AllegroVersion version>
struct x17 {
    uint32_t t;
    uint32_t k;

    uint32_t next;
    uint32_t parent;

    uint32_t un3;
    COND_FIELD(version >= A_172, uint32_t, un4);

    uint32_t width;
    int32_t coords[4];

    uint32_t TAIL;
    operator x17<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

// A net
template <AllegroVersion version>
struct x1B {
    uint32_t t;
    uint32_t k;

    // Points to another `x1B`
    uint32_t next;

    uint32_t net_name;

    // Null
    uint32_t un2;

    // 17.x?
    COND_FIELD(version >= A_172, uint32_t, un4);

    // Bit mask, not fully decoded
    // 0x0000 0002 = has net name?
    uint32_t type;

    // `x04`
    uint32_t ptr1;
    uint32_t ptr2;

    // `x03`
    // Points to a string like
    // `@\cc256xqfn-em_102612a_added_cap\.schematic(sch_1):aud_fsync_1v8`
    uint32_t path_str_ptr;

    // `x26`
    uint32_t ptr4;

    // `x1E`
    uint32_t model_ptr;

    // Null?
    uint32_t un3[2];

    // `x22`
    uint32_t ptr6;

    uint32_t TAIL;
    operator x1B<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

template <AllegroVersion version>
struct x1C_header {
    uint16_t t;
    uint8_t n;
    uint8_t un9;
    uint32_t k;
    uint32_t next;
    uint32_t pad_str;
    uint32_t un0_0;
    uint32_t un0_1;
    uint32_t pad_path;  // x03
    uint16_t un0_3;
    uint32_t un0_4;
    uint32_t un0_5;
    uint32_t un1;
    uint16_t un2_0;
    uint16_t un2_1;
    uint16_t un3;
    uint16_t size_hint;
    std::array<int32_t, 4> coords2;
    uint32_t un4;
    uint32_t un5;
    COND_FIELD(version >= A_172, uint32_t, un7);
    uint32_t un6;
    COND_FIELD(version == A_165 || version == A_166, uint32_t[8], un8);

    uint32_t TAIL;
    operator x1C_header<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_165, A_172};
};

template <AllegroVersion version>
struct t13 {
    uint32_t str_ptr;  // Often null
    uint32_t t;

    COND_FIELD(version >= A_172, uint32_t, z0);

    int32_t w;
    int32_t h;
    int32_t x2;
    int32_t x3;

    // This should be _after_ `x4`, but conditional fields at the end of
    // the struct are flakey?
    COND_FIELD(version >= A_172, uint32_t, z);

    int32_t x4;

    operator t13<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

static_assert(sizeof(t13<A_160>) == 28);
static_assert(sizeof(t13<A_174>) == 36);

// x1 shows how to draw pads
template <AllegroVersion version>
struct x1C {
    x1C_header<version> hdr;
    std::vector<t13<version>> parts;

    operator x1C<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_165, A_172};
};

template <AllegroVersion version>
struct x1D {
    uint32_t t;
    uint32_t k;
    uint32_t un[3];
    uint16_t size_a;
    uint16_t size_b;

    uint32_t TAIL;
    static constexpr AllegroVersion versions[1] = {A_160};
};

struct x1E_hdr {
    uint32_t t;
    uint32_t k;
    uint32_t un1;

    uint16_t un2;
    uint16_t un3;

    uint32_t str_ptr;

    uint32_t size;
};

// Model info
struct x1E {
    x1E_hdr hdr;
    char *s;
};

template <AllegroVersion version>
struct x1F {
    uint32_t t;
    uint32_t k;
    uint32_t un[4];
    uint16_t un1;
    uint16_t size;

    uint32_t TAIL;
    static constexpr AllegroVersion versions[1] = {A_160};
};

template <AllegroVersion version>
struct x20 {
    uint32_t t;
    uint32_t k;
    uint32_t ptr1;
    uint32_t un[7];

    uint32_t TAIL;
    static constexpr AllegroVersion versions[1] = {A_160};
};

struct x21_header {
    uint16_t t;
    uint16_t r;
    uint32_t size;
    uint32_t k;
};

struct meta_netlist_path {
    x21_header hdr;
    uint32_t un1;
    uint32_t bitmask;
    uint32_t TAIL;
    std::string path;
};

struct stackup_material {
    x21_header hdr;
    uint32_t a;
    uint32_t layer_id;
    char material[20];
    char thickness[20];
    char thermal_conductivity[20];
    char electrical_conductivity[20];
    char d_k[20];
    char kind[20];  // E.g. DIELECTRIC, PLANE, SURFACE, CONDUCTOR
    char d_f[20];
    char unknown[20];
};

template <AllegroVersion version>
struct x22 {
    uint32_t t;
    uint32_t k;
    COND_FIELD(version >= A_172, uint32_t, un1);
    uint32_t un[8];

    uint32_t TAIL;
    operator x22<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

// Connection (rat). Draws a line between two connected pads.
template <AllegroVersion version>
struct x23 {
    uint16_t type;
    uint8_t subtype;
    uint8_t layer;

    uint32_t k;

    // Another `x23`
    uint32_t ptr0;

    uint32_t bitmask[2];

    // Matching placed symbol pad (`x32`)
    uint32_t ptr1;

    // Another `x23`
    uint32_t ptr2;

    // Another `x32`
    uint32_t ptr3;

    int32_t coords[5];

    uint32_t un[4];
    COND_FIELD(version >= A_164, uint32_t[4], un2);
    COND_FIELD(version >= A_174, uint32_t, un1);

    uint32_t TAIL;
    operator x23<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_164, A_174};
};

template <AllegroVersion version>
struct x24 {
    uint32_t t;
    uint32_t k;
    uint32_t un[11];
    COND_FIELD(version >= A_172, uint32_t, un1);

    uint32_t TAIL;
    operator x24<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

template <AllegroVersion version>
struct x26 {
    uint32_t t;
    uint32_t k;

    uint32_t member_ptr;

    COND_FIELD(version >= A_172, uint32_t, un);

    // Points to instance of `x22`, `x2C`, or `x33`.
    // Indicates the group that the member is a member of.
    uint32_t group_ptr;

    // Always null?
    uint32_t const_ptr;

    COND_FIELD(version >= A_174, uint32_t, un1);

    uint32_t TAIL;
    operator x26<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_172, A_174};
};

struct x27 {
    uint32_t t;
    std::unordered_set<uint32_t> keys;
};

// Shape
template <AllegroVersion version>
struct x28 {
    uint16_t type;
    uint8_t subtype;
    uint8_t layer;

    uint32_t k;
    uint32_t un1;

    // Points to one of: header value, `x04`, `x2B`, `x2D`
    uint32_t ptr1;

    // Null?
    uint32_t un2;

    COND_FIELD(version >= A_172, uint32_t[2], un5);

    // Points to `x28`, `x2D`, or `x33`
    uint32_t next;

    // Points to `x05` or `x09` (much more frequently `x09`)
    uint32_t ptr3;

    // Points to `x34`?
    uint32_t ptr4;

    // Line segments that form shape (x01, x15, x16, x17)
    uint32_t first_segment_ptr;

    // Null?
    uint32_t un3;

    // Has a few random bits set?
    uint32_t un4;

    // Points to `x26`, `x2C`
    COND_FIELD(version >= A_172, uint32_t, ptr7);

    // Points to `x03`
    uint32_t ptr6;

    COND_FIELD(version < A_172, uint32_t, ptr7_16x);

    // Bounding box
    int32_t coords[4];

    uint32_t TAIL;
    operator x28<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

struct x2A_hdr {
    uint16_t t;
    uint16_t size;
    // uint32_t k;
};

struct x2A_local_entry {
    std::string s;
    uint32_t suffix;
};

struct x2A_reference_entry {
    uint32_t ptr;
    uint32_t suffix;

    // Always null?
    uint32_t un1;
};

// Layer names
struct x2A {
    x2A_hdr hdr;
    bool references;
    std::vector<x2A_local_entry> local_entries;
    std::vector<x2A_reference_entry> reference_entries;
    uint32_t k;
};

template <AllegroVersion version>
struct x2B {
    uint32_t t;
    uint32_t k;
    uint32_t footprint_string_ref;

    uint32_t un1;  // Always null?

    int32_t coords[4];

    // Points to another x2B object? Just the next item, not interesting.
    uint32_t next;

    // Points to placed symbol (x2D)
    uint32_t ptr2;

    // Points to pad of some type (x0D)?
    uint32_t ptr3;

    // Symbol pad (x32)
    uint32_t ptr4;

    // x14?
    uint32_t ptr5;

    // Usually (but not always) points to footprint file path.
    uint32_t str_ptr;

    // x0E
    uint32_t ptr6;

    // x24
    uint32_t ptr7;

    // x28
    uint32_t ptr8;

    COND_FIELD(version >= A_164, uint32_t, un2);  // Always null?

    // 17.x?
    COND_FIELD(version >= A_172, uint32_t, un3);

    uint32_t TAIL;
    operator x2B<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_164, A_172};
};

// Table?
template <AllegroVersion version>
struct x2C {
    uint32_t t;
    uint32_t k;
    uint32_t next;

    COND_FIELD(version >= A_172, uint32_t, un2);
    COND_FIELD(version >= A_172, uint32_t, un3);
    COND_FIELD(version >= A_172, uint32_t, un4);

    // May be null
    uint32_t string_ptr;

    COND_FIELD(version < A_172, uint32_t, un5);

    // Points to instance of `x37` or `x3C` (or null).
    uint32_t ptr1;

    // Points to instance of `x03` (string or constant)
    uint32_t ptr2;

    // Points to instance of `x26` or `x2C`
    uint32_t ptr3;

    // Often but not always `0x0`.
    uint32_t bitmask;

    uint32_t TAIL;
    operator x2C<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

// Placed symbol
template <AllegroVersion version>
struct x2D {
    uint16_t t;
    uint8_t layer;
    uint8_t un0;

    uint32_t k;
    uint32_t un1;  // Points to x2B or x2D

    // Points to x2B?
    COND_FIELD(version >= A_172, uint32_t, un4);

    // Points to instance (x07) or null
    COND_FIELD(version < A_172, uint32_t, inst_ref_16x);

    uint16_t un2;
    uint16_t un3;

    COND_FIELD(version >= A_172, uint32_t, un5);

    // Bit 0 = part is rotated to non-90 deg angle?
    uint32_t bitmask1;

    uint32_t rotation;
    int32_t coords[2];

    // Points to instance (x07) or null
    COND_FIELD(version >= A_172, uint32_t, inst_ref);

    uint32_t ptr1;  // x14

    uint32_t first_pad_ptr;  // x32

    uint32_t ptr3;  // x03 or x30

    // ptr4[1] = bounding box?
    uint32_t ptr4[3];  // x24 or x28 or x0E

    // x26 instance indicating group membership
    uint32_t group_assignment_ptr;

    uint32_t TAIL;
    operator x2D<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

template <AllegroVersion version>
struct x2E {
    uint32_t t;
    uint32_t k;
    uint32_t un[7];
    COND_FIELD(version >= A_172, uint32_t, un1);

    uint32_t TAIL;
    operator x2E<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

template <AllegroVersion version>
struct x2F {
    uint32_t t;
    uint32_t k;
    uint32_t un[6];

    uint32_t TAIL;
    operator x2F<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_160};
};

// `x30` wraps a string graphic object and includes info like rotation and
// layer.
enum TextAlignment : uint8_t {
    TextAlignLeft = 1,
    TextAlignRight,
    TextAlignCenter
};

enum TextReversal : uint8_t { TextStraight = 0, TextReversed = 1 };

struct TextProperties {
    uint8_t key;
    uint8_t bm1;
    TextAlignment align;
    TextReversal reversed;
};

template <AllegroVersion version>
struct x30 {
    uint16_t type;
    uint8_t subtype;
    uint8_t layer;
    uint32_t k;
    uint32_t next;

    COND_FIELD(version >= A_172, uint32_t, un4);
    COND_FIELD(version >= A_172, uint32_t, un5);
    COND_FIELD(version >= A_172, TextProperties, font);
    COND_FIELD(version >= A_172, uint32_t, ptr3);
    COND_FIELD(version >= A_174, uint32_t, un7);

    // Pointer to string graphic object
    uint32_t str_graphic_ptr;
    uint32_t un1;
    COND_FIELD(version < A_172, TextProperties, font_16x);

    COND_FIELD(version >= A_172, uint32_t, ptr4);

    int32_t coords[2];
    uint32_t un3;

    uint32_t rotation;
    COND_FIELD(version < A_172, uint32_t, ptr3_16x);

    uint32_t TAIL;
    operator x30<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_172, A_174};
};

// String graphic
template <AllegroVersion version>
struct x31 {
    uint16_t t;
    uint8_t subtype;
    uint8_t layer;

    uint32_t k;

    // Points to x30
    uint32_t str_graphic_wrapper_ptr;

    int32_t coords[2];

    uint16_t un;

    uint16_t len;
    COND_FIELD(version >= A_174, uint32_t, un2);

    uint32_t TAIL;
    operator x31<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_174};

    std::string s;
};

// Symbol pins
template <AllegroVersion version>
struct x32 {
    uint16_t type;
    uint8_t subtype;
    uint8_t layer;

    uint32_t k;
    uint32_t un1;

    uint32_t ptr1;

    // Don't look for layer here, seems almost always null.
    uint32_t bitmask1;

    COND_FIELD(version >= A_172, uint32_t, prev);
    uint32_t next;

    uint32_t ptr3;
    uint32_t ptr4;
    uint32_t ptr5;
    uint32_t ptr6;
    uint32_t ptr7;
    uint32_t ptr8;

    uint32_t previous;

    COND_FIELD(version >= A_172, uint32_t, un2);

    uint32_t ptr10;
    uint32_t ptr11;

    int32_t coords[4];

    uint32_t TAIL;
    operator x32<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

template <AllegroVersion version>
struct x33 {
    uint16_t t;
    uint8_t subtype;
    uint8_t layer;

    uint32_t k;
    uint32_t un1;

    uint32_t ptr1;

    // Some bit field?
    uint32_t un2;
    COND_FIELD(version >= A_172, uint32_t, un4);

    uint32_t ptr2;

    COND_FIELD(version >= A_172, uint32_t, ptr7);

    int32_t coords[2];

    // Points to instance of `x05` or `0x09` (or null).
    uint32_t ptr3;

    uint32_t ptr4;

    // Null or pointer to (always empty?) string
    uint32_t ptr5;

    uint32_t ptr6;

    // Occassionally non-zero integers? Maybe bit fields?
    uint32_t un3[2];

    int32_t bb_coords[4];

    uint32_t TAIL;
    operator x33<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

// Keepout/keepin/etc.region
template <AllegroVersion version>
struct x34 {
    uint16_t t;
    uint8_t subtype;
    uint8_t layer;

    uint32_t k;
    uint32_t un1;

    uint32_t ptr1;  // x28
    COND_FIELD(version >= A_172, uint32_t, un2);
    uint32_t bitmask1;
    uint32_t ptr2;  // x01
    uint32_t ptr3;  // x03

    uint32_t un;

    uint32_t TAIL;
    operator x34<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};
};

// List of strings, such as `"OBJECT_INSTANCE"`, `"VIASTRUCTUREID"`,
// `"0x00072768"`, or `"DIFF_IMPEDANCE_RULE"`. No obvious pattern?
//
// Variety of other undecoded fields follow, too.
template <AllegroVersion version>
struct x36_x02 {
    uint8_t str[32];
    uint32_t xs[14];
    COND_FIELD(version >= A_164, uint32_t[3], ys);
    COND_FIELD(version >= A_172, uint32_t[2], zs);

    uint32_t TAIL;
};

// List of strings, such as `"NO_TYPE"`, `"DEFAULT"`, `"16MIL"`, or
// `"ETH_DIFF"`. No obvious pattern?
template <AllegroVersion version>
struct x36_x03 {
    COND_FIELD(version >= A_172, uint8_t[64], str);
    COND_FIELD(version < A_172, uint8_t[32], str_16x);
    COND_FIELD(version >= A_174, uint32_t, un2);

    uint32_t TAIL;
};

template <AllegroVersion version>
struct x36_x06 {
    uint16_t n;
    uint8_t r;
    uint8_t s;
    uint32_t un1;
    COND_FIELD(version < A_172, uint32_t[50], un2);
    uint32_t TAIL;
};

// Font dimension information
template <AllegroVersion version>
struct x36_x08 {
    uint32_t a, b;
    uint32_t char_height;
    uint32_t char_width;
    COND_FIELD(version >= A_174, uint32_t, un2);
    uint32_t xs[4];
    COND_FIELD(version >= A_172, uint32_t[8], ys);

    uint32_t TAIL;
    operator x36_x08<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_172, A_174};
};

template <AllegroVersion version>
struct x36_x0F {
    uint32_t k;

    // Point to `x2C` instances
    uint32_t ptrs[3];

    // Points to `x03` instance
    uint32_t ptr2;
};

template <AllegroVersion version>
struct x36 {
    uint16_t t;
    uint16_t c;
    uint32_t k;
    uint32_t next;
    COND_FIELD(version >= A_172, uint32_t, un1);
    uint32_t size;

    uint32_t count;
    uint32_t last_idx;
    uint32_t un3;

    COND_FIELD(version >= A_174, uint32_t, un2);

    uint32_t TAIL;

    std::vector<x36_x08<version>> x08s;
    std::vector<x36_x0F<version>> x0Fs;

    operator x36<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_172, A_174};
};

template <AllegroVersion version>
struct x37 {
    uint32_t t;
    uint32_t k;
    uint32_t ptr1;
    uint32_t un2;
    uint32_t capacity;
    uint32_t count;
    uint32_t un3;
    uint32_t ptrs[100];
    COND_FIELD(version >= A_174, uint32_t, un4);

    uint32_t TAIL;
    operator x37<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_174};
};

template <AllegroVersion version>
struct x38 {
    uint32_t t;
    uint32_t k;
    uint32_t next;
    uint32_t ptr1;
    COND_FIELD(version < A_166, char[20], layer_name);
    COND_FIELD(version >= A_166, uint32_t, ptr);
    COND_FIELD(version >= A_166, uint32_t, un2);
    uint32_t un1[7];
    COND_FIELD(version >= A_174, uint32_t, un3);

    uint32_t TAIL;
    operator x38<A_174>() const;
    static constexpr AllegroVersion versions[2] = {A_166, A_174};

    std::string s;
};

static_assert(sizeof_until_tail<x38<A_160>>() == 64);
static_assert(sizeof_until_tail<x38<A_165>>() == 64);
static_assert(sizeof_until_tail<x38<A_166>>() == 52);

template <AllegroVersion version>
struct x39 {
    uint32_t t;
    uint32_t k;
    uint32_t parent;
    uint32_t ptr1;
    uint16_t x[22];

    uint32_t TAIL;
    operator x39<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_160};
};

template <AllegroVersion version>
struct x3A {
    uint16_t t;
    uint8_t subtype;
    uint8_t layer;

    uint32_t k;
    uint32_t next;

    uint32_t un;

    COND_FIELD(version >= A_174, uint32_t, un1);

    uint32_t TAIL;
    operator x3A<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_174};
};

// List of SI models
template <AllegroVersion version>
struct x3B {
    uint16_t t;
    uint16_t subtype;

    uint32_t len;
    char name[128];
    char type[32];
    uint32_t un1;
    uint32_t un2;
    COND_FIELD(version >= A_172, uint32_t, un3);

    uint32_t TAIL;
    operator x3B<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_172};

    std::string model_str;
};

// Pair (or very occassionally three-tuple)
// Usually points to two nets (diffpair) or two symbol pads (connected pads)
template <AllegroVersion version>
struct x3C {
    uint32_t t;
    uint32_t k;
    COND_FIELD(version >= A_174, uint32_t, un);
    uint32_t size;

    uint32_t TAIL;
    operator x3C<A_174>() const;
    static constexpr AllegroVersion versions[1] = {A_174};

    std::vector<uint32_t> ptrs;
};

template <AllegroVersion version>
class File {
   public:
    File(mapped_region input_region);

    header *hdr;
    std::vector<std::tuple<uint32_t, uint32_t>> layers;

    std::unordered_map<uint32_t, void *> ptrs;

    std::map<uint32_t, char *> strings;
    std::map<uint32_t, x05<version>> x05_map;
    std::map<uint32_t, x0C<version>> x0C_map;
    std::map<uint32_t, x0D<version>> x0D_map;
    std::map<uint32_t, x0E<version>> x0E_map;
    std::map<uint32_t, x0F<version>> x0F_map;
    std::map<uint32_t, x11<version>> x11_map;
    std::map<uint32_t, x12> x12_map;
    std::map<uint32_t, x1C<version>> x1C_map;
    std::map<uint32_t, x1D<version>> x1D_map;
    std::map<uint32_t, x1E> x1E_map;
    std::map<uint32_t, x1F<version>> x1F_map;
    std::map<uint32_t, x20<version>> x20_map;
    std::map<uint32_t, x22<version>> x22_map;
    std::map<uint32_t, x23<version>> x23_map;
    std::map<uint32_t, x24<version>> x24_map;
    std::map<uint32_t, x26<version>> x26_map;
    x27 x27_db;
    std::map<uint32_t, x28<version>> x28_map;
    std::map<uint32_t, x2A> x2A_map;
    std::map<uint32_t, x2B<version>> x2B_map;
    std::map<uint32_t, x2C<version>> x2C_map;
    std::map<uint32_t, x2D<version>> x2D_map;
    std::map<uint32_t, x2E<version>> x2E_map;
    std::map<uint32_t, x2F<version>> x2F_map;
    std::map<uint32_t, x30<version>> x30_map;
    std::map<uint32_t, x31<version>> x31_map;
    std::map<uint32_t, x32<version>> x32_map;
    std::map<uint32_t, x33<version>> x33_map;
    std::map<uint32_t, x34<version>> x34_map;
    std::map<uint32_t, x36<version>> x36_map;
    std::map<uint32_t, x37<version>> x37_map;
    std::map<uint32_t, x38<version>> x38_map;
    std::map<uint32_t, x39<version>> x39_map;
    std::map<uint32_t, x3A<version>> x3A_map;
    std::map<uint32_t, x3B<version>> x3B_map;
    std::map<uint32_t, x3C<version>> x3C_map;

    std::map<uint32_t, stackup_material> stackup_materials;
    std::optional<meta_netlist_path> netlist_path;

    uint8_t layer_count = 0;
    uint32_t x27_end_pos;

    x01<A_174> get_x01(uint32_t k);
    const x03<A_174> get_x03(uint32_t k);
    const x04<A_174> get_x04(uint32_t k);
    const x06<A_174> get_x06(uint32_t k);
    const x07<A_174> get_x07(uint32_t k);
    const x08<A_174> get_x08(uint32_t k);
    const x09<A_174> get_x09(uint32_t k);
    const x0A<A_174> get_x0A(uint32_t k);
    const x10<A_174> get_x10(uint32_t k);
    const x14<A_174> get_x14(uint32_t k);
    x15<A_174> get_x15(uint32_t k);
    x16<A_174> get_x16(uint32_t k);
    x17<A_174> get_x17(uint32_t k);
    const x1B<A_174> get_x1B(uint32_t k);

    bool is_type(uint32_t k, uint8_t t);

    // This is not done in the constructor because the header hasn't been set
    // yet, so we can't read what magic we are.
    void prepare();

    operator File<A_174>() const;

    template <typename T>
    struct Iter {
       public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using pointer = uint32_t;
        using reference = const T &;

        Iter(File &f, const pointer k,
             value_type (File<version>::*func)(pointer))
            : f(f), k(k), func(func){};
        Iter &operator++() {
            reference inst = (f.*func)(k);
            k = inst.next;
            return *this;
        }
        bool operator==(Iter other) const { return k == other.k; }
        bool operator!=(Iter other) const { return !(*this == other); }
        value_type operator*() const { return (f.*func)(k); }

       private:
        File &f;
        value_type (File<version>::*func)(pointer);
        pointer k;
    };

    template <typename T>
    class IterBase {
       public:
        IterBase(Iter<T> begin, Iter<T> end) : _begin(begin), _end(end){};
        Iter<T> begin() { return _begin; }
        Iter<T> end() { return _end; }

       private:
        Iter<T> _begin, _end;
    };

    IterBase<x04<version>> iter_x04() {
        return IterBase<x04<version>>(
            Iter<x04<version>>(*this, this->hdr->ll_x04.head, &File::get_x04),
            Iter<x04<version>>(*this, this->hdr->ll_x04.tail, &File::get_x04));
    };

    IterBase<x06<version>> iter_x06() {
        return IterBase<x06<version>>(
            Iter<x06<version>>(*this, this->hdr->ll_x06.head, &File::get_x06),
            Iter<x06<version>>(*this, this->hdr->ll_x06.tail, &File::get_x06));
    };

    IterBase<x0A<version>> iter_x0A() {
        return IterBase<x0A<version>>(
            Iter<x0A<version>>(*this, this->hdr->ll_x0A.head, &File::get_x0A),
            Iter<x0A<version>>(*this, this->hdr->ll_x0A.tail, &File::get_x0A));
    };

    IterBase<x0A<version>> iter_x0A_2() {
        return IterBase<x0A<version>>(
            Iter<x0A<version>>(*this, this->hdr->ll_x0A_2.head, &File::get_x0A),
            Iter<x0A<version>>(*this, this->hdr->ll_x0A_2.tail,
                               &File::get_x0A));
    };

    IterBase<x14<version>> iter_x14() {
        return IterBase<x14<version>>(
            Iter<x14<version>>(*this, this->hdr->ll_x14.head, &File::get_x14),
            Iter<x14<version>>(*this, this->hdr->ll_x14.tail, &File::get_x14));
    };

    IterBase<x1B<version>> iter_x1B() {
        return IterBase<x1B<version>>(
            Iter<x1B<version>>(*this, this->hdr->ll_x1B.head, &File::get_x1B),
            Iter<x1B<version>>(*this, this->hdr->ll_x1B.tail, &File::get_x1B));
    };

   private:
    mapped_region region;

    void cache_upgrade_funcs();

    x01<A_174> (*x01_upgrade)(void *);
    x03<A_174> (*x03_upgrade)(void *);
    x04<A_174> (*x04_upgrade)(void *);
    x06<A_174> (*x06_upgrade)(void *);
    x07<A_174> (*x07_upgrade)(void *);
    x08<A_174> (*x08_upgrade)(void *);
    x09<A_174> (*x09_upgrade)(void *);
    x0A<A_174> (*x0A_upgrade)(void *);
    x10<A_174> (*x10_upgrade)(void *);
    x14<A_174> (*x14_upgrade)(void *);
    x15<A_174> (*x15_upgrade)(void *);
    x16<A_174> (*x16_upgrade)(void *);
    x17<A_174> (*x17_upgrade)(void *);
    x1B<A_174> (*x1B_upgrade)(void *);
};

#endif
