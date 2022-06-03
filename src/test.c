#include <check.h>
#include "s21_decimal.h"

// START_TEST(s21_trun_1) {
//     s21_decimal src1;
//     src1.value_type = s21_usual;
//     src1.bits[3] = 0x000A0000;
//     src1.bits[2] = 0x0;
//     src1.bits[1] = 0xFFFFFFFF;
//     src1.bits[0] = 0xFFFFFFFF;
//     s21_decimal result;
//     result.bits[3] = 0x0;
//     result.bits[2] = 0x0;
//     result.bits[1] = 0x0;
//     result.bits[0] = 0x6DF37F67;
//     s21_decimal res_od = s21_truncate(src1);
//     ck_assert_float_eq(res_od.bits[0], result.bits[0]);
//     ck_assert_float_eq(res_od.bits[1], result.bits[1]);
//     ck_assert_float_eq(res_od.bits[2], result.bits[2]);
//     ck_assert_float_eq(res_od.bits[3], result.bits[3]);

//     float s_result, f_result;
//     s21_from_decimal_to_float(src1, &s_result);
//     s21_from_decimal_to_float(result, &f_result);
//     printf("%f %f\n", s_result, f_result);
// }
// END_TEST
// START_TEST(s21_trun_2) {
//     s21_decimal src1;
//     src1.value_type = s21_usual;
//     src1.bits[3] = 0x800C0000;
//     src1.bits[2] = 0x0;
//     src1.bits[1] = 0xFFFFFFFF;
//     src1.bits[0] = 0xFFFFFFFF;
//     s21_decimal result;
//     result.bits[3] = 0x80000000;
//     result.bits[2] = 0x0;
//     result.bits[1] = 0x0;
//     result.bits[0] = 0x1197998;
//     s21_decimal res_od = s21_truncate(src1);
//     ck_assert_float_eq(res_od.bits[0], result.bits[0]);
//     ck_assert_float_eq(res_od.bits[1], result.bits[1]);
//     ck_assert_float_eq(res_od.bits[2], result.bits[2]);
//     ck_assert_float_eq(res_od.bits[3], result.bits[3]);
// }
// END_TEST
// START_TEST(s21_trun_3) {
//     s21_decimal src1;
//     src1.value_type = s21_usual;
//     src1.bits[3] = 0x80000000;
//     src1.bits[2] = 0x0;
//     src1.bits[1] = 0xFFFFFFFF;
//     src1.bits[0] = 0xFFFFFFFF;
//     s21_decimal result;
//     result.bits[3] = 0x80000000;
//     result.bits[2] = 0x0;
//     result.bits[1] = 0xFFFFFFFF;
//     result.bits[0] = 0xFFFFFFFF;
//     s21_decimal res_od = s21_truncate(src1);
//     ck_assert_float_eq(res_od.bits[0], result.bits[0]);
//     ck_assert_float_eq(res_od.bits[1], result.bits[1]);
//     ck_assert_float_eq(res_od.bits[2], result.bits[2]);
//     ck_assert_float_eq(res_od.bits[3], result.bits[3]);
// }
// END_TEST
// START_TEST(s21_trun_4) {
//     s21_decimal src1;
//     src1.value_type = s21_usual;
//     src1.bits[0] = 0b00000000000000000000000001101101;
//     src1.bits[1] = 0b00000000000011000110010101011011;
//     src1.bits[2] = 0b00000000000000000011000000111001;
//     src1.bits[3] = 0b00000000000011100000000000000000;
//     s21_decimal result;
//     result.bits[0] = 0b10000111101111000001011000011110;
//     result.bits[1] = 0b00000000000000000000000000000000;
//     result.bits[2] = 0b00000000000000000000000000000000;
//     result.bits[3] = 0b00000000000000000000000000000000;
//     s21_decimal res_od = s21_truncate(src1);
//     ck_assert_float_eq(res_od.bits[0], result.bits[0]);
//     ck_assert_float_eq(res_od.bits[1], result.bits[1]);
//     ck_assert_float_eq(res_od.bits[2], result.bits[2]);
//     ck_assert_float_eq(res_od.bits[3], result.bits[3]);
// }
// END_TEST
// START_TEST(s21_trun_5) {
//     s21_decimal src1;
//     src1.value_type = s21_usual;
//     src1.bits[0] = 0b11011110101001001000000010110110;
//     src1.bits[1] = 0b01011111101001011101111110100000;
//     src1.bits[2] = 0b00011111111010011010111101100000;
//     src1.bits[3] = 0b00000000000010010000000000000000;
//     s21_decimal result;
//     result.bits[0] = 0b00110100101011011100000100111110;
//     result.bits[1] = 0b10001001000100001000011110111001;
//     result.bits[2] = 0b00000000000000000000000000000000;
//     result.bits[3] = 0b00000000000000000000000000000000;
//     s21_decimal res_od = s21_truncate(src1);
//     ck_assert_float_eq(res_od.bits[0], result.bits[0]);
//     ck_assert_float_eq(res_od.bits[1], result.bits[1]);
//     ck_assert_float_eq(res_od.bits[2], result.bits[2]);
//     ck_assert_float_eq(res_od.bits[3], result.bits[3]);
// }
// END_TEST
// START_TEST(s21_trun_6) {
//     s21_decimal src1;
//     src1.value_type = s21_usual;
//     src1.bits[3] = 0x80090000;
//     src1.bits[2] = 0;
//     src1.bits[1] = 0xFFFFEA34;
//     src1.bits[0] = 0xFF837E4F;
//     s21_decimal result = s21_truncate(src1);
//     s21_decimal result_origin;
//     result_origin.bits[3] = 0x80000000;
//     result_origin.bits[2] = 0;
//     result_origin.bits[1] = 0x4;
//     result_origin.bits[0] = 0x4B829C70;
//     ck_assert_int_eq(result_origin.bits[3], result.bits[3]);
//     ck_assert_int_eq(result_origin.bits[2], result.bits[2]);
//     ck_assert_int_eq(result_origin.bits[1], result.bits[1]);
//     ck_assert_int_eq(result_origin.bits[0], result.bits[0]);
// }
// END_TEST
START_TEST(s21_trun_7) {
    s21_decimal src, result, result2;
    src.value_type = s21_usual;
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
START_TEST(s21_trun_8) {
    s21_decimal src, res;
    src.value_type = s21_usual;
    src.bits[0] = 0b01111000101001111011110011000110;  // = 0.987654
    src.bits[1] = 0b10011111111111001111101010000100;
    src.bits[2] = 0b00011111111010011010111101101101;
    src.bits[3] = 0b00000000000111000000000000000000;
    s21_decimal result = {{0, 0, 0, 0}, s21_usual};  // = 0
    s21_truncate(src, &res);
    ck_assert_int_eq(res.bits[0], result.bits[0]);
    ck_assert_int_eq(res.bits[1], result.bits[1]);
    ck_assert_int_eq(res.bits[2], result.bits[2]);
    ck_assert_int_eq(res.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_trun_9) {
    s21_decimal src, res;
    s21_from_float_to_decimal(1.0 / 0.0, &src);
    s21_truncate(src, &res);
    ck_assert_float_eq(res.value_type, s21_infinity);
}
END_TEST
START_TEST(s21_trun_10) {
    float f_result;
    s21_decimal src, res;
    s21_from_float_to_decimal(1.9, &src);
    s21_truncate(src, &res);
    s21_from_decimal_to_float(res, &f_result);
    ck_assert_float_eq(f_result, 1.0);
}
END_TEST

START_TEST(s21_round_1) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b01100011000011111111111111111111;
    src1.bits[1] = 0b01101011110001110101111000101101;
    src1.bits[2] = 0b00000000000000000000000000000101;
    src1.bits[3] = 0b10000000000010100000000000000000;
    s21_decimal result;
    result.bits[0] = 0b01010100000010111110010000000000;
    result.bits[1] = 0b00000000000000000000000000000010;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b10000000000000000000000000000000;
    s21_decimal res_od = s21_round(src1);
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_round_2) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b01010101001110101110101110110001;
    src1.bits[1] = 0b00001101101101001101101001011111;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b10000000000100100000000000000000;
    s21_decimal result;
    result.bits[0] = 0b00000000000000000000000000000001;
    result.bits[1] = 0b00000000000000000000000000000000;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b10000000000000000000000000000000;
    s21_decimal res_od = s21_round(src1);
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_round_3) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b10010011111100000001110001010010;
    src1.bits[1] = 0b00000000000000000000000100100000;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b00000000000010100000000000000000;
    s21_decimal result;
    result.bits[0] = 0b00000000000000000000000001111100;
    result.bits[1] = 0b00000000000000000000000000000000;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal res_od = s21_round(src1);
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_round_4) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b11001101110001110111110001000000;
    src1.bits[1] = 0b00111110001001010000001001100000;
    src1.bits[2] = 0b00100000010011111100111001011110;
    src1.bits[3] = 0b10000000000001000000000000000000;
    s21_decimal result;
    result.bits[0] = 0b10100000111111100100111000000100;
    result.bits[1] = 0b00011011110011101100110011101101;
    result.bits[2] = 0b00000000000000001101001111000010;
    result.bits[3] = 0b10000000000000000000000000000000;
    s21_decimal res_od = s21_round(src1);
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_round_5) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b10100000111111100100111000000100;
    src1.bits[1] = 0b00011011110011101100110011101101;
    src1.bits[2] = 0b00000000000000001101001111000010;
    src1.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal res_od = s21_round(src1);
    s21_decimal result;
    result.bits[0] = 0b10100000111111100100111000000100;
    result.bits[1] = 0b00011011110011101100110011101101;
    result.bits[2] = 0b00000000000000001101001111000010;
    result.bits[3] = 0b00000000000000000000000000000000;
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_round_6) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b10010111011100111001111111111111;
    src1.bits[1] = 0b00111100000010000011000110001101;
    src1.bits[2] = 0b00000000000000000000000000000001;
    src1.bits[3] = 0b00000000000001110000000000000000;
    s21_decimal res_od = s21_round(src1);
    s21_decimal result;
    result.bits[3] = 0;
    result.bits[2] = 0;
    result.bits[1] = 0b00000000000000000000001000010010;
    result.bits[0] = 0b00110110101101101000110001000000;
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_round_7) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b10010111011100111001111111111111;
    src1.bits[1] = 0b00111100000010000011000110001101;
    src1.bits[2] = 0b00000000000000000000000000000001;
    src1.bits[3] = 0b10000000000001110000000000000000;
    s21_decimal res_od = s21_round(src1);
    s21_decimal result;
    result.bits[3] = 0b10000000000000000000000000000000;
    result.bits[2] = 0;
    result.bits[1] = 0b00000000000000000000001000010010;
    result.bits[0] = 0b00110110101101101000110001000000;
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_round_8) {
    s21_decimal src1;
    float a = -1.0 / 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_decimal res_od = s21_round(src1);
    ck_assert_float_eq(res_od.value_type, s21_neg_infinity);
}
END_TEST

