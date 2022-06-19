#include "s21_decimal.h"

void s21_set0bitstype(s21_decimal *ptr) {
  s21_set0bits(ptr);
  ptr->value_type = s21_usual;
}

int s21_get_bit(const s21_decimal value, int bit) {
  return value.bits[bit / 32] & (1u << (bit % 32));
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

int s21_get_scale(const s21_decimal *value) {
  return (char)(value->bits[3] >> 16);
}

void s21_set_scale(s21_decimal *value, int scale) {
  if (scale >= 0 && scale <= 28) {
    value->bits[3] &= ~(0xFF << 16);
    value->bits[3] |= scale << 16;
  }
}

void s21_shift_left(s21_decimal *value, int offset) {
  if (s21_last_bit(*value) + offset > 95) {
    value->value_type = s21_infinity;
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
  s21_decimal add = {{1, 0, 0, 0}, 0};

  value->bits[0] = ~value->bits[0];
  value->bits[1] = ~value->bits[1];
  value->bits[2] = ~value->bits[2];

  res = s21_add_bits(value, &add);

  value->bits[0] = res.bits[0];
  value->bits[1] = res.bits[1];
  value->bits[2] = res.bits[2];

  value->value_type = s21_ADDCODE;
}

void s21_level_scale(s21_decimal *value1, s21_decimal *value2) {
  s21_decimal *bigger = value2, *smaller = value1, tmp = {{0, 0, 0, 0}, 0};

  if (s21_get_scale(value1) > s21_get_scale(value2)) {
    bigger = value1;
    smaller = value2;
  }

  while (s21_get_scale(value1) != s21_get_scale(value2)) {
    if (tmp.value_type == s21_usual) {
      s21_decimal tmp1, tmp2;
      tmp1 = *smaller, tmp2 = *smaller;

      s21_shift_left(&tmp1, 1);
      s21_shift_left(&tmp2, 3);
      tmp = s21_add_bits(&tmp1, &tmp2);  // equals to multiplying by 10
      if (tmp.value_type == s21_usual) {
        s21_copy_bits(tmp, smaller);
        s21_set_scale(smaller, s21_get_scale(smaller) + 1);
      }
    } else {
      s21_decimal remainder, ten = {{10, 0, 0, 0}, 0}, zero = {{0, 0, 0, 0}, 0},
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
  s21_decimal res = {{0, 0, 0, 0}, 0};
  int buffer = 0;

  if (s21_are_inf(value1, value2)) {
    res.value_type = s21_infinity;
  } else {
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
    // TODO(alex): s21_ADDCODE.
    if (i == 95 && buffer == 1 && value1->value_type != s21_ADDCODE &&
        value2->value_type != s21_ADDCODE)
      res.value_type = s21_infinity;
    else
      res.value_type = s21_usual;
  }
  }

  return res;
}

s21_decimal check_boundary(s21_decimal value1, s21_decimal value2) {
  s21_decimal res = {{0, 0, 0, 0}, 0};

  res.value_type = value1.value_type > value2.value_type
                       ? value1.value_type: value2.value_type;

  if (value1.value_type != s21_usual && value2.value_type != s21_usual &&
      value1.value_type != value2.value_type)
    res.value_type = s21_nan;
  return res;
}

s21_decimal s21_add(s21_decimal value1, s21_decimal value2) {
  s21_decimal res = check_boundary(value1, value2);

  if (res.value_type == s21_usual || res.value_type == s21_ADDCODE) {
    if (!s21_getsign(&value1) && !s21_getsign(&value2)) {  // positive
      if (s21_get_scale(&value1) != s21_get_scale(&value2))
        s21_level_scale(&value1, &value2);

      s21_decimal tmp = s21_add_bits(&value1, &value2);

      if (tmp.value_type == s21_infinity && s21_get_scale(&value1) == 0) {
        res.value_type = s21_infinity;
      } else if (tmp.value_type == s21_infinity &&
                 s21_get_scale(&value1) > 0) {  // lower scale
        while (res.value_type == s21_infinity &&
               (s21_get_scale(&value1) > 0 && s21_get_scale(&value2) > 0)) {  // divide by 10
          s21_decimal ten = {{10, 0, 0, 0}, 0}, rem1, rem2, zero = {{0, 0, 0, 0}, 0};
          s21_decimal tmp1 = s21_div_bits(value1, ten, &rem1);
          s21_decimal tmp2 = s21_div_bits(value2, ten, &rem2);

          s21_are_zero(tmp1, zero) == 1 ? s21_copy_bits(tmp1, &value1)  // check for 0
                                         : s21_copy_bits(rem1, &value1);
          s21_are_zero(tmp2, zero) == 1 ? s21_copy_bits(tmp2, &value2)
                                         : s21_copy_bits(rem2, &value2);

          s21_set_scale(&value1, s21_get_scale(&value1) - 1);
          s21_set_scale(&value2, s21_get_scale(&value2) - 1);
          return s21_add(value1, value2);
        }

      } else {
        res = tmp;
        res.bits[3] = value1.bits[3];
      }

    } else if (s21_getsign(&value1) && !s21_getsign(&value2)) {  // 1 - negative, 2 - positive
      s21_setsign(&value1, 0);
      res = s21_sub(value2, value1);
    } else if (!s21_getsign(&value1) && s21_getsign(&value2)) {  // 2 - negative, 1 - positive
      s21_setsign(&value2, 0);
      res = s21_sub(value1, value2);

    } else {  // both negative
      s21_setsign(&value1, 0);
      s21_setsign(&value2, 0);
      res = s21_add(value1, value2);
      s21_setsign(&res, 1);
      if (res.value_type == s21_infinity) {
        res.value_type = s21_neg_infinity;
        s21_set0bits(&res);
      }
    }
  }
  // TODO(alex): addcode
  if (res.value_type == s21_ADDCODE)
    res.value_type = s21_usual;

  return res;
}

s21_decimal s21_sub(s21_decimal number_1, s21_decimal number_2) {
  s21_decimal res = {{0, 0, 0, 0}, number_1.value_type ? number_1.value_type : number_2.value_type};

  if (res.value_type == s21_usual) {
    if (s21_get_scale(&number_1) != s21_get_scale(&number_2))
      s21_level_scale(&number_1, &number_2);

    int sign;
    if (s21_getsign(&number_1) != s21_getsign(&number_2)) {  // signs don't equal
      sign = s21_getsign(&number_1);
      s21_setsign(&number_1, 0);
      s21_setsign(&number_2, 0);
      res = s21_add(number_1, number_2);
      s21_setsign(&res, sign);
    } else {  // signs equal
      if (s21_are_equal(number_1, number_2)) {  // digits don't equals
        int sign1 = s21_getsign(&number_1);
        int sign2 = s21_getsign(&number_2);
        s21_setsign(&number_1, 0);
        s21_setsign(&number_2, 0);
        s21_decimal *smallPtr, *bigPtr;

        if (!s21_is_less(number_1, number_2)) {
          smallPtr = &number_1;
          bigPtr = &number_2;
          sign = !sign2;
        } else {
          smallPtr = &number_2;
          bigPtr = &number_1;
          sign = sign1;
        }

        to_addcode(smallPtr);
        res = s21_add(*smallPtr, *bigPtr);
        s21_setsign(&res, sign);
      }
    }
  }

  return res;
}

int s21_are_inf(s21_decimal *value1, s21_decimal *value2) {
  int res = 0;

  if (value1->value_type == s21_infinity && value2->value_type != s21_infinity) {
    res = 1;
  } else if (value1->value_type != s21_infinity && value2->value_type == s21_infinity) {
    res = -1;
  } else if (value1->value_type == s21_infinity && value2->value_type == s21_infinity) {
    res = 2;
  }
  return res;  // 0 - не бесконечны, 1 - первое бесконечно, 2 - оба бесконечны, -1 - второе бесконечно
}

int s21_are_neg_inf(s21_decimal *a, s21_decimal *b) {
  int res = 0;
  if (a->value_type == s21_neg_infinity && b->value_type != s21_neg_infinity)
    res = 1;
  if (a->value_type != s21_neg_infinity && b->value_type == s21_neg_infinity)
    res = -1;
  if (a->value_type == s21_neg_infinity && b->value_type == s21_neg_infinity)
    res = 2;
  return res;  // 0 - не бесконечны, 1 - первое бесконечно, 2 - оба бесконечны, -1 - второе бесконечно
}

int s21_are_neg(s21_decimal *value1, s21_decimal *value2) {
  int res = 0;
  int sign1 = s21_getsign(value1);
  int sign2 = s21_getsign(value2);

  if (!sign1 && sign2) res = 1;
  if (sign1 && !sign2) res = -1;

  return res;  // 0 - оба одного знака, 1 - первое положительное, -1 - второе положительное
}

void s21_check_scale(s21_decimal *value1, s21_decimal *value2) {
  if (s21_get_scale(value1) != s21_get_scale(value2)) {
    s21_level_scale(value1, value2);
  }
}

int s21_are_zero(s21_decimal a, s21_decimal b) {
  return (!a.bits[0] && !b.bits[0] && !a.bits[1] && !b.bits[1] &&
        !a.bits[2] && !b.bits[2]) ? 0 : 1;
}

int s21_1st_greater(s21_decimal value1, s21_decimal value2) {
  int is_greater = -1;
  if (((value1.value_type == s21_nan || value2.value_type == s21_nan)) ||
    (!s21_are_zero(value1, value2) && !s21_are_inf(&value1, &value2) && !s21_are_neg_inf(&value1, &value2)))
    is_greater = 1;

  if (is_greater == -1) {
    // 0 - не бесконечны, 1 - первое бесконечно, 2 - оба бесконечны, -1 - второе бесконечно
    int if_inf = s21_are_inf(&value1, &value2);
    if (if_inf == 1) is_greater = 0;
    if (if_inf == -1 || if_inf == 2) is_greater = 1;

    int if_neg_inf = s21_are_neg_inf(&value1, &value2);
    if (if_neg_inf == -1) is_greater = 0;
    if (if_neg_inf == 1 || if_neg_inf == 2) is_greater = 1;
  }

  if (is_greater == -1) {
    // 0 - оба одного знака, 1 - первое положительное, -1 - второе положительное
    int who_is_negative = s21_are_neg(&value1, &value2);
    if (who_is_negative == 1) is_greater = 0;
    if (who_is_negative == -1) is_greater = 1;

    s21_check_scale(&value1, &value2);
  }

  for (int i = 95; i >= 0 && is_greater == -1; i--) {
    if (s21_get_bit(value1, i) && !s21_get_bit(value2, i)) is_greater = 0;
    if (s21_get_bit(value2, i) && !s21_get_bit(value1, i)) is_greater = 1;
  }
  if ((is_greater != -1) && (s21_getsign(&value1) && s21_getsign(&value2)))
    is_greater = !is_greater;
  return is_greater;  // 0 - больше, 1 - меньше
}

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  return s21_1st_greater(dec2, dec1);  // 1st: 0 - меньше, 1 - больше
}

int s21_are_equal(s21_decimal value1, s21_decimal value2) {
  int is_equal = -1;
  if ((value1.value_type == s21_nan || value2.value_type == s21_nan)) is_equal = 1;

  if (is_equal == -1) {
    if (!s21_are_zero(value1, value2)) is_equal = 0;

    int if_inf = s21_are_inf(&value1, &value2);
    if (if_inf == 1 || if_inf == -1) is_equal = 1;
    if (if_inf == 2) is_equal = 0;

    int if_neg_inf = s21_are_neg_inf(&value1, &value2);
    if (if_neg_inf == 1 || if_neg_inf == -1) is_equal = 1;
    if (if_neg_inf == 2) is_equal = 0;
  }

  if (is_equal == -1) {
    if (s21_are_neg(&value1, &value2)) is_equal = 1;
    s21_check_scale(&value1, &value2);
  }

  for (int i = 95; i >= 0 && is_equal == -1; i--)
    if (s21_get_bit(value1, i) != s21_get_bit(value2, i))
      is_equal = 1;
  return (is_equal != 1) ? 0 : 1;
}

int s21_gte(s21_decimal dec1, s21_decimal dec2) {
  return (s21_1st_greater(dec1, dec2) && s21_are_equal(dec1, dec2));  // 0 - больше или равно, 1 - меньше
}

void s21_copy_bits(s21_decimal source, s21_decimal *dest) {
  dest->bits[0] = source.bits[0];
  dest->bits[1] = source.bits[1];
  dest->bits[2] = source.bits[2];
}

/**
 * @brief Конвертирует цисло из int в decimal
 * @param src Число типа int
 * @param dst Структура decimal'а
 * @return Результат работы, где 0 - это false, а 1 - это true
 */
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  char result = TRUE;
  if (dst) {
    dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
    dst->value_type = 0;
    if (src < 0) {
      s21_setsign(dst, 1);
      src *= -1;
    }
    dst->bits[0] = src;
    dst->value_type = s21_usual;
  } else {
    result = FALSE;
  }
  return result;
}

/**
 * @brief Конвертирует цисло из decimal в int
 * @param src Число decimal
 * @param dst Указатель на int
 * @return Результат работы, где 0 - это false, а 1 - это true
 */
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int result = 1;
  if (src.value_type == s21_usual) {
    *dst = src.bits[0];
    *dst *= s21_getsign(&src) ? -1 : 1;
    *dst /= (int)pow(10, s21_get_scale(&src));
    result = 0;
  }
  return result;
}

