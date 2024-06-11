#include "types.h"

template <>
T01ArcSegment<kA160>::operator T01ArcSegment<kAMax>() const {
  T01ArcSegment<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.un0 = this->un0;
  new_inst.subtype = this->subtype;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.parent = this->parent;
  new_inst.un1 = this->un1;
  new_inst.width = this->width;
  for (int i = 0; i < 4; i++) {
    new_inst.coords[i] = this->coords[i];
  }
  new_inst.x = this->x;
  new_inst.y = this->y;
  new_inst.r = this->r;
  for (int i = 0; i < 4; i++) {
    new_inst.bbox[i] = this->bbox[i];
  }
  return new_inst;
}

template <>
x03<kA160>::operator x03<kAMax>() const {
  x03<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.subtype = this->subtype;
  new_inst.has_str = this->has_str;
  // new_inst.s = this->s;
  new_inst.ptr = this->ptr;
  return new_inst;
}

template <>
x04<kA160>::operator x04<kAMax>() const {
  x04<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.ptr1 = this->ptr1;
  new_inst.ptr2 = this->ptr2;
  return new_inst;
}

template <>
T05Line<kA160>::operator T05Line<kAMax>() const {
  T05Line<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;
  new_inst.k = this->k;
  new_inst.ptr0 = this->ptr0;
  new_inst.ptr1 = this->ptr1;
  new_inst.un1[0] = this->un1[0];
  new_inst.un1[1] = this->un1[1];
  new_inst.ptr2[0] = this->ptr2[0];
  new_inst.ptr2[1] = this->ptr2[1];
  new_inst.un2 = this->un2;
  new_inst.ptr3[0] = this->ptr3[0];
  new_inst.ptr3[1] = this->ptr3[1];
  new_inst.first_segment_ptr = this->first_segment_ptr;
  new_inst.ptr5 = this->ptr5;
  new_inst.un3 = this->un3;
  return new_inst;
}

template <>
x06<kA160>::operator x06<kAMax>() const {
  x06<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.ptr1 = this->ptr1;
  new_inst.ptr2 = this->ptr2;
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  new_inst.ptr5 = this->ptr5;
  new_inst.ptr6 = this->ptr6;
  return new_inst;
}

template <>
T07Instance<kA160>::operator T07Instance<kAMax>() const {
  T07Instance<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = k;
  new_inst.un1 = this->un1;
  new_inst.ptr1 = this->ptr1;
  new_inst.un2 = this->un5;
  new_inst.refdes_string_ref = this->refdes_string_ref;
  new_inst.ptr2 = this->ptr2;
  new_inst.ptr3 = this->ptr3;
  new_inst.un3 = this->un3;
  new_inst.ptr4 = this->ptr4;
  return new_inst;
}

template <>
x08<kA160>::operator x08<kAMax>() const {
  x08<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.str_ptr = this->str_ptr_16x;
  new_inst.ptr1 = 0;
  new_inst.ptr2 = this->ptr2;
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  new_inst.un1 = 0;
  return new_inst;
}

template <>
x09<kA160>::operator x09<kAMax>() const {
  x09<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  for (uint8_t i = 0; i < 4; i++) {
    new_inst.un1[i] = this->un1[i];
  }
  new_inst.ptr1 = this->ptr1;
  new_inst.ptr2 = this->ptr2;
  new_inst.un2 = this->un2;
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  return new_inst;
}

template <>
x09<kA172>::operator x09<kAMax>() const {
  x09<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  for (uint8_t i = 0; i < 4; i++) {
    new_inst.un1[i] = this->un1[i];
  }
  new_inst.ptr1 = this->ptr1;
  new_inst.ptr2 = this->ptr2;
  new_inst.un2 = this->un2;
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  return new_inst;
}

template <>
T0ADRC<kA160>::operator T0ADRC<kAMax>() const {
  T0ADRC<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.un1 = this->un1;
  for (uint8_t i = 0; i < 4; i++) {
    new_inst.coords[i] = this->coords[i];
  }
  new_inst.un2 = 0;
  new_inst.un3 = 0;
  for (uint8_t i = 0; i < 4; i++) {
    new_inst.un4[i] = this->un4[i];
  }
  for (uint8_t i = 0; i < 4; i++) {
    new_inst.un5[i] = this->un5[i];
  }
  return new_inst;
}

template <>
T0ADRC<kA172>::operator T0ADRC<kAMax>() const {
  T0ADRC<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.un1 = this->un1;
  for (uint8_t i = 0; i < 4; i++) {
    new_inst.coords[i] = this->coords[i];
  }
  new_inst.un2 = this->un2;
  new_inst.un3 = 0;
  for (uint8_t i = 0; i < 4; i++) {
    new_inst.un4[i] = this->un4[i];
  }
  for (uint8_t i = 0; i < 4; i++) {
    new_inst.un5[i] = this->un5[i];
  }
  return new_inst;
}

template <>
x0C<kA160>::operator x0C<kAMax>() const {
  x0C<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;
  new_inst.k = this->k;
  new_inst.next = this->next;
  for (int i = 0; i < 11; i++) {
    new_inst.un[i] = this->un[i];
  }
  return new_inst;
}

template <>
x0C<kA172>::operator x0C<kAMax>() const {
  x0C<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;
  new_inst.k = this->k;
  new_inst.next = this->next;
  for (int i = 0; i < 11; i++) {
    new_inst.un[i] = this->un[i];
  }
  new_inst.un2 = this->un2;
  new_inst.un3 = this->un3;
  return new_inst;
}

template <>
x0D<kA160>::operator x0D<kAMax>() const {
  x0D<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.str_ptr = this->str_ptr;
  new_inst.ptr2 = this->ptr2;
  for (int i = 0; i < 2; i++) {
    new_inst.coords[i] = this->coords[i];
  }
  new_inst.pad_ptr = this->pad_ptr;
  new_inst.un1 = this->un1;
  new_inst.bitmask = this->bitmask;
  new_inst.rotation = this->rotation;
  return new_inst;
}

template <>
x0D<kA172>::operator x0D<kAMax>() const {
  x0D<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.str_ptr = this->str_ptr;
  new_inst.ptr2 = this->ptr2;
  for (int i = 0; i < 2; i++) {
    new_inst.coords[i] = this->coords[i];
  }
  new_inst.pad_ptr = this->pad_ptr;
  new_inst.un1 = this->un1;
  new_inst.un2 = this->un2;
  new_inst.bitmask = this->bitmask;
  new_inst.rotation = this->rotation;
  return new_inst;
}

template <>
x0E<kA160>::operator x0E<kAMax>() const {
  x0E<kAMax> new_inst;
  new_inst.t = this->t;
  for (int i = 0; i < 13; i++) {
    new_inst.un[i] = this->un[i];
  }
  new_inst.un1[0] = 0;
  new_inst.un1[1] = 0;
  return new_inst;
}

template <>
T0FFootprint<kA160>::operator T0FFootprint<kAMax>() const {
  T0FFootprint<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.ptr1 = this->ptr1;
  for (int i = 0; i < 32; i++) {
    new_inst.s[i] = this->s[i];
  }
  new_inst.ptr2 = this->ptr2;
  new_inst.ptr3 = this->ptr3;
  new_inst.un = this->un;
  return new_inst;
}

template <>
T0FFootprint<kA172>::operator T0FFootprint<kAMax>() const {
  T0FFootprint<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.ptr1 = this->ptr1;
  for (int i = 0; i < 32; i++) {
    new_inst.s[i] = this->s[i];
  }
  new_inst.ptr2 = this->ptr2;
  new_inst.ptr3 = this->ptr3;
  new_inst.un = this->un;
  return new_inst;
}

template <>
x10<kA160>::operator x10<kAMax>() const {
  x10<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.ptr1 = this->ptr1;
  new_inst.ptr2 = this->ptr2;
  new_inst.un1 = this->un1;
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  new_inst.path_str = this->path_str;
  return new_inst;
}

template <>
x10<kA172>::operator x10<kAMax>() const {
  x10<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.ptr1 = this->ptr1;
  new_inst.ptr2 = this->ptr2;
  new_inst.un1 = this->un1;
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  new_inst.path_str = this->path_str;
  return new_inst;
}

