#ifndef TYPES_H
#define TYPES_H

#include <array>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/version.hpp>
#if BOOST_VERSION >= 108100
#include <boost/unordered/unordered_flat_map.hpp>
#else
#include <unordered_map>
#endif
#include <cstddef>
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "cadence_fp.h"

using namespace boost::interprocess;

#define HAS_ENTRY(MAP, KEY) (fs->MAP.count(KEY) > 0)

#if _MSC_VER
#define COND_FIELD(COND, T, NAME)                                           \
  [[msvc::no_unique_address]] std::conditional_t<(COND), T, std::monostate> \
      NAME;
#else
#define COND_FIELD(COND, T, NAME) \
  [[no_unique_address]] std::conditional_t<(COND), T, std::monostate> NAME;
#endif

enum AllegroVersion {
  kA160 = 0x00130000,
  kA162 = 0x00130400,
  kA164 = 0x00130C00,
  kA165 = 0x00131000,
  kA166 = 0x00131500,
  kA172 = 0x00140400,
  kA174 = 0x00140900,
  kA175 = 0x00141500,
  kAMax = 0x00150000
};

// This alternative to `sizeof` is used where conditional fields are at the end
// of a `struct`. Without a `uint32_t TAIL` at the end, the size is incorrect.
template <typename T>
constexpr size_t sizeof_until_tail() {
  return offsetof(T, TAIL);
}

template <uint8_t J>
class ExpectRefType {
 public:
  uint32_t value{};

  // Allow this object to be used as if it were a `T` directly.
  operator uint32_t() const { return value; };
  uint8_t expected_type() const { return J; }
};

template <AllegroVersion start, AllegroVersion end,
          template <AllegroVersion> class T>
constexpr T<end> upgrade(const T<start> &a) {
  constexpr uint8_t n = sizeof(T<start>::versions) / sizeof(AllegroVersion);
  if constexpr (n >= 1 && start < T<start>::versions[0]) {
    // Reinterpret the element as T<A_160> so we can use the explicitly-
    // defined conversion function.
    return *reinterpret_cast<const T<kA160> *>(&a);
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
    t = *reinterpret_cast<const T<kA160> *>(&a);
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
  memcpy(static_cast<void *>(&t), x, sizeof_until_tail<T<end>>());
  return t;
}

// Linked list
struct LinkedListPtrs {
  uint32_t tail;
  uint32_t head;
};

enum class Units : uint8_t {
  kImperial = 0x01,
  kMetric = 0x03,
};

struct Header {
  uint32_t magic;
  uint32_t un1[4];
  uint32_t object_count;
  uint32_t un2[9];
  LinkedListPtrs ll_x04;
  LinkedListPtrs ll_x06;
  LinkedListPtrs ll_x0C_2;
  LinkedListPtrs ll_x0E_x28;
  LinkedListPtrs ll_x14;
  LinkedListPtrs ll_x1B;
  LinkedListPtrs ll_x1C;
  LinkedListPtrs ll_x24_x28;
  LinkedListPtrs ll_unused_1;
  LinkedListPtrs ll_x2B;
  LinkedListPtrs ll_x03_x30;
  LinkedListPtrs ll_x0A_2;
  LinkedListPtrs ll_x1D_x1E_x1F;
  LinkedListPtrs ll_unused_2;
  LinkedListPtrs ll_x38;
  LinkedListPtrs ll_x2C;
  LinkedListPtrs ll_x0C;
  LinkedListPtrs ll_unused_3;

  // Is there only ever one x35? This points to both the start and end?
  uint32_t x35_start;
  uint32_t x35_end;

  LinkedListPtrs ll_x36;
  LinkedListPtrs ll_x21;
  LinkedListPtrs ll_unused_4;
  LinkedListPtrs ll_x0A;
  uint32_t un3;
  char allegro_version[60];
  uint32_t un4;
  uint32_t max_key;
  uint32_t un5[17];

  Units units;
  uint8_t un6;
  uint16_t un7;

  uint32_t un8[2];
  uint32_t x27_end_offset;
  uint32_t un9;
  uint32_t strings_count;

  uint32_t un10[53];
  uint32_t unit_divisor;
  uint32_t un11[112];
};

static_assert(offsetof(Header, unit_divisor) == 620);

// Line segment (with some curve, I think)
template <AllegroVersion version>
struct T01ArcSegment {
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
  COND_FIELD(version >= kA172, uint32_t, un6);

  uint32_t width;

  // Start and end coordinates
  int32_t coords[4];

  cadence_fp x;
  cadence_fp y;
  cadence_fp r;

  int32_t bbox[4];

  uint32_t TAIL;
  operator T01ArcSegment<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

static_assert(sizeof_until_tail<T01ArcSegment<kA160>>() == 80);
static_assert(sizeof_until_tail<T01ArcSegment<kA172>>() == 84);
static_assert(sizeof_until_tail<T01ArcSegment<kAMax>>() == 84);

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

  COND_FIELD(version >= kA172, uint32_t, un1);
  x03_hdr_subtype subtype;
  COND_FIELD(version >= kA172, uint32_t, un2);

  uint32_t TAIL;
  operator x03<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};

  bool has_str;
  std::string s;
  uint32_t ptr;
};

template <AllegroVersion version>
struct x04 {
  uint32_t t;
  uint32_t k;
  ExpectRefType<0x04> next;

  // Points to `x1B`
  uint32_t ptr1;
  // ExpectRefType<0x1B> ptr1;

  // Points to `x05`
  uint32_t ptr2;
  // ExpectRefType<0x1B> ptr2;

  COND_FIELD(version >= kA174, uint32_t, un1);

  uint32_t TAIL;
  operator x04<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA174};
};

// Line (composed of multiple line segments, x01, x15, x16, and x17)
template <AllegroVersion version>
struct T05Line {
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

  COND_FIELD(version >= kA172, uint32_t, un4[3]);

  // Points to one of: `0x01`, `x15, `x16`, `x17`.
  uint32_t first_segment_ptr;

  // Points to instance of `0x03` (nullable).
  ExpectRefType<0x03> ptr5;

  // Null
  uint32_t un3;

