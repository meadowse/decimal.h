#include <check.h>
#include <stdio.h>

#include "s21_decimal.h"

START_TEST(truncate1) {
  s21_decimal src, result, result2;
  src.value_type = s21_usual;
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
  src.value_type = s21_usual;
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
  src.value_type = s21_usual;
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
  src.value_type = s21_usual;
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
  src.value_type = s21_usual;
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
  src.value_type = s21_usual;
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
START_TEST(truncate8) {
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
START_TEST(truncate9) {
  s21_decimal src, res;
  s21_from_float_to_decimal(1.0 / 0.0, &src);
  s21_truncate(src, &res);
  ck_assert_float_eq(res.value_type, s21_infinity);
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
START_TEST(truncate12) {
  float f_result, a = 1 * 0.0, b = 0.0 * 2;
  s21_decimal src, res;
  s21_from_float_to_decimal(a / b, &src);
  ck_assert_int_eq(1, s21_truncate(src, &res));
  s21_from_decimal_to_float(res, &f_result);
  ck_assert_float_nan(f_result);
}
END_TEST
START_TEST(truncate13) {
  float f_result;
  s21_decimal src, res;
  s21_from_float_to_decimal(1.0 / 0.0, &src);
  ck_assert_int_eq(1, s21_truncate(src, &res));
  s21_from_decimal_to_float(res, &f_result);
  ck_assert_float_eq(s21_infinity, res.value_type);
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
  ck_assert_float_eq(s21_neg_infinity, res.value_type);
  ck_assert_float_eq(f_result, -1.0 / 0.0);
  ck_assert_float_infinite(f_result);
}
END_TEST

START_TEST(round1) {
  s21_decimal src, res, res2;
  src.value_type = 0;
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
  src.value_type = 0;
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
  src.value_type = 0;
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
  src.value_type = 0;
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
  src.value_type = 0;
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
  src.value_type = 0;
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
  src.value_type = 0;
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
  float a = -1.0 / 0.0, c = 0.0 * 2;
  int b;

  s21_from_float_to_decimal(a, &src);
  ck_assert_int_eq(s21_round(src, &res), 1);
  ck_assert_int_eq(res.value_type, s21_neg_infinity);

  s21_from_float_to_decimal(1.0 / 0.0, &src);
  ck_assert_int_eq(s21_round(src, &res), 1);
  ck_assert_int_eq(res.value_type, s21_infinity);

  a = 1 * 0.0;
  s21_from_float_to_decimal(a / c, &src);
  ck_assert_int_eq(s21_round(src, &res), 1);
  ck_assert_int_eq(res.value_type, s21_nan);

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

  s21_decimal x = {{95008, 0, 0, 0b10000000000000000000000000000000}, 0};
  s21_set_scale(&x, 6);
  s21_decimal z = {{0, 0, 0, 0}, 0};
  s21_round(x, &z);
  char res1[1000], res2[1000] = "0 0 0 2147483648 0";
  snprintf(res1, sizeof(char) * 1000, "%u %u %u %u %u", (unsigned)z.bits[0],
           (unsigned)z.bits[1], (unsigned)z.bits[2], (unsigned)z.bits[3],
           (unsigned)z.value_type);
  ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(floor1) {
  s21_decimal src, res, res2;
  src.value_type = 0;
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
  src.value_type = 0;
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
  src.value_type = 0;
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
  src.value_type = 0;
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
  src.value_type = 0;
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
  src.value_type = 0;
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
  src.value_type = 0;
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
  float a = 1 * 0.0, b = 0.0 * 2;
  s21_decimal src, res;
  src.value_type = 0;

  s21_from_float_to_decimal(a / b, &src);
  ck_assert_int_eq(s21_floor(src, &res), 1);
  // printf("%d\n", res.value_type);
  ck_assert_int_eq(res.value_type, s21_nan);

  src.value_type = 0;
  s21_from_float_to_decimal(1 / 0.0, &src);
  ck_assert_int_eq(s21_floor(src, &res), 1);
  ck_assert_int_eq(res.value_type, s21_infinity);

  src.value_type = 0;
  s21_from_float_to_decimal(-1 / 0.0, &src);
  ck_assert_int_eq(s21_floor(src, &res), 1);
  ck_assert_int_eq(res.value_type, s21_neg_infinity);

  src.value_type = 0;
  s21_from_float_to_decimal(1.5, &src);
  ck_assert_int_eq(s21_floor(src, &res), 0);
  s21_from_decimal_to_float(res, &a);
  ck_assert_float_eq(a, floor(1.5));

  s21_from_float_to_decimal(-1.5, &src);
  ck_assert_int_eq(s21_floor(src, &res), 0);
  s21_from_decimal_to_float(res, &a);
  ck_assert_float_eq(a, floor(-1.5));

  s21_set0bits(&src);
  src.value_type = 0;
  s21_set0bits(&res);
  res.value_type = 0;
  s21_from_float_to_decimal(10.1, &src);
  ck_assert_int_eq(s21_floor(src, &res), 0);
  s21_from_decimal_to_float(res, &a);
  ck_assert_float_eq(a, floor(10.1));

  s21_set0bits(&src);
  src.value_type = 0;
  s21_set0bits(&res);
  res.value_type = 0;
  s21_from_float_to_decimal(-10.1, &src);
  ck_assert_int_eq(s21_floor(src, &res), 0);
  s21_from_decimal_to_float(res, &a);
  ck_assert_float_eq(a, floor(-10.1));

  s21_set0bits(&src);
  src.value_type = 0;
  s21_set0bits(&res);
  res.value_type = 0;
  s21_from_float_to_decimal(0.1, &src);
  ck_assert_int_eq(s21_floor(src, &res), 0);
  s21_from_decimal_to_float(res, &a);
  ck_assert_float_eq(a, floor(0.1));

  s21_set0bits(&src);
  src.value_type = 0;
  s21_set0bits(&res);
  res.value_type = 0;
  s21_from_float_to_decimal(-0.1, &src);
  ck_assert_int_eq(s21_floor(src, &res), 0);
  s21_from_decimal_to_float(res, &a);
  ck_assert_float_eq(a, floor(-0.1));

  s21_set0bits(&src);
  src.value_type = 0;
  s21_set0bits(&res);
  res.value_type = 0;
  s21_from_float_to_decimal(0, &src);
  ck_assert_int_eq(s21_floor(src, &res), 0);
  s21_from_decimal_to_float(res, &a);
  ck_assert_float_eq(a, floor(0));

  s21_decimal x = {{15008, 0, 0, 0b10000000000000000000000000000000}, 0};
  s21_set_scale(&x, 10);
  s21_decimal z = {{0, 0, 0, 0}, 0};
  s21_floor(x, &z);
  char res1[1000], res2[1000] = "1 0 0 2147483648 0";
  snprintf(res1, sizeof(char) * 1000, "%u %u %u %u %u", (unsigned)z.bits[0],
           (unsigned)z.bits[1], (unsigned)z.bits[2], (unsigned)z.bits[3],
           (unsigned)z.value_type);
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
  s21_from_float_to_decimal(1.0 / 0.0, &source);
  s21_negate(source, &result);
  s21_from_decimal_to_float(result, &f_result);
  ck_assert_float_eq(s21_neg_infinity, result.value_type);
  ck_assert_float_eq(f_result, -1.0 / 0.0);
  ck_assert_float_infinite(f_result);
}
END_TEST
START_TEST(negate6) {
  float f_result = 0;
  s21_decimal source, result;
  s21_from_float_to_decimal(-1.0 / 0.0, &source);
  s21_negate(source, &result);
  s21_from_decimal_to_float(result, &f_result);
  ck_assert_float_eq(s21_infinity, result.value_type);
  ck_assert_float_eq(f_result, 1.0 / 0.0);
  ck_assert_float_infinite(f_result);
}
END_TEST
START_TEST(negate7) {
  float f_result = 0, a = 1 * 0.0, b = 0.0 * 2;
  s21_decimal source, result;
  s21_from_float_to_decimal(a / b, &source);
  ck_assert_int_eq(1, s21_negate(source, &result));
  s21_from_decimal_to_float(result, &f_result);
  ck_assert_float_nan(f_result);
}
END_TEST

// мои тесты
START_TEST(test_s21_is_less) {
  s21_decimal case_max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}, 0};
  s21_decimal case_min = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}, 0};
  s21_decimal case1 = {{0, 0, 0, 0}, 0};
  s21_decimal case2 = {{1, 0, 0, 0}, 0};

  ck_assert_msg(s21_is_less(case1, case2) == 0, "less1");
  ck_assert_msg(s21_is_less(case_max, case_min) == 1, "less2");
  ck_assert_msg(s21_is_less(case_min, case_max) == 0, "less3");
  ck_assert_msg(s21_is_less(case_min, case1) == 0, "less4");
  ck_assert_msg(s21_is_less(case2, case2) == 1, "less5");
}