template <>
x11<kA160>::operator x11<kAMax>() const {
  x11<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.ptr1 = this->ptr1;
  new_inst.ptr2 = this->ptr2;
  new_inst.ptr3 = this->ptr3;
  new_inst.un = this->un;
  return new_inst;
}

template <>
x14<kA160>::operator x14<kAMax>() const {
  x14<kAMax> new_inst;
  new_inst.type = this->type;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.ptr1 = this->ptr1;
  new_inst.un2 = this->un2;
  new_inst.ptr2 = this->ptr2;
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  return new_inst;
}

template <>
T15LineSegment<kA160>::operator T15LineSegment<kAMax>() const {
  T15LineSegment<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.parent = this->parent;
  new_inst.un3 = this->un3;
  new_inst.width = this->width;
  for (int i = 0; i < 4; i++) {
    new_inst.coords[i] = this->coords[i];
  }
  return new_inst;
}

template <>
T16LineSegment<kA160>::operator T16LineSegment<kAMax>() const {
  T16LineSegment<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.parent = this->parent;
  new_inst.bitmask = this->bitmask;
  new_inst.width = this->width;
  for (int i = 0; i < 4; i++) {
    new_inst.coords[i] = this->coords[i];
  }
  return new_inst;
}

template <>
T17LineSegment<kA160>::operator T17LineSegment<kAMax>() const {
  T17LineSegment<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.parent = this->parent;
  new_inst.un3 = this->un3;
  new_inst.width = this->width;
  for (int i = 0; i < 4; i++) {
    new_inst.coords[i] = this->coords[i];
  }
  return new_inst;
}

template <>
T1BNet<kA160>::operator T1BNet<kAMax>() const {
  T1BNet<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.net_name = this->net_name;
  new_inst.un2 = this->un2;
  new_inst.type = this->type;
  new_inst.ptr1 = this->ptr1;
  new_inst.ptr2 = this->ptr2;
  new_inst.path_str_ptr = this->path_str_ptr;
  new_inst.ptr4 = this->ptr4;
  new_inst.model_ptr = this->model_ptr;
  new_inst.un3[0] = this->un3[0];
  new_inst.un3[1] = this->un3[1];
  new_inst.ptr6 = this->ptr6;
  return new_inst;
}

template <>
t13<kA160>::operator t13<kAMax>() const {
  t13<kAMax> new_inst;
  new_inst.str_ptr = this->str_ptr;
  new_inst.t = this->t;
  new_inst.z0 = 0;
  new_inst.w = this->w;
  new_inst.h = this->h;
  new_inst.x2 = this->x2;
  new_inst.x3 = this->x3;
  new_inst.z = this->x4;
  new_inst.x4 = 0;
  return new_inst;
}

template <>
x1C<kA160>::operator x1C<kAMax>() const {
  x1C<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.n = this->n;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.pad_str = this->pad_str;
  new_inst.pad_path = this->pad_path;
  new_inst.pad_info = this->pad_info;
  new_inst.layer_count = this->layer_count;
  new_inst.un1 = this->un1;
  new_inst.un2 = this->un2;
  new_inst.un3 = this->un3;
  return new_inst;
}

template <>
x1C<kA165>::operator x1C<kAMax>() const {
  x1C<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.n = this->n;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.pad_str = this->pad_str;
  new_inst.pad_path = this->pad_path;
  new_inst.pad_info = this->pad_info;
  new_inst.layer_count = this->layer_count;
  new_inst.un1 = this->un1;
  new_inst.un2 = this->un2;
  new_inst.un3 = this->un3;
  return new_inst;
}

template <>
x22<kA160>::operator x22<kAMax>() const {
  x22<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  for (uint8_t i = 0; i < 8; i++) {
    new_inst.un[i] = this->un[i];
  }
  new_inst.un1 = 0;
  return new_inst;
}

template <>
x23<kA160>::operator x23<kAMax>() const {
  return *reinterpret_cast<const x23<kAMax> *>(this);
}

template <>
x23<kA164>::operator x23<kAMax>() const {
  return *reinterpret_cast<const x23<kAMax> *>(this);
}

template <>
x24<kA160>::operator x24<kAMax>() const {
  return *reinterpret_cast<const x24<kAMax> *>(this);
}

template <>
x26<kA160>::operator x26<kAMax>() const {
  x26<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.member_ptr = this->member_ptr;
  new_inst.group_ptr = this->group_ptr;
  new_inst.const_ptr = this->const_ptr;
  return new_inst;
}

template <>
x26<kA172>::operator x26<kAMax>() const {
  x26<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.member_ptr = this->member_ptr;
  new_inst.group_ptr = this->group_ptr;
  new_inst.const_ptr = this->const_ptr;
  return new_inst;
}

template <>
x28<kA160>::operator x28<kAMax>() const {
  x28<kAMax> new_inst;
  new_inst.type = this->type;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;

  new_inst.k = this->k;
  new_inst.un1 = this->un1;
  new_inst.ptr1 = this->ptr1;
  new_inst.un2 = this->un2;
  new_inst.next = this->next;
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  new_inst.first_segment_ptr = this->first_segment_ptr;
  new_inst.un3 = this->un3;
  new_inst.un4 = this->un4;
  new_inst.ptr7 = this->ptr7_16x;
  new_inst.ptr6 = this->ptr6;
  for (int i = 0; i < 4; i++) {
    new_inst.coords[i] = this->coords[i];
  }
  return new_inst;
}

template <>
x2B<kA160>::operator x2B<kAMax>() const {
  x2B<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.un1 = this->un1;
  for (int i = 0; i < 4; i++) {
    new_inst.coords[i] = this->coords[i];
  }
  new_inst.next = this->next;
  new_inst.ptr2 = this->ptr2;
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  new_inst.ptr5 = this->ptr5;
  new_inst.str_ptr = this->str_ptr;
  new_inst.ptr6 = this->ptr6;
  new_inst.ptr7 = this->ptr7;
  new_inst.ptr8 = this->ptr8;
  new_inst.un3 = 0;
  return new_inst;
}

template <>
x2B<kA164>::operator x2B<kAMax>() const {
  x2B<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.un1 = this->un1;
  for (int i = 0; i < 4; i++) {
    new_inst.coords[i] = this->coords[i];
  }
  new_inst.next = this->next;
  new_inst.ptr2 = this->ptr2;
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  new_inst.ptr5 = this->ptr5;
  new_inst.str_ptr = this->str_ptr;
  new_inst.ptr6 = this->ptr6;
  new_inst.ptr7 = this->ptr7;
  new_inst.ptr8 = this->ptr8;
  new_inst.un2 = 0;
  new_inst.un3 = 0;
  return new_inst;
}

template <>
x2C<kA160>::operator x2C<kAMax>() const {
  x2C<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.un2 = this->un5;
  new_inst.string_ptr = this->string_ptr;
  new_inst.ptr1 = this->ptr1;
  new_inst.ptr2 = this->ptr2;
  new_inst.ptr3 = this->ptr3;
  new_inst.bitmask = this->bitmask;
  return new_inst;
}

template <>
x2D<kA160>::operator x2D<kAMax>() const {
  x2D<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.layer = this->layer;
  new_inst.un0 = this->un0;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.inst_ref = this->inst_ref_16x;
  new_inst.un2 = this->un2;
  new_inst.un3 = this->un3;
  new_inst.bitmask1 = this->bitmask1;
  new_inst.rotation = this->rotation;
  new_inst.coords[0] = this->coords[0];
  new_inst.coords[1] = this->coords[1];
  new_inst.ptr1 = this->ptr1;
  new_inst.first_pad_ptr = this->first_pad_ptr;
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4[0] = this->ptr4[0];
  new_inst.ptr4[1] = this->ptr4[1];
  new_inst.ptr4[2] = this->ptr4[2];
  new_inst.group_assignment_ptr = this->group_assignment_ptr;
  return new_inst;
}

template <>
x2E<kA160>::operator x2E<kAMax>() const {
  return *reinterpret_cast<const x2E<kAMax> *>(this);
}

