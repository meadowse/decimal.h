#include "s21_decimal.h"

void s21_set0bitstype(s21_decimal *value) {
    s21_set0bits(value);
    // value->value_type = 0;
}

int s21_get_bit(const s21_decimal value, int bit) {
    return (value.bits[bit / 32] & (1u << (bit % 32))) ? 1 : 0;
}

void s21_set_bit(s21_decimal *value, int bit, int new_bit) {
    unsigned int mask = 1u << (bit % 32);
    if (new_bit) {
        value->bits[bit / 32] |= mask;
    } else {
        value->bits[bit / 32] &= ~mask;
    }
}

void s21_setsign(s21_decimal *value, int sign) {
    if (sign == 1) {
        value->bits[3] |= 0x80000000;
    } else {
        value->bits[3] &= ~0x80000000;
    }
}

int s21_getsign(const s21_decimal *value) {
    return !!(value->bits[3] & 0x80000000);  // use !! to get 1 in any event except 0
}

int s21_get_scale(const s21_decimal *value) { return (char)(value->bits[3] >> 16); }

void s21_set_scale(s21_decimal *value, int scale) {
    if (scale >= 0 && scale <= 28) {
        value->bits[3] &= ~(0xFF << 16);
        value->bits[3] |= scale << 16;
    }
}

void s21_shift_left(s21_decimal *value, int offset) {
    if (s21_last_bit(*value) + offset > 95) {
        // value->value_type = s21_infinity;
        s21_set_inf(value);
    } else {
        for (int i = 0; i < offset; i++) {
            int bit_31st = s21_get_bit(*value, 31);
            int bit_63d = s21_get_bit(*value, 63);
            value->bits[0] <<= 1;
            value->bits[1] <<= 1;
            value->bits[2] <<= 1;
            if (bit_31st) s21_set_bit(value, 32, 1);
            if (bit_63d) s21_set_bit(value, 64, 1);
        }
    }
}

int s21_last_bit(s21_decimal value) {
    int lb = 95;
    for (; lb >= 0 && s21_get_bit(value, lb) == 0; lb--) {
    }
    return lb;  // 0-95 - номер значащего бита, -1 - если все биты пустые
}

void to_addcode(s21_decimal *value) {
    s21_decimal res;
    s21_decimal add = {{1, 0, 0, 0}};

    value->bits[0] = ~value->bits[0];
    value->bits[1] = ~value->bits[1];
    value->bits[2] = ~value->bits[2];

    res = s21_add_bits(value, &add);

    value->bits[0] = res.bits[0];
    value->bits[1] = res.bits[1];
    value->bits[2] = res.bits[2];

    // value->value_type = s21_ADDCODE;
    s21_set_bit(value, 126, 1);  // ref
}

void s21_level_scale(s21_decimal *value1, s21_decimal *value2) {
    s21_decimal *bigger = value2, *smaller = value1, tmp = {{0, 0, 0, 0}};

    if (s21_get_scale(value1) > s21_get_scale(value2)) {
        bigger = value1;
        smaller = value2;
    }

    while (s21_get_scale(value1) != s21_get_scale(value2)) {
        // if (tmp.value_type == s21_usual) {
        if (!s21_check_inf(tmp)) {
            s21_decimal tmp1, tmp2;
            tmp1 = *smaller, tmp2 = *smaller;

            s21_shift_left(&tmp1, 1);
            s21_shift_left(&tmp2, 3);
            tmp = s21_add_bits(&tmp1, &tmp2);  // equals to multiplying by 10

            // if (tmp.value_type == s21_usual) {
            if (!s21_check_inf(tmp)) {
                s21_copy_bits(tmp, smaller);
                s21_set_scale(smaller, s21_get_scale(smaller) + 1);
            }
        } else {
            s21_decimal remainder, ten = {{10, 0, 0, 0}}, zero = {{0, 0, 0, 0}},
                                   tmp2 = s21_div_bits(*bigger, ten, &remainder);

            if (s21_are_zero(tmp2, zero) == 1) {
                s21_copy_bits(tmp2, bigger);
            } else {
                s21_copy_bits(remainder, bigger);  // bigger = 0
            }
            s21_set_scale(bigger, s21_get_scale(bigger) - 1);
        }
    }
}

s21_decimal s21_add_bits(s21_decimal *value1, s21_decimal *value2) {
    s21_decimal res = {{0, 0, 0, 0}};

    if (s21_are_inf_new(value1, value2)) {
        // res.value_type = s21_infinity;
        s21_set_inf(&res);
    } else {
        int buffer = 0;
        for (int i = 0; i < 96; i++) {
            int bit_value1 = s21_get_bit(*value1, i);
            int bit_value2 = s21_get_bit(*value2, i);

            if (!bit_value1 && !bit_value2) {
                if (buffer) {
                    s21_set_bit(&res, i, 1);
                    buffer = 0;
                } else {
                    s21_set_bit(&res, i, 0);
                }
            } else if (bit_value1 != bit_value2) {
                if (buffer) {
                    s21_set_bit(&res, i, 0);
                    buffer = 1;
                } else {
                    s21_set_bit(&res, i, 1);
                }
            } else {
                if (buffer) {
                    s21_set_bit(&res, i, 1);
                    buffer = 1;
                } else {
                    s21_set_bit(&res, i, 0);
                    buffer = 1;
                }
            }

            // if (i == 95 && buffer == 1 && value1->value_type != s21_ADDCODE &&
            //     value2->value_type != s21_ADDCODE)
            //   res.value_type = s21_infinity;
            // else
            //   res.value_type = s21_usual;

            if (i == 95 && buffer == 1 && s21_get_bit(*value1, 126) != 1 && s21_get_bit(*value2, 126) != 1)
                s21_set_inf(&res);
            // res.value_type = s21_infinity;
            //   else
            //     res.value_type = s21_usual;  // ref
        }
    }

    return res;
}

