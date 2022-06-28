#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <math.h>
#include <string.h>

typedef enum {
  s21_usual = 0,
  s21_infinity = 1,
  s21_neg_infinity = 2,
  s21_nan = 3,
  s21_ADDCODE = 4
} s21_value_type;

typedef struct {
  int bits[4];
  s21_value_type value_type;
} s21_decimal;

typedef union {
    int in;
    float fl;
  } int_float;

s21_decimal s21_add(s21_decimal, s21_decimal);
s21_decimal s21_sub(s21_decimal, s21_decimal);
s21_decimal s21_mod(s21_decimal, s21_decimal);
int s21_floor(s21_decimal, s21_decimal *);
int s21_round(s21_decimal, s21_decimal *);
int s21_truncate(s21_decimal, s21_decimal*);
int s21_negate(s21_decimal, s21_decimal*);

int s21_is_less(s21_decimal, s21_decimal);
int s21_1st_greater(s21_decimal, s21_decimal);
int s21_gte(s21_decimal, s21_decimal);
int s21_are_equal(s21_decimal, s21_decimal);

int s21_get_bit(const s21_decimal, int);
int s21_getsign(const s21_decimal*);
int s21_get_scale(const s21_decimal *);
void s21_level_scale(s21_decimal *, s21_decimal *);
int s21_last_bit(s21_decimal);
void s21_copy_bits(s21_decimal, s21_decimal *);
void convert_to_addcode(s21_decimal *);
void s21_set_scale(s21_decimal *, int);
void s21_shift_left(s21_decimal *, int);
void s21_set0bitstype(s21_decimal *);
void s21_set_bit(s21_decimal *, int, int);
void s21_setsign(s21_decimal *, int);
void s21_set0bits(s21_decimal *);
s21_decimal s21_add_bits(s21_decimal *, s21_decimal *);
s21_decimal s21_div_bits(s21_decimal, s21_decimal, s21_decimal *);
s21_decimal check_boundary(s21_decimal, s21_decimal);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int getFloatExp(float *src);
int s21_are_zero(s21_decimal, s21_decimal);
int s21_are_inf(s21_decimal *, s21_decimal *);
int s21_are_neg_inf(s21_decimal *, s21_decimal *);
int s21_are_neg(s21_decimal *, s21_decimal *);
void s21_check_scale(s21_decimal *, s21_decimal *);

#endif  // SRC_S21_DECIMAL_H_