template <>
x30<kA160>::operator x30<kAMax>() const {
  x30<kAMax> new_inst;
  new_inst.type = this->type;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.str_graphic_ptr = this->str_graphic_ptr;
  new_inst.un1 = this->un1;
  new_inst.font = this->font_16x;
  new_inst.coords[0] = this->coords[0];
  new_inst.coords[1] = this->coords[1];
  new_inst.un3 = this->un3;
  new_inst.rotation = this->rotation;
  new_inst.ptr3 = this->ptr3_16x;
  return new_inst;
}

template <>
x30<kA172>::operator x30<kAMax>() const {
  x30<kAMax> new_inst;
  new_inst.type = this->type;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.str_graphic_ptr = this->str_graphic_ptr;
  new_inst.un1 = this->un1;
  new_inst.un4 = this->un4;
  new_inst.un5 = this->un5;
  new_inst.font = this->font;
  new_inst.coords[0] = this->coords[0];
  new_inst.coords[1] = this->coords[1];
  new_inst.un3 = this->un3;
  new_inst.rotation = this->rotation;
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  return new_inst;
}

template <>
x31<kA160>::operator x31<kAMax>() const {
  x31<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;
  new_inst.k = this->k;
  new_inst.str_graphic_wrapper_ptr = this->str_graphic_wrapper_ptr;
  new_inst.coords[0] = this->coords[0];
  new_inst.coords[1] = this->coords[1];
  new_inst.un = this->un;
  new_inst.len = this->len;
  return new_inst;
}

template <>
x32<kA160>::operator x32<kAMax>() const {
  x32<kAMax> new_inst;
  new_inst.type = this->type;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;

  new_inst.k = this->k;
  new_inst.un1 = this->un1;

  new_inst.ptr1 = this->ptr1;

  new_inst.bitmask1 = this->bitmask1;

  new_inst.next = this->next;

  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  new_inst.ptr5 = this->ptr5;
  new_inst.ptr6 = this->ptr6;
  new_inst.ptr7 = this->ptr7;
  new_inst.ptr8 = this->ptr8;

  new_inst.previous = this->previous;

  new_inst.ptr10 = this->ptr10;
  new_inst.ptr11 = this->ptr11;

  for (int i = 0; i < 4; i++) {
    new_inst.coords[i] = this->coords[i];
  }

  // new_inst.un2     = this->un2;
  return new_inst;
}

template <>
x33<kA160>::operator x33<kAMax>() const {
  x33<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;

  new_inst.k = this->k;

  new_inst.un1 = this->un1;
  new_inst.ptr1 = this->ptr1;
  new_inst.un2 = this->un2;
  new_inst.ptr2 = this->ptr2;
  new_inst.ptr7 = 0;
  new_inst.coords[0] = this->coords[0];
  new_inst.coords[1] = this->coords[1];
  new_inst.ptr3 = this->ptr3;
  new_inst.ptr4 = this->ptr4;
  new_inst.ptr5 = this->ptr5;
  new_inst.ptr6 = this->ptr6;
  new_inst.un3[0] = this->un3[0];
  new_inst.un3[1] = this->un3[1];
  for (uint8_t i = 0; i < 4; i++) {
    new_inst.bb_coords[i] = this->bb_coords[i];
  }
  return new_inst;
}

template <>
x34<kA160>::operator x34<kAMax>() const {
  x34<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.subtype = this->subtype;
  new_inst.layer = this->layer;

  new_inst.k = this->k;

  new_inst.next = this->next;
  new_inst.ptr1 = this->ptr1;
  new_inst.bitmask1 = this->bitmask1;
  new_inst.ptr2 = this->ptr2;
  new_inst.ptr3 = this->ptr3;
  return new_inst;
}

template <>
x36_x08<kA160>::operator x36_x08<kAMax>() const {
  x36_x08<kAMax> new_inst;
  new_inst.a = this->a;
  new_inst.b = this->b;
  new_inst.char_height = this->char_height;
  new_inst.char_width = this->char_width;
  for (uint8_t i = 0; i < 4; i++) {
    new_inst.xs[i] = this->xs[i];
  }
  return new_inst;
}

template <>
x36_x08<kA172>::operator x36_x08<kAMax>() const {
  x36_x08<kAMax> new_inst;
  new_inst.a = this->a;
  new_inst.b = this->b;
  new_inst.char_height = this->char_height;
  new_inst.char_width = this->char_width;
  for (uint8_t i = 0; i < 4; i++) {
    new_inst.xs[i] = this->xs[i];
  }
  return new_inst;
}

template <>
x36<kA160>::operator x36<kAMax>() const {
  x36<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.c = this->c;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.un1 = 0;
  new_inst.size = this->size;
  new_inst.count = this->count;
  new_inst.last_idx = this->last_idx;
  new_inst.un3 = this->un3;
  new_inst.un2 = 0;
  for (const auto &x08_inst : this->x08s) {
    new_inst.x08s.push_back(upgrade<kA160, kAMax>(x08_inst));
  }
  for (const auto &x0F_inst : this->x0Fs) {
    x36_x0F i = *reinterpret_cast<const x36_x0F<kAMax> *>(&x0F_inst);
    new_inst.x0Fs.push_back(i);
  }
  return new_inst;
}

template <>
x36<kA172>::operator x36<kAMax>() const {
  x36<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.c = this->c;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.un1 = this->un1;
  new_inst.size = this->size;
  new_inst.count = this->count;
  new_inst.last_idx = this->last_idx;
  new_inst.un3 = this->un3;
  new_inst.un2 = 0;
  for (const auto &x08_inst : this->x08s) {
    new_inst.x08s.push_back(upgrade<kA172, kAMax>(x08_inst));
  }
  for (const auto &x0F_inst : this->x0Fs) {
    x36_x0F i = *reinterpret_cast<const x36_x0F<kAMax> *>(&x0F_inst);
    new_inst.x0Fs.push_back(i);
  }
  return new_inst;
}

template <>
x37<kA160>::operator x37<kAMax>() const {
  x37<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.ptr1 = this->ptr1;
  new_inst.un2 = this->un2;
  new_inst.capacity = this->capacity;
  new_inst.count = this->count;
  new_inst.un3 = this->un3;
  for (uint8_t i = 0; i < 100; i++) {
    new_inst.ptrs[i] = this->ptrs[i];
  }
  new_inst.un4 = 0;
  return new_inst;
}

template <>
t38_film<kA160>::operator t38_film<kAMax>() const {
  t38_film<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.layer_list = this->layer_list;
  new_inst.layer_name_str = 0;
  new_inst.un2 = 0;
  new_inst.un3 = 0;
  new_inst.s = std::string(this->film_name);
  return new_inst;
}

template <>
t38_film<kA166>::operator t38_film<kAMax>() const {
  t38_film<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.next = this->next;
  new_inst.layer_list = this->layer_list;
  new_inst.layer_name_str = this->layer_name_str;
  new_inst.un2 = this->un2;
  new_inst.un3 = 0;
  new_inst.s = std::string("Mystery film name?");
  return new_inst;
}

template <>
t39_film_layer_list<kA160>::operator t39_film_layer_list<kAMax>() const {
  return *reinterpret_cast<const t39_film_layer_list<kAMax> *>(this);
}

template <>
t3A_film_layer_list_node<kA160>::operator t3A_film_layer_list_node<kAMax>()
    const {
  return *reinterpret_cast<const t3A_film_layer_list_node<kAMax> *>(this);
}

template <>
x3B<kA160>::operator x3B<kAMax>() const {
  x3B<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.subtype = this->subtype;
  new_inst.len = this->len;
  for (uint8_t i = 0; i < 128; i++) {
    new_inst.name[i] = this->name[i];
  }
  for (uint8_t i = 0; i < 32; i++) {
    new_inst.type[i] = this->type[i];
  }
  new_inst.un1 = this->un1;
  new_inst.un2 = this->un2;
  return new_inst;
}

template <>
x3C<kA160>::operator x3C<kAMax>() const {
  x3C<kAMax> new_inst;
  new_inst.t = this->t;
  new_inst.k = this->k;
  new_inst.un = 0;
  new_inst.size = this->size;
  return new_inst;
}

template <AllegroVersion version>
File<version>::File(mapped_region input_region)
    : region(std::move(input_region)) {}

// Explicit instantiation
template File<kAMax>::File(mapped_region region);

template <>
void File<kAMax>::prepare() {
  this->ptrs.reserve(this->hdr->object_count);
  cache_upgrade_funcs();
}