s21_decimal s21_check_boundary(s21_decimal value1, s21_decimal value2) {
    s21_decimal res = {{0, 0, 0, 0}};

    //   res.value_type = value1.value_type > value2.value_type ?
    //   value1.value_type
    //                                                          :
    //                                                          value2.value_type;

    // if (value1.value_type != s21_usual && value2.value_type != s21_usual &&
    //     value1.value_type != value2.value_type)
    //     res.value_type = s21_nan;
    if (s21_check_inf(value1) || s21_check_inf(value2)) s21_set_inf(&res);
    return res;
}

int s21_add(s21_decimal value1, s21_decimal value2, s21_decimal *res) {
    int ret = 0;
    *res = s21_check_boundary(value1, value2);

    // if (res->value_type == s21_usual || res->value_type == s21_ADDCODE ) {
    // if (res->value_type == s21_usual || s21_get_bit(*res, 126)) {
    if (!s21_check_inf(*res) || s21_get_bit(*res, 126)) {
        if (!s21_getsign(&value1) && !s21_getsign(&value2)) {  // positive
            if (s21_get_scale(&value1) != s21_get_scale(&value2)) s21_level_scale(&value1, &value2);

            s21_decimal tmp = s21_add_bits(&value1, &value2);

            //   if (tmp.value_type == s21_infinity && s21_get_scale(&value1) == 0) {
            if (s21_check_inf(tmp) && s21_get_scale(&value1) == 0) {
                // res->value_type = s21_infinity;
                s21_set_inf(res);
                //   } else if (tmp.value_type == s21_infinity &&
            } else if (s21_check_inf(tmp) && s21_get_scale(&value1) > 0) {  // lower scale
                // while (res->value_type == s21_infinity &&
                while (s21_check_inf(*res) &&
                       (s21_get_scale(&value1) > 0 && s21_get_scale(&value2) > 0)) {  // divide by 10
                    s21_decimal ten = {{10, 0, 0, 0}}, rem1, rem2, zero = {{0, 0, 0, 0}};
                    s21_decimal tmp1 = s21_div_bits(value1, ten, &rem1);
                    s21_decimal tmp2 = s21_div_bits(value2, ten, &rem2);

                    s21_are_zero(tmp1, zero) == 1 ? s21_copy_bits(tmp1, &value1)  // check for 0
                                                  : s21_copy_bits(rem1, &value1);
                    s21_are_zero(tmp2, zero) == 1 ? s21_copy_bits(tmp2, &value2)
                                                  : s21_copy_bits(rem2, &value2);

                    s21_set_scale(&value1, s21_get_scale(&value1) - 1);
                    s21_set_scale(&value2, s21_get_scale(&value2) - 1);
                    ret = s21_add(value1, value2, res);
                    return ret;
                }

            } else {
                *res = tmp;
                int tmp_add = s21_get_bit(*res, 126);
                res->bits[3] = value1.bits[3];
                s21_set_bit(res, 126, tmp_add);
            }
        } else if (s21_getsign(&value1) && !s21_getsign(&value2)) {  // 1 - negative, 2 - positive
            s21_setsign(&value1, 0);
            s21_sub(value2, value1, res);
        } else if (!s21_getsign(&value1) && s21_getsign(&value2)) {  // 2 - negative, 1 - positive
            s21_setsign(&value2, 0);
            s21_sub(value1, value2, res);
        } else {  // both negative
            s21_setsign(&value1, 0);
            s21_setsign(&value2, 0);
            s21_add(value1, value2, res);
            s21_setsign(res, 1);
            //   if (res->value_type == s21_infinity) {
            if (s21_check_inf(*res)) {
                s21_set_inf(res);
                s21_setsign(res, 1);
                // res->value_type = s21_neg_infinity;
                // s21_set0bits(res);
            }
        }
    }

    // if (res->value_type == s21_ADDCODE) res->value_type = s21_usual;
    //   if (s21_get_bit(*res, 126) == 1) res->value_type = s21_usual;  // ref
    //   if (res->value_type == s21_infinity) ret = 1;
    if (s21_check_inf(*res)) {
        ret = 1;
        // if (s21_getsign(res)) ret = 2;
        if (s21_getsign(&value1) || s21_getsign(&value2)) {
            ret = 2;
            s21_setsign(res, 1);
        }
    }
    //   if (res->value_type == s21_neg_infinity) ret = 2;
    return ret;
}