  uint32_t TAIL;
  operator T05Line<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

static_assert(sizeof_until_tail<T05Line<kA160>>() == 60);
static_assert(sizeof_until_tail<T05Line<kA172>>() == 68);

template <AllegroVersion version>
struct x06 {
  uint32_t t;
  uint32_t k;

  // Pointer to x06
  // uint32_t next;
  ExpectRefType<0x06> next;

  // Pointer to string
  uint32_t ptr1;

  uint32_t ptr2;

  // Points to instance
  ExpectRefType<0x07> ptr3;

  // Points to footprint
  ExpectRefType<0x0F> ptr4;

  // Points to x08
  ExpectRefType<0x08> ptr5;

  // Points to x03, schematic symbol (e.g. RESISTOR)
  ExpectRefType<0x03> ptr6;

  // Added in 17.x?
  COND_FIELD(version >= kA172, uint32_t, un2);

  uint32_t TAIL;
  operator x06<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

static_assert(sizeof_until_tail<x06<kA160>>() == 36);
static_assert(sizeof_until_tail<x06<kA172>>() == 40);

// Instance
template <AllegroVersion version>
struct T07Instance {
  uint32_t t;
  uint32_t k;
  uint32_t un1;  // Points to another instance

  COND_FIELD(version >= kA172, uint32_t, ptr0);
  COND_FIELD(version >= kA172, uint32_t, un4);
  COND_FIELD(version >= kA172, uint32_t, un2);

  ExpectRefType<0x2D> ptr1;

  COND_FIELD(version < kA172, uint32_t, un5);

  uint32_t refdes_string_ref;

  uint32_t ptr2;

  ExpectRefType<0x03> ptr3;

  uint32_t un3;  // Always null?

  ExpectRefType<0x32> ptr4;

  uint32_t TAIL;
  operator T07Instance<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

static_assert(sizeof_until_tail<T07Instance<kA172>>() == 48);
static_assert(sizeof_until_tail<T07Instance<kAMax>>() == 48);

template <AllegroVersion version>
struct x08 {
  uint32_t t;
  uint32_t k;

  // x08?
  COND_FIELD(version >= kA172, uint32_t, ptr1);

  // String
  // Note: String pointers swap position around `ptr2` in different versions.
  COND_FIELD(version < kA172, uint32_t, str_ptr_16x);

  // x06
  uint32_t ptr2;

  // String
  COND_FIELD(version >= kA172, uint32_t, str_ptr);

  // x11
  ExpectRefType<0x11> ptr3;

  COND_FIELD(version >= kA172, uint32_t, un1);

  // Can be string, usually null
  uint32_t ptr4;

  uint32_t TAIL;
  operator x08<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

template <AllegroVersion version>
struct x09 {
  uint32_t t;
  uint32_t k;

  // Always null?
  uint32_t un1[4];
  COND_FIELD(version >= kA172, uint32_t, un3);

  // Points to `x28`, `x32`, or `x33` (sometimes null)
  uint32_t ptr1;

  // Points to `x05` or `x09`
  uint32_t ptr2;

  // Always null?
  uint32_t un2;

  // Points to `x28`, `x32`, or `x33
  uint32_t ptr3;

  uint32_t ptr4;

  COND_FIELD(version >= kA174, uint32_t, un4);

  uint32_t TAIL;
  operator x09<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA172, kA174};
};

// DRC, not fully decoded
template <AllegroVersion version>
struct T0ADRC {
  uint16_t t;
  uint8_t subtype;
  uint8_t layer;
  uint32_t k;
  uint32_t next;
  uint32_t un1;  // Always null?
  COND_FIELD(version >= kA172, uint32_t, un2);
  int32_t coords[4];
  uint32_t un4[4];
  uint32_t un5[5];
  COND_FIELD(version >= kA174, uint32_t, un3);

  uint32_t TAIL;
  operator T0ADRC<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA172, kA174};
};

template <AllegroVersion version>
struct x0C {
  uint16_t t;
  uint8_t subtype;
  uint8_t layer;
  uint32_t k;
  uint32_t next;
  COND_FIELD(version >= kA172, uint32_t, un2);
  COND_FIELD(version >= kA172, uint32_t, un3);
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
  COND_FIELD(version >= kA174, uint32_t, un4);

  uint32_t TAIL;
  operator x0C<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

template <AllegroVersion version>
struct x0D {
  uint32_t t;
  uint32_t k;
  uint32_t str_ptr;
  uint32_t ptr2;      // Points to a random different `x0D`?
  int32_t coords[2];  // Relative to symbol origin
  COND_FIELD(version >= kA174, uint32_t, un3);
  uint32_t pad_ptr;  // Points to `x1C`
  uint32_t un1;      // Always null?
  COND_FIELD(version >= kA172, uint32_t, un2);
  uint32_t bitmask;
  uint32_t rotation;

  uint32_t TAIL;
  operator x0D<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA172, kA174};
};

template <AllegroVersion version>
struct x0E {
  uint32_t t;
  uint32_t k;
  uint32_t un[13];

  COND_FIELD(version >= kA172, uint32_t[2], un1);

  uint32_t TAIL;
  operator x0E<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

// Footprint
// There's a copy for every instance, not just every kind of footprint.
template <AllegroVersion version>
struct T0FFootprint {
  uint32_t t;
  uint32_t k;
  uint32_t ptr1;  // Refers to `G1`, `G2`, `G3`, etc. string...?
  char s[32];
  uint32_t ptr2;  // Points to x06
  uint32_t ptr3;  // Points to x11
  uint32_t un;    // Always null?

  COND_FIELD(version >= kA172, uint32_t, un2);
  COND_FIELD(version >= kA174, uint32_t, un3);

  uint32_t TAIL;
  operator T0FFootprint<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA172, kA174};
};

template <AllegroVersion version>
struct x10 {
  uint32_t t;
  uint32_t k;
  COND_FIELD(version >= kA172, uint32_t, un2);

  // Instance
  uint32_t ptr1;

  COND_FIELD(version >= kA174, uint32_t, un3);

  uint32_t ptr2;

  // Always null?
  uint32_t un1;

  uint32_t ptr3;

  // Points to a footprint (x0F) or so that I can't print right now?
  ExpectRefType<0x0F> ptr4;