template <AllegroVersion version>
void File<version>::cache_upgrade_funcs() {
  // Need to lookup true version, because it's not kAMax
  switch (this->hdr->magic) {
    case 0x00130000:
    case 0x00130200:
      this->x01_upgrade = new_upgrade<kA160, kAMax, T01ArcSegment>;
      this->x03_upgrade = new_upgrade<kA160, kAMax, x03>;
      this->x04_upgrade = new_upgrade<kA160, kAMax, x04>;
      this->x05_upgrade = new_upgrade<kA160, kAMax, T05Line>;
      this->x06_upgrade = new_upgrade<kA160, kAMax, x06>;
      this->x07_upgrade = new_upgrade<kA160, kAMax, T07Instance>;
      this->x08_upgrade = new_upgrade<kA160, kAMax, x08>;
      this->x09_upgrade = new_upgrade<kA160, kAMax, x09>;
      this->x0A_upgrade = new_upgrade<kA160, kAMax, T0ADRC>;
      this->x0C_upgrade = new_upgrade<kA160, kAMax, x0C>;
      this->x0D_upgrade = new_upgrade<kA160, kAMax, x0D>;
      this->x0E_upgrade = new_upgrade<kA160, kAMax, x0E>;
      this->x10_upgrade = new_upgrade<kA160, kAMax, x10>;
      this->x14_upgrade = new_upgrade<kA160, kAMax, x14>;
      this->x15_upgrade = new_upgrade<kA160, kAMax, T15LineSegment>;
      this->x16_upgrade = new_upgrade<kA160, kAMax, T16LineSegment>;
      this->x17_upgrade = new_upgrade<kA160, kAMax, T17LineSegment>;
      this->x1B_upgrade = new_upgrade<kA160, kAMax, T1BNet>;
      this->x1C_upgrade = new_upgrade<kA160, kAMax, x1C>;
      this->t13_upgrade = new_upgrade<kA160, kAMax, t13>;
      this->x1D_upgrade = new_upgrade<kA160, kAMax, x1D>;
      this->x1F_upgrade = new_upgrade<kA160, kAMax, x1F>;
      this->x23_upgrade = new_upgrade<kA160, kAMax, x23>;
      this->x24_upgrade = new_upgrade<kA160, kAMax, x24>;
      this->x26_upgrade = new_upgrade<kA160, kAMax, x26>;
      this->x28_upgrade = new_upgrade<kA160, kAMax, x28>;
      this->x2B_upgrade = new_upgrade<kA160, kAMax, x2B>;
      this->x2C_upgrade = new_upgrade<kA160, kAMax, x2C>;
      this->x2D_upgrade = new_upgrade<kA160, kAMax, x2D>;
      this->x2E_upgrade = new_upgrade<kA160, kAMax, x2E>;
      this->x30_upgrade = new_upgrade<kA160, kAMax, x30>;
      this->x31_upgrade = new_upgrade<kA160, kAMax, x31>;
      this->x32_upgrade = new_upgrade<kA160, kAMax, x32>;
      this->x33_upgrade = new_upgrade<kA160, kAMax, x33>;
      this->x34_upgrade = new_upgrade<kA160, kAMax, x34>;
      this->x37_upgrade = new_upgrade<kA160, kAMax, x37>;
      this->x38_upgrade = new_upgrade<kA160, kAMax, t38_film>;
      this->x39_upgrade = new_upgrade<kA160, kAMax, t39_film_layer_list>;
      this->x3A_upgrade = new_upgrade<kA160, kAMax, t3A_film_layer_list_node>;
      this->x3C_upgrade = new_upgrade<kA160, kAMax, x3C>;
      break;
    case 0x00130402:
      this->x01_upgrade = new_upgrade<kA162, kAMax, T01ArcSegment>;
      this->x03_upgrade = new_upgrade<kA162, kAMax, x03>;
      this->x04_upgrade = new_upgrade<kA162, kAMax, x04>;
      this->x05_upgrade = new_upgrade<kA162, kAMax, T05Line>;
      this->x06_upgrade = new_upgrade<kA162, kAMax, x06>;
      this->x07_upgrade = new_upgrade<kA162, kAMax, T07Instance>;
      this->x08_upgrade = new_upgrade<kA162, kAMax, x08>;
      this->x09_upgrade = new_upgrade<kA162, kAMax, x09>;
      this->x0A_upgrade = new_upgrade<kA162, kAMax, T0ADRC>;
      this->x0C_upgrade = new_upgrade<kA162, kAMax, x0C>;
      this->x0D_upgrade = new_upgrade<kA162, kAMax, x0D>;
      this->x0E_upgrade = new_upgrade<kA162, kAMax, x0E>;
      this->x10_upgrade = new_upgrade<kA162, kAMax, x10>;
      this->x14_upgrade = new_upgrade<kA162, kAMax, x14>;
      this->x15_upgrade = new_upgrade<kA162, kAMax, T15LineSegment>;
      this->x16_upgrade = new_upgrade<kA162, kAMax, T16LineSegment>;
      this->x17_upgrade = new_upgrade<kA162, kAMax, T17LineSegment>;
      this->x1B_upgrade = new_upgrade<kA162, kAMax, T1BNet>;
      this->x1C_upgrade = new_upgrade<kA162, kAMax, x1C>;
      this->t13_upgrade = new_upgrade<kA162, kAMax, t13>;
      this->x1D_upgrade = new_upgrade<kA162, kAMax, x1D>;
      this->x1F_upgrade = new_upgrade<kA162, kAMax, x1F>;
      this->x23_upgrade = new_upgrade<kA162, kAMax, x23>;
      this->x24_upgrade = new_upgrade<kA162, kAMax, x24>;
      this->x26_upgrade = new_upgrade<kA162, kAMax, x26>;
      this->x28_upgrade = new_upgrade<kA162, kAMax, x28>;
      this->x2B_upgrade = new_upgrade<kA162, kAMax, x2B>;
      this->x2C_upgrade = new_upgrade<kA162, kAMax, x2C>;
      this->x2D_upgrade = new_upgrade<kA162, kAMax, x2D>;
      this->x2E_upgrade = new_upgrade<kA162, kAMax, x2E>;
      this->x30_upgrade = new_upgrade<kA162, kAMax, x30>;
      this->x31_upgrade = new_upgrade<kA162, kAMax, x31>;
      this->x32_upgrade = new_upgrade<kA162, kAMax, x32>;
      this->x33_upgrade = new_upgrade<kA162, kAMax, x33>;
      this->x34_upgrade = new_upgrade<kA162, kAMax, x34>;
      this->x37_upgrade = new_upgrade<kA162, kAMax, x37>;
      this->x38_upgrade = new_upgrade<kA162, kAMax, t38_film>;
      this->x39_upgrade = new_upgrade<kA162, kAMax, t39_film_layer_list>;
      this->x3A_upgrade = new_upgrade<kA162, kAMax, t3A_film_layer_list_node>;
      this->x3C_upgrade = new_upgrade<kA162, kAMax, x3C>;
      break;
    case 0x00130C03:
      this->x01_upgrade = new_upgrade<kA164, kAMax, T01ArcSegment>;
      this->x03_upgrade = new_upgrade<kA164, kAMax, x03>;
      this->x04_upgrade = new_upgrade<kA164, kAMax, x04>;
      this->x05_upgrade = new_upgrade<kA164, kAMax, T05Line>;
      this->x06_upgrade = new_upgrade<kA164, kAMax, x06>;
      this->x07_upgrade = new_upgrade<kA164, kAMax, T07Instance>;
      this->x08_upgrade = new_upgrade<kA164, kAMax, x08>;
      this->x09_upgrade = new_upgrade<kA164, kAMax, x09>;
      this->x0A_upgrade = new_upgrade<kA164, kAMax, T0ADRC>;
      this->x0C_upgrade = new_upgrade<kA164, kAMax, x0C>;
      this->x0D_upgrade = new_upgrade<kA164, kAMax, x0D>;
      this->x0E_upgrade = new_upgrade<kA164, kAMax, x0E>;
      this->x10_upgrade = new_upgrade<kA164, kAMax, x10>;
      this->x14_upgrade = new_upgrade<kA164, kAMax, x14>;
      this->x15_upgrade = new_upgrade<kA164, kAMax, T15LineSegment>;
      this->x16_upgrade = new_upgrade<kA164, kAMax, T16LineSegment>;
      this->x17_upgrade = new_upgrade<kA164, kAMax, T17LineSegment>;
      this->x1B_upgrade = new_upgrade<kA164, kAMax, T1BNet>;
      this->x1C_upgrade = new_upgrade<kA164, kAMax, x1C>;
      this->t13_upgrade = new_upgrade<kA164, kAMax, t13>;
      this->x1D_upgrade = new_upgrade<kA164, kAMax, x1D>;
      this->x1F_upgrade = new_upgrade<kA164, kAMax, x1F>;
      this->x23_upgrade = new_upgrade<kA164, kAMax, x23>;
      this->x24_upgrade = new_upgrade<kA164, kAMax, x24>;
      this->x26_upgrade = new_upgrade<kA164, kAMax, x26>;
      this->x28_upgrade = new_upgrade<kA164, kAMax, x28>;
      this->x2B_upgrade = new_upgrade<kA164, kAMax, x2B>;
      this->x2C_upgrade = new_upgrade<kA164, kAMax, x2C>;
      this->x2D_upgrade = new_upgrade<kA164, kAMax, x2D>;
      this->x2E_upgrade = new_upgrade<kA164, kAMax, x2E>;
      this->x30_upgrade = new_upgrade<kA164, kAMax, x30>;
      this->x31_upgrade = new_upgrade<kA164, kAMax, x31>;
      this->x32_upgrade = new_upgrade<kA164, kAMax, x32>;
      this->x33_upgrade = new_upgrade<kA164, kAMax, x33>;
      this->x34_upgrade = new_upgrade<kA164, kAMax, x34>;
      this->x37_upgrade = new_upgrade<kA164, kAMax, x37>;
      this->x38_upgrade = new_upgrade<kA164, kAMax, t38_film>;
      this->x39_upgrade = new_upgrade<kA164, kAMax, t39_film_layer_list>;
      this->x3A_upgrade = new_upgrade<kA164, kAMax, t3A_film_layer_list_node>;
      this->x3C_upgrade = new_upgrade<kA164, kAMax, x3C>;
      break;
    case 0x00131003:
      this->x01_upgrade = new_upgrade<kA165, kAMax, T01ArcSegment>;
      this->x03_upgrade = new_upgrade<kA165, kAMax, x03>;
      this->x04_upgrade = new_upgrade<kA165, kAMax, x04>;
      this->x05_upgrade = new_upgrade<kA165, kAMax, T05Line>;
      this->x06_upgrade = new_upgrade<kA165, kAMax, x06>;
      this->x07_upgrade = new_upgrade<kA165, kAMax, T07Instance>;
      this->x08_upgrade = new_upgrade<kA165, kAMax, x08>;
      this->x09_upgrade = new_upgrade<kA165, kAMax, x09>;
      this->x0A_upgrade = new_upgrade<kA165, kAMax, T0ADRC>;
      this->x0C_upgrade = new_upgrade<kA165, kAMax, x0C>;
      this->x0D_upgrade = new_upgrade<kA165, kAMax, x0D>;
      this->x0E_upgrade = new_upgrade<kA165, kAMax, x0E>;
      this->x10_upgrade = new_upgrade<kA165, kAMax, x10>;
      this->x14_upgrade = new_upgrade<kA165, kAMax, x14>;
      this->x15_upgrade = new_upgrade<kA165, kAMax, T15LineSegment>;
      this->x16_upgrade = new_upgrade<kA165, kAMax, T16LineSegment>;
      this->x17_upgrade = new_upgrade<kA165, kAMax, T17LineSegment>;
      this->x1B_upgrade = new_upgrade<kA165, kAMax, T1BNet>;
      this->x1C_upgrade = new_upgrade<kA165, kAMax, x1C>;
      this->t13_upgrade = new_upgrade<kA165, kAMax, t13>;
      this->x1D_upgrade = new_upgrade<kA165, kAMax, x1D>;
      this->x1F_upgrade = new_upgrade<kA165, kAMax, x1F>;
      this->x23_upgrade = new_upgrade<kA165, kAMax, x23>;
      this->x24_upgrade = new_upgrade<kA165, kAMax, x24>;
      this->x26_upgrade = new_upgrade<kA165, kAMax, x26>;
      this->x28_upgrade = new_upgrade<kA165, kAMax, x28>;
      this->x2B_upgrade = new_upgrade<kA165, kAMax, x2B>;
      this->x2C_upgrade = new_upgrade<kA165, kAMax, x2C>;
      this->x2D_upgrade = new_upgrade<kA165, kAMax, x2D>;
      this->x2E_upgrade = new_upgrade<kA165, kAMax, x2E>;
      this->x30_upgrade = new_upgrade<kA165, kAMax, x30>;
      this->x31_upgrade = new_upgrade<kA165, kAMax, x31>;
      this->x32_upgrade = new_upgrade<kA165, kAMax, x32>;
      this->x33_upgrade = new_upgrade<kA165, kAMax, x33>;
      this->x34_upgrade = new_upgrade<kA165, kAMax, x34>;
      this->x37_upgrade = new_upgrade<kA165, kAMax, x37>;
      this->x38_upgrade = new_upgrade<kA165, kAMax, t38_film>;
      this->x39_upgrade = new_upgrade<kA165, kAMax, t39_film_layer_list>;
      this->x3A_upgrade = new_upgrade<kA165, kAMax, t3A_film_layer_list_node>;
      this->x3C_upgrade = new_upgrade<kA165, kAMax, x3C>;
      break;
    case 0x00131503:
    case 0x00131504:
      this->x01_upgrade = new_upgrade<kA166, kAMax, T01ArcSegment>;
      this->x03_upgrade = new_upgrade<kA166, kAMax, x03>;
      this->x04_upgrade = new_upgrade<kA166, kAMax, x04>;
      this->x05_upgrade = new_upgrade<kA166, kAMax, T05Line>;
      this->x06_upgrade = new_upgrade<kA166, kAMax, x06>;
      this->x07_upgrade = new_upgrade<kA166, kAMax, T07Instance>;
      this->x08_upgrade = new_upgrade<kA166, kAMax, x08>;
      this->x09_upgrade = new_upgrade<kA166, kAMax, x09>;
      this->x0A_upgrade = new_upgrade<kA166, kAMax, T0ADRC>;
      this->x0C_upgrade = new_upgrade<kA166, kAMax, x0C>;
      this->x0D_upgrade = new_upgrade<kA166, kAMax, x0D>;
      this->x0E_upgrade = new_upgrade<kA166, kAMax, x0E>;
      this->x10_upgrade = new_upgrade<kA166, kAMax, x10>;
      this->x14_upgrade = new_upgrade<kA166, kAMax, x14>;
      this->x15_upgrade = new_upgrade<kA166, kAMax, T15LineSegment>;
      this->x16_upgrade = new_upgrade<kA166, kAMax, T16LineSegment>;
      this->x17_upgrade = new_upgrade<kA166, kAMax, T17LineSegment>;
      this->x1B_upgrade = new_upgrade<kA166, kAMax, T1BNet>;
      this->x1C_upgrade = new_upgrade<kA166, kAMax, x1C>;
      this->t13_upgrade = new_upgrade<kA166, kAMax, t13>;
      this->x1D_upgrade = new_upgrade<kA166, kAMax, x1D>;
      this->x1F_upgrade = new_upgrade<kA166, kAMax, x1F>;
      this->x23_upgrade = new_upgrade<kA166, kAMax, x23>;
      this->x24_upgrade = new_upgrade<kA166, kAMax, x24>;
      this->x26_upgrade = new_upgrade<kA166, kAMax, x26>;
      this->x28_upgrade = new_upgrade<kA166, kAMax, x28>;
      this->x2B_upgrade = new_upgrade<kA166, kAMax, x2B>;
      this->x2C_upgrade = new_upgrade<kA166, kAMax, x2C>;
      this->x2D_upgrade = new_upgrade<kA166, kAMax, x2D>;
      this->x2E_upgrade = new_upgrade<kA166, kAMax, x2E>;
      this->x30_upgrade = new_upgrade<kA166, kAMax, x30>;
      this->x31_upgrade = new_upgrade<kA166, kAMax, x31>;
      this->x32_upgrade = new_upgrade<kA166, kAMax, x32>;
      this->x33_upgrade = new_upgrade<kA166, kAMax, x33>;
      this->x34_upgrade = new_upgrade<kA166, kAMax, x34>;
      this->x37_upgrade = new_upgrade<kA166, kAMax, x37>;
      this->x38_upgrade = new_upgrade<kA166, kAMax, t38_film>;
      this->x39_upgrade = new_upgrade<kA166, kAMax, t39_film_layer_list>;
      this->x3A_upgrade = new_upgrade<kA166, kAMax, t3A_film_layer_list_node>;
      this->x3C_upgrade = new_upgrade<kA166, kAMax, x3C>;
      break;
    case 0x00140400:
    case 0x00140500:
    case 0x00140501:
    case 0x00140502:
    case 0x00140600:
    case 0x00140700:
      this->x01_upgrade = new_upgrade<kA172, kAMax, T01ArcSegment>;
      this->x03_upgrade = new_upgrade<kA172, kAMax, x03>;
      this->x04_upgrade = new_upgrade<kA172, kAMax, x04>;
      this->x05_upgrade = new_upgrade<kA172, kAMax, T05Line>;
      this->x06_upgrade = new_upgrade<kA172, kAMax, x06>;
      this->x07_upgrade = new_upgrade<kA172, kAMax, T07Instance>;
      this->x08_upgrade = new_upgrade<kA172, kAMax, x08>;
      this->x09_upgrade = new_upgrade<kA172, kAMax, x09>;
      this->x0A_upgrade = new_upgrade<kA172, kAMax, T0ADRC>;
      this->x0C_upgrade = new_upgrade<kA172, kAMax, x0C>;
      this->x0D_upgrade = new_upgrade<kA172, kAMax, x0D>;
      this->x0E_upgrade = new_upgrade<kA172, kAMax, x0E>;
      this->x10_upgrade = new_upgrade<kA172, kAMax, x10>;
      this->x14_upgrade = new_upgrade<kA172, kAMax, x14>;
      this->x15_upgrade = new_upgrade<kA172, kAMax, T15LineSegment>;
      this->x16_upgrade = new_upgrade<kA172, kAMax, T16LineSegment>;
      this->x17_upgrade = new_upgrade<kA172, kAMax, T17LineSegment>;
      this->x1B_upgrade = new_upgrade<kA172, kAMax, T1BNet>;
      this->x1C_upgrade = new_upgrade<kA172, kAMax, x1C>;
      this->t13_upgrade = new_upgrade<kA172, kAMax, t13>;
      this->x1D_upgrade = new_upgrade<kA172, kAMax, x1D>;
      this->x1F_upgrade = new_upgrade<kA172, kAMax, x1F>;
      this->x23_upgrade = new_upgrade<kA172, kAMax, x23>;
      this->x24_upgrade = new_upgrade<kA172, kAMax, x24>;
      this->x26_upgrade = new_upgrade<kA172, kAMax, x26>;
      this->x28_upgrade = new_upgrade<kA172, kAMax, x28>;
      this->x2B_upgrade = new_upgrade<kA172, kAMax, x2B>;
      this->x2C_upgrade = new_upgrade<kA172, kAMax, x2C>;
      this->x2D_upgrade = new_upgrade<kA172, kAMax, x2D>;
      this->x2E_upgrade = new_upgrade<kA172, kAMax, x2E>;
      this->x30_upgrade = new_upgrade<kA172, kAMax, x30>;
      this->x31_upgrade = new_upgrade<kA172, kAMax, x31>;
      this->x32_upgrade = new_upgrade<kA172, kAMax, x32>;
      this->x33_upgrade = new_upgrade<kA172, kAMax, x33>;
      this->x34_upgrade = new_upgrade<kA172, kAMax, x34>;
      this->x37_upgrade = new_upgrade<kA172, kAMax, x37>;
      this->x38_upgrade = new_upgrade<kA172, kAMax, t38_film>;
      this->x39_upgrade = new_upgrade<kA172, kAMax, t39_film_layer_list>;
      this->x3A_upgrade = new_upgrade<kA172, kAMax, t3A_film_layer_list_node>;
      this->x3C_upgrade = new_upgrade<kA172, kAMax, x3C>;
      break;
    case 0x00140900:
    case 0x00140901:
    case 0x00140902:
    case 0x00140E00:
      this->x01_upgrade = new_upgrade<kA174, kAMax, T01ArcSegment>;
      this->x03_upgrade = new_upgrade<kA174, kAMax, x03>;
      this->x04_upgrade = new_upgrade<kA174, kAMax, x04>;
      this->x05_upgrade = new_upgrade<kA174, kAMax, T05Line>;
      this->x06_upgrade = new_upgrade<kA174, kAMax, x06>;
      this->x07_upgrade = new_upgrade<kA174, kAMax, T07Instance>;
      this->x08_upgrade = new_upgrade<kA174, kAMax, x08>;
      this->x09_upgrade = new_upgrade<kA174, kAMax, x09>;
      this->x0A_upgrade = new_upgrade<kA174, kAMax, T0ADRC>;
      this->x0C_upgrade = new_upgrade<kA174, kAMax, x0C>;
      this->x0D_upgrade = new_upgrade<kA174, kAMax, x0D>;
      this->x0E_upgrade = new_upgrade<kA174, kAMax, x0E>;
      this->x10_upgrade = new_upgrade<kA174, kAMax, x10>;
      this->x14_upgrade = new_upgrade<kA174, kAMax, x14>;
      this->x15_upgrade = new_upgrade<kA174, kAMax, T15LineSegment>;
      this->x16_upgrade = new_upgrade<kA174, kAMax, T16LineSegment>;
      this->x17_upgrade = new_upgrade<kA174, kAMax, T17LineSegment>;
      this->x1B_upgrade = new_upgrade<kA174, kAMax, T1BNet>;
      this->x1C_upgrade = new_upgrade<kA174, kAMax, x1C>;
      this->t13_upgrade = new_upgrade<kA174, kAMax, t13>;
      this->x1D_upgrade = new_upgrade<kA174, kAMax, x1D>;
      this->x1F_upgrade = new_upgrade<kA174, kAMax, x1F>;
      this->x23_upgrade = new_upgrade<kA174, kAMax, x23>;
      this->x24_upgrade = new_upgrade<kA174, kAMax, x24>;
      this->x26_upgrade = new_upgrade<kA174, kAMax, x26>;
      this->x28_upgrade = new_upgrade<kA174, kAMax, x28>;
      this->x2B_upgrade = new_upgrade<kA174, kAMax, x2B>;
      this->x2C_upgrade = new_upgrade<kA174, kAMax, x2C>;
      this->x2D_upgrade = new_upgrade<kA174, kAMax, x2D>;
      this->x2E_upgrade = new_upgrade<kA174, kAMax, x2E>;
      this->x30_upgrade = new_upgrade<kA174, kAMax, x30>;
      this->x31_upgrade = new_upgrade<kA174, kAMax, x31>;
      this->x32_upgrade = new_upgrade<kA174, kAMax, x32>;
      this->x33_upgrade = new_upgrade<kA174, kAMax, x33>;
      this->x34_upgrade = new_upgrade<kA174, kAMax, x34>;
      this->x37_upgrade = new_upgrade<kA174, kAMax, x37>;
      this->x38_upgrade = new_upgrade<kA174, kAMax, t38_film>;
      this->x39_upgrade = new_upgrade<kA174, kAMax, t39_film_layer_list>;
      this->x3A_upgrade = new_upgrade<kA174, kAMax, t3A_film_layer_list_node>;
      this->x3C_upgrade = new_upgrade<kA174, kAMax, x3C>;
      break;
    case 0x00141502:
      this->x01_upgrade = new_upgrade<kAMax, kAMax, T01ArcSegment>;
      this->x03_upgrade = new_upgrade<kAMax, kAMax, x03>;
      this->x04_upgrade = new_upgrade<kAMax, kAMax, x04>;
      this->x05_upgrade = new_upgrade<kAMax, kAMax, T05Line>;
      this->x06_upgrade = new_upgrade<kAMax, kAMax, x06>;
      this->x07_upgrade = new_upgrade<kAMax, kAMax, T07Instance>;
      this->x08_upgrade = new_upgrade<kAMax, kAMax, x08>;
      this->x09_upgrade = new_upgrade<kAMax, kAMax, x09>;
      this->x0A_upgrade = new_upgrade<kAMax, kAMax, T0ADRC>;
      this->x0C_upgrade = new_upgrade<kAMax, kAMax, x0C>;
      this->x0D_upgrade = new_upgrade<kAMax, kAMax, x0D>;
      this->x0E_upgrade = new_upgrade<kAMax, kAMax, x0E>;
      this->x10_upgrade = new_upgrade<kAMax, kAMax, x10>;
      this->x14_upgrade = new_upgrade<kAMax, kAMax, x14>;
      this->x15_upgrade = new_upgrade<kAMax, kAMax, T15LineSegment>;
      this->x16_upgrade = new_upgrade<kAMax, kAMax, T16LineSegment>;
      this->x17_upgrade = new_upgrade<kAMax, kAMax, T17LineSegment>;
      this->x1B_upgrade = new_upgrade<kAMax, kAMax, T1BNet>;
      this->x1C_upgrade = new_upgrade<kAMax, kAMax, x1C>;
      this->t13_upgrade = new_upgrade<kAMax, kAMax, t13>;
      this->x1D_upgrade = new_upgrade<kAMax, kAMax, x1D>;
      this->x1F_upgrade = new_upgrade<kAMax, kAMax, x1F>;
      this->x23_upgrade = new_upgrade<kAMax, kAMax, x23>;
      this->x24_upgrade = new_upgrade<kAMax, kAMax, x24>;
      this->x26_upgrade = new_upgrade<kAMax, kAMax, x26>;
      this->x28_upgrade = new_upgrade<kAMax, kAMax, x28>;
      this->x2B_upgrade = new_upgrade<kAMax, kAMax, x2B>;
      this->x2C_upgrade = new_upgrade<kAMax, kAMax, x2C>;
      this->x2D_upgrade = new_upgrade<kAMax, kAMax, x2D>;
      this->x2E_upgrade = new_upgrade<kAMax, kAMax, x2E>;
      this->x30_upgrade = new_upgrade<kAMax, kAMax, x30>;
      this->x31_upgrade = new_upgrade<kAMax, kAMax, x31>;
      this->x32_upgrade = new_upgrade<kAMax, kAMax, x32>;
      this->x33_upgrade = new_upgrade<kAMax, kAMax, x33>;
      this->x34_upgrade = new_upgrade<kAMax, kAMax, x34>;
      this->x37_upgrade = new_upgrade<kAMax, kAMax, x37>;
      this->x38_upgrade = new_upgrade<kAMax, kAMax, t38_film>;
      this->x39_upgrade = new_upgrade<kAMax, kAMax, t39_film_layer_list>;
      this->x3A_upgrade = new_upgrade<kAMax, kAMax, t3A_film_layer_list_node>;
      this->x3C_upgrade = new_upgrade<kAMax, kAMax, x3C>;
      break;
  }
}