int s21_sub(s21_decimal number_1, s21_decimal number_2, s21_decimal *res) {
    int ret = 0;
    s21_set0bits(res);
    // res->value_type = number_1.value_type ? number_1.value_type :
    // number_2.value_type;
    if (s21_check_inf(number_1) || s21_check_inf(number_1)) s21_set_inf(res);

    if (!s21_check_inf(*res)) {
        if (s21_get_scale(&number_1) != s21_get_scale(&number_2)) s21_level_scale(&number_1, &number_2);

        int sign;
        if (s21_getsign(&number_1) != s21_getsign(&number_2)) {  // signs don't equal
            sign = s21_getsign(&number_1);
            s21_setsign(&number_1, 0);
            s21_setsign(&number_2, 0);
            s21_add(number_1, number_2, res);
            s21_setsign(res, sign);
        } else {                                      // signs equal
            if (!s21_is_equal(number_1, number_2)) {  // digits don't equals
                int sign1 = s21_getsign(&number_1);
                int sign2 = s21_getsign(&number_2);
                s21_setsign(&number_1, 0);
                s21_setsign(&number_2, 0);
                s21_decimal *smallPtr, *bigPtr;

                if (s21_is_less(number_1, number_2)) {
                    smallPtr = &number_1;
                    bigPtr = &number_2;
                    sign = !sign2;
                } else {
                    smallPtr = &number_2;
                    bigPtr = &number_1;
                    sign = sign1;
                }

                to_addcode(smallPtr);
                s21_add(*smallPtr, *bigPtr, res);
                s21_setsign(res, sign);
            }
        }
    }
    //   if (res->value_type == s21_infinity) ret = 1;
    //   if (res->value_type == s21_neg_infinity) ret = 2;
    if (s21_check_inf(*res)) {
        ret = 1;
        // if (s21_getsign(res)) ret = 2;
        if (s21_getsign(&number_1) || s21_getsign(&number_2)) {
            ret = 2;
            s21_setsign(res, 1);
        }
    }
    return ret;
}

// int s21_are_inf(s21_decimal *value1, s21_decimal *value2) {
//     //   int res = 0;

//     //   if (value1->value_type == s21_infinity &&
//     //       value2->value_type != s21_infinity) {
//     //     res = 1;
//     //   } else if (value1->value_type != s21_infinity &&
//     //              value2->value_type == s21_infinity) {
//     //     res = -1;
//     //   } else if (value1->value_type == s21_infinity &&
//     //              value2->value_type == s21_infinity) {
//     //     res = 2;
//     //   }
//     //   return res;  // 0 - не бесконечны, 1 - первое бесконечно, 2 - оба
//     //   бесконечны,
//     //                // -1 - второе бесконечно
// }

int s21_are_inf_new(s21_decimal *value1, s21_decimal *value2) {
    int res = 0;
    if (!s21_check_inf(*value2) && s21_check_inf(*value1)) {
        res = 1;
    }
    if (!s21_check_inf(*value1) && s21_check_inf(*value2)) {
        res = -1;
    }
    if (s21_check_inf(*value2) && s21_check_inf(*value1)) {
        res = 2;
    }
    return res;  // 0 - не бесконечны, 1 - первое бесконечно, 2 - оба
                 // бесконечны, -1 - второе бесконечно
}

int s21_check_inf(s21_decimal value) {
    int ret = 0;
    if ((s21_get_scale(&value) == 28) && (value.bits[0] == 0) && (value.bits[1] == 0) && (value.bits[2] == 0))
        ret = 1;
    return ret;  // 0 - non inf, 1 - inf
}

void s21_set_inf(s21_decimal *value) {
    s21_set_scale(value, 28);
    value->bits[0] = 0;
    value->bits[1] = 0;
    value->bits[2] = 0;
}

// int s21_are_neg_inf(s21_decimal * a, s21_decimal * b) {
//     //   int res = 0;
//     //   if (a->value_type == s21_neg_infinity &&
//     //       b->value_type != s21_neg_infinity)
//     //     res = 1;
//     //   if (a->value_type != s21_neg_infinity &&
//     //       b->value_type == s21_neg_infinity)
//     //     res = -1;
//     //   if (a->value_type == s21_neg_infinity &&
//     //       b->value_type == s21_neg_infinity)
//     //     res = 2;
//     //   return res;  // 0 - не бесконечны, 1 - первое бесконечно, 2 -
//     оба
//     //                // бесконечны, -1 - второе бесконечно
// }

int s21_are_neg(s21_decimal *value1, s21_decimal *value2) {
    int res = 0;
    int sign1 = s21_getsign(value1);
    int sign2 = s21_getsign(value2);

    if (!sign1 && sign2) res = 1;
    if (sign1 && !sign2) res = -1;

    return res;  // 0 - оба одного знака, 1 - первое положительное, -1 -
                 // второе положительное
}

void s21_check_scale(s21_decimal *value1, s21_decimal *value2) {
    if (s21_get_scale(value1) != s21_get_scale(value2)) {
        s21_level_scale(value1, value2);
    }
}