  // Something like:
  // "@beagle_xm_revcb.schematic(sch_1):ins21222351@beagle_d.\tfp410.normal\(chips)"
  // or occasionally like:
  // ".\pstchip.dat"
  // or even like:
  // "L:/hdllib2/library/mechanical/rpv64101#2f02r1/chips/chips.prt"
  ExpectRefType<0x03> path_str;

  uint32_t TAIL;
  operator x10<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA172, kA174};
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

  COND_FIELD(version >= kA174, uint32_t, un1);

  uint32_t TAIL;
  operator x11<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA174};
};

template <AllegroVersion version>
struct x12 {
  uint32_t t;
  uint32_t k;

  // x10 or x12
  uint32_t ptr1;

  // x11
  uint32_t ptr2;

  // x32
  uint32_t ptr3;

  uint32_t un0;
  COND_FIELD(version >= kA165, uint32_t, un1);
  COND_FIELD(version >= kA174, uint32_t, un2);

  uint32_t TAIL;
  operator x12<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA165, kA174};
};

template <AllegroVersion version>
struct T14Path {
  uint16_t type;
  uint8_t subtype;
  uint8_t layer;

  uint32_t k;
  ExpectRefType<0x14> next;

  uint32_t ptr1;

  uint32_t un2;
  COND_FIELD(version >= kA172, uint32_t, un3);

  uint32_t ptr2;

  // Null or sometimes `x03`
  ExpectRefType<0x03> ptr3;

  // `x26`
  ExpectRefType<0x26> ptr4;

  uint32_t TAIL;
  operator T14Path<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

// Line segment
template <AllegroVersion version>
struct T15LineSegment {
  uint32_t t;
  uint32_t k;

  uint32_t next;
  uint32_t parent;

  // Usually null, sometimes one bit is set
  uint32_t un3;
  COND_FIELD(version >= kA172, uint32_t, un4);

  // Often 0
  uint32_t width;

  int32_t coords[4];

  uint32_t TAIL;
  operator T15LineSegment<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

// Line segment
template <AllegroVersion version>
struct T16LineSegment {
  uint32_t t;
  uint32_t k;

  uint32_t next;
  uint32_t parent;

  // Some bit mask?
  uint32_t bitmask;

  COND_FIELD(version >= kA172, uint32_t, un);

  uint32_t width;
  int32_t coords[4];

  uint32_t TAIL;
  operator T16LineSegment<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

// Line segment
template <AllegroVersion version>
struct T17LineSegment {
  uint32_t t;
  uint32_t k;

  uint32_t next;
  uint32_t parent;

  uint32_t un3;
  COND_FIELD(version >= kA172, uint32_t, un4);

  uint32_t width;
  int32_t coords[4];

  uint32_t TAIL;
  operator T17LineSegment<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

// A net
template <AllegroVersion version>
struct T1BNet {
  uint32_t t;
  uint32_t k;

  // Points to another `t1B_net`
  ExpectRefType<0x1B> next;

  uint32_t net_name;

  // Null
  uint32_t un2;

  // 17.x?
  COND_FIELD(version >= kA172, uint32_t, un4);

  // Bit mask, not fully decoded
  // 0x0000 0002 = has net name?
  uint32_t type;

  // `x04`
  ExpectRefType<0x04> ptr1;
  uint32_t ptr2;

  // `x03`
  // Points to a string like
  // `@\cc256xqfn-em_102612a_added_cap\.schematic(sch_1):aud_fsync_1v8`
  ExpectRefType<0x03> path_str_ptr;

  // `x26`
  uint32_t ptr4;

  // `x1E`
  uint32_t model_ptr;

  // Null?
  uint32_t un3[2];

  // `x22`
  ExpectRefType<0x22> ptr6;

  uint32_t TAIL;
  operator T1BNet<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

template <AllegroVersion version>
struct t13 {
  uint32_t str_ptr;  // Often null
  uint32_t t;

  COND_FIELD(version >= kA172, uint32_t, z0);

  int32_t w;
  int32_t h;
  int32_t x2;
  int32_t x3;

  // This should be _after_ `x4`, but conditional fields at the end of
  // the struct are flakey?
  COND_FIELD(version >= kA172, uint32_t, z);

  int32_t x4;

  uint32_t TAIL;
  operator t13<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

static_assert(sizeof_until_tail<t13<kA160>>() == 28);
static_assert(sizeof_until_tail<t13<kA174>>() == 36);

// x1C shows how to draw pads
enum PadType : uint8_t {
  ThroughVia = 0,
  Via = 1,
  SmtPin = 2,
  Slot = 4,
  HoleA = 8,  // Maybe? Low confidence
  HoleB = 10  // Maybe?
};

struct PadInfo {
  PadType pad_type : 4;
  uint8_t a : 4;
  uint8_t b;
  uint8_t c;
  uint8_t d;
};

template <AllegroVersion version>
struct T1CPad {
  uint16_t t;
  uint8_t n;
  uint8_t un1;
  uint32_t k;
  uint32_t next;
  uint32_t pad_str;
  uint32_t un2;
  uint32_t un3;
  uint32_t pad_path;
  COND_FIELD(version < kA172, uint32_t[4], un4);
  PadInfo pad_info;
  COND_FIELD(version >= kA172, uint32_t[3], un5);
  COND_FIELD(version < kA172, uint16_t, un6);
  uint16_t layer_count;
  COND_FIELD(version >= kA172, uint16_t, un7);
  uint32_t un8[7];
  COND_FIELD(version >= kA172, uint32_t[28], un9);
  COND_FIELD(version == kA165 || version == kA166, uint32_t[8], un10);

  uint32_t TAIL;
  operator T1CPad<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA165, kA172};

  std::vector<t13<version>> parts;
};

static_assert(sizeof_until_tail<T1CPad<kA164>>() == 20 * 4);
static_assert(sizeof_until_tail<T1CPad<kA165>>() == 28 * 4);
static_assert(sizeof_until_tail<T1CPad<kA172>>() == 47 * 4);

static_assert(offsetof(T1CPad<kA164>, layer_count) == 50);
static_assert(offsetof(T1CPad<kA172>, layer_count) == 44);
static_assert(offsetof(T1CPad<kA164>, pad_info) == 44);
static_assert(offsetof(T1CPad<kA172>, pad_info) == 28);

template <AllegroVersion version>
struct x1D {
  uint32_t t;
  uint32_t k;
  uint32_t un[3];
  uint16_t size_a;
  uint16_t size_b;