START_TEST(s21_floor_1) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b00001111111111111111111111111111;
    src1.bits[1] = 0b00111110001001010000001001100001;
    src1.bits[2] = 0b00100000010011111100111001011110;
    src1.bits[3] = 0b00000000000010100000000000000000;
    s21_decimal res_od = s21_floor(src1);
    s21_decimal result;
    result.bits[0] = 0b10100111011000111111111111111111;
    result.bits[1] = 0b00001101111000001011011010110011;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b00000000000000000000000000000000;
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_floor_2) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b00001111111111111111111111111111;
    src1.bits[1] = 0b00111110001001010000001001100001;
    src1.bits[2] = 0b00100000010011111100111001011110;
    src1.bits[3] = 0b10000000000010100000000000000000;
    s21_decimal res_od = s21_floor(src1);
    s21_decimal result;
    result.bits[0] = 0b10100111011001000000000000000000;
    result.bits[1] = 0b00001101111000001011011010110011;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b10000000000000000000000000000000;
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_floor_3) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b10111101001001111000010001010010;
    src1.bits[1] = 0b00000000000000000000000000011100;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b10000000000010010000000000000000;
    s21_decimal res_od = s21_floor(src1);
    s21_decimal result;
    result.bits[0] = 0b00000000000000000000000001111100;
    result.bits[1] = 0b00000000000000000000000000000000;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b10000000000000000000000000000000;
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_floor_4) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b10010011111100000001110001010010;
    src1.bits[1] = 0b00000000000000000000000100100000;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b00000000000010100000000000000000;
    s21_decimal res_od = s21_floor(src1);
    s21_decimal result;
    result.bits[0] = 0b00000000000000000000000001111011;
    result.bits[1] = 0b00000000000000000000000000000000;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b00000000000000000000000000000000;
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_floor_5) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b01010101001110101110101110110001;
    src1.bits[1] = 0b00001101101101001101101001011111;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b10000000000100100000000000000000;
    s21_decimal res_od = s21_floor(src1);
    s21_decimal result;
    result.bits[0] = 0b00000000000000000000000000000001;
    result.bits[1] = 0b00000000000000000000000000000000;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b10000000000000000000000000000000;
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_floor_6) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b01010101001110101110101110110001;
    src1.bits[1] = 0b00001101101101001101101001011111;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b00000000000100100000000000000000;
    s21_decimal res_od = s21_floor(src1);
    s21_decimal result;
    result.bits[0] = 0b00000000000000000000000000000000;
    result.bits[1] = 0b00000000000000000000000000000000;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b00000000000000000000000000000000;
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST
START_TEST(s21_floor_7) {
    s21_decimal src1;
    src1.value_type = s21_usual;
    src1.bits[0] = 0b10010111011100111001111111111111;
    src1.bits[1] = 0b00111100000010000011000110001101;
    src1.bits[2] = 0b00000000000000000000000000000001;
    src1.bits[3] = 0b00000000000001110000000000000000;
    s21_decimal result = s21_floor(src1);
    s21_decimal result_origin;
    result_origin.bits[3] = 0;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0b00000000000000000000001000010010;
    result_origin.bits[0] = 0b00110110101101101000110000111111;
    ck_assert_int_eq(result_origin.bits[3], result.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result.bits[0]);
}
END_TEST
START_TEST(s21_floor_8) {
    float a = 1*0.0, b = 0.0*2;
    s21_decimal src1;
    src1.value_type = 0;
    // float a = 0.0 / 0.0;
    s21_from_float_to_decimal(a/b, &src1);
    s21_decimal res_od = s21_floor(src1);
    ck_assert_float_eq(res_od.value_type, s21_nan);
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
    s21_negate(source, &result);
    s21_from_decimal_to_float(result, &f_result);
    ck_assert_float_eq(123.456e7, f_result);
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
    s21_from_float_to_decimal(1.0/0.0, &source);
    s21_negate(source, &result);
    s21_from_decimal_to_float(result, &f_result);
    ck_assert_float_eq(s21_neg_infinity, result.value_type);
    ck_assert_float_eq(f_result, -1.0/0.0);
    ck_assert_float_infinite(f_result);
}
END_TEST
START_TEST(negate6) {
    float f_result = 0;
    s21_decimal source, result;
    s21_from_float_to_decimal(-1.0/0.0, &source);
    s21_negate(source, &result);
    s21_from_decimal_to_float(result, &f_result);
    ck_assert_float_eq(s21_infinity, result.value_type);
    ck_assert_float_eq(f_result, 1.0/0.0);
    ck_assert_float_infinite(f_result);
}
END_TEST
START_TEST(negate7) {
    float f_result = 0, a = 1*0.0, b = 0.0*2;
    s21_decimal source, result;
    s21_from_float_to_decimal(a/b, &source);
    s21_negate(source, &result);
    s21_from_decimal_to_float(result, &f_result);
    ck_assert_float_nan(f_result);
}
END_TEST

