#include <check.h>
#include <stdio.h>

#include "s21_decimal.h"

START_TEST(less) {
    s21_decimal dst1, dst2;

    s21_from_float_to_decimal(1.2345, &dst1);
    s21_from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(s21_is_less(dst1, dst2), 0);  // 1.2345 < 1.2
    ck_assert_int_eq(s21_is_less(dst2, dst1), 1);  // 1.2 < 1.2345

    s21_negate(dst1, &dst1);
    s21_negate(dst2, &dst2);
    ck_assert_int_eq(s21_is_less(dst1, dst2), 1);  // -1.2345 < -1.2
    ck_assert_int_eq(s21_is_less(dst2, dst1), 0);  // -1.2 < -1.2345

    s21_negate(dst1, &dst1);
    ck_assert_int_eq(s21_is_less(dst1, dst2), 0);  //  1.2345 < -1.2 fail
    ck_assert_int_eq(s21_is_less(dst2, dst1), 1);  //  -1.2 < 1.2345 fail

    s21_from_float_to_decimal(0.0, &dst1);
    s21_from_float_to_decimal(0.0, &dst2);
    ck_assert_int_eq(s21_is_less(dst1, dst2), 0);  // 0 < 0

    // dst1.value_type = s21_infinity;
    s21_set_inf(&dst1);
    // dst2.value_type = s21_usual;
    ck_assert_int_eq(s21_is_less(dst1, dst2), 0);  // +INFINITY < 0
    ck_assert_int_eq(s21_is_less(dst2, dst1), 1);  // 0 < +INFINITY

    // dst1.value_type = s21_neg_infinity;
    s21_setsign(&dst1, 1);
    s21_set_inf(&dst1);
    // dst2.value_type = s21_usual;
    ck_assert_int_eq(s21_is_less(dst1, dst2), 1);  // -INFINITY < 0
    ck_assert_int_eq(s21_is_less(dst2, dst1), 0);  // 0 < -INFINITY

    // dst1.value_type = s21_nan;
    // dst2.value_type = s21_usual;
    // ck_assert_int_eq(s21_is_less(dst1, dst2), 0);  // NAN < 0
}
END_TEST

START_TEST(greater) {
    s21_decimal dst1, dst2;

    s21_from_float_to_decimal(1.2345, &dst1);
    s21_from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(s21_is_greater(dst1, dst2), 1);  // 1.2345 > 1.2
    ck_assert_int_eq(s21_is_greater(dst2, dst1), 0);  // 1.2 > 1.2345

    s21_negate(dst1, &dst1);
    s21_negate(dst2, &dst2);
    ck_assert_int_eq(s21_is_greater(dst1, dst2), 0);  // -1.2345 > -1.2
    ck_assert_int_eq(s21_is_greater(dst2, dst1), 1);  // -1.2 > 1.2345

    s21_negate(dst1, &dst1);
    ck_assert_int_eq(s21_is_greater(dst1, dst2), 1);  //  1.2345 > -1.2
    ck_assert_int_eq(s21_is_greater(dst2, dst1), 0);  //  -1.2 > 1.2345

    s21_from_float_to_decimal(0.0, &dst1);
    s21_from_float_to_decimal(0.0, &dst2);
    ck_assert_int_eq(s21_is_greater(dst1, dst2), 0);  // 0 > 0

    //   dst1.value_type = s21_infinity;
    s21_set_inf(&dst1);

    // dst2.value_type = s21_usual;
    ck_assert_int_eq(s21_is_greater(dst1, dst2), 1);  // +INFINITY > 0
    ck_assert_int_eq(s21_is_greater(dst2, dst1), 0);  // 0 > +INFINITY

    //   dst1.value_type = s21_neg_infinity;
    s21_setsign(&dst1, 1);
    s21_set_inf(&dst1);

    // dst2.value_type = s21_usual;
    ck_assert_int_eq(s21_is_greater(dst1, dst2), 0);  // -INFINITY > 0
    ck_assert_int_eq(s21_is_greater(dst2, dst1), 1);  // 0 > -INFINITY

    // dst1.value_type = s21_nan;
    // dst2.value_type = s21_usual;
    // ck_assert_int_eq(s21_is_greater(dst1, dst2), 0);  // NAN > 0
    // ck_assert_int_eq(s21_is_greater(dst2, dst1), 0);  // 0 > NAN
}
END_TEST

START_TEST(less_or_equal) {
    s21_decimal dst1, dst2;

    s21_from_float_to_decimal(1.2345, &dst1);
    s21_from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(s21_is_less_or_equal(dst1, dst2), 0);  // 1.2345 <= 1.2
    ck_assert_int_eq(s21_is_less_or_equal(dst2, dst1), 1);  // 1.2 <= 1.2345

    s21_negate(dst1, &dst1);
    s21_negate(dst2, &dst2);
    ck_assert_int_eq(s21_is_less_or_equal(dst1, dst2),
                     1);  // -1.2345 <= -1.2
    ck_assert_int_eq(s21_is_less_or_equal(dst2, dst1),
                     0);  // -1.2 <= -1.2345

    s21_negate(dst1, &dst1);
    ck_assert_int_eq(s21_is_less_or_equal(dst1, dst2),
                     0);  //  1.2345 <= -1.2 // fail
    ck_assert_int_eq(s21_is_less_or_equal(dst2, dst1),
                     1);  //  -1.2 <= 1.2345

    s21_from_float_to_decimal(0.0, &dst1);
    s21_from_float_to_decimal(0.0, &dst2);
    ck_assert_int_eq(s21_is_less_or_equal(dst1, dst2), 1);  // 0 <= 0

    // dst1.value_type = s21_infinity;
    s21_set_inf(&dst1);
    // dst2.value_type = s21_usual;
    ck_assert_int_eq(s21_is_less_or_equal(dst1, dst2),
                     0);                                    // +INFINITY <= 0
    ck_assert_int_eq(s21_is_less_or_equal(dst2, dst1), 1);  // 0 <= +INFINITY

    // dst1.value_type = s21_infinity;
    // dst2.value_type = s21_infinity;
    s21_set_inf(&dst1);
    s21_set_inf(&dst2);
    ck_assert_int_eq(s21_is_less_or_equal(dst2, dst1),
                     1);  // +INFINITY <= +INFINITY

    // dst1.value_type = s21_neg_infinity;
    s21_set_inf(&dst1);
    s21_setsign(&dst1, 1);
    // dst2.value_type = s21_usual;
    ck_assert_int_eq(s21_is_less_or_equal(dst1, dst2), 1);  // -INFINITY <= 0
    ck_assert_int_eq(s21_is_less_or_equal(dst2, dst1),
                     1);  // 0 <= -INFINITY

    // dst1.value_type = s21_neg_infinity;
    // dst2.value_type = s21_neg_infinity;
    s21_set_inf(&dst1);
    s21_setsign(&dst1, 1);
    s21_set_inf(&dst2);
    s21_setsign(&dst2, 1);
    ck_assert_int_eq(s21_is_less_or_equal(dst2, dst1),
                     0);  // -INFINITY <= -INFINITY

    // dst1.value_type = s21_nan;
    // dst2.value_type = s21_usual;
    // ck_assert_int_eq(s21_is_less_or_equal(dst1, dst2), 0);  // NAN <= 0
    // ck_assert_int_eq(s21_is_less_or_equal(dst2, dst1),
    //                  0);  // 0 <= NAN

    s21_from_float_to_decimal(1726.73, &dst1);
    s21_from_float_to_decimal(1726.73, &dst2);
    ck_assert_int_eq(s21_is_less_or_equal(dst1, dst2),
                     1);  // 1726.73 <= 1726.73
}
END_TEST

START_TEST(greater_or_equal) {
    s21_decimal dst1, dst2;

    s21_from_float_to_decimal(1.2345, &dst1);
    s21_from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(s21_is_greater_or_equal(dst1, dst2),
                     1);  // 1.2345 >= 1.2
    ck_assert_int_eq(s21_is_greater_or_equal(dst2, dst1),
                     0);  // 1.2 >= 1.2345

    s21_negate(dst1, &dst1);
    s21_negate(dst2, &dst2);
    ck_assert_int_eq(s21_is_greater_or_equal(dst1, dst2),
                     0);  // -1.2345 >= -1.2
    ck_assert_int_eq(s21_is_greater_or_equal(dst2, dst1),
                     1);  // -1.2 >= 1.2345

    s21_negate(dst1, &dst1);
    ck_assert_int_eq(s21_is_greater_or_equal(dst1, dst2),
                     1);  //  1.2345 >= -1.2
    ck_assert_int_eq(s21_is_greater_or_equal(dst2, dst1),
                     0);  //  -1.2 >= 1.2345

    s21_from_float_to_decimal(0.0, &dst1);
    s21_from_float_to_decimal(0.0, &dst2);
    ck_assert_int_eq(s21_is_greater_or_equal(dst1, dst2), 1);  // 0 >= 0

    s21_set_inf(&dst1);
    // s21_setsign(&dst1, 1);
    // dst2.value_type = s21_usual;
    ck_assert_int_eq(s21_is_greater_or_equal(dst1, dst2),
                     1);  // +INFINITY >= 0
    ck_assert_int_eq(s21_is_greater_or_equal(dst2, dst1),
                     0);  // 0 >= +INFINITY

    // dst1.value_type = s21_infinity;
    // dst2.value_type = s21_infinity;
    s21_set_inf(&dst1);
    s21_set_inf(&dst2);
    ck_assert_int_eq(s21_is_greater_or_equal(dst1, dst2),
                     1);  // +INFINITY >= +INFINITY

    // dst1.value_type = s21_neg_infinity;
    s21_set_inf(&dst1);
    s21_setsign(&dst1, 1);
    // dst2.value_type = s21_usual;
    s21_set0bitstype(&dst2);
    ck_assert_int_eq(s21_is_greater_or_equal(dst1, dst2),
                     0);  // -INFINITY >= 0

    // ck_assert_int_eq(s21_is_greater_or_equal(dst2, dst1),
    //                  1);  // 0 >= -INFINITY
    // printf("isg: %d\n", s21_are_inf_new(&dst1, &dst2));
    ck_assert_int_eq(s21_is_greater(dst2, dst1),
                     1);  // 0 >= -INFINITY

    // dst1.value_type = s21_neg_infinity;
    // dst2.value_type = s21_neg_infinity;
    s21_set_inf(&dst1);
    s21_setsign(&dst1, 1);
    s21_set_inf(&dst2);
    s21_setsign(&dst2, 1);
    ck_assert_int_eq(s21_is_greater_or_equal(dst1, dst2),
                     1);  // -INFINITY >= -INFINITY

    // dst1.value_type = s21_nan;
    // dst2.value_type = s21_usual;
    // ck_assert_int_eq(s21_is_greater_or_equal(dst1, dst2),
    //                  0);  // NAN >= 0
    // ck_assert_int_eq(s21_is_greater_or_equal(dst2, dst1),
    //                  0);  // 0 >= NAN
}
END_TEST