START_TEST(test_s21_is_less_or_equal) {
  s21_decimal case_max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}, 0};
  s21_decimal case_min = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}, 0};
  s21_decimal case1 = {{0, 0, 0, 0}, 0};
  s21_decimal case2 = {{1, 0, 0, 0}, 0};
  s21_decimal case3 = {{0x7FFFFFFF, 0, 0, 0}, 0};

  ck_assert_msg(s21_is_less_or_equal(case1, case2) == 0, "less_or_equal1");
  ck_assert_msg(s21_is_less_or_equal(case_max, case_min) == 1, "less_or_equal2");
  ck_assert_msg(s21_is_less_or_equal(case_min, case_max) == 0, "less_or_equal3");
  ck_assert_msg(s21_is_less_or_equal(case_min, case1) == 0, "less_or_equal4");
  ck_assert_msg(s21_is_less_or_equal(case2, case3) == 0, "less_or_equal5");
}
END_TEST

START_TEST(test_s21_is_greater) {
  s21_decimal case_max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}, 0};
  s21_decimal case_min = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}, 0};
  s21_decimal case1 = {{0, 0, 0, 0}, 0};
  s21_decimal case2 = {{1, 0, 0, 0}, 0};
  s21_decimal case3 = {{0x7FFFFFFF, 0, 0, 0}, 0};

  ck_assert_msg(s21_is_greater(case1, case2) == 1, "greater1");
  ck_assert_msg(s21_is_greater(case_max, case_min) == 0, "greater2");
  ck_assert_msg(s21_is_greater(case_min, case_max) == 1, "greater3");
  ck_assert_msg(s21_is_greater(case_min, case1) == 1, "greater4");
  ck_assert_msg(s21_is_greater(case2, case3) == 1, "greater5");
}
END_TEST