int main(void) {
    Suite *s1 = suite_create("Suite");
    SRunner *sr = srunner_create(s1);

    TCase *tc1_1 = tcase_create("tests");
    suite_add_tcase(s1, tc1_1);

    // tcase_add_test(tc1_1, s21_trun_1);
    // tcase_add_test(tc1_1, s21_trun_2);
    // tcase_add_test(tc1_1, s21_trun_3);
    // tcase_add_test(tc1_1, s21_trun_4);
    // tcase_add_test(tc1_1, s21_trun_5);
    // tcase_add_test(tc1_1, s21_trun_6);
    tcase_add_test(tc1_1, s21_trun_7);
    tcase_add_test(tc1_1, s21_trun_8);
    tcase_add_test(tc1_1, s21_trun_9);
    tcase_add_test(tc1_1, s21_trun_10);

    tcase_add_test(tc1_1, s21_round_1);
    tcase_add_test(tc1_1, s21_round_2);
    tcase_add_test(tc1_1, s21_round_3);
    tcase_add_test(tc1_1, s21_round_4);
    tcase_add_test(tc1_1, s21_round_5);
    tcase_add_test(tc1_1, s21_round_6);
    tcase_add_test(tc1_1, s21_round_7);
    tcase_add_test(tc1_1, s21_round_8);

    tcase_add_test(tc1_1, s21_floor_1);
    tcase_add_test(tc1_1, s21_floor_2);
    tcase_add_test(tc1_1, s21_floor_3);
    tcase_add_test(tc1_1, s21_floor_4);
    tcase_add_test(tc1_1, s21_floor_5);
    tcase_add_test(tc1_1, s21_floor_6);
    tcase_add_test(tc1_1, s21_floor_7);
    tcase_add_test(tc1_1, s21_floor_8);

    tcase_add_test(tc1_1, negate1);
    tcase_add_test(tc1_1, negate2);
    tcase_add_test(tc1_1, negate3);
    tcase_add_test(tc1_1, negate4);
    tcase_add_test(tc1_1, negate5);
    tcase_add_test(tc1_1, negate6);
    tcase_add_test(tc1_1, negate7);

    srunner_run_all(sr, CK_ENV);
    srunner_free(sr);

    return 0;
}