template <>
std::ptrdiff_t File<kAMax>::offset(void *p) {
  return (char *)p - (char *)region.get_address();
}

template <>
T01ArcSegment<kAMax> File<kAMax>::get_x01(uint32_t k) {
  return this->x01_upgrade(this->ptrs[k]);
}

template <>
const x03<kAMax> File<kAMax>::get_x03(uint32_t k) {
  size_t size;
  switch (this->hdr->magic) {
    case 0x00130000:
    case 0x00130200:
      size = sizeof_until_tail<x03<kA160>>();
      break;
    case 0x00130402:
      size = sizeof_until_tail<x03<kA162>>();
      break;
    case 0x00130C03:
      size = sizeof_until_tail<x03<kA164>>();
      break;
    case 0x00131003:
      size = sizeof_until_tail<x03<kA165>>();
      break;
    case 0x00131503:
    case 0x00131504:
      size = sizeof_until_tail<x03<kA166>>();
      break;
    case 0x00140400:
    case 0x00140500:
    case 0x00140600:
    case 0x00140700:
      size = sizeof_until_tail<x03<kA172>>();
      break;
    case 0x00140900:
    case 0x00140901:
    case 0x00140902:
    case 0x00140E00:
      size = sizeof_until_tail<x03<kA174>>();
      break;
    case 0x00141502:
      size = sizeof_until_tail<x03<kA175>>();
      break;
    default:
      size = 0;
  }

  void *p = this->ptrs[k];
  x03<kAMax> i = this->x03_upgrade(p);
  void *next_ptr = ((char *)p) + size;
  switch (i.subtype.t & 0xFF) {
    case 0x64:
    case 0x66:
    case 0x67:
    case 0x6A:
      i.ptr = *static_cast<uint32_t *>(next_ptr);
      i.has_str = false;
      break;
    case 0x6D:
    case 0x6E:
    case 0x6F:
    case 0x68:
    case 0x6B:
    case 0x71:
    case 0x73:
    case 0x78:
      i.s = std::string(static_cast<char *>(next_ptr));
      i.has_str = true;
      break;
  }
  return i;
}