/**
 * @brief Получает знак в числе float
 * @param src Цисло float
 * @return Знак float'a
 */
int getFloatSign(float *src) { return *(int *)src >> 31; }

/**
 * @brief Получает экспоненту из float'a
 * @param src число float
 * @return Возвращает экспоненту
 */
int getFloatExp(float *src) { return ((*(int *)src & ~SIGN) >> 23) - 127; }

/**
 * @brief Делает перевод из float в децимал
 * @param src Значение float
 * @param dst Значение децимал
 * @return Результат выполнения функции
 */
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
  dst->value_type = s21_usual;
  int result = FALSE, sign = getFloatSign(&src), exp = getFloatExp(&src);

  if (isinf(src) && !sign)
    dst->value_type = s21_infinity;
  else if (isinf(src) && sign)
    dst->value_type = s21_neg_infinity;
  else if (isnan(src))
    dst->value_type = s21_nan;

  if (dst && dst->value_type == s21_usual && src != 0) {
    double temp = (double)fabs(src);
    int off = 0;
    for (; off < 28 && (int)temp / (int)pow(2, 21) == 0; temp *= 10, off++) {
    }
    temp = round(temp);
    if (off <= 28 && (exp > -94 && exp < 96)) {
      floatbits mant;
      temp = (float)temp;
      for (; fmod(temp, 10) == 0 && off > 0; off--, temp /= 10) {
      }
      mant.fl = temp;
      exp = getFloatExp(&mant.fl);
      dst->bits[exp / 32] |= 1 << exp % 32;
      for (int i = exp - 1, j = 22; j >= 0; i--, j--)
        if ((mant.ui & (1 << j)) != 0) dst->bits[i / 32] |= 1 << i % 32;
      dst->bits[3] = (sign << 31) | (off << 16);
      result = TRUE;
    }
  }
  return result;
}