int s21_are_zero(s21_decimal a, s21_decimal b) {
    return (!a.bits[0] && !b.bits[0] && !a.bits[1] && !b.bits[1] && !a.bits[2] && !b.bits[2]) ? 0 : 1;
}

int s21_is_greater(s21_decimal value1, s21_decimal value2) {
    int is_greater = -1;
    // if (((value1.value_type == s21_nan || value2.value_type == s21_nan)) ||
    //   if ((!s21_are_zero(value1, value2) && !s21_are_inf(&value1, &value2)
    //   &&
    //        !s21_are_neg_inf(&value1, &value2)))
    if ((!s21_are_zero(value1, value2) && !s21_are_inf_new(&value1, &value2))) is_greater = 1;

    if (is_greater == -1) {
        // 0 - не бесконечны, 1 - первое бесконечно, 2 - оба бесконечны, -1 -
        // второе бесконечно
        // int if_inf = s21_are_inf(&value1, &value2);
        int if_inf = s21_are_inf_new(&value1, &value2);
        if (if_inf == 1) is_greater = 0;
        if (if_inf == -1 || if_inf == 2) is_greater = 1;

        // int if_neg_inf = s21_are_neg_inf(&value1, &value2);
        int if_neg_inf = s21_are_inf_new(&value1, &value2);
        if (s21_getsign(&value1) || s21_getsign(&value2)) {
            if (if_neg_inf == -1) is_greater = 0;
            if (if_neg_inf == 1 || if_neg_inf == 2) is_greater = 1;
        }
    }

    if (is_greater == -1) {
        // 0 - оба одного знака, 1 - первое положительное, -1 - второе
        // положительное
        int who_is_negative = s21_are_neg(&value1, &value2);
        if (who_is_negative == 1) is_greater = 0;
        if (who_is_negative == -1) is_greater = 1;

        s21_check_scale(&value1, &value2);
    }

    for (int i = 95; i >= 0 && is_greater == -1; i--) {
        if (s21_get_bit(value1, i) && !s21_get_bit(value2, i)) is_greater = 0;
        if (s21_get_bit(value2, i) && !s21_get_bit(value1, i)) is_greater = 1;
    }
    if ((is_greater != -1) && (s21_getsign(&value1) && s21_getsign(&value2))) is_greater = !is_greater;
    return !is_greater;  // 0 - больше, 1 - меньше
}

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
    return s21_is_greater(dec2, dec1);  // 1st: 0 - меньше, 1 - больше
}

int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2) {
    int ret;
    //   if (dec1.value_type == s21_nan || dec2.value_type == s21_nan) {
    //     ret = 1;
    //   } else {
    ret = (s21_is_greater(dec1, dec2));
    //   }
    return !ret;
}

int s21_is_equal(s21_decimal value1, s21_decimal value2) {
    int is_equal = -1;
    //   if ((value1.value_type == s21_nan || value2.value_type == s21_nan))
    //     is_equal = 1;

    if (s21_getsign(&value1) != s21_getsign(&value2)) is_equal = 1;

    if (is_equal == -1) {
        if (!s21_are_zero(value1, value2)) is_equal = 0;

        //     //   return res;  // 0 - не бесконечны, 1 - первое бесконечно, 2 -
        //     оба
        //     //   бесконечны,
        //     //                // -1 - второе бесконечно

        // int if_inf = s21_are_inf(&value1, &value2);
        int if_inf = s21_are_inf_new(&value1, &value2);
        if (if_inf == 1 || if_inf == -1) is_equal = 1;
        if (if_inf == 2) is_equal = 0;

        // int if_neg_inf = s21_are_inf_new(&value1, &value2);
        // if (s21_getsign(&value1) && s21_getsign(&value2)) {
        //     if (if_neg_inf == 1 || if_neg_inf == -1) is_equal = 1;
        //     if (if_neg_inf == 2) is_equal = 0;
        // }
    }

    if (is_equal == -1) {
        // if (s21_are_neg(&value1, &value2)) is_equal = 1;
        // if (s21_are_inf_new(&value1, &value2) && s21_getsign(&value1) &&
        // s21_getsign(&value2)) is_equal = 1;
        s21_check_scale(&value1, &value2);
    }

    for (int i = 95; i >= 0 && is_equal == -1; i--)
        if (s21_get_bit(value1, i) != s21_get_bit(value2, i)) is_equal = 1;
    return (is_equal != 1) ? 1 : 0;
}

int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) { return !(s21_is_equal(dec1, dec2)); }

int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
    return (s21_is_greater(dec1, dec2) || s21_is_equal(dec1, dec2));  // 1 - больше или равно, 0 - меньше
}

void s21_copy_bits(s21_decimal source, s21_decimal *dest) {
    dest->bits[0] = source.bits[0];
    dest->bits[1] = source.bits[1];
    dest->bits[2] = source.bits[2];
}

void s21_print(s21_decimal value) {
    for (int i = 127; i >= 0; i--) {
        printf("%d:%d ", i, s21_get_bit(value, i));
    }
    printf("\n");
}