START_TEST(equal) {
    s21_decimal dst1, dst2;

    s21_from_float_to_decimal(1.2345, &dst1);
    s21_from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(s21_is_equal(dst1, dst2), 0);  // 1.2345 = 1.2

    s21_from_float_to_decimal(1.2, &dst1);
    s21_from_float_to_decimal(1.2, &dst2);
    // dst1.value_type = s21_neg_infinity;
    // dst2.value_type = s21_neg_infinity;
    s21_set_inf(&dst1);
    s21_set_inf(&dst2);
    s21_setsign(&dst1, 1);
    s21_setsign(&dst2, 1);
    ck_assert_int_eq(s21_is_equal(dst1, dst2), 1);  // 1.2 = 1.2

    s21_from_float_to_decimal(-234.17, &dst1);
    s21_from_float_to_decimal(234.17, &dst2);
    ck_assert_int_eq(s21_is_equal(dst1, dst2), 0);

    s21_from_float_to_decimal(234.17, &dst1);
    s21_from_float_to_decimal(234.17, &dst2);
    ck_assert_int_eq(s21_is_equal(dst1, dst2), 1);

    // dst1.value_type = s21_infinity;
    // dst2.value_type = s21_infinity;
    s21_set_inf(&dst1);
    s21_set_inf(&dst2);
    ck_assert_int_eq(s21_is_equal(dst2, dst1),
                     1);  // +INFINITY = +INFINITY

    // dst1.value_type = s21_infinity;
    // dst2.value_type = s21_neg_infinity;
    s21_set_inf(&dst1);
    s21_set_inf(&dst2);
    s21_setsign(&dst2, 1);
    ck_assert_int_eq(s21_is_equal(dst2, dst1),
                     0);  // +INFINITY != -INFINITY

    // dst1.value_type = s21_neg_infinity;
    // dst2.value_type = s21_neg_infinity;
    s21_set_inf(&dst1);
    s21_set_inf(&dst2);
    s21_setsign(&dst2, 1);
    s21_setsign(&dst1, 1);
    ck_assert_int_eq(s21_is_equal(dst2, dst1),
                     1);  // -INFINITY = -INFINITY

    // dst1.value_type = s21_nan;
    // dst2.value_type = s21_infinity;
    // ck_assert_int_eq(s21_is_equal(dst2, dst1),
    //                  0);  // NAN = -INFINITY

    // dst1.value_type = s21_nan;
    // dst2.value_type = s21_nan;
    // ck_assert_int_eq(s21_is_equal(dst2, dst1),
    //                  0);  // NAN != NAN
}
END_TEST

START_TEST(not_equal) {
    s21_decimal dst1, dst2;

    s21_from_float_to_decimal(1.2345, &dst1);
    s21_from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(s21_is_not_equal(dst1, dst2), 1);  // 1.2345 = 1.2

    s21_from_float_to_decimal(1.2, &dst1);
    s21_from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(s21_is_not_equal(dst1, dst2), 0);  // 1.2 = 1.2

    // dst1.value_type = s21_neg_infinity;
    // dst2.value_type = s21_neg_infinity;
    s21_set_inf(&dst1);
    s21_set_inf(&dst2);
    s21_setsign(&dst2, 1);
    s21_setsign(&dst1, 1);
    ck_assert_int_eq(s21_is_not_equal(dst1, dst2), 0);

    // dst1.value_type = s21_infinity;
    // dst2.value_type = s21_infinity;
    s21_set_inf(&dst1);
    s21_set_inf(&dst2);
    ck_assert_int_eq(s21_is_not_equal(dst1, dst2), 0);

    // dst1.value_type = s21_nan;
    // dst2.value_type = s21_nan;
    // ck_assert_int_eq(s21_is_not_equal(dst1, dst2), 1);

    s21_from_float_to_decimal(234.17, &dst1);
    s21_from_float_to_decimal(234.17, &dst2);
    ck_assert_int_eq(s21_is_not_equal(dst1, dst2), 0);
}
END_TEST

START_TEST(int_to_decimal) {
    s21_decimal val;

    s21_from_int_to_decimal(0, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);

    s21_from_int_to_decimal(-128, &val);
    ck_assert_int_eq(val.bits[0], 128);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], -2147483648);

    s21_from_int_to_decimal(127, &val);
    ck_assert_int_eq(val.bits[0], 127);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);

    s21_from_int_to_decimal(-2147483648, &val);
    ck_assert_int_eq(val.bits[0], -2147483648);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], -2147483648);

    s21_from_int_to_decimal(2147483647, &val);
    ck_assert_int_eq(val.bits[0], 2147483647);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST

START_TEST(decimal_to_int1) {
    s21_decimal src;
    int result = 0, number = 0;
    //   src.value_type = 0;
    src.bits[0] = INT_MAX;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = s21_from_decimal_to_int(src, &number);
    ck_assert_int_eq(number, INT_MAX);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(decimal_to_int2) {
    s21_decimal src;
    int result = 0, number = 0;
    //   src.value_type = 0;
    src.bits[0] = 133141;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = s21_from_decimal_to_int(src, &number);
    ck_assert_int_eq(number, 133141);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(decimal_to_int3) {
    s21_decimal src;
    int result = 0, number = 0;
    //   src.value_type = 0;
    src.bits[0] = 2147483648;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    s21_setsign(&src, 1);
    result = s21_from_decimal_to_int(src, &number);
    ck_assert_int_eq(number, -2147483648);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(decimal_to_int4) {
    s21_decimal src;
    int result = 0, number = 0;
    //   src.value_type = 0;
    src.bits[0] = 123451234;
    src.bits[1] = 0;
    src.bits[2] = 0;
    s21_setsign(&src, 1);
    result = s21_from_decimal_to_int(src, &number);
    ck_assert_int_eq(number, -123451234);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(decimal_to_int5) {
    s21_decimal src;
    int result = 0, number = 0;
    //   src.value_type = 0;
    src.bits[0] = 18;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147483648;
    result = s21_from_decimal_to_int(src, &number);
    ck_assert_int_eq(number, -18);
    ck_assert_int_eq(result, 0);

    src.bits[0] = 49;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    s21_set_scale(&src, 1);
    s21_setsign(&src, 1);
    result = s21_from_decimal_to_int(src, &number);
    ck_assert_int_eq(number, -4);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(decimal_to_int6) {
    s21_decimal src;
    int result = 0, number = 0;
    //   src.value_type = 0;
    src.bits[0] = 2147483648;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = s21_from_decimal_to_int(src, &number);
    ck_assert_int_eq(result, 1);

    src.bits[0] = 2147483649;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    s21_setsign(&src, 1);
    result = s21_from_decimal_to_int(src, &number);
    ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(from_float_to_decimal1) {
    s21_decimal val;

    s21_from_float_to_decimal(0.03, &val);
    ck_assert_int_eq(val.bits[0], 3);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 131072);

    s21_from_float_to_decimal(127.1234, &val);
    ck_assert_int_eq(val.bits[0], 1271234);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 262144);
}
END_TEST
START_TEST(from_float_to_decimal2) {
    s21_decimal val;
    s21_from_float_to_decimal(-128.023, &val);
    ck_assert_int_eq(val.bits[0], 12802299);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], -2147155968);
}
END_TEST
START_TEST(from_float_to_decimal3) {
    s21_decimal val;
    s21_from_float_to_decimal(-2.1474836E+09, &val);
    ck_assert_int_eq(val.bits[0], -2147483648);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], -2147483648);
}
END_TEST
START_TEST(from_float_to_decimal4) {
    s21_decimal val;
    s21_from_float_to_decimal(22.14836E+03, &val);
    ck_assert_int_eq(val.bits[0], 2214836);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 131072);
}
END_TEST
START_TEST(from_float_to_decimal5) {
    s21_decimal val;
    s21_from_float_to_decimal(1.02E+09, &val);
    ck_assert_int_eq(val.bits[0], 1020000000);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST
START_TEST(from_float_to_decimal6) {
    s21_decimal val;
    s21_from_float_to_decimal(-333.2222, &val);
    ck_assert_int_eq(val.bits[0], 3332222);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], -2147221504);

    float dst;
    s21_from_float_to_decimal(0.0000001, &val);
    s21_from_decimal_to_float(val, &dst);
    // ck_assert_float_eq(0.0000001, dst);
    ck_assert_float_eq_tol(dst, 0.0000001, 1e-6);

    int ret;
    ret = s21_from_float_to_decimal(FLT_MAX, &val);
    ck_assert_int_eq(1, ret);

    ret = s21_from_float_to_decimal(FLT_MIN, &val);
    ck_assert_int_eq(1, ret);

    ret = s21_from_float_to_decimal(-FLT_MAX, &val);
    ck_assert_int_eq(1, ret);

    ret = s21_from_float_to_decimal(-FLT_MIN, &val);
    ck_assert_int_eq(1, ret);

    ret = s21_from_float_to_decimal(1e-29, &val);
    ck_assert_int_eq(1, ret);

    ret = s21_from_float_to_decimal(-1e-29, &val);
    ck_assert_int_eq(1, ret);

    ret = s21_from_float_to_decimal(79228162514264337593543950336.0,
                                    &val);  // todo (alex): 79228162514264337593543950335.0
    ck_assert_int_eq(1, ret);

    ret = s21_from_float_to_decimal(-79228162514264337593543950336.0, &val);
    ck_assert_int_eq(1, ret);
}
END_TEST
START_TEST(from_float_to_decimal7) {
    s21_decimal val;
    float a = 1.0 / 0.0;
    s21_from_float_to_decimal(a, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    // ck_assert_int_eq(val.bits[3], 0);
    ck_assert_int_eq(s21_check_inf(val), 1);
}
END_TEST
START_TEST(from_float_to_decimal8) {
    s21_decimal val;
    float a = -1.0 / 0.0;
    s21_from_float_to_decimal(a, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    // ck_assert_int_eq(val.bits[3], 0);
    // ck_assert_int_eq(val.value_type, s21_neg_infinity);
    ck_assert_int_eq(s21_check_inf(val), 1);
    ck_assert_int_eq(s21_getsign(&val), 1);
}
END_TEST
START_TEST(from_float_to_decimal9) {
    s21_decimal val;
    float c = 2 * 0, b = 3 * 0, a = b / c;
    s21_from_float_to_decimal(a, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
    // ck_assert_int_eq(val.value_type, s21_nan);
}
END_TEST

START_TEST(decimal_to_float1) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    //   src.value_type = 0;
    src.bits[0] = 18122;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147680256;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, -18.122);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(decimal_to_float2) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    //   src.value_type = 0;
    src.bits[0] = 1812;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147483648;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, -1812);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(decimal_to_float3) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    //   src.value_type = 0;
    src.bits[0] = 0XFFFFFF;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, 16777215);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(decimal_to_float4) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    //   src.value_type = 0;
    src.bits[0] = 23450987;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147745792;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, -2345.0987);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(decimal_to_float5) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    //   src.value_type = 0;
    src.bits[0] = 4294967295;
    src.bits[1] = 4294967295;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, 0xFFFFFFFFFFFFFFFF);
    ck_assert_int_eq(result, 0);

    src.bits[0] = 0xFFFFFFFF;
    src.bits[1] = 0xFFFFFFFF;
    src.bits[2] = 0xFFFFFFFF;
    src.bits[3] = 0x0;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, 79228162514264337593543950336.0);
    ck_assert_int_eq(result, 0);

    src.bits[0] = 0xFFFFFFFF;
    src.bits[1] = 0xFFFFFFFF;
    src.bits[2] = 0xFFFFFFFF;
    src.bits[3] = 0x0;
    s21_setsign(&src, 1);
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, -79228162514264337593543950336.0);
    ck_assert_int_eq(result, 0);

    src.bits[0] = 0x1;
    src.bits[1] = 0x0;
    src.bits[2] = 0x0;
    src.bits[3] = 0x0;
    s21_set_scale(&src, 28);
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq_tol(number, 1e-28, 1e-6);
    ck_assert_int_eq(result, 0);

    src.bits[0] = 0x1;
    src.bits[1] = 0x0;
    src.bits[2] = 0x0;
    src.bits[3] = 0x0;
    s21_set_scale(&src, 28);
    s21_setsign(&src, 1);
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq_tol(number, -1e-28, 1e-6);
    ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(add1) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    int a = -1234;
    float b = 1.234;
    float res_our_dec = 0.0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -1232.766;
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add2) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    int a = -1234;
    float b = -1.234;
    float res_our_dec = 0.0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -1235.234;
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
// START_TEST(add3) {
//     s21_decimal src1, src2, res_od;
//     int ret = 0;
//     int a = -46;
//     float c = 2 * 0, d = 3 * 0, b = d / c;
//     float res_our_dec = 0.0;
//     s21_from_int_to_decimal(a, &src1);
//     s21_from_float_to_decimal(b, &src2);
//     ret = s21_add(src1, src2, &res_od);
//     s21_from_decimal_to_float(res_od, &res_our_dec);
//     // ck_assert_int_eq(res_od.value_type, s21_nan);
//     // ck_assert_int_eq(res_od.bits[3], 0);
//     ck_assert_int_eq(res_od.bits[2], 0);
//     ck_assert_int_eq(res_od.bits[1], 0);
//     ck_assert_int_eq(res_od.bits[0], 0);
//     ck_assert_float_nan(res_our_dec);
//     ck_assert_int_eq(ret, 0);
// }
// END_TEST
START_TEST(add4) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -1234;
    float b = -221.234;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -1455.234;
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add5) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    int a = -1234;
    float b = -221.234;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -1455.234;
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add6) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -1.0 / 0.0;
    int b = 202;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(s21_check_inf(res_od), 1);
    ck_assert_float_eq(s21_getsign(&res_od), 1);
    // -inf ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_int_eq(ret, 2);
}
END_TEST
START_TEST(add7) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -9403;
    float b = 0.00234;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -9402.99766;
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add8) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    int a = -9403;
    int b = 234;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = -9169;
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add9) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -940.3;
    float b = 0.000234;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -940.299766;
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add10) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -0.9403;
    float b = 0.000234;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -0.940066;
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add11) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -0.9403;
    float b = 2.0234;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = 1.0831;
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add12) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = 1.0 / 0.0;
    float b = 112.0234;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    //   ck_assert_float_eq(res_od.value_type, s21_infinity);
    ck_assert_float_eq(s21_check_inf(res_od), 1);
    //   ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_int_eq(ret, 1);
}
END_TEST
START_TEST(add13) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -0.9403;
    float b = -112.0234;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -112.9637;
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add14) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -0.94e03;
    float b = -112.0234;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -1052.0234;
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add15) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -0.94e03;
    float b = -112.0e2;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -12140;
    s21_add(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add16) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    int a = 9403;
    int b = 202;
    int res_origin = 9605;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_add(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);

    // float aa = 79228162514264337593543950335.0;
    // float bb = -0.6;
    // float res_origin2 = aa - bb;
    // float res_our_dec2;
    // src1.bits[0] = 0xffffffff;
    // src1.bits[1] = 0xffffffff;
    // src1.bits[2] = 0xffffffff;
    // src1.bits[3] = 0x0;
    // s21_from_float_to_decimal(bb, &src2);
    // ret = s21_add(src1, src2, &res_od);
    // s21_from_decimal_to_float(src1, &aa);
    // s21_from_decimal_to_float(res_od, &res_our_dec2);
    // ck_assert_float_eq(res_our_dec2, res_origin2);
    // ck_assert_int_eq(ret, 0);  //todo(alex): bank
}
END_TEST
START_TEST(add17) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[3] = 0x300000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xF;
    src1.bits[0] = 0x67E4FEEF;
    src2.bits[3] = 0x300000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFF;
    src2.bits[0] = 0x67E4FFFF;
    s21_decimal result_origin;
    result_origin.bits[3] = 0x300000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x10E;
    result_origin.bits[0] = 0xCFC9FEEE;
    ret = s21_add(src1, src2, &result_our);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add18) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[3] = 0x30000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFF;
    src1.bits[0] = 0x67E4F;
    src2.bits[3] = 0x60000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xEA;
    src2.bits[0] = 0x67E4F;
    s21_decimal result_origin;
    result_origin.bits[3] = 0x60000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x3E502;
    result_origin.bits[0] = 0x1963E2E7;
    ret = s21_add(src1, src2, &result_our);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add19) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[3] = 0x30000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFFFF;
    src1.bits[0] = 0xFF837E4F;
    src2.bits[3] = 0x80070000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFFF;
    src2.bits[0] = 0xFF837E4F;
    s21_decimal result_origin;
    result_origin.bits[3] = 0x70000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x270FEFED;
    result_origin.bits[0] = 0x00F26FA1;
    ret = s21_add(src1, src2, &result_our);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add20) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[3] = 0x80030000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFFFF;
    src1.bits[0] = 0xFF837E4F;
    src2.bits[3] = 0x80070000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFFF;
    src2.bits[0] = 0xFF837E4F;
    s21_decimal result_origin;
    result_origin.bits[3] = 0x80070000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x27100FEC;
    result_origin.bits[0] = 0xFFF96C3F;
    ret = s21_add(src1, src2, &result_our);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(add21) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[3] = 0x80030000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFFFF;
    src1.bits[0] = 0xFF837E4F;
    src2.bits[3] = 0x70000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFFF;
    src2.bits[0] = 0xFF837E4F;
    s21_decimal result_origin;
    result_origin.bits[3] = 0x80070000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x270FEFED;
    result_origin.bits[0] = 0x00F26FA1;
    ret = s21_add(src1, src2, &result_our);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(sub1) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    int a = 9403;
    int b = 202;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = 9201;
    ret = s21_sub(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub2) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    int a = 9403;
    float b = 202.098;
    float res_our_dec = 0.0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = 9200.902;
    ret = s21_sub(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub3) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -9403;
    float b = 202.098;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = a - b;
    ret = s21_sub(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub4) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = 9403.0;
    float b = 202.09e8;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -20208990597;
    ret = s21_sub(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub5) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = 9403.0e2;
    float b = 202.09e8;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -20208059700;
    ret = s21_sub(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub6) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -9403.0e2;
    int b = -202;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    float res_origin = a - b;
    ret = s21_sub(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub7) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -1.0 / 0.0;
    int b = 202;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_sub(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(s21_check_inf(res_od), 1);
    ck_assert_float_eq(s21_getsign(&res_od), 1);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_int_eq(ret, 2);

    a = 1.0 / 0.0;
    b = 202;
    res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_sub(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    // ck_assert_float_eq(res_od.value_type, s21_infinity);
    ck_assert_float_eq(s21_check_inf(res_od), 1);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_int_eq(ret, 1);

    // float c = 2 * 0, d = 3 * 0;
    // a = d / c;
    // b = 202;
    // res_our_dec = 0.0;
    // s21_from_float_to_decimal(a, &src1);
    // s21_from_int_to_decimal(b, &src2);
    // ret = s21_sub(src1, src2, &res_od);
    // s21_from_decimal_to_float(res_od, &res_our_dec);
    // // ck_assert_float_eq(res_od.value_type, s21_nan);
    // // ck_assert_int_eq(res_od.bits[3], 0);
    // ck_assert_int_eq(res_od.bits[2], 0);
    // ck_assert_int_eq(res_od.bits[1], 0);
    // ck_assert_int_eq(res_od.bits[0], 0);
    // ck_assert_float_nan(res_our_dec);
    // // ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub8) {
    s21_decimal src1, src2, res_od;
    int ret;
    float a = -94;
    float b = -202;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = a - b;
    ret = s21_sub(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);

    a = 2020;
    b = 94099992;
    res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    res_origin = a - b;
    ret = s21_sub(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub9) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[3] = 0b00000000000000010000000000000000;
    src1.bits[2] = 0;
    src1.bits[1] = 0;
    src1.bits[0] = 0b00000000000000000110011011110000;
    src2.bits[3] = 0b00000000000000110000000000000000;
    src2.bits[2] = 0;
    src2.bits[1] = 0;
    src2.bits[0] = 0b00000000000001000101000111010110;
    s21_decimal result_origin;
    result_origin.bits[3] = 0b00000000000000110000000000000000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0;
    result_origin.bits[0] = 0b00000000001000111110001111101010;
    ret = s21_sub(src1, src2, &result_our);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub10) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[3] = 0b00000000000000110000000000000000;
    src1.bits[2] = 0;
    src1.bits[1] = 0;
    src1.bits[0] = 0b00000000000001000101000111010110;
    src2.bits[3] = 0b00000000000000010000000000000000;
    src2.bits[2] = 0;
    src2.bits[1] = 0;
    src2.bits[0] = 0b00000000000000000000101110111100;
    s21_decimal result_origin;
    result_origin.bits[3] = 0b10000000000000110000000000000000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0;
    result_origin.bits[0] = 0b00000000000000000100001110011010;
    ret = s21_sub(src1, src2, &result_our);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub11) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[3] = 0x80030000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFFFF;
    src1.bits[0] = 0xFF837E4F;
    src2.bits[3] = 0x80070000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFFF;
    src2.bits[0] = 0xFF837E4F;
    s21_decimal result_origin;
    result_origin.bits[3] = 0x80070000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x270FEFED;
    result_origin.bits[0] = 0x00F26FA1;
    ret = s21_sub(src1, src2, &result_our);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub12) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[3] = 0x30000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFFFF;
    src1.bits[0] = 0xFF837E4F;
    src2.bits[3] = 0x80070000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFFF;
    src2.bits[0] = 0xFF837E4F;
    s21_decimal result_origin;
    result_origin.bits[3] = 0x70000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x27100FEC;
    result_origin.bits[0] = 0xFFF96C3F;
    ret = s21_sub(src1, src2, &result_our);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub13) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[3] = 0b10000000000000010000000000000000;
    src1.bits[2] = 0;
    src1.bits[1] = 0;
    src1.bits[0] = 0b00000000000000000000000100110000;
    src2.bits[3] = 0b10000000000001100000000000000000;
    src2.bits[2] = 0;
    src2.bits[1] = 0;
    src2.bits[0] = 0b00010001001000010001101001100000;
    s21_decimal result_origin;
    result_origin.bits[3] = 0b00000000000001100000000000000000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0;
    result_origin.bits[0] = 0b00001111010100010011110001100000;
    ret = s21_sub(src1, src2, &result_our);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub14) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[3] = 0x80030000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFFFF;
    src1.bits[0] = 0xFF837E4F;
    src2.bits[3] = 0x70000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFFF;
    src2.bits[0] = 0xFF837E4F;
    s21_decimal result_origin;
    result_origin.bits[3] = 0x80070000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x27100FEC;
    result_origin.bits[0] = 0xFFF96C3F;
    ret = s21_sub(src1, src2, &result_our);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub15) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[0] = 0b00000000000000000000011111100100;
    src1.bits[1] = 0b00000000000000000000000000000000;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b00000000000000000000000000000000;
    src2.bits[0] = 0b00000101100110111101101000011001;
    src2.bits[1] = 0b00000000000000000000000000000000;
    src2.bits[2] = 0b00000000000000000000000000000000;
    src2.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal result;
    ret = s21_sub(src1, src2, &result_our);
    result.bits[0] = 0b00000101100110111101001000110101;
    result.bits[1] = 0b00000000000000000000000000000000;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b10000000000000000000000000000000;
    ck_assert_int_eq(result.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(sub16) {
    s21_decimal src1, src2, result_our;
    int ret = 0;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[0] = 0b00000000000000000000011111100100;
    src1.bits[1] = 0b00000000000000000000000000000000;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b10000000000000000000000000000000;
    src2.bits[0] = 0b00000000000000000010010010111011;
    src2.bits[1] = 0b00000000000000000000000000000000;
    src2.bits[2] = 0b00000000000000000000000000000000;
    src2.bits[3] = 0b10000000000000000000000000000000;
    s21_decimal result;
    ret = s21_sub(src1, src2, &result_our);
    result.bits[0] = 0b00000000000000000001110011010111;
    result.bits[1] = 0b00000000000000000000000000000000;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b00000000000000000000000000000000;

    ck_assert_int_eq(result.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result.bits[0], result_our.bits[0]);
    ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(mul1) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = 9403.0e2;
    int b = 202;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    float res_origin = 189940600;
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mul1_1) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = 1.23456789;
    int b = 2.34567891;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    float res_origin = a * b;
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_ldouble_eq_tol(res_our_dec, res_origin, 1e-6);
    ck_assert_int_eq(ret, 0);
}
START_TEST(mul2) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = 9403.0e2;
    float b = 9403.0e2;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = 884164090000;
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mul3) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    int a = 9403;
    int b = 202;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = 1899406;
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mul4) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    int a = -32768;
    int b = 2;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = -65536;
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    // s21_print(res_od);
    // res_od = check_for_mul(src1, src2);
    // s21_print(res_od);
    // printf("%d\n", s21_check_inf(res_od));
    ck_assert_int_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(mul41) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    int a = -3;
    int b = 2;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = -6;
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(mul5) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    int a = -32768;
    int b = 32768;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = -1073741824;
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mul6) {
    s21_decimal src1, src2, res_od;
    // int ret = 0;
    float a = 1.0 / 0.0;
    int b = 0;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    s21_mul(src1, src2, &res_od);  // ret =
    s21_from_decimal_to_float(res_od, &res_our_dec);
    // ck_assert_float_nan(res_our_dec);
    // ck_assert_int_eq(res_od.value_type, s21_nan);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    // ck_assert_int_eq(ret, 0);

    // s21_set_inf(&src1);
    // s21_print(src1);
    // printf("%d\n", s21_check_inf(src1));
}
END_TEST
START_TEST(mul7) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = 1.0 / 0.0;
    float b = 132;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_float_eq(s21_check_inf(res_od), 1);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    ck_assert_int_eq(ret, 1);
}
END_TEST
START_TEST(mul8) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = 1.0 / 0.0;
    float b = 1.0 / 0.0;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_float_eq(s21_check_inf(res_od), 1);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    ck_assert_int_eq(ret, 1);
}
END_TEST
START_TEST(mul9) {
    s21_decimal src1, src2, res_od;
    // int ret = 0;
    float a = 1.0 / 0.0;
    float b = 0.0 * 2 / 3 * 0.0;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    s21_mul(src1, src2, &res_od);  // ret =
    s21_from_decimal_to_float(res_od, &res_our_dec);
    // ck_assert_float_nan(res_our_dec);
    // ck_assert_float_eq(res_od.value_type, s21_nan);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    // ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mul10) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = 1.0 / 0.0;
    float b = -1.0 / 0.0;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_float_eq(s21_check_inf(res_od), 1);
    ck_assert_float_eq(s21_getsign(&res_od), 1);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    ck_assert_int_eq(ret, 2);
}
END_TEST
START_TEST(mul11) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -1.0 / 0.0;
    float b = 123;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_float_eq(s21_check_inf(res_od), 1);
    ck_assert_float_eq(s21_getsign(&res_od), 1);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    ck_assert_int_eq(ret, 2);
}
END_TEST
START_TEST(mul12) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -1.0 / 0.0;
    float b = -123;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_float_eq(s21_check_inf(res_od), 1);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    ck_assert_int_eq(ret, 1);
}
END_TEST
START_TEST(mul13) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = 1.0 / 0.0;
    float b = 132;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_float_eq(s21_check_inf(res_od), 1);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    ck_assert_int_eq(ret, 1);
}
END_TEST
START_TEST(mul14) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float a = -1.0 / 0.0;
    float b = 123;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_float_eq(s21_check_inf(res_od), 1);
    ck_assert_float_eq(s21_getsign(&res_od), 1);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    ck_assert_int_eq(ret, 2);
}
END_TEST
START_TEST(mul15) {
    s21_decimal src1, src2, res_od;
    // int ret = 0;
    float a = 1.0 / 0.0;
    int b = 0;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    s21_mul(src1, src2, &res_od);  // ret =
    s21_from_decimal_to_float(res_od, &res_our_dec);
    // ck_assert_float_nan(res_our_dec);
    // ck_assert_float_eq(res_od.value_type, s21_nan);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    // ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mul16) {
    s21_decimal src1, src2, res_od;
    // int ret = 0;
    float a = -1.0 / 0.0;
    float b = 0.0 * 2 / 3 * 0.0;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    s21_mul(src1, src2, &res_od);  // ret =
    s21_from_decimal_to_float(res_od, &res_our_dec);
    // ck_assert_float_eq(res_od.value_type, s21_nan);
    // ck_assert_float_eq(res_od.value_type, s21_nan);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    // ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mul17) {
    s21_decimal src1, src2, res_od;
    // int ret = 0;
    float a = -1.0 / 0.0;
    float b = 0.0 * 2 / 3 * 0.0;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    s21_mul(src1, src2, &res_od);  // ret =
    s21_from_decimal_to_float(res_od, &res_our_dec);
    // ck_assert_float_eq(res_od.value_type, s21_nan);
    // ck_assert_float_eq(res_od.value_type, s21_nan);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    // ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mul18) {
    s21_decimal src1, src2, res_od;
    int ret = 0;
    float c = 2 * 0.0, d = 3 * 0.0;
    float a = c / d;
    float b = d / c;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    // ck_assert_float_eq(res_od.value_type, s21_nan);
    // ck_assert_float_eq(res_od.value_type, s21_nan);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(mod1) {
    s21_decimal src1, src2, res_mod;
    int a = 3;
    int b = 2;
    int res_origin = a % b;
    int res = 0, ret = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);

    a = 1;
    b = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    // ck_assert_int_eq(ret, 3);

    //   float res2;
    //   s21_from_float_to_decimal(70000000000000000000000000000.0, &src1);
    //   //todo(alex): sort out s21_from_float_to_decimal(0.001, &src2); ret =
    //   s21_mod(src1, src2, &res_mod); s21_from_decimal_to_float(res_mod, &res2);
    //   ck_assert_int_eq(ret, 0);
    // ck_assert_float_eq(0.000, res2);
}
END_TEST
START_TEST(mod2) {
    s21_decimal src1, src2, res_mod;
    int a = 98765;
    int b = 1234;
    int res_origin = a % b;
    int res = 0, ret = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mod3) {
    s21_decimal src1, src2, res_mod;
    int a = 30198;
    int b = 20210;
    int res_origin = a % b;
    int res = 0, ret = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);

    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mod4) {
    s21_decimal src1, src2, res_mod;
    int a = -98765;
    int b = 1234;
    int res_origin = a % b;
    int res = 0, ret = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mod41) {
    s21_decimal src1, src2, res_mod;
    int a = -29;
    int b = 7;
    int res_origin = a % b;
    int res = 0, ret = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mod5) {
    s21_decimal src1, src2, res_mod;
    int a = 98765;
    int b = 127234;
    int res_origin = a % b;
    int res = 0, ret = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mod6) {
    s21_decimal src1, src2, res_mod;
    int a = 342576;
    int b = 1542134;
    int res_origin = a % b;
    int res = 0, ret = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mod7) {
    s21_decimal src1, src2, res_mod;
    float a = 1.2;
    float b = 0.3;
    float res_origin = fmod(a, b);
    float res = 0.0;
    int ret = 0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    ck_assert_float_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mod8) {
    s21_decimal src1, src2, res_mod;
    float a = 1.2;
    int b = 3;
    float res_origin = fmod(a, b);
    float res = 0, ret = 0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    ck_assert_float_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);

    a = 0, b = 2.2;
    res_origin = fmod(a, b);
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    ck_assert_float_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);

    a = 1.0 / 0.0, b = 2.2;
    // res_origin = fmod(a, b);
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    // ck_assert_float_nan(res);
    ck_assert_int_eq(ret, 0);

    a = 1.0, b = 0;
    // res_origin = fmod(a, b);
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    // ck_assert_float_nan(res);
    // ck_assert_int_eq(ret, 3);

    a = 2.0, b = 1 / 0.0;
    res_origin = fmod(a, b);
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    ck_assert_float_eq(res_origin, res);  // ck_assert_float_nan(res);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(mod9) {  // 0, inf, -inf, nan
    s21_decimal src1, src2, res_mod;
    float b = 1.0 / 0.0;
    int a = 3, ret = 0;
    float res_origin = fmod(a, b);
    float res = 0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);

    ck_assert_float_eq(res_origin, res);  // ck_assert_float_nan(res);
    ck_assert_int_eq(ret, 0);

    b = -1.0 / 0.0;
    a = 3;
    res_origin = fmod(a, b);
    res = 0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    ck_assert_float_eq(res_origin, res);  // ck_assert_float_nan(res);
    ck_assert_int_eq(ret, 0);

    a = 1.0 / 0.0;
    b = 33;
    res_origin = fmod(a, b);
    res = 0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    ck_assert_float_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);

    a = -1.0 / 0.0;
    b = 3;
    res_origin = fmod(a, b);
    res = 0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    ck_assert_float_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);

    a = 0.0 * 2 / 3 * 0.0;
    b = 3;
    res_origin = fmod(a, b);
    res = 0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    ck_assert_float_eq(res_origin, res);
    ck_assert_int_eq(ret, 0);

    float c = 3 * 0.0, d = 2 * 0.0;
    b = c / d;
    a = 3;
    res_origin = fmod(a, b);
    res = 0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    // ck_assert_float_nan(res);
    ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(div1) {
    s21_decimal src1, src2, res_od;
    float a = 9403.0e2;
    int b = 202, ret;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    float res_origin = 4654.950495049504950495049505;
    ret = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(div2) {
    s21_decimal src1, src2, res_od;
    int ret;
    int a = -32768;
    int b = 2;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = -16384;
    ret = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(div3) {
    s21_decimal src1, src2, res_od;
    int ret;
    float a = -9403.0e2;
    float b = -2020.29;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = 465.4282306005573457275935633;
    ret = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(div4) {
    s21_decimal src1, src2, res_od;
    int ret;
    float a = -9403.0e2;
    float b = 2020.29;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -465.4282306005573457275935633;
    ret = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(div5) {
    s21_decimal src1, src2, res_od;
    int ret;
    float a = -9403.0e20;
    float b = 2.28e17;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = a / b;
    ret = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(div6) {
    s21_decimal src1, src2, res_od;
    int ret;
    float a = -0.9;
    float b = 30.323;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = a / b;
    ret = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(div7) {
    s21_decimal src1, src2, res_od;
    int ret;
    float a = -0.9e3;
    float b = 30.32;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = a / b;
    ret = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(div8) {
    s21_decimal src1, src2, res_od;
    int ret;
    float a = -0.9e3;
    int b = 30;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    float res_origin = a / b;
    ret = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(div9) {
    s21_decimal src1, src2, res_od;
    int ret;
    float a = -0.9;
    float b = 0.000076;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = a / b;
    ret = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(div10) {
    s21_decimal src1, src2, res_od;  // src3,
    int ret;
    // src1.value_type = s21_infinity;
    // src2.value_type = s21_infinity;
    s21_set_inf(&src1);
    s21_set_inf(&src2);

    // src3.value_type = s21_nan;
    ret = s21_div(src1, src2, &res_od);
    // ck_assert_int_eq(res_od.value_type, src3.value_type);
    float res;
    s21_from_decimal_to_float(res_od, &res);
    // ck_assert_float_nan(res);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(div11) {
    s21_decimal src1, src2, res_od;
    int ret;
    s21_from_int_to_decimal(10, &src1);
    s21_from_int_to_decimal(0, &src2);
    //   src3.value_type = s21_infinity;
    //   s21_set_inf(&src3);
    ret = s21_div(src1, src2, &res_od);
    ck_assert_int_eq(s21_check_inf(res_od), 1);
    float res;
    s21_from_decimal_to_float(res_od, &res);
    ck_assert_float_infinite(res);
    ck_assert_int_eq(ret, 3);
}
END_TEST
// START_TEST(div12) {
//     s21_decimal src1, src2, res_od;
//     int ret;
//     s21_from_int_to_decimal(-10, &src1);
//     src1.value_type = s21_infinity;
//     src2.value_type = s21_neg_infinity;

//     s21_set_inf(&src1);
//     s21_set_inf(&src2);
//     s21_setsign(&src2);
//     ret = s21_div(src1, src2, &res_od);
//     // ck_assert_int_eq(res_od.value_type, s21_nan);
//     float res;
//     s21_from_decimal_to_float(res_od, &res);
//     // ck_assert_float_nan(res);
//     // ck_assert_int_eq(ret, 0);
// }
// END_TEST
START_TEST(div13) {
    s21_decimal src1, src2, res_od;
    int ret;
    float a = 1.0 / 0.0;
    float b = 1.0 / 0.0;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    // ck_assert_float_eq(res_od.value_type, s21_nan);
    // ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    float res;
    s21_from_decimal_to_float(res_od, &res);
    // ck_assert_float_nan(res);
    ck_assert_int_eq(ret, 0);

    a = 1.0 / 0.0;
    b = 2;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_int_eq(ret, 1);

    a = -1.0 / 0.0;
    b = 2;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_infinite(res_our_dec);
    ck_assert_int_eq(ret, 2);
}
END_TEST
START_TEST(div15) {
    s21_decimal src1, src2, res_od;
    int ret;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[0] = 0b01110001001110010011101000100111;
    src1.bits[1] = 0b00000000000000000000000011100010;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b00000000000001010000000000000000;
    src2.bits[0] = 0b00000000000000010111101111011011;
    src2.bits[1] = 0b00000000000000000000000000000000;
    src2.bits[2] = 0b00000000000000000000000000000000;
    src2.bits[3] = 0b00000000000000100000000000000000;
    s21_decimal result_origin;
    //   result_origin.value_type = s21_usual;
    result_origin.bits[0] = 0b00100000111100011100110110110001;
    result_origin.bits[1] = 0b01011000000100101111110011101001;
    result_origin.bits[2] = 0b00100000010100001110111000110111;
    result_origin.bits[3] = 0b00000000000110000000000000000000;
    ret = s21_div(src1, src2, &res_od);
    float a, b;
    s21_from_decimal_to_float(result_origin, &a);
    s21_from_decimal_to_float(res_od, &b);
    ck_assert_float_eq(a, b);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(div16) {
    s21_decimal src1, src2, res_od;
    int ret;
    //   src1.value_type = src2.value_type = s21_usual;
    src1.bits[0] = 0b00010001100000110100010101000111;
    src1.bits[1] = 0b00000000000000101011011100110010;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b00000000000001010000000000000000;
    src2.bits[0] = 0b01111001000001000011011001001011;
    src2.bits[1] = 0b00000000000000000101010011111111;
    src2.bits[2] = 0b00000000000000000000000000000000;
    src2.bits[3] = 0b00000000000001010000000000000000;
    s21_decimal result_origin;
    //   result_origin.value_type = s21_usual;
    result_origin.bits[0] = 0b01000111110000001001001101101011;
    result_origin.bits[1] = 0b10111000000101111000101110100000;
    result_origin.bits[2] = 0b00011010011011010111110000110100;
    result_origin.bits[3] = 0b00000000000110110000000000000000;
    ret = s21_div(src1, src2, &res_od);
    float a, b;

    // s21_from_decimal_to_float(src1, &a);
    // s21_from_decimal_to_float(src2, &b);
    // printf("div: %f %f %f", a, b, a / b);

    s21_from_decimal_to_float(result_origin, &a);
    s21_from_decimal_to_float(res_od, &b);
    ck_assert_float_eq(a, b);
    ck_assert_int_eq(ret, 0);
}
END_TEST
START_TEST(div17) {
    s21_decimal src1, src2, res_od;
    int ret;
    int a = -17272;
    float b = -1.0 / 0.0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_div(src1, src2, &res_od);
    s21_decimal zero = {{0, 0, 0, 0}};
    ck_assert_int_eq(!!(s21_is_equal(res_od, zero) == 1), 1);
    ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    float c;
    s21_from_decimal_to_float(res_od, &c);
    ck_assert_float_eq(a / b, c);
    ck_assert_int_eq(ret, 0);
}
END_TEST
// START_TEST(div18) {
//     s21_decimal res_od;  // src3, src2, src1,
//     int ret;
//     src1.value_type = s21_infinity;
//     // src2.value_type = s21_nan;
//     // src3.value_type = s21_nan;
//     // ret = s21_div(src1, src2, &res_od);
//     // ck_assert_int_eq(res_od.value_type, src3.value_type);
//     float res;
//     s21_from_decimal_to_float(res_od, &res);
//     // ck_assert_float_nan(res);
//     ck_assert_int_eq(ret, 0);
// }
// END_TEST
START_TEST(div19) {
    s21_decimal src1, src2, res_od;
    int ret;
    float a = -115.2;
    float b = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    ret = s21_div(src1, src2, &res_od);

    ck_assert_int_eq(s21_check_inf(res_od), 1);
    ck_assert_int_eq(s21_getsign(&res_od), 1);

    //   ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
    float res;
    s21_from_decimal_to_float(res_od, &res);
    ck_assert_float_infinite(res);
    ck_assert_int_eq(ret, 3);
}
END_TEST

START_TEST(truncate1) {
    s21_decimal src, result, result2;
    //   src.value_type = s21_usual;
    src.bits[3] = 0x000A0000;  // 1844674432
    src.bits[2] = 0x0;
    src.bits[1] = 0xFFFFFFFF;
    src.bits[0] = 0xFFFFFFFF;
    result.bits[3] = 0x0;
    result.bits[2] = 0x0;
    result.bits[1] = 0x0;
    result.bits[0] = 0x6DF37F67;
    s21_truncate(src, &result2);
    ck_assert_float_eq(result2.bits[0], result.bits[0]);
    ck_assert_float_eq(result2.bits[1], result.bits[1]);
    ck_assert_float_eq(result2.bits[2], result.bits[2]);
    ck_assert_float_eq(result2.bits[3], result.bits[3]);
}
END_TEST
START_TEST(truncate2) {
    s21_decimal src, result, result2;
    //   src.value_type = s21_usual;
    src.bits[3] = 0x800C0000;  // -18446744.0
    src.bits[2] = 0x0;
    src.bits[1] = 0xFFFFFFFF;
    src.bits[0] = 0xFFFFFFFF;
    result.bits[3] = 0x80000000;
    result.bits[2] = 0x0;
    result.bits[1] = 0x0;
    result.bits[0] = 0x1197998;
    s21_truncate(src, &result2);
    ck_assert_float_eq(result2.bits[0], result.bits[0]);
    ck_assert_float_eq(result2.bits[1], result.bits[1]);
    ck_assert_float_eq(result2.bits[2], result.bits[2]);
    ck_assert_float_eq(result2.bits[3], result.bits[3]);
}
END_TEST
START_TEST(truncate3) {
    s21_decimal src, result, result2;
    //   src.value_type = s21_usual;
    src.bits[3] = 0x80000000;  // -18446744073709551616.0
    src.bits[2] = 0x0;
    src.bits[1] = 0xFFFFFFFF;
    src.bits[0] = 0xFFFFFFFF;
    result.bits[3] = 0x80000000;
    result.bits[2] = 0x0;
    result.bits[1] = 0xFFFFFFFF;
    result.bits[0] = 0xFFFFFFFF;
    s21_truncate(src, &result2);
    ck_assert_float_eq(result2.bits[0], result.bits[0]);
    ck_assert_float_eq(result2.bits[1], result.bits[1]);
    ck_assert_float_eq(result2.bits[2], result.bits[2]);
    ck_assert_float_eq(result2.bits[3], result.bits[3]);
}
END_TEST
START_TEST(truncate4) {
    s21_decimal src, result, result2;
    //   src.value_type = s21_usual;
    src.bits[0] = 0b00000000000000000000000001101101;  // 2277250560
    src.bits[1] = 0b00000000000011000110010101011011;
    src.bits[2] = 0b00000000000000000011000000111001;
    src.bits[3] = 0b00000000000011100000000000000000;
    result.bits[0] = 0b10000111101111000001011000011110;
    result.bits[1] = 0b00000000000000000000000000000000;
    result.bits[2] = 0;
    result.bits[3] = 0;
    s21_truncate(src, &result2);
    ck_assert_float_eq(result2.bits[0], result.bits[0]);
    ck_assert_float_eq(result2.bits[1], result.bits[1]);
    ck_assert_float_eq(result2.bits[2], result.bits[2]);
    ck_assert_float_eq(result2.bits[3], result.bits[3]);
}
END_TEST
START_TEST(truncate5) {
    s21_decimal src, result, result2;
    //   src.value_type = s21_usual;
    src.bits[0] = 0b11011110101001001000000010110110;  // 9876543516404875264
    src.bits[1] = 0b01011111101001011101111110100000;
    src.bits[2] = 0b00011111111010011010111101100000;
    src.bits[3] = 0b00000000000010010000000000000000;
    result.bits[0] = 0b00110100101011011100000100111110;
    result.bits[1] = 0b10001001000100001000011110111001;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b00000000000000000000000000000000;
    s21_truncate(src, &result2);
    ck_assert_float_eq(result2.bits[0], result.bits[0]);
    ck_assert_float_eq(result2.bits[1], result.bits[1]);
    ck_assert_float_eq(result2.bits[2], result.bits[2]);
    ck_assert_float_eq(result2.bits[3], result.bits[3]);
}
END_TEST
START_TEST(truncate6) {
    s21_decimal src, result, result2;
    //   src.value_type = s21_usual;
    src.bits[3] = 0x80090000;  // -18446721024.0
    src.bits[2] = 0;
    src.bits[1] = 0xFFFFEA34;
    src.bits[0] = 0xFF837E4F;
    s21_truncate(src, &result);
    result2.bits[3] = 0x80000000;
    result2.bits[2] = 0;
    result2.bits[1] = 0x4;
    result2.bits[0] = 0x4B829C70;
    ck_assert_int_eq(result2.bits[3], result.bits[3]);
    ck_assert_int_eq(result2.bits[2], result.bits[2]);
    ck_assert_int_eq(result2.bits[1], result.bits[1]);
    ck_assert_int_eq(result2.bits[0], result.bits[0]);
}
END_TEST
START_TEST(truncate7) {
    s21_decimal src, result, result2;
    //   src.value_type = s21_usual;
    src.bits[0] = 0b11010000101000110011000001101101;  // = 2277250560.0
    src.bits[1] = 0b00011111100110100110101101011000;
    src.bits[2] = 0;
    src.bits[3] = 0b00000000000010010000000000000000;
    s21_truncate(src, &result);
    result2.bits[3] = 0;  // = 2277250560.0
    result2.bits[2] = 0;
    result2.bits[1] = 0;
    result2.bits[0] = 0b10000111101111000001011000011110;
    ck_assert_int_eq(result2.bits[3], result.bits[3]);
    ck_assert_int_eq(result2.bits[2], result.bits[2]);
    ck_assert_int_eq(result2.bits[1], result.bits[1]);
    ck_assert_int_eq(result2.bits[0], result.bits[0]);
}
END_TEST
START_TEST(truncate8) {
    s21_decimal src, res;
    //   src.value_type = s21_usual;
    src.bits[0] = 0b01111000101001111011110011000110;  // = 0.987654
    src.bits[1] = 0b10011111111111001111101010000100;
    src.bits[2] = 0b00011111111010011010111101101101;
    src.bits[3] = 0b00000000000111000000000000000000;
    s21_decimal result = {{0, 0, 0, 0}};  // = 0
    s21_truncate(src, &res);
    ck_assert_int_eq(res.bits[0], result.bits[0]);
    ck_assert_int_eq(res.bits[1], result.bits[1]);
    ck_assert_int_eq(res.bits[2], result.bits[2]);
    ck_assert_int_eq(res.bits[3], result.bits[3]);
}
END_TEST
START_TEST(truncate9) {
    s21_decimal src, res;
    s21_from_float_to_decimal(1.0 / 0.0, &src);
    s21_truncate(src, &res);
    ck_assert_float_eq(1, s21_check_inf(res));
}
END_TEST
START_TEST(truncate10) {
    float f_result;
    s21_decimal src, res;
    s21_from_float_to_decimal(1.9, &src);
    s21_truncate(src, &res);
    s21_from_decimal_to_float(res, &f_result);
    ck_assert_float_eq(f_result, 1.0);
}
END_TEST
START_TEST(truncate11) {
    float f_result;
    s21_decimal src, res;
    s21_from_float_to_decimal(-2.1, &src);
    s21_truncate(src, &res);
    s21_from_decimal_to_float(res, &f_result);
    ck_assert_float_eq(f_result, -2.0);
}
END_TEST
// START_TEST(truncate12) {
//     float f_result, a = 1 * 0.0, b = 0.0 * 2;
//     s21_decimal src, res;
//     s21_from_float_to_decimal(a / b, &src);
//     ck_assert_int_eq(1, s21_truncate(src, &res));
//     s21_from_decimal_to_float(res, &f_result);
//     // ck_assert_float_nan(f_result);
// }
END_TEST
START_TEST(truncate13) {
    float f_result;
    s21_decimal src, res;
    s21_from_float_to_decimal(1.0 / 0.0, &src);
    ck_assert_int_eq(1, s21_truncate(src, &res));
    s21_from_decimal_to_float(res, &f_result);
    ck_assert_float_eq(1, s21_check_inf(res));
    ck_assert_float_eq(f_result, 1.0 / 0.0);
    ck_assert_float_infinite(f_result);
}
END_TEST
START_TEST(truncate14) {
    float f_result;
    s21_decimal src, res;
    s21_from_float_to_decimal(-1.0 / 0.0, &src);
    ck_assert_int_eq(1, s21_truncate(src, &res));
    s21_from_decimal_to_float(res, &f_result);
    ck_assert_float_eq(1, s21_check_inf(res));
    ck_assert_float_eq(1, s21_getsign(&res));
    ck_assert_float_eq(f_result, -1.0 / 0.0);
    ck_assert_float_infinite(f_result);
}
END_TEST

START_TEST(round1) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b01100011000011111111111111111111;
    src.bits[1] = 0b01101011110001110101111000101101;
    src.bits[2] = 0b00000000000000000000000000000101;
    src.bits[3] = 0b10000000000010100000000000000000;
    res.bits[0] = 0b01010100000010111110010000000000;
    res.bits[1] = 0b00000000000000000000000000000010;
    res.bits[2] = 0b00000000000000000000000000000000;
    res.bits[3] = 0b10000000000000000000000000000000;
    s21_round(src, &res2);
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(round2) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b01010101001110101110101110110001;
    src.bits[1] = 0b00001101101101001101101001011111;
    src.bits[2] = 0b00000000000000000000000000000000;
    src.bits[3] = 0b10000000000100100000000000000000;
    res.bits[0] = 0b00000000000000000000000000000001;
    res.bits[1] = 0b00000000000000000000000000000000;
    res.bits[2] = 0b00000000000000000000000000000000;
    res.bits[3] = 0b10000000000000000000000000000000;
    s21_round(src, &res2);
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(round3) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b10010011111100000001110001010010;
    src.bits[1] = 0b00000000000000000000000100100000;
    src.bits[2] = 0b00000000000000000000000000000000;
    src.bits[3] = 0b00000000000010100000000000000000;
    res.bits[0] = 0b00000000000000000000000001111100;
    res.bits[1] = 0b00000000000000000000000000000000;
    res.bits[2] = 0b00000000000000000000000000000000;
    res.bits[3] = 0b00000000000000000000000000000000;
    s21_round(src, &res2);
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(round4) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b11001101110001110111110001000000;
    src.bits[1] = 0b00111110001001010000001001100000;
    src.bits[2] = 0b00100000010011111100111001011110;
    src.bits[3] = 0b10000000000001000000000000000000;
    res.bits[0] = 0b10100000111111100100111000000100;
    res.bits[1] = 0b00011011110011101100110011101101;
    res.bits[2] = 0b00000000000000001101001111000010;
    res.bits[3] = 0b10000000000000000000000000000000;
    s21_round(src, &res2);
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(round5) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b10100000111111100100111000000100;
    src.bits[1] = 0b00011011110011101100110011101101;
    src.bits[2] = 0b00000000000000001101001111000010;
    src.bits[3] = 0b00000000000000000000000000000000;
    res.bits[0] = 0b10100000111111100100111000000100;
    res.bits[1] = 0b00011011110011101100110011101101;
    res.bits[2] = 0b00000000000000001101001111000010;
    res.bits[3] = 0b00000000000000000000000000000000;
    s21_round(src, &res2);
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(round6) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b10010111011100111001111111111111;
    src.bits[1] = 0b00111100000010000011000110001101;
    src.bits[2] = 0b00000000000000000000000000000001;
    src.bits[3] = 0b00000000000001110000000000000000;
    res.bits[3] = 0;
    res.bits[2] = 0;
    res.bits[1] = 0b00000000000000000000001000010010;
    res.bits[0] = 0b00110110101101101000110001000000;
    s21_round(src, &res2);
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(round7) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b10010111011100111001111111111111;
    src.bits[1] = 0b00111100000010000011000110001101;
    src.bits[2] = 0b00000000000000000000000000000001;
    src.bits[3] = 0b10000000000001110000000000000000;
    res.bits[3] = 0b10000000000000000000000000000000;
    res.bits[2] = 0;
    res.bits[1] = 0b00000000000000000000001000010010;
    res.bits[0] = 0b00110110101101101000110001000000;
    s21_round(src, &res2);
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(round8) {
    s21_decimal src, res;
    float a = -1.0 / 0.0;  //, c = 0.0 * 2;
    int b;

    s21_from_float_to_decimal(a, &src);
    ck_assert_int_eq(s21_round(src, &res), 1);
    ck_assert_int_eq(s21_check_inf(res), 1);
    ck_assert_int_eq(s21_getsign(&res), 1);

    s21_from_float_to_decimal(1.0 / 0.0, &src);
    ck_assert_int_eq(s21_round(src, &res), 1);
    ck_assert_int_eq(s21_check_inf(res), 1);

    // a = 1 * 0.0;
    // s21_from_float_to_decimal(a / c, &src);
    // ck_assert_int_eq(s21_round(src, &res), 1);
    // ck_assert_int_eq(res.value_type, s21_nan);

    s21_from_float_to_decimal(8.6, &src);
    s21_round(src, &res);
    s21_from_decimal_to_int(res, &b);
    ck_assert_int_eq(b, 9);

    s21_from_float_to_decimal(8.4, &src);
    s21_round(src, &res);
    s21_from_decimal_to_int(res, &b);
    ck_assert_int_eq(b, 8);

    s21_from_float_to_decimal(-8.4, &src);
    s21_round(src, &res);
    s21_from_decimal_to_int(res, &b);
    ck_assert_int_eq(b, -8);

    s21_from_float_to_decimal(0, &src);
    s21_round(src, &res);
    s21_from_decimal_to_int(res, &b);
    ck_assert_int_eq(b, 0);

    s21_from_float_to_decimal(1.5, &src);
    s21_round(src, &res);
    s21_from_decimal_to_int(res, &b);
    ck_assert_int_eq(b, 2);

    s21_from_float_to_decimal(-1.5, &src);
    s21_round(src, &res);
    s21_from_decimal_to_int(res, &b);
    ck_assert_int_eq(b, -2);

    s21_decimal x = {{95008, 0, 0, 0b10000000000000000000000000000000}};
    s21_set_scale(&x, 6);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "0 0 0 2147483648";
    snprintf(res1, sizeof(char) * 1000, "%u %u %u %u", (unsigned)z.bits[0], (unsigned)z.bits[1],
             (unsigned)z.bits[2], (unsigned)z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(floor1) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b00001111111111111111111111111111;
    src.bits[1] = 0b00111110001001010000001001100001;
    src.bits[2] = 0b00100000010011111100111001011110;
    src.bits[3] = 0b00000000000010100000000000000000;
    s21_floor(src, &res2);
    res.bits[0] = 0b10100111011000111111111111111111;
    res.bits[1] = 0b00001101111000001011011010110011;
    res.bits[2] = 0b00000000000000000000000000000000;
    res.bits[3] = 0b00000000000000000000000000000000;
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(floor2) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b00001111111111111111111111111111;
    src.bits[1] = 0b00111110001001010000001001100001;
    src.bits[2] = 0b00100000010011111100111001011110;
    src.bits[3] = 0b10000000000010100000000000000000;
    s21_floor(src, &res2);
    res.bits[0] = 0b10100111011001000000000000000000;
    res.bits[1] = 0b00001101111000001011011010110011;
    res.bits[2] = 0b00000000000000000000000000000000;
    res.bits[3] = 0b10000000000000000000000000000000;
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(floor3) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b10111101001001111000010001010010;
    src.bits[1] = 0b00000000000000000000000000011100;
    src.bits[2] = 0b00000000000000000000000000000000;
    src.bits[3] = 0b10000000000010010000000000000000;
    s21_floor(src, &res2);
    res.bits[0] = 0b00000000000000000000000001111100;
    res.bits[1] = 0b00000000000000000000000000000000;
    res.bits[2] = 0b00000000000000000000000000000000;
    res.bits[3] = 0b10000000000000000000000000000000;
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(floor4) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b10010011111100000001110001010010;
    src.bits[1] = 0b00000000000000000000000100100000;
    src.bits[2] = 0b00000000000000000000000000000000;
    src.bits[3] = 0b00000000000010100000000000000000;
    s21_floor(src, &res2);
    res.bits[0] = 0b00000000000000000000000001111011;
    res.bits[1] = 0b00000000000000000000000000000000;
    res.bits[2] = 0b00000000000000000000000000000000;
    res.bits[3] = 0b00000000000000000000000000000000;
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(floor5) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b01010101001110101110101110110001;
    src.bits[1] = 0b00001101101101001101101001011111;
    src.bits[2] = 0b00000000000000000000000000000000;
    src.bits[3] = 0b10000000000100100000000000000000;
    s21_floor(src, &res2);
    res.bits[0] = 0b00000000000000000000000000000001;
    res.bits[1] = 0b00000000000000000000000000000000;
    res.bits[2] = 0b00000000000000000000000000000000;
    res.bits[3] = 0b10000000000000000000000000000000;
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(floor6) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b01010101001110101110101110110001;
    src.bits[1] = 0b00001101101101001101101001011111;
    src.bits[2] = 0b00000000000000000000000000000000;
    src.bits[3] = 0b00000000000100100000000000000000;
    s21_floor(src, &res2);
    res.bits[0] = 0b00000000000000000000000000000000;
    res.bits[1] = 0b00000000000000000000000000000000;
    res.bits[2] = 0b00000000000000000000000000000000;
    res.bits[3] = 0b00000000000000000000000000000000;
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(floor7) {
    s21_decimal src, res, res2;
    //   src.value_type = 0;
    src.bits[0] = 0b10010111011100111001111111111111;
    src.bits[1] = 0b00111100000010000011000110001101;
    src.bits[2] = 0b00000000000000000000000000000001;
    src.bits[3] = 0b00000000000001110000000000000000;
    s21_floor(src, &res2);
    res.bits[3] = 0;
    res.bits[2] = 0;
    res.bits[1] = 0b00000000000000000000001000010010;
    res.bits[0] = 0b00110110101101101000110000111111;
    ck_assert_float_eq(res2.bits[0], res.bits[0]);
    ck_assert_float_eq(res2.bits[1], res.bits[1]);
    ck_assert_float_eq(res2.bits[2], res.bits[2]);
    ck_assert_float_eq(res2.bits[3], res.bits[3]);
}
END_TEST
START_TEST(floor8) {
    float a = 1 * 0.0;  //, b = 0.0 * 2;
    s21_decimal src, res;
    //   src.value_type = 0;

    // s21_from_float_to_decimal(a / b, &src);
    // ck_assert_int_eq(s21_floor(src, &res), 1);
    // ck_assert_int_eq(res.value_type, s21_nan);

    //   src.value_type = 0;
    s21_from_float_to_decimal(1 / 0.0, &src);
    ck_assert_int_eq(s21_floor(src, &res), 1);
    ck_assert_int_eq(s21_check_inf(res), 1);

    //   src.value_type = 0;
    s21_from_float_to_decimal(-1 / 0.0, &src);
    ck_assert_int_eq(s21_floor(src, &res), 1);
    // ck_assert_int_eq(res.value_type, s21_neg_infinity);
    ck_assert_int_eq(s21_check_inf(res), 1);
    ck_assert_int_eq(s21_getsign(&res), 1);

    //   src.value_type = 0;
    s21_from_float_to_decimal(1.5, &src);
    ck_assert_int_eq(s21_floor(src, &res), 0);
    s21_from_decimal_to_float(res, &a);
    ck_assert_float_eq(a, floor(1.5));

    s21_from_float_to_decimal(-1.5, &src);
    ck_assert_int_eq(s21_floor(src, &res), 0);
    s21_from_decimal_to_float(res, &a);
    ck_assert_float_eq(a, floor(-1.5));

    s21_set0bits(&src);
    //   src.value_type = 0;
    s21_set0bits(&res);
    //   res.value_type = 0;
    s21_from_float_to_decimal(10.1, &src);
    ck_assert_int_eq(s21_floor(src, &res), 0);
    s21_from_decimal_to_float(res, &a);
    ck_assert_float_eq(a, floor(10.1));

    s21_set0bits(&src);
    //   src.value_type = 0;
    s21_set0bits(&res);
    //   res.value_type = 0;
    s21_from_float_to_decimal(-10.1, &src);
    ck_assert_int_eq(s21_floor(src, &res), 0);
    s21_from_decimal_to_float(res, &a);
    ck_assert_float_eq(a, floor(-10.1));

    s21_set0bits(&src);
    //   src.value_type = 0;
    s21_set0bits(&res);
    //   res.value_type = 0;
    s21_from_float_to_decimal(0.1, &src);
    ck_assert_int_eq(s21_floor(src, &res), 0);
    s21_from_decimal_to_float(res, &a);
    ck_assert_float_eq(a, floor(0.1));

    s21_set0bits(&src);
    //   src.value_type = 0;
    s21_set0bits(&res);
    //   res.value_type = 0;
    s21_from_float_to_decimal(-0.1, &src);
    ck_assert_int_eq(s21_floor(src, &res), 0);
    s21_from_decimal_to_float(res, &a);
    ck_assert_float_eq(a, floor(-0.1));

    s21_set0bits(&src);
    //   src.value_type = 0;
    s21_set0bits(&res);
    //   res.value_type = 0;
    s21_from_float_to_decimal(0, &src);
    ck_assert_int_eq(s21_floor(src, &res), 0);
    s21_from_decimal_to_float(res, &a);
    ck_assert_float_eq(a, floor(0));

    s21_decimal x = {{15008, 0, 0, 0b10000000000000000000000000000000}};
    s21_set_scale(&x, 10);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_floor(x, &z);
    char res1[1000], res2[1000] = "1 0 0 2147483648";
    snprintf(res1, sizeof(char) * 1000, "%u %u %u %u", (unsigned)z.bits[0], (unsigned)z.bits[1],
             (unsigned)z.bits[2], (unsigned)z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(negate1) {
    int int_result = 0;
    s21_decimal source, result;
    s21_from_int_to_decimal(1, &source);
    s21_negate(source, &result);
    s21_from_decimal_to_int(result, &int_result);
    ck_assert_int_eq(-1, int_result);
}
END_TEST
START_TEST(negate2) {
    float f_result = 0;
    s21_decimal source, result;
    s21_from_float_to_decimal(123.456, &source);
    s21_negate(source, &result);
    s21_from_decimal_to_float(result, &f_result);
    ck_assert_float_eq(-123.456, f_result);
}
END_TEST
START_TEST(negate3) {
    float f_result = 0;
    s21_decimal source, result;
    s21_from_float_to_decimal(-123.456e7, &source);
    // s21_print(source);
    // printf("%d %d\n", s21_check_inf(source), s21_get_scale(&source));
    s21_negate(source, &result);
    // s21_print(result);
    s21_from_decimal_to_float(result, &f_result);
    // s21_print(result);
    // ck_assert_float_eq(123.456e7, f_result);
    ck_assert_float_eq_tol(123.456e7, f_result, 1e-6);
}
END_TEST
START_TEST(negate4) {
    int int_result = 0;
    s21_decimal source, result;
    s21_from_int_to_decimal(0, &source);
    s21_negate(source, &result);
    s21_from_decimal_to_int(result, &int_result);
    ck_assert_int_eq(0, int_result);
}
END_TEST
START_TEST(negate5) {
    float f_result = 0;
    s21_decimal source, result;
    s21_from_float_to_decimal(1.0 / 0.0, &source);
    s21_negate(source, &result);
    s21_from_decimal_to_float(result, &f_result);
    ck_assert_int_eq(1, s21_check_inf(result));
    ck_assert_int_eq(1, s21_getsign(&result));
    ck_assert_float_eq(f_result, -1.0 / 0.0);
    ck_assert_float_infinite(f_result);
}
END_TEST
START_TEST(negate6) {
    float f_result = 0;
    s21_decimal source, result;
    s21_from_float_to_decimal(-1.0 / 0.0, &source);
    // s21_print(source);
    s21_negate(source, &result);
    // s21_print(result);
    // printf("%d %d %d %d %d\n", s21_check_inf(result), s21_get_scale(&result),
    // result.bits[0], result.bits[1],
    //        result.bits[2]);
    // if ((s21_get_scale(&result) == 28) && (result.bits[0] == 0) &&
    // (result.bits[1] == 0) &&
    //     (result.bits[2] == 0))
    //     printf("111\n");

    s21_from_decimal_to_float(result, &f_result);
    ck_assert_int_eq(1, s21_check_inf(result));
    ck_assert_float_eq(f_result, 1.0 / 0.0);
    ck_assert_float_infinite(f_result);
}
END_TEST
// START_TEST(negate7) {
//     float f_result = 0, a = 1 * 0.0, b = 0.0 * 2;
//     s21_decimal source, result;
//     s21_from_float_to_decimal(a / b, &source);
//     ck_assert_int_eq(1, s21_negate(source, &result));
//     s21_from_decimal_to_float(result, &f_result);
//     // ck_assert_float_nan(f_result);
// }
// END_TEST

int main(void) {
    Suite *s1 = suite_create("Suite");
    SRunner *sr = srunner_create(s1);

    TCase *tc1_1 = tcase_create("tests");
    suite_add_tcase(s1, tc1_1);

    tcase_add_test(tc1_1, less);
    tcase_add_test(tc1_1, greater);
    tcase_add_test(tc1_1, less_or_equal);
    tcase_add_test(tc1_1, greater_or_equal);
    tcase_add_test(tc1_1, equal);
    tcase_add_test(tc1_1, not_equal);

    tcase_add_test(tc1_1, int_to_decimal);

    tcase_add_test(tc1_1, decimal_to_int1);
    tcase_add_test(tc1_1, decimal_to_int2);
    tcase_add_test(tc1_1, decimal_to_int3);
    tcase_add_test(tc1_1, decimal_to_int4);
    tcase_add_test(tc1_1, decimal_to_int5);
    tcase_add_test(tc1_1, decimal_to_int6);

    tcase_add_test(tc1_1, from_float_to_decimal1);
    tcase_add_test(tc1_1, from_float_to_decimal2);
    tcase_add_test(tc1_1, from_float_to_decimal3);
    tcase_add_test(tc1_1, from_float_to_decimal4);
    tcase_add_test(tc1_1, from_float_to_decimal5);
    tcase_add_test(tc1_1, from_float_to_decimal6);
    tcase_add_test(tc1_1, from_float_to_decimal7);
    tcase_add_test(tc1_1, from_float_to_decimal8);
    tcase_add_test(tc1_1, from_float_to_decimal9);

    tcase_add_test(tc1_1, decimal_to_float1);
    tcase_add_test(tc1_1, decimal_to_float2);
    tcase_add_test(tc1_1, decimal_to_float3);
    tcase_add_test(tc1_1, decimal_to_float4);
    tcase_add_test(tc1_1, decimal_to_float5);

    tcase_add_test(tc1_1, add1);
    tcase_add_test(tc1_1, add2);
    // tcase_add_test(tc1_1, add3);
    tcase_add_test(tc1_1, add4);
    tcase_add_test(tc1_1, add5);
    tcase_add_test(tc1_1, add6);
    tcase_add_test(tc1_1, add7);
    tcase_add_test(tc1_1, add8);
    tcase_add_test(tc1_1, add9);
    tcase_add_test(tc1_1, add10);
    tcase_add_test(tc1_1, add11);
    tcase_add_test(tc1_1, add12);
    tcase_add_test(tc1_1, add13);
    tcase_add_test(tc1_1, add14);
    tcase_add_test(tc1_1, add15);
    tcase_add_test(tc1_1, add16);
    tcase_add_test(tc1_1, add17);
    tcase_add_test(tc1_1, add18);
    tcase_add_test(tc1_1, add19);
    tcase_add_test(tc1_1, add20);
    tcase_add_test(tc1_1, add21);

    tcase_add_test(tc1_1, sub1);
    tcase_add_test(tc1_1, sub2);
    tcase_add_test(tc1_1, sub3);
    tcase_add_test(tc1_1, sub4);
    tcase_add_test(tc1_1, sub5);
    tcase_add_test(tc1_1, sub6);
    tcase_add_test(tc1_1, sub7);
    tcase_add_test(tc1_1, sub8);
    tcase_add_test(tc1_1, sub9);
    tcase_add_test(tc1_1, sub10);
    tcase_add_test(tc1_1, sub11);
    tcase_add_test(tc1_1, sub12);
    tcase_add_test(tc1_1, sub13);
    tcase_add_test(tc1_1, sub14);
    tcase_add_test(tc1_1, sub15);
    tcase_add_test(tc1_1, sub16);

    tcase_add_test(tc1_1, mul1);
    tcase_add_test(tc1_1, mul1_1);
    tcase_add_test(tc1_1, mul2);
    tcase_add_test(tc1_1, mul3);
    tcase_add_test(tc1_1, mul4);
    tcase_add_test(tc1_1, mul41);
    tcase_add_test(tc1_1, mul5);
    tcase_add_test(tc1_1, mul6);
    tcase_add_test(tc1_1, mul7);
    tcase_add_test(tc1_1, mul8);
    tcase_add_test(tc1_1, mul9);
    tcase_add_test(tc1_1, mul10);
    tcase_add_test(tc1_1, mul11);
    tcase_add_test(tc1_1, mul12);
    tcase_add_test(tc1_1, mul13);
    tcase_add_test(tc1_1, mul14);
    tcase_add_test(tc1_1, mul15);
    tcase_add_test(tc1_1, mul16);
    tcase_add_test(tc1_1, mul17);
    tcase_add_test(tc1_1, mul18);

    tcase_add_test(tc1_1, mod1);
    tcase_add_test(tc1_1, mod2);
    tcase_add_test(tc1_1, mod3);
    tcase_add_test(tc1_1, mod4);
    tcase_add_test(tc1_1, mod41);
    tcase_add_test(tc1_1, mod5);
    tcase_add_test(tc1_1, mod6);
    tcase_add_test(tc1_1, mod7);
    tcase_add_test(tc1_1, mod8);
    tcase_add_test(tc1_1, mod9);

    tcase_add_test(tc1_1, div1);
    tcase_add_test(tc1_1, div2);
    tcase_add_test(tc1_1, div3);
    tcase_add_test(tc1_1, div4);
    tcase_add_test(tc1_1, div5);
    tcase_add_test(tc1_1, div6);
    tcase_add_test(tc1_1, div7);
    tcase_add_test(tc1_1, div8);
    tcase_add_test(tc1_1, div9);
    tcase_add_test(tc1_1, div10);
    tcase_add_test(tc1_1, div11);
    // tcase_add_test(tc1_1, div12);
    tcase_add_test(tc1_1, div13);
    tcase_add_test(tc1_1, div15);
    tcase_add_test(tc1_1, div16);
    tcase_add_test(tc1_1, div17);
    // tcase_add_test(tc1_1, div18);
    tcase_add_test(tc1_1, div19);

    tcase_add_test(tc1_1, truncate1);
    tcase_add_test(tc1_1, truncate2);
    tcase_add_test(tc1_1, truncate3);
    tcase_add_test(tc1_1, truncate4);
    tcase_add_test(tc1_1, truncate5);
    tcase_add_test(tc1_1, truncate6);
    tcase_add_test(tc1_1, truncate7);
    tcase_add_test(tc1_1, truncate8);
    tcase_add_test(tc1_1, truncate9);
    tcase_add_test(tc1_1, truncate10);
    tcase_add_test(tc1_1, truncate11);
    // tcase_add_test(tc1_1, truncate12);
    tcase_add_test(tc1_1, truncate13);
    tcase_add_test(tc1_1, truncate14);

    tcase_add_test(tc1_1, round1);
    tcase_add_test(tc1_1, round2);
    tcase_add_test(tc1_1, round3);
    tcase_add_test(tc1_1, round4);
    tcase_add_test(tc1_1, round5);
    tcase_add_test(tc1_1, round6);
    tcase_add_test(tc1_1, round7);
    tcase_add_test(tc1_1, round8);

    tcase_add_test(tc1_1, floor1);
    tcase_add_test(tc1_1, floor2);
    tcase_add_test(tc1_1, floor3);
    tcase_add_test(tc1_1, floor4);
    tcase_add_test(tc1_1, floor5);
    tcase_add_test(tc1_1, floor6);
    tcase_add_test(tc1_1, floor7);
    tcase_add_test(tc1_1, floor8);

    tcase_add_test(tc1_1, negate1);
    tcase_add_test(tc1_1, negate2);
    tcase_add_test(tc1_1, negate3);
    tcase_add_test(tc1_1, negate4);
    tcase_add_test(tc1_1, negate5);
    tcase_add_test(tc1_1, negate6);
    // tcase_add_test(tc1_1, negate7);

    srunner_run_all(sr, CK_ENV);
    srunner_free(sr);

    return 0;
}