// TODO(alex): optimiation.
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int result = FALSE;
  float a = 1*0.0, b = 0.0*2;
  if (src.value_type == s21_usual) {
    double temp = 0;
    int off = 0;
    for (int i = 0; i < 96; i++)
      if ((src.bits[i / 32] & (1 << i % 32)) != 0) temp += pow(2, i);
    if ((off = (src.bits[3] & ~SIGN) >> 16) > 0) {
      for (int i = off; i > 0; i--) temp /= 10.0;
    }
    *dst = (float)temp;
    *dst *= src.bits[3] >> 31 ? -1 : 1;
    result = TRUE;
  }
  if (src.value_type == s21_infinity)
    *dst = 1.0 / 0.0;
  if (src.value_type == s21_neg_infinity)
    *dst = -1.0 / 0.0;
  if (src.value_type == s21_nan)
    *dst = a / b;
  return result;
}

// TODO(alex): debug.
s21_decimal s21_div_bits(s21_decimal a, s21_decimal b,
                          s21_decimal *rem) {
  s21_set0bitstype(rem);
  s21_decimal res = {{0, 0, 0, 0}, 0};
  for (int i = s21_last_bit(a); i >= 0; i--) {
    if (s21_get_bit(a, i)) s21_set_bit(rem, 0, 1);
    if (!s21_gte(*rem, b)) {
      *rem = s21_sub(*rem, b);
      if (i != 0) s21_shift_left(rem, 1);
      if (s21_get_bit(a, i - 1)) s21_set_bit(rem, 0, 1);
      s21_shift_left(&res, 1);
      s21_set_bit(&res, 0, 1);
    } else {
      s21_shift_left(&res, 1);
      if (i != 0) s21_shift_left(rem, 1);
      if ((i - 1) >= 0 && s21_get_bit(a, i - 1))
        s21_set_bit(rem, 0, 1);
    }
  }
  return res;
}