  uint32_t TAIL;
  static constexpr AllegroVersion versions[1] = {kA160};
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
  static constexpr AllegroVersion versions[1] = {kA160};
};

template <AllegroVersion version>
struct x20 {
  uint32_t t;
  uint32_t k;
  uint32_t ptr1;
  uint32_t un[7];

  COND_FIELD(version >= kA174, uint32_t[10], un1);

  uint32_t TAIL;
  static constexpr AllegroVersion versions[2] = {kA160, kA174};
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
  COND_FIELD(version >= kA172, uint32_t, un1);
  uint32_t un[8];

  uint32_t TAIL;
  operator x22<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

// Connection (rat). Draws a line between two connected pads.
template <AllegroVersion version>
struct T23Rat {
  uint16_t type;
  uint8_t subtype;
  uint8_t layer;

  uint32_t k;
  uint32_t next;

  uint32_t bitmask[2];

  // Matching placed symbol pad (`x32`)
  // uint32_t ptr1;
  ExpectRefType<0x32> ptr1;

  // Another `x23`
  uint32_t ptr2;

  // Another `x32`
  uint32_t ptr3;

  int32_t coords[5];

  uint32_t un[4];
  COND_FIELD(version >= kA164, uint32_t[4], un2);
  COND_FIELD(version >= kA174, uint32_t, un1);

  uint32_t TAIL;
  operator T23Rat<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA164, kA174};
};

template <AllegroVersion version>
struct T24Rectangle {
  uint16_t type;
  uint8_t subtype;
  uint8_t layer;

  uint32_t k;
  uint32_t next;
  uint32_t ptr1;
  uint32_t un1;
  COND_FIELD(version >= kA172, uint32_t, un2);
  int32_t coords[4];
  ExpectRefType<0x03> ptr2;
  uint32_t un[3];

  uint32_t TAIL;
  operator T24Rectangle<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

static_assert(offsetof(T24Rectangle<kA164>, coords) == 20);
static_assert(offsetof(T24Rectangle<kA172>, coords) == 24);
static_assert(offsetof(T24Rectangle<kA164>, ptr2) == 36);

template <AllegroVersion version>
struct x26 {
  uint32_t t;
  uint32_t k;

  uint32_t member_ptr;

  COND_FIELD(version >= kA172, uint32_t, un);

  // Points to instance of `x22`, `x2C`, or `x33`.
  // Indicates the group that the member is a member of.
  uint32_t group_ptr;

  // Always null?
  uint32_t const_ptr;

  COND_FIELD(version >= kA174, uint32_t, un1);

  uint32_t TAIL;
  operator x26<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA172, kA174};
};

// Shape
template <AllegroVersion version>
struct T28Shape {
  uint16_t type;
  uint8_t subtype;
  uint8_t layer;

  uint32_t k;
  uint32_t next;

  // Points to one of: header value, `x04`, `x2B`, `x2D`
  uint32_t ptr1;

  // Null?
  uint32_t un2;

  COND_FIELD(version >= kA172, uint32_t[2], un5);

  // Points to `x28`, `x2D`, or `x33`
  uint32_t ptr2;

  // Points to `x05` or `x09` (much more frequently `x09`)
  uint32_t ptr3;

  // Points to `x34`?
  ExpectRefType<0x34> ptr4;

  // Line segments that form shape (x01, x15, x16, x17)
  uint32_t first_segment_ptr;

  // Null?
  uint32_t un3;

  // Has a few random bits set?
  uint32_t un4;

  // Points to `x26`, `x2C`
  COND_FIELD(version >= kA172, uint32_t, ptr7);

  // Points to `x03`
  uint32_t ptr6;

  COND_FIELD(version < kA172, uint32_t, ptr7_16x);

  // Bounding box
  int32_t coords[4];

  uint32_t TAIL;
  operator T28Shape<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

struct x2A_hdr {
  uint16_t t;
  uint16_t size;
  // uint32_t k;
};

struct x2A_layer_properties {
  uint8_t has_bot_reference : 1;
  uint8_t has_top_reference : 1;
  uint8_t : 0;  // Skips to next byte
  uint8_t is_power : 1;
  uint8_t is_inner : 1;
  uint8_t is_inner2 : 1;
  uint8_t is_power2 : 1;
  uint8_t : 3;
  uint8_t is_signal : 1;
  uint8_t : 3;
  uint8_t is_top : 1;
  uint8_t is_bot : 1;
  uint8_t : 0;
  uint8_t empty;
};

static_assert(sizeof(x2A_layer_properties) == 4);

struct x2A_local_entry {
  std::string s;
  x2A_layer_properties properties;
};

struct x2A_reference_entry {
  uint32_t ptr;
  x2A_layer_properties properties;

  // Always null?
  uint32_t un1;
};

// Layer names
struct T2ACustomLayer {
  x2A_hdr hdr;
  bool references;
  std::vector<x2A_local_entry> local_entries;
  std::vector<x2A_reference_entry> reference_entries;
  uint32_t k;
};

template <AllegroVersion version>
struct T2BSymbol {
  uint32_t t;
  uint32_t k;
  uint32_t footprint_string_ref;

  uint32_t un1;  // Always null?

  int32_t coords[4];

  // Points to another x2B object? Just the next item, not interesting.
  ExpectRefType<0x2B> next;

  // Points to placed symbol (x2D)
  ExpectRefType<0x2D> ptr2;

  // Points to pad of some type (x0D)?
  ExpectRefType<0x0D> ptr3;

  // Symbol pad (x32)
  ExpectRefType<0x32> ptr4;

  // x14?
  ExpectRefType<0x14> ptr5;

  // Usually (but not always) points to footprint file path.
  uint32_t str_ptr;

  // x0E
  uint32_t ptr6;

  // x24
  uint32_t ptr7;

  // x28
  uint32_t ptr8;

  COND_FIELD(version >= kA164, uint32_t, un2);  // Always null?

  // 17.x?
  COND_FIELD(version >= kA172, uint32_t, un3);

  uint32_t TAIL;
  operator T2BSymbol<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA164, kA172};
};

// Table?
template <AllegroVersion version>
struct x2C {
  uint32_t t;
  uint32_t k;
  uint32_t next;