int s21_from_int_to_decimal(int source, s21_decimal *dst) {
    int res = 0;
    if (dst) {
        s21_set0bitstype(dst);
        if (source < 0) {
            s21_setsign(dst, 1);
            source *= -1;
        }
        dst->bits[0] = source;
        // dst->value_type = 0;
    } else {
        res = 1;
    }
    return res;
}

int s21_from_decimal_to_int(s21_decimal source, int *dst) {
    int res = 1;
    s21_decimal i_max = {{INT_MAX, 0, 0, 0}}, i_min = i_max, one = {{1, 0, 0, 0}}, source2;
    s21_add(i_max, one, &i_min);
    s21_setsign(&i_min, 1);
    if ((!s21_check_inf(source)) && (!s21_is_greater(source, i_max)) && (!s21_is_less(source, i_min))) {
        s21_truncate(source, &source2);
        *dst = source2.bits[0];

        *dst /= pow(10, s21_get_scale(&source2));
        *dst *= s21_getsign(&source) ? -1 : 1;
        res = 0;
    }
    return res;
}

int getFloatExp(float *src) {
    int_float mant;
    mant.fl = *src;
    return ((mant.in & ~0x80000000) >> 23) - 127;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    s21_set0bitstype(dst);
    int result = 1, exp = getFloatExp(&src);
    unsigned sign = src < 0 ? 1 : 0;

    if (isinf(src) && !sign) s21_set_inf(dst);
    // dst->value_type = s21_infinity;
    else if (isinf(src) && sign) {
        s21_set_inf(dst);
        s21_setsign(dst, 1);
    }
    // dst->value_type = s21_neg_infinity;
    //   else if (isnan(src))
    //     dst->value_type = s21_nan;

    //   if (dst && dst->value_type == s21_usual && src != 0) {
    if (dst && !s21_check_inf(*dst) && src != 0) {
        double temp = (double)fabs(src);
        int off = 0;
        for (; off < 28 && (int)temp / (int)pow(2, 21) == 0; temp *= 10, off++) {
        }
        temp = round(temp);
        if (off <= 28 && (exp > -94 && exp < 96)) {
            int_float mant;
            temp = (float)temp;
            for (; fmod(temp, 10) == 0 && off > 0; off--, temp /= 10) {
            }
            mant.fl = temp;
            exp = getFloatExp(&mant.fl);
            dst->bits[exp / 32] |= 1 << exp % 32;
            for (int i = exp - 1, j = 22; j >= 0; i--, j--)
                if ((mant.in & (1 << j)) != 0) dst->bits[i / 32] |= 1 << i % 32;
            dst->bits[3] = (sign << 31) | (off << 16);
            result = 0;
        }
    }
    return result;
}

// TODO(alex): optimiation.
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    int result = 1;
    // float a = 1 * 0.0; //, b = 0.0 * 2;
    //   if (src.value_type == s21_usual) {
    if (!s21_check_inf(src)) {
        double temp = 0;
        int off = 0;
        for (unsigned i = 0; i < 96; i++)
            if (s21_get_bit(src, i)) temp += pow(2, i);
        if ((off = (src.bits[3] & ~0x80000000) >> 16) > 0) {
            for (int i = off; i > 0; i--) temp /= 10.0;
        }
        *dst = (float)temp;
        *dst *= s21_getsign(&src) ? -1 : 1;
        result = 0;
    }
    //   if (src.value_type == s21_infinity) *dst = 1.0 / 0.0;
    if (s21_check_inf(src)) *dst = 1.0 / 0.0;
    //   if (src.value_type == s21_neg_infinity) *dst = -1.0 / 0.0;
    if (s21_check_inf(src) && s21_getsign(&src)) *dst = -1.0 / 0.0;
    //   if (src.value_type == s21_nan) *dst = a / b;
    return result;
}

// TODO(alex): debug.
s21_decimal s21_div_bits(s21_decimal a, s21_decimal b, s21_decimal *rem) {
    s21_set0bitstype(rem);
    s21_decimal res = {{0, 0, 0, 0}};
    for (int i = s21_last_bit(a); i >= 0; i--) {
        if (s21_get_bit(a, i)) s21_set_bit(rem, 0, 1);
        if (s21_is_greater_or_equal(*rem, b)) {
            s21_sub(*rem, b, rem);
            if (i != 0) s21_shift_left(rem, 1);
            if (s21_get_bit(a, i - 1)) s21_set_bit(rem, 0, 1);
            s21_shift_left(&res, 1);
            s21_set_bit(&res, 0, 1);
        } else {
            s21_shift_left(&res, 1);
            if (i != 0) s21_shift_left(rem, 1);
            if ((i - 1) >= 0 && s21_get_bit(a, i - 1)) s21_set_bit(rem, 0, 1);
        }
    }
    return res;
}

