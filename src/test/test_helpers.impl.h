#include <gtest/gtest.h>

#include <boost/format.hpp>

#include "lib/structure/types.h"

template <typename T, uint8_t J, AllegroVersion version>
auto CheckType(T& i, const ExpectRefType<J>& field, File<version>& fs)
    -> testing::AssertionResult {
  if (fs.is_type(field, field.expected_type())) {
    return testing::AssertionSuccess();
  }

  // This use of `boost::format` can be changed to `std::format` when
  // upgrading to C++20.
  //
  // Also, the typecast is required to avoid being interpreted as a character
  // and printing the ASCII representation.
  return testing::AssertionFailure()
         << boost::format(
                "key 0x%|08X| expected to be of type 0x%|02X|, but isn't "
                "(ptr "
                "found in obj with k=0x%|08X|)") %
                field.value % static_cast<uint32_t>(field.expected_type()) %
                i.k;
}

template <typename T, uint8_t J, AllegroVersion version>
auto CheckNullableType(T& i, const ExpectRefType<J>& field, File<version>& fs)
    -> testing::AssertionResult {
  if (field == 0x0) {
    return testing::AssertionSuccess();
  }
  return CheckType(i, field, fs);
}

template <AllegroVersion version>
void validate_x04(const x04<version>& /* unused */, File<version>& /* unused */) {}

template <AllegroVersion version>
void validate_x05(const T05Line<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckNullableType(i, i.ptr5, fs));
}

template <AllegroVersion version>
void validate_x06(const x06<version>& i, File<version>& fs) {
  if (i.next != fs.hdr->ll_x06.tail) {
    EXPECT_TRUE(CheckType(i, i.next, fs));
  }
  EXPECT_TRUE(CheckType(i, i.ptr3, fs));
  EXPECT_TRUE(CheckType(i, i.ptr4, fs));
  EXPECT_TRUE(CheckNullableType(i, i.ptr5, fs));
  EXPECT_TRUE(CheckNullableType(i, i.ptr6, fs));
}

template <AllegroVersion version>
void validate_x07(const T07Instance<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckNullableType(i, i.ptr1, fs));
  EXPECT_TRUE(CheckNullableType(i, i.ptr3, fs));
  EXPECT_TRUE(CheckNullableType(i, i.ptr4, fs));
}

template <AllegroVersion version>
void validate_x08(const x08<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckNullableType(i, i.ptr3, fs));
}

template <AllegroVersion version>
void validate_x0C(const T0CDrillIndicator<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckNullableType(i, i.group_ptr, fs));
}

template <AllegroVersion version>
void validate_x0F(const T0FFootprint<version>& /* unused */, File<version>& /* unused */) {}

template <AllegroVersion version>
void validate_x10(const x10<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckNullableType(i, i.ptr4, fs));
  EXPECT_TRUE(CheckNullableType(i, i.path_str, fs));
}

template <AllegroVersion version>
void validate_x14(const T14Path<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckNullableType(i, i.ptr3, fs));
  EXPECT_TRUE(CheckNullableType(i, i.ptr4, fs));
}

template <AllegroVersion version>
void validate_x1B(const T1BNet<version>& i, File<version>& fs) {
  if (i.next != fs.hdr->ll_x1B.tail) {
    EXPECT_TRUE(CheckType(i, i.next, fs));
  }
  EXPECT_TRUE(CheckNullableType(i, i.ptr1, fs));
  EXPECT_TRUE(CheckNullableType(i, i.path_str_ptr, fs));
  EXPECT_TRUE(CheckNullableType(i, i.ptr6, fs));
}

template <AllegroVersion version>
void validate_x23(const T23Rat<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckNullableType(i, i.ptr1, fs));
}

template <AllegroVersion version>
void validate_x24(const T24Rectangle<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckNullableType(i, i.ptr2, fs));
}

template <AllegroVersion version>
void validate_x28(const T28Shape<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckNullableType(i, i.ptr4, fs));
}

template <AllegroVersion version>
void validate_x2B(const T2BSymbol<version>& i, File<version>& fs) {
  if (i.next != fs.hdr->ll_x2B.tail) {
    EXPECT_TRUE(CheckType(i, i.next, fs));
  }
  EXPECT_TRUE(CheckNullableType(i, i.ptr2, fs));
  EXPECT_TRUE(CheckNullableType(i, i.ptr3, fs));
  EXPECT_TRUE(CheckNullableType(i, i.ptr4, fs));
  EXPECT_TRUE(CheckNullableType(i, i.ptr5, fs));
}

template <AllegroVersion version>
void validate_x2C(const x2C<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckNullableType(i, i.ptr2, fs));
}

template <AllegroVersion version>
void validate_x2D(const T2DSymbolInstance<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckNullableType(i, i.ptr1, fs));
  EXPECT_TRUE(CheckNullableType(i, i.first_pad_ptr, fs));
  EXPECT_TRUE(CheckNullableType(i, i.first_pad_ptr, fs));
}

template <AllegroVersion version>
void validate_x30(const T30StringGraphic<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckType(i, i.str_graphic_ptr, fs));
}

template <AllegroVersion version>
void validate_x31(const T31String<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckType(i, i.str_graphic_wrapper_ptr, fs));
}

template <AllegroVersion version>
void validate_x34(const x34<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckType(i, i.ptr1, fs));
  EXPECT_TRUE(CheckType(i, i.ptr2, fs));
  EXPECT_TRUE(CheckType(i, i.ptr3, fs));
}