template <>
const x04<kAMax> File<kAMax>::get_x04(uint32_t k) {
  return this->x04_upgrade(this->ptrs[k]);
}

template <>
const T05Line<kAMax> File<kAMax>::get_x05(uint32_t k) {
  return this->x05_upgrade(this->ptrs[k]);
}

template <>
const x06<kAMax> File<kAMax>::get_x06(uint32_t k) {
  return this->x06_upgrade(this->ptrs[k]);
}

template <>
const T07Instance<kAMax> File<kAMax>::get_x07(uint32_t k) {
  return this->x07_upgrade(this->ptrs[k]);
}

template <>
const x08<kAMax> File<kAMax>::get_x08(uint32_t k) {
  return this->x08_upgrade(this->ptrs[k]);
}

template <>
const x09<kAMax> File<kAMax>::get_x09(uint32_t k) {
  return this->x09_upgrade(this->ptrs[k]);
}

template <>
const T0ADRC<kAMax> File<kAMax>::get_x0A(uint32_t k) {
  return this->x0A_upgrade(this->ptrs[k]);
}

template <>
const x0C<kAMax> File<kAMax>::get_x0C(uint32_t k) {
  return this->x0C_upgrade(this->ptrs[k]);
}

template <>
const x0D<kAMax> File<kAMax>::get_x0D(uint32_t k) {
  return this->x0D_upgrade(this->ptrs[k]);
}