s21_decimal check_for_mul(s21_decimal value1, s21_decimal value2) {
    s21_decimal res = {{0, 0, 0, 0}};

    // проверки аргументов
    // s21_decimal zero = {{0, 0, 0, 0}, s21_usual};
    //   int there_is_nan =
    //       ((value1.value_type == s21_nan || value2.value_type == s21_nan))
    //       ? 1 : 0;
    // int there_is_neg_inf = s21_getsign(&value1) || s21_getsign(&value2) ? 1 :
    // 0;
    //   int there_is_inf = s21_are_inf(&value1, &value2) != 0 ? 1 : 0;
    // int there_is_inf = s21_are_inf_new(&value1, &value2) != 0 ? 1 : 0;

    // int there_is_zero = s21_is_equal(value1, zero) == 1 ||
    // s21_is_equal(value2, zero) == 1 ? 1 : 0;

    // int there_is_plus_normal = (s21_is_greater(value1, zero) == 1 &&
    // value1.value_type == s21_usual) ||
    //                                    (s21_is_greater(value2, zero) == 1 &&
    //                                    value2.value_type == s21_usual)
    //                                ? 1
    //                                : 0;
    // int there_is_plus_normal = (s21_is_greater(value1, zero) == 1 &&
    // s21_check_inf(value1) == 0) ||
    //                                    (s21_is_greater(value2, zero) == 1 &&
    //                                    s21_check_inf(value2) == 0)
    //                                ? 1
    //                                : 0;

    //   int there_is_neg_normal =
    //       (s21_is_less(value1, zero) == 1 && value1.value_type == s21_usual) ||
    //               (s21_is_less(value2, zero) == 1 && value2.value_type ==
    //               s21_usual)
    //           ? 1
    //           : 0;
    // int there_is_neg_normal = (s21_is_less(value1, zero) == 1 &&
    // s21_check_inf(value1) == 0) ||
    //                                   (s21_is_less(value2, zero) == 1 &&
    //                                   s21_check_inf(value2) == 0)
    //                               ? 1
    //                               : 0;

    //   res.value_type = value1.value_type ? value1.value_type :
    //   value2.value_type;
    if (s21_check_inf(value1) || s21_check_inf(value2)) {
        s21_set_inf(&res);
        s21_setsign(&res, s21_getsign(&value1) ^ s21_getsign(&value2));
    }

    //   if (there_is_nan || (there_is_zero && there_is_inf) ||
    //       (there_is_zero && there_is_neg_inf)) {
    //     res.value_type = s21_nan;
    //   } else

    // todo(alex): sort out
    // if ((there_is_plus_normal && there_is_inf) || (there_is_neg_normal &&
    // there_is_neg_inf) ||
    //     (s21_check_inf(value1) && s21_check_inf(value2))) {
    //     s21_set_inf(&res);

    // } else if ((there_is_neg_normal && there_is_inf) || (there_is_inf &&
    // there_is_neg_inf) ||
    //            (there_is_plus_normal && there_is_neg_inf)) {
    //     s21_set_inf(&res);
    //     s21_setsign(&res, 1);
    // }

    return res;
}

int s21_mul(s21_decimal number_1, s21_decimal number_2, s21_decimal *res) {
    int ret = 0;
    *res = check_for_mul(number_1, number_2);

    // if (res->value_type == s21_usual) {
    if (s21_check_inf(*res) == 0) {
        int sign_result;

        if (s21_getsign(&number_1) != s21_getsign(&number_2)) {
            sign_result = 1;
        } else {
            sign_result = 0;
        }
        int last_bit_1 = s21_last_bit(number_1);
        s21_decimal tmp_res = {{0, 0, 0, 0}};

        // само умножение
        for (int i = 0; i <= last_bit_1; i++) {
            s21_set0bitstype(&tmp_res);
            int value_bit_1 = s21_get_bit(number_1, i);

            if (value_bit_1) {
                tmp_res = number_2;
                s21_shift_left(&tmp_res, i);
                *res = s21_add_bits(res, &tmp_res);
            }
        }

        // если на предыдущем этапе было выставлено INFINITY,
        // то если позволяет скейл, привести в нужный диапазон
        // и попробовать умножить снова

        // while (res->value_type != s21_usual &&
        while (s21_check_inf(*res) && (s21_get_scale(&number_1) > 0 || s21_get_scale(&number_2) > 0)) {
            // более точное число поделим на 10, если позволяет скейл

            s21_decimal *chosen_numberPtr, *other_numberPtr;
            if (s21_last_bit(number_1) > s21_last_bit(number_2) && s21_get_scale(&number_1) > 0) {
                chosen_numberPtr = &number_1;
                other_numberPtr = &number_2;
            } else if (s21_last_bit(number_2) > s21_last_bit(number_1) && s21_get_scale(&number_2) > 0) {
                chosen_numberPtr = &number_2;
                other_numberPtr = &number_1;
            } else {
                break;
            }

            int chos_number_scale = s21_get_scale(chosen_numberPtr);
            s21_decimal ten = {{10, 0, 0, 0}};
            s21_decimal remainder;
            s21_decimal tmpDiv = s21_div_bits(*chosen_numberPtr, ten, &remainder);

            // обезопасимся от обнуления
            s21_decimal zero = {{0, 0, 0, 0}};
            if (s21_are_zero(tmpDiv, zero) == 1) {
                s21_copy_bits(tmpDiv, chosen_numberPtr);
            } else {
                s21_copy_bits(remainder, chosen_numberPtr);
            }

            s21_set_scale(chosen_numberPtr, --chos_number_scale);
            s21_mul(*chosen_numberPtr, *other_numberPtr, res);
            return ret;
        }
        int scale = s21_get_scale(&number_1) + s21_get_scale(&number_2);
        s21_set_scale(res, scale);

        s21_setsign(res, sign_result);
    }

    //   if (res->value_type != s21_usual) s21_set0bits(res);
    // if (s21_check_inf(*res)) s21_set0bits(res);
    //   if (res->value_type == s21_neg_infinity) ret = 2;
    //   if (res->value_type == s21_infinity) ret = 1;

    if (s21_check_inf(*res)) {
        ret = 1;
        if (s21_getsign(res)) ret = 2;
    }
    return ret;
}