  COND_FIELD(version >= kA172, uint32_t, un2);
  COND_FIELD(version >= kA172, uint32_t, un3);
  COND_FIELD(version >= kA172, uint32_t, un4);

  // May be null
  uint32_t string_ptr;

  COND_FIELD(version < kA172, uint32_t, un5);

  // Points to instance of `x37` or `x3C` (or null).
  uint32_t ptr1;

  // Points to instance of `x03` (string or constant)
  ExpectRefType<0x03> ptr2;

  // Points to instance of `x26` or `x2C`
  uint32_t ptr3;

  // Often but not always `0x0`.
  uint32_t bitmask;

  uint32_t TAIL;
  operator x2C<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

// Placed symbol
template <AllegroVersion version>
struct T2DSymbolInstance {
  uint16_t t;
  uint8_t layer;
  uint8_t un0;

  uint32_t k;
  uint32_t next;  // Points to x2B or x2D

  // Points to x2B?
  COND_FIELD(version >= kA172, uint32_t, un4);

  // Points to instance (x07) or null
  COND_FIELD(version < kA172, uint32_t, inst_ref_16x);

  uint16_t un2;
  uint16_t un3;

  COND_FIELD(version >= kA172, uint32_t, un5);

  // Bit 0 = part is rotated to non-90 deg angle?
  uint32_t bitmask1;

  uint32_t rotation;
  int32_t coords[2];

  // Points to instance (x07) or null
  COND_FIELD(version >= kA172, uint32_t, inst_ref);

  ExpectRefType<0x14> ptr1;
  ExpectRefType<0x32> first_pad_ptr;

  uint32_t ptr3;  // x03 or x30

  // ptr4[1] = bounding box?
  uint32_t ptr4[3];  // x24 or x28 or x0E

  // x26 instance indicating group membership
  uint32_t group_assignment_ptr;

  uint32_t TAIL;
  operator T2DSymbolInstance<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

static_assert(offsetof(T2DSymbolInstance<kA166>, ptr1) == 36);

template <AllegroVersion version>
struct x2E {
  uint32_t t;
  uint32_t k;
  uint32_t un[7];
  COND_FIELD(version >= kA172, uint32_t, un1);

  uint32_t TAIL;
  operator x2E<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

template <AllegroVersion version>
struct x2F {
  uint32_t t;
  uint32_t k;
  uint32_t un[6];

  uint32_t TAIL;
  operator x2F<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA160};
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
struct T30StringGraphic {
  uint16_t type;
  uint8_t subtype;
  uint8_t layer;
  uint32_t k;
  uint32_t next;

  COND_FIELD(version >= kA172, uint32_t, un4);
  COND_FIELD(version >= kA172, uint32_t, un5);
  COND_FIELD(version >= kA172, TextProperties, font);
  COND_FIELD(version >= kA172, uint32_t, ptr3);
  COND_FIELD(version >= kA174, uint32_t, un7);

  // Pointer to string graphic object
  ExpectRefType<0x31> str_graphic_ptr;
  uint32_t un1;
  COND_FIELD(version < kA172, TextProperties, font_16x);

  COND_FIELD(version >= kA172, uint32_t, ptr4);

  int32_t coords[2];
  uint32_t un3;

  uint32_t rotation;
  COND_FIELD(version < kA172, uint32_t, ptr3_16x);

  uint32_t TAIL;
  operator T30StringGraphic<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA172, kA174};
};

// String graphic
template <AllegroVersion version>
struct T31String {
  uint16_t t;
  uint8_t subtype;
  uint8_t layer;

  uint32_t k;

  // Points to x30
  ExpectRefType<0x30> str_graphic_wrapper_ptr;

  int32_t coords[2];

  uint16_t un;

  uint16_t len;
  COND_FIELD(version >= kA174, uint32_t, un2);

  uint32_t TAIL;
  operator T31String<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA174};

  std::string s;
};

// Symbol pins
template <AllegroVersion version>
struct T32SymbolPin {
  uint16_t type;
  uint8_t subtype;
  uint8_t layer;

  uint32_t k;
  uint32_t un1;

  uint32_t ptr1;

  // Don't look for layer here, seems almost always null.
  uint32_t bitmask1;

  COND_FIELD(version >= kA172, uint32_t, prev);
  uint32_t next;

  uint32_t ptr3;
  uint32_t ptr4;
  uint32_t ptr5;
  uint32_t ptr6;
  uint32_t ptr7;
  uint32_t ptr8;

  uint32_t previous;

  COND_FIELD(version >= kA172, uint32_t, un2);

  uint32_t ptr10;
  uint32_t ptr11;

  int32_t coords[4];

  uint32_t TAIL;
  operator T32SymbolPin<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
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
  COND_FIELD(version >= kA172, uint32_t, un4);

  uint32_t ptr2;

  COND_FIELD(version >= kA172, uint32_t, ptr7);

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
  operator x33<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

// Keepout/keepin/etc.region
template <AllegroVersion version>
struct x34 {
  uint16_t t;
  uint8_t subtype;
  uint8_t layer;

  uint32_t k;
  uint32_t next;

  ExpectRefType<0x28> ptr1;
  COND_FIELD(version >= kA172, uint32_t, un2);
  uint32_t bitmask1;
  ExpectRefType<0x01> ptr2;
  ExpectRefType<0x03> ptr3;

  uint32_t un;

  uint32_t TAIL;
  operator x34<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};
};

// List of strings, such as `"OBJECT_INSTANCE"`, `"VIASTRUCTUREID"`,
// `"0x00072768"`, or `"DIFF_IMPEDANCE_RULE"`. No obvious pattern?
//
// Variety of other undecoded fields follow, too.
template <AllegroVersion version>
struct x36_x02 {
  uint8_t str[32];
  uint32_t xs[14];
  COND_FIELD(version >= kA164, uint32_t[3], ys);
  COND_FIELD(version >= kA172, uint32_t[2], zs);

  uint32_t TAIL;
};

// List of strings, such as `"NO_TYPE"`, `"DEFAULT"`, `"16MIL"`, or
// `"ETH_DIFF"`. No obvious pattern?
template <AllegroVersion version>
struct x36_x03 {
  COND_FIELD(version >= kA172, uint8_t[64], str);
  COND_FIELD(version < kA172, uint8_t[32], str_16x);
  COND_FIELD(version >= kA174, uint32_t, un2);