template <>
const x0E<kAMax> File<kAMax>::get_x0E(uint32_t k) {
  return this->x0E_upgrade(this->ptrs[k]);
}

template <>
const x10<kAMax> File<kAMax>::get_x10(uint32_t k) {
  return this->x10_upgrade(this->ptrs[k]);
}

template <>
const x14<kAMax> File<kAMax>::get_x14(uint32_t k) {
  return this->x14_upgrade(this->ptrs[k]);
}

template <>
const T15LineSegment<kAMax> File<kAMax>::get_x15(uint32_t k) {
  return this->x15_upgrade(this->ptrs[k]);
}

template <>
const T16LineSegment<kAMax> File<kAMax>::get_x16(uint32_t k) {
  return this->x16_upgrade(this->ptrs[k]);
}

template <>
const T17LineSegment<kAMax> File<kAMax>::get_x17(uint32_t k) {
  return this->x17_upgrade(this->ptrs[k]);
}

template <>
const T1BNet<kAMax> File<kAMax>::get_x1B(uint32_t k) {
  return this->x1B_upgrade(this->ptrs[k]);
}

template <>
const x1C<kAMax> File<kAMax>::get_x1C(uint32_t k) {
  void *p = this->ptrs[k];
  x1C<kAMax> i = this->x1C_upgrade(this->ptrs[k]);

  uint32_t obj_size, t13_size;
  if (this->hdr->magic < kA165) {
    obj_size = sizeof_until_tail<x1C<kA160>>();
    t13_size = sizeof_until_tail<t13<kA160>>();
  } else if (this->hdr->magic < kA172) {
    obj_size = sizeof_until_tail<x1C<kA165>>();
    t13_size = sizeof_until_tail<t13<kA165>>();
  } else {
    obj_size = sizeof_until_tail<x1C<kA172>>();
    t13_size = sizeof_until_tail<t13<kA172>>();
  }

  uint32_t count;
  if (this->hdr->magic < kA172) {
    count = 10 + i.layer_count * 3;
  } else {
    count = 21 + i.layer_count * 4;
  }

  void *next_ptr = ((char *)p) + obj_size;
  for (uint32_t j = 0; j < count; j++) {
    t13<kAMax> t13_inst = this->t13_upgrade(next_ptr);
    i.parts.push_back(t13_inst);
    next_ptr = ((char *)next_ptr) + t13_size;
  }

  return i;
}