int s21_truncate(s21_decimal value, s21_decimal *res) {
  s21_decimal ten = {{10, 0, 0, 0}, 0}, tmp = ten;
  int ret = 0, sign = s21_getsign(&value), scale = s21_get_scale(&value);

  *res = value;
  if (!value.value_type) {
    for (int i = scale; i > 0; i--) {  // divide by 10 in the loop within the scale
      *res = s21_div_bits(value, ten, &tmp);
      value = *res;
    }
    if (sign) s21_setsign(res, 1);
  } else {   // inf or nan
    *res = value;
    ret = 1;
  }

  // *resu = res;
  return ret;
}

/**
 * @brief Округление до целого числа
 * @param dec1 Число к округлению
 * @return Округленный децимал
 */
s21_decimal s21_round(s21_decimal dec1) {
  int valid_value = (dec1.value_type == s21_usual ? 1 : 0);

  s21_decimal res = {{0, 0, 0, 0}, s21_usual};
  s21_decimal one = {{1, 0, 0, 0}, s21_usual};
  s21_decimal five = {{5, 0, 0, 0}, s21_usual};

  int sign = s21_getsign(&dec1);
  s21_setsign(&dec1, 0);

  s21_decimal trunc;
  s21_truncate(dec1, &trunc);
  s21_decimal buf = s21_sub(dec1, trunc);

  s21_set_scale(&five, 1);

  if (valid_value) {
    res = trunc;
    if (s21_gte(buf, five) == TRUE) {
      res = s21_add(res, one);
    }
    s21_setsign(&res, sign);
  } else {
    res.value_type = dec1.value_type;
  }

  return res;
}

