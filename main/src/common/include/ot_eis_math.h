#ifndef OT_EIS_MATH_H
#define OT_EIS_MATH_H


/*
 * ..and if you can't take the strict
 * types, you can specify one yourself.
 *
 * Or not use min/max/clamp at all, of course.
 */
#define ot_eis_min_t(type, x, y) ({            \
    type __min1 = (x);            \
    type __min2 = (y);            \
    __min1 < __min2 ? __min1: __min2; })

#define ot_eis_max_t(type, x, y) ({            \
    type __max1 = (x);            \
    type __max2 = (y);            \
    __max1 > __max2 ? __max1: __max2; })

#define ot_eis_min3_t(type, x, y, z) ot_eis_min_t(type, ot_eis_min_t(type, x, y), z)
#define ot_eis_max3_t(type, x, y, z) ot_eis_min_t(type, ot_eis_max_t(type, x, y), z)

#define ot_eis_min(x, y) ({            \
    __typeof__(x) _min1 = (x);     \
    __typeof__(y) _min2 = (y);     \
    (void) (&_min1 == &_min2);     \
    _min1 < _min2 ? _min1 : _min2; \
})

#define ot_eis_max(x, y) ({            \
    __typeof__(x) _max1 = (x);     \
    __typeof__(y) _max2 = (y);     \
    ((void*)&_max1 == (void*)&_max2);     \
    _max1 > _max2 ? _max1 : _max2; \
})

#define ot_eis_min3(x, y, z) ot_eis_min((__typeof__(x))ot_eis_min(x, y), z)
#define ot_eis_max3(x, y, z) ot_eis_max((__typeof__(x))ot_eis_max(x, y), z)

#define ot_eis_up_align(x, a)           ((((x) + ((a) - 1)) / (a)) * (a))
#define ot_eis_down_align(x, a)         (((x) / (a)) * (a))

#endif