  uint32_t TAIL;
};

template <AllegroVersion version>
struct x36_x06 {
  uint16_t n;
  uint8_t r;
  uint8_t s;
  uint32_t un1;
  COND_FIELD(version < kA172, uint32_t[50], un2);
  uint32_t TAIL;
};

// Font dimension information
template <AllegroVersion version>
struct x36_x08 {
  uint32_t a, b;
  uint32_t char_height;
  uint32_t char_width;
  COND_FIELD(version >= kA174, uint32_t, un2);
  uint32_t xs[4];
  COND_FIELD(version >= kA172, uint32_t[8], ys);

  uint32_t TAIL;
  operator x36_x08<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA172, kA174};
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
  COND_FIELD(version >= kA172, uint32_t, un1);
  uint32_t size;

  uint32_t count;
  uint32_t last_idx;
  uint32_t un3;

  COND_FIELD(version >= kA174, uint32_t, un2);

  uint32_t TAIL;

  std::vector<x36_x08<version>> x08s;
  std::vector<x36_x0F<version>> x0Fs;

  operator x36<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA172, kA174};
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
  COND_FIELD(version >= kA174, uint32_t, un4);

  uint32_t TAIL;
  operator x37<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA174};
};

template <AllegroVersion version>
struct T38Film {
  uint32_t t;
  uint32_t k;
  uint32_t next;
  uint32_t layer_list;
  COND_FIELD(version < kA166, char[20], film_name);
  COND_FIELD(version >= kA166, uint32_t, layer_name_str);
  COND_FIELD(version >= kA166, uint32_t, un2);
  uint32_t un1[7];
  COND_FIELD(version >= kA174, uint32_t, un3);

  uint32_t TAIL;
  operator T38Film<kAMax>() const;
  static constexpr AllegroVersion versions[2] = {kA166, kA174};

  std::string s;
};

static_assert(sizeof_until_tail<T38Film<kA160>>() == 64);
static_assert(sizeof_until_tail<T38Film<kA165>>() == 64);
static_assert(sizeof_until_tail<T38Film<kA166>>() == 52);

template <AllegroVersion version>
struct T39FilmLayerList {
  uint32_t t;
  uint32_t k;
  uint32_t parent;
  uint32_t head;
  uint16_t x[22];

  uint32_t TAIL;
  operator T39FilmLayerList<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA160};
};

template <AllegroVersion version>
struct T3AFilmLayerListNode {
  uint16_t t;
  uint8_t subtype;
  uint8_t layer;

  uint32_t k;
  uint32_t next;

  uint32_t un;

  COND_FIELD(version >= kA174, uint32_t, un1);

  uint32_t TAIL;
  operator T3AFilmLayerListNode<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA174};
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
  COND_FIELD(version >= kA172, uint32_t, un3);

  uint32_t TAIL;
  operator x3B<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA172};

  std::string model_str;
};

// Pair (or very occassionally three-tuple)
// Usually points to two nets (diffpair) or two symbol pads (connected pads)
template <AllegroVersion version>
struct x3C {
  uint32_t t;
  uint32_t k;
  COND_FIELD(version >= kA174, uint32_t, un);
  uint32_t size;

  uint32_t TAIL;
  operator x3C<kAMax>() const;
  static constexpr AllegroVersion versions[1] = {kA174};

  std::vector<uint32_t> ptrs;
};

template <AllegroVersion version>
class File {
 public:
  File(mapped_region input_region);

  Header *hdr;
  std::vector<std::tuple<uint32_t, uint32_t>> layers;

  // `unordered_flat_map` provides a very significant performance improvement
  // in large designs. Current runtime for a 700 MB file improves from ~1.2s
  // to ~0.5s.
#if BOOST_VERSION >= 108100
  boost::unordered_flat_map<uint32_t, void *> ptrs;
#else
  std::unordered_map<uint32_t, void *> ptrs;
#endif

  std::map<uint32_t, char *> strings;
  std::map<uint32_t, x1E> x1E_map;
  std::map<uint32_t, T2ACustomLayer> x2A_map;
  std::map<uint32_t, x36<version>> x36_map;
  std::map<uint32_t, x3B<version>> x3B_map;

  std::map<uint32_t, stackup_material> stackup_materials;
  std::optional<meta_netlist_path> netlist_path;

  uint8_t layer_count = 0;

  T01ArcSegment<kAMax> get_x01(uint32_t k);
  const x03<kAMax> get_x03(uint32_t k);
  const x04<kAMax> get_x04(uint32_t k);
  const T05Line<kAMax> get_x05(uint32_t k);
  const x06<kAMax> get_x06(uint32_t k);
  const T07Instance<kAMax> get_x07(uint32_t k);
  const x08<kAMax> get_x08(uint32_t k);
  const x09<kAMax> get_x09(uint32_t k);
  const T0ADRC<kAMax> get_x0A(uint32_t k);
  const x0C<kAMax> get_x0C(uint32_t k);
  const x0D<kAMax> get_x0D(uint32_t k);
  const x0E<kAMax> get_x0E(uint32_t k);
  const x10<kAMax> get_x10(uint32_t k);
  const T14Path<kAMax> get_x14(uint32_t k);
  const T15LineSegment<kAMax> get_x15(uint32_t k);
  const T16LineSegment<kAMax> get_x16(uint32_t k);
  const T17LineSegment<kAMax> get_x17(uint32_t k);
  const T1BNet<kAMax> get_x1B(uint32_t k);
  const T1CPad<kAMax> get_x1C(uint32_t k);
  const x1D<kAMax> get_x1D(uint32_t k);
  const x1F<kAMax> get_x1F(uint32_t k);
  const T23Rat<kAMax> get_x23(uint32_t k);
  const T24Rectangle<kAMax> get_x24(uint32_t k);
  const x26<kAMax> get_x26(uint32_t k);
  const T28Shape<kAMax> get_x28(uint32_t k);
  const T2BSymbol<kAMax> get_x2B(uint32_t k);
  const x2C<kAMax> get_x2C(uint32_t k);
  const T2DSymbolInstance<kAMax> get_x2D(uint32_t k);
  const x2E<kAMax> get_x2E(uint32_t k);
  const T30StringGraphic<kAMax> get_x30(uint32_t k);
  const T31String<kAMax> get_x31(uint32_t k);
  const T32SymbolPin<kAMax> get_x32(uint32_t k);
  const x33<kAMax> get_x33(uint32_t k);
  const x34<kAMax> get_x34(uint32_t k);
  const x37<kAMax> get_x37(uint32_t k);
  const T38Film<kAMax> get_t38_film(uint32_t k);
  const T39FilmLayerList<kAMax> get_x39(uint32_t k);
  const T3AFilmLayerListNode<kAMax> get_x3A(uint32_t k);
  const x3C<kAMax> get_x3C(uint32_t k);