/**
 * @brief Округление в сторону минус бесконечности
 * @param dec1 Число к округлению
 * @return Округленный децимал
 */
s21_decimal s21_floor(s21_decimal dec1) {
  s21_decimal dec1_copy = dec1;
  int valid_value = (dec1.value_type == s21_usual ? 1 : 0);
  int sign_dec1 = s21_getsign(&dec1);
  int scale_dec1 = s21_get_scale(&dec1);

  s21_decimal one = {{1, 0, 0, 0}, s21_usual};
  s21_decimal ten = {{10, 0, 0, 0}, s21_usual};
  s21_decimal buf;
  s21_set0bitstype(&buf);
  for (int i = scale_dec1; i > 0; i--) dec1 = s21_div_bits(dec1, ten, &buf);
  s21_set_scale(&dec1, 0);

  if (s21_are_equal(dec1, dec1_copy) == TRUE) valid_value = 0;
  if (sign_dec1 && valid_value) {
    dec1 = s21_add(dec1, one);
    s21_setsign(&dec1, 1);
  }

  return dec1;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int ret = 0;
  *result = value;
  if (result->value_type != s21_nan) {
    s21_setsign(result, s21_getsign(result)^1);
    if (result->value_type == s21_infinity) {
      result->value_type = s21_neg_infinity;
    } else {
      if (result->value_type == s21_neg_infinity)
        result->value_type = s21_infinity;
    }
  } else {  // inf or nan
    ret = 1;
  }
  return ret;
}

void s21_set0bits(s21_decimal *ptr) {
  memset(ptr->bits, 0, sizeof(ptr->bits));
}
