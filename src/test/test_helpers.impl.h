#include <gtest/gtest.h>

#include <format>
#include <iostream>
#include <string>
#include <string_view>

#include "lib/structure/types.h"

template <typename T, uint8_t J, AllegroVersion version>
testing::AssertionResult CheckType(T& i, const ExpectRefType<J>& field,
                                   File<version>& fs) {
  if (fs.is_type(field, field.expected_type())) {
    return testing::AssertionSuccess();
  } else {
    return testing::AssertionFailure() << std::format(
               "key {:#012x} expected to be of type {:#04x}, but isn't (ptr "
               "found in obj with k={:#012x})",
               field.value, field.expected_type(), i.k);
  }
}

template <typename T, uint8_t J, AllegroVersion version>
testing::AssertionResult CheckNullableType(T& i, const ExpectRefType<J>& field,
                                           File<version>& fs) {
  if (field == 0x0) {
    return testing::AssertionSuccess();
  } else {
    return CheckType(i, field, fs);
  }
}

template <AllegroVersion version>
void validate_x04(const x04<version>& i, File<version>& fs) {
  if (i.next != fs.hdr->ll_x04.tail) {
    EXPECT_TRUE(CheckType(i, i.next, fs));
  }
}

template <AllegroVersion version>
void validate_x06(const x06<version>& i, File<version>& fs) {
  if (i.next != fs.hdr->ll_x06.tail) {
    EXPECT_TRUE(CheckType(i, i.next, fs));
  }
}

template <AllegroVersion version>
void validate_x14(const x14<version>& i, File<version>& fs) {
  if (i.next != fs.hdr->ll_x14.tail) {
    EXPECT_TRUE(CheckType(i, i.next, fs));
  }
  EXPECT_TRUE(CheckNullableType(i, i.ptr3, fs));
  EXPECT_TRUE(CheckNullableType(i, i.ptr4, fs));
}

template <AllegroVersion version>
void validate_x1B(const t1B_net<version>& i, File<version>& fs) {
  if (i.next != fs.hdr->ll_x1B.tail) {
    EXPECT_TRUE(CheckType(i, i.next, fs));
  }
  EXPECT_TRUE(CheckNullableType(i, i.ptr1, fs));
  EXPECT_TRUE(CheckNullableType(i, i.path_str_ptr, fs));
  EXPECT_TRUE(CheckNullableType(i, i.ptr6, fs));
}

template <AllegroVersion version>
void validate_x23(const x23<version>& i, File<version>& fs) {
  EXPECT_TRUE(fs.is_type(i.ptr1, i.ptr1.expected_type()));
}

template <AllegroVersion version>
void validate_x2B(const x2B<version>& i, File<version>& fs) {
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
void validate_x2D(const x2D<version>& i, File<version>& fs) {
  EXPECT_TRUE(CheckNullableType(i, i.ptr1, fs));
  EXPECT_TRUE(CheckNullableType(i, i.first_pad_ptr, fs));
  EXPECT_TRUE(CheckNullableType(i, i.first_pad_ptr, fs));
}

template <AllegroVersion version>
void check_header_values(File<version>& fs) {
  if (fs.hdr->ll_x04.head != 0) {
    // Iterating will crash if the pointers are incorrect
    for (auto& i : fs.iter_x04()) {
      validate_x04(i, fs);
    }
  }
  if (fs.hdr->ll_x06.head != 0) {
    for (auto& i : fs.iter_x06()) {
      validate_x06(i, fs);
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
      validate_x14(i, fs);
    }
  }
  if (fs.hdr->ll_x1B.head != 0) {
    for (auto& i : fs.iter_t1B_net()) {
      validate_x1B(i, fs);
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
    validate_x2B(i_x2B, fs);
    for (auto& i_x2D : fs.iter_x2D(i_x2B.k)) {
      validate_x2D(i_x2D, fs);
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
    for (auto& i : fs.iter_t38_film()) {
    }
  }
  if (fs.hdr->ll_x2C.head != 0) {
    for (auto& i : fs.iter_x2C()) {
      validate_x2C(i, fs);
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
  EXPECT_TRUE(fs.hdr->units == Units::kImperial ||
              fs.hdr->units == Units::kMetric);
}