  bool is_type(uint32_t k, uint8_t t);

  // This is not done in the constructor because the header hasn't been set
  // yet, so we can't read what magic we are.
  void prepare();

  operator File<kAMax>() const;

  template <typename T>
  struct Iter {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const T;
    using pointer = uint32_t;
    using reference = const T &;

    Iter(File &f, const pointer k, value_type (File<version>::*func)(pointer))
        : f(f), func(func), k(k){};
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

  IterBase<x04<version>> iter_x04(uint32_t i_x1B) {
    auto &i = this->get_x1B(i_x1B);
    if (i.ptr1 == 0) {
      return IterBase<x04<version>>(
          Iter<x04<version>>(*this, i.next, &File::get_x04),
          Iter<x04<version>>(*this, i.next, &File::get_x04));
    } else {
      return IterBase<x04<version>>(
          Iter<x04<version>>(*this, i.ptr1, &File::get_x04),
          Iter<x04<version>>(*this, i.k, &File::get_x04));
    }
  };

  IterBase<x06<version>> iter_x06() {
    return IterBase<x06<version>>(
        Iter<x06<version>>(*this, this->hdr->ll_x06.head, &File::get_x06),
        Iter<x06<version>>(*this, this->hdr->ll_x06.tail, &File::get_x06));
  };

  IterBase<T0ADRC<version>> iter_x0A() {
    return IterBase<T0ADRC<version>>(
        Iter<T0ADRC<version>>(*this, this->hdr->ll_x0A.head, &File::get_x0A),
        Iter<T0ADRC<version>>(*this, this->hdr->ll_x0A.tail, &File::get_x0A));
  };

  IterBase<T0ADRC<version>> iter_x0A_2() {
    return IterBase<T0ADRC<version>>(
        Iter<T0ADRC<version>>(*this, this->hdr->ll_x0A_2.head, &File::get_x0A),
        Iter<T0ADRC<version>>(*this, this->hdr->ll_x0A_2.tail, &File::get_x0A));
  };

  IterBase<x0C<version>> iter_x0C() {
    return IterBase<x0C<version>>(
        Iter<x0C<version>>(*this, this->hdr->ll_x0C.head, &File::get_x0C),
        Iter<x0C<version>>(*this, this->hdr->ll_x0C.tail, &File::get_x0C));
  };

  IterBase<x0C<version>> iter_x0C_2() {
    return IterBase<x0C<version>>(
        Iter<x0C<version>>(*this, this->hdr->ll_x0C_2.head, &File::get_x0C),
        Iter<x0C<version>>(*this, this->hdr->ll_x0C_2.tail, &File::get_x0C));
  };

  IterBase<T14Path<version>> iter_x14() {
    return IterBase<T14Path<version>>(
        Iter<T14Path<version>>(*this, this->hdr->ll_x14.head, &File::get_x14),
        Iter<T14Path<version>>(*this, this->hdr->ll_x14.tail, &File::get_x14));
  };

  IterBase<T14Path<version>> iter_x14(const T2DSymbolInstance<version> &inst) {
    if (inst.ptr1 == 0) {
      return IterBase<T14Path<version>>(
          Iter<T14Path<version>>(*this, inst.k, &File::get_x14),
          Iter<T14Path<version>>(*this, inst.k, &File::get_x14));
    } else {
      return IterBase<T14Path<version>>(
          Iter<T14Path<version>>(*this, inst.ptr1, &File::get_x14),
          Iter<T14Path<version>>(*this, inst.k, &File::get_x14));
    }
  };

  IterBase<T1BNet<version>> iter_t1B_net() {
    if (this->hdr->ll_x1B.head == 0) {
      return IterBase<T1BNet<version>>(
          Iter<T1BNet<version>>(*this, 0, &File::get_x1B),
          Iter<T1BNet<version>>(*this, 0, &File::get_x1B));
    } else {
      return IterBase<T1BNet<version>>(
          Iter<T1BNet<version>>(*this, this->hdr->ll_x1B.head, &File::get_x1B),
          Iter<T1BNet<version>>(*this, this->hdr->ll_x1B.tail, &File::get_x1B));
    }
  };

  IterBase<T1CPad<version>> iter_x1C() {
    return IterBase<T1CPad<version>>(
        Iter<T1CPad<version>>(*this, this->hdr->ll_x1C.head, &File::get_x1C),
        Iter<T1CPad<version>>(*this, this->hdr->ll_x1C.tail, &File::get_x1C));
  };

  IterBase<T2BSymbol<version>> iter_x2B() {
    if (this->hdr->ll_x2B.head == 0) {
      return IterBase<T2BSymbol<version>>(
          Iter<T2BSymbol<version>>(*this, 0, &File::get_x2B),
          Iter<T2BSymbol<version>>(*this, 0, &File::get_x2B));
    } else {
      return IterBase<T2BSymbol<version>>(
          Iter<T2BSymbol<version>>(*this, this->hdr->ll_x2B.head,
                                   &File::get_x2B),
          Iter<T2BSymbol<version>>(*this, this->hdr->ll_x2B.tail,
                                   &File::get_x2B));
    }
  };

  IterBase<x2C<version>> iter_x2C() {
    return IterBase<x2C<version>>(
        Iter<x2C<version>>(*this, this->hdr->ll_x2C.head, &File::get_x2C),
        Iter<x2C<version>>(*this, this->hdr->ll_x2C.tail, &File::get_x2C));
  };