template <>
const x1D<kAMax> File<kAMax>::get_x1D(uint32_t k) {
  return this->x1D_upgrade(this->ptrs[k]);
}

template <>
const x1F<kAMax> File<kAMax>::get_x1F(uint32_t k) {
  return this->x1F_upgrade(this->ptrs[k]);
}

template <>
const x23<kAMax> File<kAMax>::get_x23(uint32_t k) {
  return this->x23_upgrade(this->ptrs[k]);
}

template <>
const x24<kAMax> File<kAMax>::get_x24(uint32_t k) {
  return this->x24_upgrade(this->ptrs[k]);
}

template <>
const x26<kAMax> File<kAMax>::get_x26(uint32_t k) {
  return this->x26_upgrade(this->ptrs[k]);
}

template <>
const x28<kAMax> File<kAMax>::get_x28(uint32_t k) {
  return this->x28_upgrade(this->ptrs[k]);
}

template <>
const x2B<kAMax> File<kAMax>::get_x2B(uint32_t k) {
  return this->x2B_upgrade(this->ptrs[k]);
}

template <>
const x2C<kAMax> File<kAMax>::get_x2C(uint32_t k) {
  return this->x2C_upgrade(this->ptrs[k]);
}

template <>
const x2D<kAMax> File<kAMax>::get_x2D(uint32_t k) {
  return this->x2D_upgrade(this->ptrs[k]);
}

template <>
const x2E<kAMax> File<kAMax>::get_x2E(uint32_t k) {
  return this->x2E_upgrade(this->ptrs[k]);
}

template <>
const x30<kAMax> File<kAMax>::get_x30(uint32_t k) {
  return this->x30_upgrade(this->ptrs[k]);
}

template <>
const x31<kAMax> File<kAMax>::get_x31(uint32_t k) {
  void *p = this->ptrs[k];
  x31<kAMax> i = this->x31_upgrade(p);
  uint32_t size;
  if (this->hdr->magic < kA174) {
    size = sizeof_until_tail<x31<kA160>>();
  } else {
    size = sizeof_until_tail<x31<kA174>>();
  }
  void *next_ptr = ((char *)p) + size;
  i.s = std::string(static_cast<char *>(next_ptr));
  return i;
}

template <>
const x32<kAMax> File<kAMax>::get_x32(uint32_t k) {
  return this->x32_upgrade(this->ptrs[k]);
}

template <>
const x33<kAMax> File<kAMax>::get_x33(uint32_t k) {
  return this->x33_upgrade(this->ptrs[k]);
}

template <>
const x34<kAMax> File<kAMax>::get_x34(uint32_t k) {
  return this->x34_upgrade(this->ptrs[k]);
}

template <>
const x37<kAMax> File<kAMax>::get_x37(uint32_t k) {
  return this->x37_upgrade(this->ptrs[k]);
}

template <>
const t38_film<kAMax> File<kAMax>::get_t38_film(uint32_t k) {
  return this->x38_upgrade(this->ptrs[k]);
}

template <>
const t39_film_layer_list<kAMax> File<kAMax>::get_x39(uint32_t k) {
  return this->x39_upgrade(this->ptrs[k]);
}

template <>
const t3A_film_layer_list_node<kAMax> File<kAMax>::get_x3A(uint32_t k) {
  return this->x3A_upgrade(this->ptrs[k]);
}

template <>
const x3C<kAMax> File<kAMax>::get_x3C(uint32_t k) {
  void *p = this->ptrs[k];
  x3C<kAMax> inst = this->x3C_upgrade(p);
  uint32_t size;
  if (this->hdr->magic < kA174) {
    size = sizeof_until_tail<x3C<kA160>>();
  } else {
    size = sizeof_until_tail<x3C<kA174>>();
  }
  uint32_t *next_ptr = (uint32_t *)(((char *)p) + size);
  for (uint32_t i = 0; i < inst.size; i++) {
    inst.ptrs.push_back(*next_ptr);
    next_ptr++;
  }
  return inst;
}

template <>
bool File<kAMax>::is_type(uint32_t k, uint8_t t) {
  return (this->ptrs.count(k) > 0) && (*(uint8_t *)this->ptrs[k] == t);
}

/*
template <AllegroVersion version>
File<version>::operator File<kAMax>() const {
    File<kAMax> new_file;

    new_file.hdr = this->hdr;
    new_file.layers = this->layers;
    new_file.strings = this->strings;
    for (const auto& [k, v] : this->x01_map)
        new_file.x01_map[k] = upgrade<version, kAMax>(v);
    new_file.x03_map = this->x03_map;
    for (const auto& [k, v] : this->x04_map)
        new_file.x04_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x05_map)
        new_file.x05_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x06_map)
        new_file.x06_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x07_map)
        new_file.x07_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x08_map)
        new_file.x08_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x09_map)
        new_file.x09_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x0A_map)
        new_file.x0A_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x0C_map)
        new_file.x0C_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x0D_map)
        new_file.x0D_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x0E_map)
        new_file.x0E_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x0F_map)
        new_file.x0F_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x10_map)
        new_file.x10_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x11_map)
        new_file.x11_map[k] = upgrade<version, kAMax>(v);
    new_file.x12_map = this->x12_map;
    for (const auto& [k, v] : this->x14_map)
        new_file.x14_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x15_map)
        new_file.x15_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x16_map)
        new_file.x16_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x17_map)
        new_file.x17_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x1B_map)
        new_file.x1B_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x1C_map)
        new_file.x1C_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x1D_map)
        new_file.x1D_map[k] = upgrade<version, kAMax>(v);
    new_file.x1E_map = this->x1E_map;
    for (const auto& [k, v] : this->x1F_map)
        new_file.x1F_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x22_map)
        new_file.x22_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x23_map)
        new_file.x23_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x24_map)
        new_file.x24_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x26_map)
        new_file.x26_map[k] = upgrade<version, kAMax>(v);
    new_file.x27_db = this->x27_db;
    for (const auto& [k, v] : this->x28_map)
        new_file.x28_map[k] = upgrade<version, kAMax>(v);
    new_file.x2A_map = this->x2A_map;
    for (const auto& [k, v] : this->x2B_map)
        new_file.x2B_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x2C_map)
        new_file.x2C_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x2D_map)
        new_file.x2D_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x2E_map)
        new_file.x2E_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x30_map)
        new_file.x30_map[k] = upgrade<version, kAMax>(v);
    new_file.x31_map = this->x31_map;
    for (const auto& [k, v] : this->x32_map)
        new_file.x32_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x33_map)
        new_file.x33_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x34_map)
        new_file.x34_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x36_map)
        new_file.x36_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x37_map)
        new_file.x37_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x38_map)
        new_file.x38_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x39_map)
        new_file.x39_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x3A_map)
        new_file.x3A_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x3B_map)
        new_file.x3B_map[k] = upgrade<version, kAMax>(v);
    for (const auto& [k, v] : this->x3C_map)
        new_file.x3C_map[k] = upgrade<version, kAMax>(v);
    new_file.layer_count = this->layer_count;
    new_file.x27_end_pos = this->x27_end_pos;

    new_file.stackup_materials = this->stackup_materials;
    new_file.netlist_path = this->netlist_path;

    return new_file;
}

// Explicit instantiations required, because there is nothing to otherwise
// create them within the library code.
template File<A_160>::operator File<kAMax>() const;
template File<A_162>::operator File<kAMax>() const;
template File<A_164>::operator File<kAMax>() const;
template File<A_165>::operator File<kAMax>() const;
template File<A_166>::operator File<kAMax>() const;
template File<A_172>::operator File<kAMax>() const;
*/