int s21_div(s21_decimal divident, s21_decimal divisor, s21_decimal *result) {
    int ret = 0;
    s21_set0bitstype(result);

    s21_decimal zero = {{0, 0, 0, 0}};
    // int divsr_is_normal_plus = s21_is_greater(divisor, zero) == 1 &&
    // divisor.value_type == s21_usual ? 1 : 0;
    int divsr_is_normal_plus = s21_is_greater(divisor, zero) == 1 && !s21_check_inf(divisor) ? 1 : 0;

    // int divsr_is_normal_minus = s21_is_less(divisor, zero) == 1 &&
    // divisor.value_type == s21_usual ? 1 : 0;
    int divsr_is_normal_minus = s21_is_less(divisor, zero) == 1 && !s21_check_inf(divisor) ? 1 : 0;

    int flag = 1;  // для отлова ситуации с normal/+-inf

    //   if (divident.value_type == s21_nan || divisor.value_type == s21_nan)
    //   {
    //     result->value_type = s21_nan;
    //   } else
    if (s21_is_equal(divisor, zero) == 1 && s21_is_less(divident, zero) == 1) {
        //    -x/0
        // result->value_type = s21_neg_infinity;
        s21_set_inf(result);
        s21_setsign(result, 1);
    } else if (s21_is_equal(divisor, zero) == 1 && s21_is_greater(divident, zero) == 1) {
        //   +x/0
        // result->value_type = s21_infinity;
        s21_set_inf(result);
    } else if (s21_is_equal(divisor, zero) == 1 && s21_is_equal(divident, zero) == 1) {
        //    0/0
        // result->value_type = s21_nan;
        //   } else if ((divident.value_type == s21_infinity ||
        //               divident.value_type == s21_neg_infinity) &&
        //              (divisor.value_type == s21_infinity ||
        //               divisor.value_type == s21_neg_infinity)) {
        //    +-inf/(+-inf)
        // result->value_type = s21_nan;
    } else if ((s21_check_inf(divident) && divsr_is_normal_plus) ||
               (s21_check_inf(divident) && s21_getsign(&divident) && divsr_is_normal_minus)) {
        //    +inf / +normal
        //    -inf / -normal
        // result->value_type = s21_infinity;
        s21_set_inf(result);
    } else if ((s21_check_inf(divident) && divsr_is_normal_minus) ||
               (s21_check_inf(divident) && s21_getsign(&divident) && divsr_is_normal_plus)) {
        //    +inf / -normal = -inf
        //    -inf / +normal
        // result->value_type = s21_neg_infinity;
        s21_set_inf(result);
        s21_setsign(result, 1);
    } else if (s21_check_inf(divisor)) {
        //    +-normal / +-inf = 0
        *result = zero;
        flag = 0;
    }

    // if (result->value_type == s21_usual && flag) {
    if (!s21_check_inf(*result) && flag) {
        int beginScale = s21_get_scale(&divident) - s21_get_scale(&divisor);
        int resultSign = s21_getsign(&divident) != s21_getsign(&divisor);

        s21_decimal remainder, tmp;

        // для предсказуемости зачищаем
        s21_set_scale(&divisor, 0);
        s21_set_scale(&divident, 0);
        s21_setsign(&divisor, 0);
        s21_setsign(&divident, 0);

        // первое целочисленное деление
        tmp = s21_div_bits(divident, divisor, &remainder);
        s21_copy_bits(tmp, result);

        // 1/10 от макс значения децимал - граница от переполнения
        s21_decimal border_value = {{-1, -1, -1, 0}};
        s21_decimal ten = {{10, 0, 0, 0}};

        s21_set_scale(&border_value, 1);
        int inside_scale = 0;

        // делим, пока не достигнем максимальной точности или пока не поделим
        // без остатка

        for (; inside_scale <= 27 && s21_is_equal(remainder, zero) == 0;) {
            if (s21_is_less(*result, border_value) == 0) {
                break;
            }
            s21_mul(remainder, ten, &remainder);
            tmp = s21_div_bits(remainder, divisor, &remainder);
            s21_mul(*result, ten, result);
            s21_add(*result, tmp, result);
            inside_scale++;
        }

        s21_decimal musor;
        // вводим итоговый скейл в требуемые границы
        int endScale = beginScale + inside_scale;
        for (; endScale > 28;) {
            *result = s21_div_bits(*result, ten, &musor);
            endScale--;
        }
        for (; endScale < 0;) {
            s21_mul(*result, ten, result);
            endScale++;
        }

        s21_set_scale(result, endScale);
        s21_setsign(result, resultSign);
    }

    //   if (result->value_type == s21_infinity) ret = 1;
    //   if (result->value_type == s21_neg_infinity) ret = 2;

    if (s21_check_inf(*result)) {
        ret = 1;
        if (s21_getsign(&divident) != s21_getsign(&divisor)) ret = 2;
    }
    if (s21_is_equal(divisor, zero)) ret = 3;
    return ret;
}