START_TEST(test_s21_is_greater_or_equal) {
  s21_decimal case_max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}, 0};
  s21_decimal case_min = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}, 0};
  s21_decimal case1 = {{0, 0, 0, 0}, 0};
  s21_decimal case2 = {{1, 0, 0, 0}, 0};

  ck_assert_msg(s21_is_greater_or_equal(case1, case2) == 1, "greater_or_equal1");
  ck_assert_msg(s21_is_greater_or_equal(case_max, case_min) == 0, "greater_or_equal2");
  ck_assert_msg(s21_is_greater_or_equal(case_min, case_max) == 1, "greater_or_equal3");
  ck_assert_msg(s21_is_greater_or_equal(case_min, case1) == 1, "greater_or_equal4");
  ck_assert_msg(s21_is_greater_or_equal(case2, case2) == 0, "greater_or_equal5");
}
END_TEST

START_TEST(test_s21_is_equal) {
  s21_decimal case_max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}, 0};
  s21_decimal case_min = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}, 0};
  s21_decimal case1 = {{0, 0, 0, 0}, 0};
  s21_decimal case2 = {{1, 0, 0, 0}, 0};
  s21_decimal case3 = {{0x7FFFFFFF, 0, 0, 0}, 0};

  ck_assert_msg(s21_is_equal(case1, case2) == 1, "equal1");
  ck_assert_msg(s21_is_equal(case2, case2) == 0, "equal2");
}
END_TEST

START_TEST(test_s21_is_not_equal) {
  s21_decimal case1 = {{0, 0, 0, 0}, 0};
  s21_decimal case2 = {{1, 0, 0, 0}, 0};

  ck_assert_msg(s21_is_not_equal(case1, case2) == 0, "not_equal1");
  ck_assert_msg(s21_is_not_equal(case2, case2) == 1, "not_equal2");
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("Suite");
  SRunner *sr = srunner_create(s1);

  TCase *tc1_1 = tcase_create("tests");
  suite_add_tcase(s1, tc1_1);

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
  tcase_add_test(tc1_1, truncate12);
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
  tcase_add_test(tc1_1, negate7);

  // мои
  tcase_add_test(tc1_1, test_s21_is_less);
  tcase_add_test(tc1_1, test_s21_is_less_or_equal);
  tcase_add_test(tc1_1, test_s21_is_greater);
  tcase_add_test(tc1_1, test_s21_is_greater_or_equal);
  tcase_add_test(tc1_1, test_s21_is_equal);
  tcase_add_test(tc1_1, test_s21_is_not_equal);


  srunner_run_all(sr, CK_ENV);
  srunner_free(sr);

  return 0;
}