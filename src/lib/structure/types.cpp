#include "types.h"

template <>
x01<A_160>::operator x01<A_174>() const {
    x01<A_174> new_inst;
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
x03<A_160>::operator x03<A_174>() const {
    x03<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.k = this->k;
    new_inst.next = this->next;
    new_inst.subtype = this->subtype;
    new_inst.has_str = this->has_str;
    new_inst.s = this->s;
    new_inst.ptr = this->ptr;
    return new_inst;
}

template <>
x04<A_160>::operator x04<A_174>() const {
    x04<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.k = this->k;
    new_inst.next = this->next;
    new_inst.ptr1 = this->ptr1;
    new_inst.ptr2 = this->ptr2;
    return new_inst;
}

template <>
x05<A_160>::operator x05<A_174>() const {
    x05<A_174> new_inst;
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
x06<A_160>::operator x06<A_174>() const {
    x06<A_174> new_inst;
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
x07<A_160>::operator x07<A_174>() const {
    x07<A_174> new_inst;
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
x08<A_160>::operator x08<A_174>() const {
    x08<A_174> new_inst;
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
x09<A_160>::operator x09<A_174>() const {
    x09<A_174> new_inst;
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
x09<A_172>::operator x09<A_174>() const {
    x09<A_174> new_inst;
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
x0A<A_160>::operator x0A<A_174>() const {
    x0A<A_174> new_inst;
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
x0A<A_172>::operator x0A<A_174>() const {
    x0A<A_174> new_inst;
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
x0C<A_160>::operator x0C<A_174>() const {
    x0C<A_174> new_inst;
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
x0C<A_172>::operator x0C<A_174>() const {
    x0C<A_174> new_inst;
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
x0D<A_160>::operator x0D<A_174>() const {
    x0D<A_174> new_inst;
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
x0D<A_172>::operator x0D<A_174>() const {
    x0D<A_174> new_inst;
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
x0E<A_160>::operator x0E<A_174>() const {
    x0E<A_174> new_inst;
    new_inst.t = this->t;
    for (int i = 0; i < 13; i++) {
        new_inst.un[i] = this->un[i];
    }
    new_inst.un1[0] = 0;
    new_inst.un1[1] = 0;
    return new_inst;
}

template <>
x0F<A_160>::operator x0F<A_174>() const {
    x0F<A_174> new_inst;
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
x0F<A_172>::operator x0F<A_174>() const {
    x0F<A_174> new_inst;
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
x10<A_160>::operator x10<A_174>() const {
    x10<A_174> new_inst;
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
x10<A_172>::operator x10<A_174>() const {
    x10<A_174> new_inst;
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
x11<A_160>::operator x11<A_174>() const {
    x11<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.k = this->k;
    new_inst.ptr1 = this->ptr1;
    new_inst.ptr2 = this->ptr2;
    new_inst.ptr3 = this->ptr3;
    new_inst.un = this->un;
    return new_inst;
}

template <>
x14<A_160>::operator x14<A_174>() const {
    x14<A_174> new_inst;
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
x15<A_160>::operator x15<A_174>() const {
    x15<A_174> new_inst;
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
x16<A_160>::operator x16<A_174>() const {
    x16<A_174> new_inst;
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
x17<A_160>::operator x17<A_174>() const {
    x17<A_174> new_inst;
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
x1B<A_160>::operator x1B<A_174>() const {
    x1B<A_174> new_inst;
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
x1C_header<A_160>::operator x1C_header<A_174>() const {
    x1C_header<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.n = this->n;
    new_inst.un9 = this->un9;
    new_inst.k = this->k;
    new_inst.next = this->next;
    new_inst.pad_str = this->pad_str;
    new_inst.un0_0 = this->un0_0;
    new_inst.un0_1 = this->un0_1;
    new_inst.pad_path = this->pad_path;
    new_inst.un0_3 = this->un0_3;
    new_inst.un0_4 = this->un0_4;
    new_inst.un0_5 = this->un0_5;
    new_inst.un1 = this->un1;
    new_inst.un2_0 = this->un2_0;
    new_inst.un2_1 = this->un2_1;
    new_inst.un3 = this->un3;
    new_inst.size_hint = this->size_hint;
    for (int i = 0; i < 4; i++) {
        new_inst.coords2[i] = this->coords2[i];
    }
    new_inst.un4 = this->un4;
    new_inst.un5 = this->un5;
    new_inst.un6 = this->un6;
    return new_inst;
}

template <>
x1C_header<A_165>::operator x1C_header<A_174>() const {
    x1C_header<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.n = this->n;
    new_inst.un9 = this->un9;
    new_inst.k = this->k;
    new_inst.next = this->next;
    new_inst.pad_str = this->pad_str;
    new_inst.un0_0 = this->un0_0;
    new_inst.un0_1 = this->un0_1;
    new_inst.pad_path = this->pad_path;
    new_inst.un0_3 = this->un0_3;
    new_inst.un0_4 = this->un0_4;
    new_inst.un0_5 = this->un0_5;
    new_inst.un1 = this->un1;
    new_inst.un2_0 = this->un2_0;
    new_inst.un2_1 = this->un2_1;
    new_inst.un3 = this->un3;
    new_inst.size_hint = this->size_hint;
    for (int i = 0; i < 4; i++) {
        new_inst.coords2[i] = this->coords2[i];
    }
    new_inst.un4 = this->un4;
    new_inst.un5 = this->un5;
    new_inst.un6 = this->un6;
    return new_inst;
}

template <>
t13<A_160>::operator t13<A_174>() const {
    t13<A_174> new_inst;
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
x1C<A_160>::operator x1C<A_174>() const {
    x1C<A_174> new_inst;
    new_inst.hdr = this->hdr;
    for (const auto& v : this->parts) new_inst.parts.push_back(v);
    return new_inst;
}

template <>
x1C<A_165>::operator x1C<A_174>() const {
    x1C<A_174> new_inst;
    new_inst.hdr = this->hdr;
    for (const auto& v : this->parts)
        new_inst.parts.push_back(*reinterpret_cast<const t13<A_160>*>(&v));
    return new_inst;
}

template <>
x22<A_160>::operator x22<A_174>() const {
    x22<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.k = this->k;
    for (uint8_t i = 0; i < 8; i++) {
        new_inst.un[i] = this->un[i];
    }
    new_inst.un1 = 0;
    return new_inst;
}

template <>
x23<A_160>::operator x23<A_174>() const {
    return *reinterpret_cast<const x23<A_174>*>(this);
}

template <>
x23<A_164>::operator x23<A_174>() const {
    return *reinterpret_cast<const x23<A_174>*>(this);
}

template <>
x24<A_160>::operator x24<A_174>() const {
    return *reinterpret_cast<const x24<A_174>*>(this);
}

template <>
x26<A_160>::operator x26<A_174>() const {
    x26<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.k = this->k;
    new_inst.member_ptr = this->member_ptr;
    new_inst.group_ptr = this->group_ptr;
    new_inst.const_ptr = this->const_ptr;
    return new_inst;
}

template <>
x26<A_172>::operator x26<A_174>() const {
    x26<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.k = this->k;
    new_inst.member_ptr = this->member_ptr;
    new_inst.group_ptr = this->group_ptr;
    new_inst.const_ptr = this->const_ptr;
    return new_inst;
}

template <>
x28<A_160>::operator x28<A_174>() const {
    x28<A_174> new_inst;
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
x2B<A_160>::operator x2B<A_174>() const {
    x2B<A_174> new_inst;
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
x2B<A_164>::operator x2B<A_174>() const {
    x2B<A_174> new_inst;
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
x2C<A_160>::operator x2C<A_174>() const {
    x2C<A_174> new_inst;
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
x2D<A_160>::operator x2D<A_174>() const {
    x2D<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.layer = this->layer;
    new_inst.un0 = this->un0;
    new_inst.k = this->k;
    new_inst.un1 = this->un1;
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
x2E<A_160>::operator x2E<A_174>() const {
    return *reinterpret_cast<const x2E<A_174>*>(this);
}

template <>
x30<A_160>::operator x30<A_174>() const {
    x30<A_174> new_inst;
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
x30<A_172>::operator x30<A_174>() const {
    x30<A_174> new_inst;
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
x31<A_160>::operator x31<A_174>() const {
    x31<A_174> new_inst;
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
x32<A_160>::operator x32<A_174>() const {
    x32<A_174> new_inst;
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
x33<A_160>::operator x33<A_174>() const {
    x33<A_174> new_inst;
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
x34<A_160>::operator x34<A_174>() const {
    x34<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.subtype = this->subtype;
    new_inst.layer = this->layer;

    new_inst.k = this->k;

    new_inst.un1 = this->un1;
    new_inst.ptr1 = this->ptr1;
    new_inst.bitmask1 = this->bitmask1;
    new_inst.ptr2 = this->ptr2;
    new_inst.ptr3 = this->ptr3;
    return new_inst;
}

template <>
x36_x08<A_160>::operator x36_x08<A_174>() const {
    x36_x08<A_174> new_inst;
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
x36_x08<A_172>::operator x36_x08<A_174>() const {
    x36_x08<A_174> new_inst;
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
x36<A_160>::operator x36<A_174>() const {
    x36<A_174> new_inst;
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
    for (const auto& x08_inst : this->x08s) {
        new_inst.x08s.push_back(upgrade<A_160, A_174>(x08_inst));
    }
    for (const auto& x0F_inst : this->x0Fs) {
        x36_x0F i = *reinterpret_cast<const x36_x0F<A_174>*>(&x0F_inst);
        new_inst.x0Fs.push_back(i);
    }
    return new_inst;
}

template <>
x36<A_172>::operator x36<A_174>() const {
    x36<A_174> new_inst;
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
    for (const auto& x08_inst : this->x08s) {
        new_inst.x08s.push_back(upgrade<A_172, A_174>(x08_inst));
    }
    for (const auto& x0F_inst : this->x0Fs) {
        x36_x0F i = *reinterpret_cast<const x36_x0F<A_174>*>(&x0F_inst);
        new_inst.x0Fs.push_back(i);
    }
    return new_inst;
}

template <>
x37<A_160>::operator x37<A_174>() const {
    x37<A_174> new_inst;
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
x38<A_160>::operator x38<A_174>() const {
    x38<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.k = this->k;
    new_inst.next = this->next;
    new_inst.ptr1 = this->ptr1;
    new_inst.ptr = 0;
    new_inst.un2 = 0;
    new_inst.un3 = 0;
    new_inst.s = std::string(this->layer_name);
    return new_inst;
}

template <>
x38<A_166>::operator x38<A_174>() const {
    x38<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.k = this->k;
    new_inst.next = this->next;
    new_inst.ptr1 = this->ptr1;
    new_inst.ptr = this->ptr;
    new_inst.un2 = this->un2;
    new_inst.un3 = 0;
    new_inst.s = this->s;
    return new_inst;
}

template <>
x39<A_160>::operator x39<A_174>() const {
    return *reinterpret_cast<const x39<A_174>*>(this);
}

template <>
x3A<A_160>::operator x3A<A_174>() const {
    return *reinterpret_cast<const x3A<A_174>*>(this);
}

template <>
x3B<A_160>::operator x3B<A_174>() const {
    x3B<A_174> new_inst;
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
x3C<A_160>::operator x3C<A_174>() const {
    x3C<A_174> new_inst;
    new_inst.t = this->t;
    new_inst.k = this->k;
    new_inst.un = 0;
    new_inst.size = this->size;
    new_inst.ptrs = this->ptrs;
    return new_inst;
}

template <AllegroVersion version>
File<version>::File(mapped_region region) {
    this->region = std::move(region);
}

template File<A_174>::File(mapped_region region);

/*
template <AllegroVersion version>
File<version>::operator File<A_174>() const {
    File<A_174> new_file;

    new_file.hdr = this->hdr;
    new_file.layers = this->layers;
    new_file.strings = this->strings;
    for (const auto& [k, v] : this->x01_map)
        new_file.x01_map[k] = upgrade<version, A_174>(v);
    new_file.x03_map = this->x03_map;
    for (const auto& [k, v] : this->x04_map)
        new_file.x04_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x05_map)
        new_file.x05_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x06_map)
        new_file.x06_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x07_map)
        new_file.x07_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x08_map)
        new_file.x08_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x09_map)
        new_file.x09_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x0A_map)
        new_file.x0A_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x0C_map)
        new_file.x0C_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x0D_map)
        new_file.x0D_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x0E_map)
        new_file.x0E_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x0F_map)
        new_file.x0F_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x10_map)
        new_file.x10_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x11_map)
        new_file.x11_map[k] = upgrade<version, A_174>(v);
    new_file.x12_map = this->x12_map;
    for (const auto& [k, v] : this->x14_map)
        new_file.x14_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x15_map)
        new_file.x15_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x16_map)
        new_file.x16_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x17_map)
        new_file.x17_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x1B_map)
        new_file.x1B_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x1C_map)
        new_file.x1C_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x1D_map)
        new_file.x1D_map[k] = upgrade<version, A_174>(v);
    new_file.x1E_map = this->x1E_map;
    for (const auto& [k, v] : this->x1F_map)
        new_file.x1F_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x22_map)
        new_file.x22_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x23_map)
        new_file.x23_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x24_map)
        new_file.x24_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x26_map)
        new_file.x26_map[k] = upgrade<version, A_174>(v);
    new_file.x27_db = this->x27_db;
    for (const auto& [k, v] : this->x28_map)
        new_file.x28_map[k] = upgrade<version, A_174>(v);
    new_file.x2A_map = this->x2A_map;
    for (const auto& [k, v] : this->x2B_map)
        new_file.x2B_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x2C_map)
        new_file.x2C_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x2D_map)
        new_file.x2D_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x2E_map)
        new_file.x2E_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x30_map)
        new_file.x30_map[k] = upgrade<version, A_174>(v);
    new_file.x31_map = this->x31_map;
    for (const auto& [k, v] : this->x32_map)
        new_file.x32_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x33_map)
        new_file.x33_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x34_map)
        new_file.x34_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x36_map)
        new_file.x36_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x37_map)
        new_file.x37_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x38_map)
        new_file.x38_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x39_map)
        new_file.x39_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x3A_map)
        new_file.x3A_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x3B_map)
        new_file.x3B_map[k] = upgrade<version, A_174>(v);
    for (const auto& [k, v] : this->x3C_map)
        new_file.x3C_map[k] = upgrade<version, A_174>(v);
    new_file.layer_count = this->layer_count;
    new_file.x27_end_pos = this->x27_end_pos;

    new_file.stackup_materials = this->stackup_materials;
    new_file.netlist_path = this->netlist_path;

    return new_file;
}

// Explicit instantiations required, because there is nothing to otherwise
// create them within the library code.
template File<A_160>::operator File<A_174>() const;
template File<A_162>::operator File<A_174>() const;
template File<A_164>::operator File<A_174>() const;
template File<A_165>::operator File<A_174>() const;
template File<A_166>::operator File<A_174>() const;
template File<A_172>::operator File<A_174>() const;
*/