  IterBase<T2DSymbolInstance<version>> iter_x2D(uint32_t i_x2B) {
    auto &i = this->get_x2B(i_x2B);
    if (i.ptr2 == 0) {
      return IterBase<T2DSymbolInstance<version>>(
          Iter<T2DSymbolInstance<version>>(*this, i.k, &File::get_x2D),
          Iter<T2DSymbolInstance<version>>(*this, i.k, &File::get_x2D));
    } else {
      return IterBase<T2DSymbolInstance<version>>(
          Iter<T2DSymbolInstance<version>>(*this, i.ptr2, &File::get_x2D),
          Iter<T2DSymbolInstance<version>>(*this, i.k, &File::get_x2D));
    }
  };

  IterBase<T30StringGraphic<version>> iter_x30(uint32_t i_x2D) {
    auto &i = this->get_x2D(i_x2D);
    if (i.ptr3 == 0) {
      return IterBase<T30StringGraphic<version>>(
          Iter<T30StringGraphic<version>>(*this, i.k, &File::get_x30),
          Iter<T30StringGraphic<version>>(*this, i.k, &File::get_x30));
    } else {
      return IterBase<T30StringGraphic<version>>(
          Iter<T30StringGraphic<version>>(*this, i.ptr3, &File::get_x30),
          Iter<T30StringGraphic<version>>(*this, i.k, &File::get_x30));
    }
  };

  IterBase<T30StringGraphic<version>> iter_x30() {
    return IterBase<T30StringGraphic<version>>(
        Iter<T30StringGraphic<version>>(*this, this->hdr->ll_x03_x30.head,
                                        &File::get_x30),
        Iter<T30StringGraphic<version>>(*this, this->hdr->ll_x03_x30.tail,
                                        &File::get_x30));
  };

  IterBase<T32SymbolPin<version>> iter_x32(uint32_t i_x2D) {
    auto &i = this->get_x2D(i_x2D);
    if (i.first_pad_ptr == 0) {
      return IterBase<T32SymbolPin<version>>(
          Iter<T32SymbolPin<version>>(*this, i.k, &File::get_x32),
          Iter<T32SymbolPin<version>>(*this, i.k, &File::get_x32));
    } else {
      return IterBase<T32SymbolPin<version>>(
          Iter<T32SymbolPin<version>>(*this, i.first_pad_ptr, &File::get_x32),
          Iter<T32SymbolPin<version>>(*this, i.k, &File::get_x32));
    }
  };

  IterBase<x34<version>> iter_x34(uint32_t i_x28) {
    auto &i = this->get_x28(i_x28);
    if (i.ptr4 == 0) {
      return IterBase<x34<version>>(
          Iter<x34<version>>(*this, i.k, &File::get_x34),
          Iter<x34<version>>(*this, i.k, &File::get_x34));
    } else {
      return IterBase<x34<version>>(
          Iter<x34<version>>(*this, i.ptr4, &File::get_x34),
          Iter<x34<version>>(*this, i.k, &File::get_x34));
    }
  };

  IterBase<T38Film<version>> iter_t38_film() {
    if (this->hdr->ll_x38.head == 0) {
      return IterBase<T38Film<version>>(
          Iter<T38Film<version>>(*this, 0, &File::get_t38_film),
          Iter<T38Film<version>>(*this, 0, &File::get_t38_film));
    } else {
      return IterBase<T38Film<version>>(
          Iter<T38Film<version>>(*this, this->hdr->ll_x38.head,
                                 &File::get_t38_film),
          Iter<T38Film<version>>(*this, this->hdr->ll_x38.tail,
                                 &File::get_t38_film));
    }
  };

  mapped_region region;

 private:
  void cache_upgrade_funcs();
  std::ptrdiff_t offset(void *);

  T01ArcSegment<kAMax> (*x01_upgrade)(void *);
  x03<kAMax> (*x03_upgrade)(void *);
  x04<kAMax> (*x04_upgrade)(void *);
  T05Line<kAMax> (*x05_upgrade)(void *);
  x06<kAMax> (*x06_upgrade)(void *);
  T07Instance<kAMax> (*x07_upgrade)(void *);
  x08<kAMax> (*x08_upgrade)(void *);
  x09<kAMax> (*x09_upgrade)(void *);
  T0ADRC<kAMax> (*x0A_upgrade)(void *);
  x0C<kAMax> (*x0C_upgrade)(void *);
  x0D<kAMax> (*x0D_upgrade)(void *);
  x0E<kAMax> (*x0E_upgrade)(void *);
  x10<kAMax> (*x10_upgrade)(void *);
  T14Path<kAMax> (*x14_upgrade)(void *);
  T15LineSegment<kAMax> (*x15_upgrade)(void *);
  T16LineSegment<kAMax> (*x16_upgrade)(void *);
  T17LineSegment<kAMax> (*x17_upgrade)(void *);
  T1BNet<kAMax> (*x1B_upgrade)(void *);
  T1CPad<kAMax> (*x1C_upgrade)(void *);
  t13<kAMax> (*t13_upgrade)(void *);
  x1D<kAMax> (*x1D_upgrade)(void *);
  x1F<kAMax> (*x1F_upgrade)(void *);
  T23Rat<kAMax> (*x23_upgrade)(void *);
  T24Rectangle<kAMax> (*x24_upgrade)(void *);
  x26<kAMax> (*x26_upgrade)(void *);
  T28Shape<kAMax> (*x28_upgrade)(void *);
  T2BSymbol<kAMax> (*x2B_upgrade)(void *);
  x2C<kAMax> (*x2C_upgrade)(void *);
  T2DSymbolInstance<kAMax> (*x2D_upgrade)(void *);
  x2E<kAMax> (*x2E_upgrade)(void *);
  T30StringGraphic<kAMax> (*x30_upgrade)(void *);
  T31String<kAMax> (*x31_upgrade)(void *);
  T32SymbolPin<kAMax> (*x32_upgrade)(void *);
  x33<kAMax> (*x33_upgrade)(void *);
  x34<kAMax> (*x34_upgrade)(void *);
  x37<kAMax> (*x37_upgrade)(void *);
  T38Film<kAMax> (*x38_upgrade)(void *);
  T39FilmLayerList<kAMax> (*x39_upgrade)(void *);
  T3AFilmLayerListNode<kAMax> (*x3A_upgrade)(void *);
  x3C<kAMax> (*x3C_upgrade)(void *);
};

#endif