int s21_mod(s21_decimal a, s21_decimal b, s21_decimal *res) {
    s21_decimal tmp, zero = {{0, 0, 0, 0}};
    int ret = 0;

    s21_set0bitstype(&tmp);  //  на всякий случай?
    s21_div(a, b, &tmp);
    //   if ((tmp.value_type == s21_infinity) ||
    //       (tmp.value_type == s21_neg_infinity)) {
    if (s21_check_inf(tmp)) {
        *res = zero;
    } else {
        s21_truncate(tmp, &tmp);  // todo(alex): rest

        s21_mul(tmp, b, &tmp);

        s21_sub(a, tmp, res);
    }
    //   if ((s21_is_equal(b, zero)) && (a.value_type != s21_nan)) {
    //     ret = 3;
    //     res->value_type = s21_nan;
    //   }
    //   if (((a.value_type == s21_infinity) || (a.value_type ==
    //   s21_neg_infinity)) &&
    //       (b.value_type != s21_nan))
    //     res->value_type = s21_nan;

    if (s21_check_inf(b)) {
        //   if (((b.value_type == s21_infinity) ||
        //        (b.value_type == s21_neg_infinity)) &&
        //       (a.value_type == s21_usual)) {
        ret = 0;
        *res = a;
    }
    //   if ((b.value_type == s21_nan)) {  //  || (b.value_type == s21_nan)
    //     res->value_type = s21_nan;
    //   }
    return ret;
}

int s21_truncate(s21_decimal value, s21_decimal *res) {
    s21_decimal ten = {{10, 0, 0, 0}}, tmp = ten;
    int ret = 0, sign = s21_getsign(&value), scale = s21_get_scale(&value);

    *res = value;
    if (!s21_check_inf(value)) {
        for (int i = scale; i > 0; i--) {  // divide by 10 in the loop within the scale
            *res = s21_div_bits(value, ten, &tmp);
            value = *res;
        }

    } else {  // inf or nan
        *res = value;
        ret = 1;
    }
    if (sign) s21_setsign(res, 1);
    return ret;
}

int s21_round(s21_decimal value, s21_decimal *res) {
    int ret = 0, sign = s21_getsign(&value);
    s21_decimal rem, whole, one = {{1, 0, 0, 0}}, five = {{5, 0, 0, 0}};

    s21_setsign(&value, 0);
    s21_truncate(value, &whole);
    s21_sub(value, whole, &rem);
    s21_set_scale(&five, 1);

    if (!s21_check_inf(value)) {
        *res = whole;
        if (s21_is_greater_or_equal(rem, five)) s21_add(*res, one, res);

    } else {
        // res->value_type = value.value_type;
        s21_set_inf(res);
        ret = 1;
    }
    s21_setsign(res, sign);
    return ret;
}

int s21_floor(s21_decimal value, s21_decimal *res) {
    s21_decimal tmp, dec1_copy = value, one = {{1, 0, 0, 0}}, ten = {{10, 0, 0, 0}};
    //   int ret = 0, type = !value.value_type, sign = s21_getsign(&value),
    int ret = 0, type = !s21_check_inf(value), sign = s21_getsign(&value), scale = s21_get_scale(&value);

    if (s21_check_inf(value)) {
        ret = 1;
    } else {
        for (int i = scale; i > 0; i--) value = s21_div_bits(value, ten, &tmp);
        s21_set_scale(&value, 0);

        if (s21_is_equal(value, dec1_copy)) type = 0;
        if (sign && type) {
            s21_add(value, one, &value);
            s21_setsign(&value, 1);
        }
    }
    *res = value;

    return ret;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
    int ret = 0;
    *result = value;
    //   if (result->value_type != s21_nan) {
    s21_setsign(result, s21_getsign(result) ^ 1);
    if (s21_check_inf(*result)) {
        //   if (result->value_type == s21_infinity) {
        // result->value_type = s21_neg_infinity;
        s21_set_inf(result);
    } else {
        if (s21_check_inf(*result) && s21_getsign(result)) {
            s21_set_inf(result);
            s21_setsign(result, 1);
        }
        //   result->value_type = s21_infinity;
    }
    //   } else {  // inf or nan
    //     ret = 1;
    //   }
    return ret;
}

void s21_set0bits(s21_decimal *ptr) { memset(ptr->bits, 0, sizeof(ptr->bits)); }