template <AllegroVersion version>
void check_header_values(File<version>& fs) {
  if (fs.hdr->ll_x04.head != 0) {
    // Iterating will crash if the pointers are incorrect
    for ([[maybe_unused]] auto& i : fs.iter_x04()) {
    }
  }
  if (fs.hdr->ll_x06.head != 0) {
    for ([[maybe_unused]] auto& i : fs.iter_x06()) {
    }
  }
  if (fs.hdr->ll_x0C_2.head != 0) {
    for ([[maybe_unused]] auto& i : fs.iter_x0C_2()) {
    }
  }
  if (fs.hdr->ll_x0E_x28.head != 0) {
    EXPECT_TRUE(fs.is_type(fs.hdr->ll_x0E_x28.head, 0x0E) ||
                fs.is_type(fs.hdr->ll_x0E_x28.head, 0x28));
  }
  if (fs.hdr->ll_x14.head != 0) {
    for ([[maybe_unused]] auto& i : fs.iter_x14()) {
    }
  }
  if (fs.hdr->ll_x1B.head != 0) {
    for ([[maybe_unused]] auto& i : fs.iter_t1B_net()) {
    }
  }
  if (fs.hdr->ll_x1C.head != 0) {
    for ([[maybe_unused]] auto& i : fs.iter_x1C()) {
    }
  }
  if (fs.hdr->ll_x24_x28.head != 0) {
    EXPECT_TRUE(fs.is_type(fs.hdr->ll_x24_x28.head, 0x24) ||
                fs.is_type(fs.hdr->ll_x24_x28.head, 0x28));
  }
  for (auto& i_x2B : fs.iter_x2B()) {
    validate_x2B(i_x2B, fs);
    for (auto& i_x2D : fs.iter_x2D(i_x2B.k)) {
      validate_x2D(i_x2D, fs);
      for ([[maybe_unused]] auto& i_x30 : fs.iter_x30(i_x2D.k)) {
      }
      for ([[maybe_unused]] auto& i_x32 : fs.iter_x32(i_x2D.k)) {
      }
    }
  }
  if (fs.hdr->ll_x03_x30.head != 0) {
    // EXPECT_TRUE(fs.x03_map.count(fs.hdr->ll_x03.head) > 0);
  }
  if (fs.hdr->ll_x0A_2.head != 0) {
    for ([[maybe_unused]] auto& i : fs.iter_x0A_2()) {
    }
  }
  if (fs.hdr->ll_x1D_x1E_x1F.head != 0) {
    EXPECT_TRUE(fs.is_type(fs.hdr->ll_x1D_x1E_x1F.head, 0x1D) ||
                fs.x1E_map.count(fs.hdr->ll_x1D_x1E_x1F.head) > 0 ||
                fs.is_type(fs.hdr->ll_x1D_x1E_x1F.head, 0x1F));
  }
  if (fs.hdr->ll_x38.head != 0) {
    for ([[maybe_unused]] auto& i : fs.iter_t38_film()) {
    }
  }
  if (fs.hdr->ll_x2C.head != 0) {
    for (auto& i : fs.iter_x2C()) {
      validate_x2C(i, fs);
    }
  }
  if (fs.hdr->ll_x0C.head != 0) {
    for ([[maybe_unused]] auto& i : fs.iter_x0C()) {
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
    for ([[maybe_unused]] auto& i : fs.iter_x0A()) {
    }
  }
  EXPECT_TRUE(fs.hdr->units == Units::kImperial ||
              fs.hdr->units == Units::kMetric);
}

template <AllegroVersion version>
void validate_objects(File<version>& fs) {
  // Note: Iterating over all objects finds objects that should have been
  // deleted, and aren't actually found by iterating normally through the
  // linked list structures.
  for (auto& [k, ptr] : fs.ptrs) {
    uint8_t t = *reinterpret_cast<uint8_t*>(ptr);
    switch (t) {
      case 0x04:
        validate_x04(fs.get_x04(k), fs);
        break;
      case 0x05:
        validate_x05(fs.get_x05(k), fs);
        break;
      case 0x06:
        validate_x06(fs.get_x06(k), fs);
        break;
      case 0x07:
        validate_x07(fs.get_x07(k), fs);
        break;
      case 0x08:
        validate_x08(fs.get_x08(k), fs);
        break;
      case 0x0C:
        validate_x0C(fs.get_x0C(k), fs);
        break;
      case 0x0F:
        validate_x0F(*reinterpret_cast<T0FFootprint<version>*>(ptr), fs);
        break;
      case 0x10:
        validate_x10(fs.get_x10(k), fs);
        break;
      case 0x14:
        validate_x14(fs.get_x14(k), fs);
        break;
      case 0x1B:
        validate_x1B(fs.get_x1B(k), fs);
        break;
      case 0x24:
        validate_x24(fs.get_x24(k), fs);
        break;
      case 0x28:
        validate_x28(fs.get_x28(k), fs);
        break;
      case 0x2B:
        validate_x2B(fs.get_x2B(k), fs);
        break;
      case 0x2C:
        validate_x2C(fs.get_x2C(k), fs);
        break;
      case 0x2D:
        validate_x2D(fs.get_x2D(k), fs);
        break;
      case 0x30:
        validate_x30(fs.get_x30(k), fs);
        break;
      case 0x31:
        validate_x31(fs.get_x31(k), fs);
        break;
      case 0x35:
        validate_x34(fs.get_x34(k), fs);
        break;
    }
  }
}
