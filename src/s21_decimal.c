#include "s21_decimal.h"

/**
 * @brief Временная функция для инициализаци структуры которая хранит число
 * * Децимал
 * @param varPtr указатель на число децимал
 */
void init_struct(s21_decimal *varPtr) {
  clear_bits(varPtr);
  varPtr->value_type = s21_usual;
}

/**
 * @brief Возвращает значение указанного бита
 * @param decVar число децимал
 * @param bit номер бита
 * @return значение указанного бита (0 - если бит равен нулю, 1 - bit on)
 */
int get_bit(const s21_decimal decVar, int bit) {
  int res = 0;
  if (bit / 32 < 4) {
    unsigned int mask = 1u << (bit % 32);
    res = decVar.bits[bit / 32] & mask;
  }
  return !!res;
}

/**
 * @brief Установить указанный бит вкл/выкл
 * @param varPtr указатель на число децимал
 * @param bit номер бита
 * @param value значение бита
 */
void set_bit(s21_decimal *varPtr, int bit, int value) {
  unsigned int mask = 1u << (bit % 32);
  if (bit / 32 < 4 && value) {
    varPtr->bits[bit / 32] |= mask;
  } else if (bit / 32 < 4 && !value) {
    varPtr->bits[bit / 32] &= ~mask;
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

int get_scale(const s21_decimal *value) {
  return (char)(value->bits[3] >> 16);
}

/**
 * @brief Установить нужную степень числа децимал
 * @param varPtr указатель на число децимал
 * @param scale значение степени числа
 */
void set_scale(s21_decimal *varPtr, int scale) {
  if (scale < 0 || scale > 28) {
    printf("wrong scale = %d\n", scale);
  } else {
    int clearMask = ~(0xFF << 16);
    varPtr->bits[3] &= clearMask;
    int mask = scale << 16;
    varPtr->bits[3] |= mask;
  }
}

/**
 * @brief побитовый сдвиг влево
 * @param varPtr указатель на число децимал
 * @param value_offset количество сдвигов влево
 */
void offset_left(s21_decimal *varPtr, int value_offset) {
  int lastbit = last_bit(*varPtr);
  if (lastbit + value_offset > 95) {
    varPtr->value_type = s21_infinity;
    return;
  }
  for (int i = 0; i < value_offset; i++) {
    int value_31bit = get_bit(*varPtr, 31);
    int value_63bit = get_bit(*varPtr, 63);
    varPtr->bits[0] <<= 1;
    varPtr->bits[1] <<= 1;
    varPtr->bits[2] <<= 1;
    if (value_31bit) set_bit(varPtr, 32, 1);
    if (value_63bit) set_bit(varPtr, 64, 1);
  }
}

/**
 * @brief
 * @param number
 * @return 0-95 - номер значащего бита, -1 - если все биты пустые
 */
int last_bit(s21_decimal number) {
  int last_bit = 95;
  for (; last_bit >= 0 && get_bit(number, last_bit) == 0; last_bit--) {
  }
  return last_bit;
}

/**
 * @brief Функция переводит число децимал в доп.код
 * @param number_1 указатель на число децимал
 */
void convert_to_addcode(s21_decimal *number_1) {
  s21_decimal res;
  s21_decimal add = {{1, 0, 0, 0}, s21_usual};

  number_1->bits[0] = ~number_1->bits[0];
  number_1->bits[1] = ~number_1->bits[1];
  number_1->bits[2] = ~number_1->bits[2];

  res = bit_addition(number_1, &add);

  number_1->bits[0] = res.bits[0];
  number_1->bits[1] = res.bits[1];
  number_1->bits[2] = res.bits[2];

  number_1->value_type = 4;
}

/**
 * @brief Уравнивает степени двух чисел типа децимал
 * @param number_1 указатель на число децимал
 * @param number_2 указатель на число децимал
 * @return пока что под вопросом что нужно возвращать
 */
int scale_equalize(s21_decimal *number_1, s21_decimal *number_2) {
  s21_decimal *bigger = NULL;
  s21_decimal *smaller = NULL;

  if (get_scale(number_1) == get_scale(number_2)) {
    return 0;
  } else if (get_scale(number_1) > get_scale(number_2)) {
    bigger = number_1;
    smaller = number_2;
  } else {
    bigger = number_2;
    smaller = number_1;
  }

  s21_decimal tmp;
  init_struct(&tmp);

  int scaleSmall;
  int scaleBig;

  // уравнивание скейлов
  while (get_scale(number_1) != get_scale(number_2)) {
    if (tmp.value_type == s21_usual) {
      // по умолчанию двигается в сторону увеличения скейла

      scaleSmall = get_scale(smaller);
      s21_decimal tmp1;
      s21_decimal tmp2;
      tmp1 = *smaller;
      tmp2 = *smaller;
      offset_left(&tmp1, 1);
      offset_left(&tmp2, 3);
      tmp = bit_addition(&tmp1, &tmp2);  // эквивалентно умножению на 10
      if (tmp.value_type == s21_usual) {
        bits_copy(tmp, smaller);
        set_scale(smaller, scaleSmall + 1);
      }
    } else {
      // в противном случае уменьшение скейла

      s21_decimal remainder;
      s21_decimal ten = {{10, 0, 0, 0}, s21_usual};

      s21_decimal tmpDiv = div_only_bits(*bigger, ten, &remainder);
      s21_decimal zero = {{0, 0, 0, 0}, s21_usual};
      if (zero_check(tmpDiv, zero) == 1) {
        // tmpDiv не обрезался по самые гланды
        bits_copy(tmpDiv, bigger);
      } else {
        // bigger при делении превратился в ноль - плохо
        bits_copy(remainder, bigger);
      }

      scaleBig = get_scale(bigger);
      set_scale(bigger, scaleBig - 1);
    }
  }
  return 0;
}

/**
 * @brief складывает биты, не трогая скейл
 * @param var1 первое слагаемое
 * @param var2 второе слагаемое
 * @return s21_decimal результат сложения
 */
s21_decimal bit_addition(s21_decimal *var1, s21_decimal *var2) {
  s21_decimal res = {{0, 0, 0, 0}, s21_usual};
  int buffer = 0;

  for (int i = 0; i < 96; i++) {
    int cur_bit_of_var1 = get_bit(*var1, i);
    int cur_bit_of_var2 = get_bit(*var2, i);

    if (!cur_bit_of_var1 && !cur_bit_of_var2) {  // оба бита выключены
      if (buffer) {
        set_bit(&res, i, 1);
        buffer = 0;
      } else {
        set_bit(&res, i, 0);
      }
    } else if (cur_bit_of_var1 != cur_bit_of_var2) {
      if (buffer) {
        set_bit(&res, i, 0);
        buffer = 1;
      } else {
        set_bit(&res, i, 1);
      }
    } else {  // оба бита включены
      if (buffer) {
        set_bit(&res, i, 1);
        buffer = 1;
      } else {
        set_bit(&res, i, 0);
        buffer = 1;
      }
    }
    if (i == 95 && buffer == 1 && var1->value_type != s21_ADDCODE &&
        var2->value_type != s21_ADDCODE)
      res.value_type = s21_infinity;  // переполнение нужно вывести инфинити
    else
      res.value_type = s21_usual;
  }
  if (is_inf(var1, var2) != 0) {
    res.value_type = s21_infinity;
  }

  return res;
}

/**
 * @brief Предварительные проверки для check_for_add
 *
 * @param number_1 первое слагаемое
 * @param number_2 второе слагаемое
 * @return s21_decimal число в выставленным value_type
 */
s21_decimal check_for_add(s21_decimal number_1, s21_decimal number_2) {
  s21_decimal res = {{0, 0, 0, 0}, s21_usual};
  /*
  num1      num2
  +inf      +-normal  = +inf    - в простой проверке
  -inf      +-normal  = -inf    - в простой проверке
  nan       any       = nan     - в простой проверке
  +inf      -inf      = nan     - отдельная
  */

  // простая проверка
  res.value_type = number_1.value_type > number_2.value_type
                       ? number_1.value_type
                       : number_2.value_type;

  if (number_1.value_type != s21_usual &&
      number_2.value_type != s21_usual &&
      number_1.value_type != number_2.value_type) {
    res.value_type = s21_nan;
  }
  return res;
}

/**
 * @brief Сложение двух чисел децимал
 * @param number_1 число децимал
 * @param number_2 число децимал
 * @return число цимал
 */
s21_decimal s21_add(s21_decimal number_1, s21_decimal number_2) {
  s21_decimal res = check_for_add(number_1, number_2);

  if (res.value_type == s21_usual || res.value_type == s21_ADDCODE) {
    if (!s21_getsign(&number_1) && !s21_getsign(&number_2)) {
      //  оба числа положительных

      if (get_scale(&number_1) != get_scale(&number_2)) {
        scale_equalize(&number_1, &number_2);
      }

      s21_decimal tmpRes;
      tmpRes = bit_addition(&number_1, &number_2);

      if (tmpRes.value_type == s21_infinity && get_scale(&number_1) == 0) {
        // значит в результате бесконечность
        res.value_type = s21_infinity;

      } else if (tmpRes.value_type == s21_infinity &&
                 get_scale(&number_1) > 0) {
        // можем понизить скейл
        while (res.value_type == s21_infinity &&
               (get_scale(&number_1) > 0 && get_scale(&number_2) > 0)) {
          // оба числа делим на 10, если позволяет скейл

          s21_decimal ten = {{10, 0, 0, 0}, s21_usual};
          s21_decimal remainder1, remainder2;
          s21_decimal tmpDiv1 = div_only_bits(number_1, ten, &remainder1);
          s21_decimal tmpDiv2 = div_only_bits(number_2, ten, &remainder2);

          // обезопасимся от обнуления
          s21_decimal zero = {{0, 0, 0, 0}, s21_usual};

          zero_check(tmpDiv1, zero) == 1 ? bits_copy(tmpDiv1, &number_1)
                                         : bits_copy(remainder1, &number_1);
          zero_check(tmpDiv2, zero) == 1 ? bits_copy(tmpDiv2, &number_2)
                                         : bits_copy(remainder2, &number_2);

          set_scale(&number_1, get_scale(&number_1) - 1);
          set_scale(&number_2, get_scale(&number_2) - 1);
          return s21_add(number_1, number_2);
        }

      } else {
        // результат можно сразу присвоить
        res = tmpRes;
        res.bits[3] = number_1.bits[3];
      }

    } else if (s21_getsign(&number_1) && !s21_getsign(&number_2)) {
      // 1 число отрецательное 2 число положительное
      // вызывается функция вычитания которая
      // создает доп код и сново вызывает сложение
      s21_setsign(&number_1, 0);
      res = s21_sub(number_2, number_1);

    } else if (!s21_getsign(&number_1) && s21_getsign(&number_2)) {
      // 1 полож 2 отрец
      s21_setsign(&number_2, 0);
      res = s21_sub(number_1, number_2);

    } else {
      // оба отрицательных
      s21_setsign(&number_1, 0);
      s21_setsign(&number_2, 0);
      res = s21_add(number_1, number_2);
      s21_setsign(&res, 1);
      if (res.value_type == s21_infinity) {
        res.value_type = s21_neg_infinity;
        clear_bits(&res);
      }
    }
  }

  if (res.value_type == s21_ADDCODE) {
    res.value_type = s21_usual;
  }

  return res;
}

/**
 * @brief Разница чисел децимал
 * @param number_1 число децимал 1
 * @param number_2 число децимал 2
 * @return Возвращает разницу
 */
s21_decimal s21_sub(s21_decimal number_1, s21_decimal number_2) {
  s21_decimal res = {{0, 0, 0, 0}, s21_usual};

  res.value_type =
      number_1.value_type ? number_1.value_type : number_2.value_type;

  if (res.value_type == s21_usual) {
    if (get_scale(&number_1) != get_scale(&number_2)) {
      scale_equalize(&number_1, &number_2);
    }

    int resultSign;

    // проверяем на знаки
    if (s21_getsign(&number_1) != s21_getsign(&number_2)) {
      // знаки разные - ситуация вырождается в ++ или --
      resultSign = s21_getsign(&number_1);
      s21_setsign(&number_1, 0);
      s21_setsign(&number_2, 0);
      res = s21_add(number_1, number_2);
      s21_setsign(&res, resultSign);

    } else {
      // знаки одинаковые - ситуация вырождается в -+ или +-
      if (s21_is_equal(number_1, number_2) == TRUE) {
        // они равны, все ок, ничего не делаем, в результате уже лежит 0

      } else {
        // числа разные, значит нужно знать какой знак проставлять и тд
        int sign1 = s21_getsign(&number_1);
        int sign2 = s21_getsign(&number_2);
        s21_setsign(&number_1, 0);
        s21_setsign(&number_2, 0);
        s21_decimal *smallPtr, *bigPtr;

        // кто из них больше по модулю
        if (s21_is_less(number_1, number_2) == TRUE) {
          smallPtr = &number_1;
          bigPtr = &number_2;
          resultSign = !sign2;
        } else {
          smallPtr = &number_2;
          bigPtr = &number_1;
          resultSign = sign1;
        }

        // меньшее из двух чисел переведем в доп код и сложим, затем проставим
        // знак
        convert_to_addcode(smallPtr);
        res = s21_add(*smallPtr, *bigPtr);
        s21_setsign(&res, resultSign);
      }
    }
  }

  return res;
}

/**
 * @brief Является ли одно из чисел NAN
 * @param dec1 Первое число децимал
 * @param dec2 Второе число децимал
 * @return FALSE - числа не NAN, TRUE - одно из децималов NAN
 */
int is_NAN(s21_decimal *dec1, s21_decimal *dec2) {
  int vt_dec1 = dec1->value_type;
  int vt_dec2 = dec2->value_type;

  return (vt_dec1 == s21_nan || vt_dec2 == s21_nan) ? TRUE : FALSE;
}

/**
 * @brief Проверка на полож бесконечность
 * @param dec1 Первое число децимал
 * @param dec2 Второе число децимал
 * @return 0 - не бесконечны, 1 - первое бесконечно, 2 - оба бесконечны, -1 -
 * второе бесконечно
 */
int is_inf(s21_decimal *dec1, s21_decimal *dec2) {
  int who_is_inf = 0;
  int vt_dec1 = dec1->value_type;
  int vt_dec2 = dec2->value_type;

  if (vt_dec1 == s21_infinity && vt_dec2 != s21_infinity) {
    who_is_inf = 1;
  } else if (vt_dec1 != s21_infinity && vt_dec2 == s21_infinity) {
    who_is_inf = -1;
  } else if (vt_dec1 == s21_infinity && vt_dec2 == s21_infinity) {
    who_is_inf = 2;
  }
  return who_is_inf;
}

/**
 * @brief Проверка на отриц бесконечность
 * @param dec1 Первое число децимал
 * @param dec2 Второе число децимал
 * @return 0 - не бесконечны, 1 - первое бесконечно, 2 - оба бесконечны, -1 -
 * второе бесконечно
 */
int is_neg_inf(s21_decimal *dec1, s21_decimal *dec2) {
  int who_is_inf = 0;
  int vt_dec1 = dec1->value_type;
  int vt_dec2 = dec2->value_type;

  if (vt_dec1 == s21_neg_infinity && vt_dec2 != s21_neg_infinity) {
    who_is_inf = 1;
  }
  if (vt_dec1 != s21_neg_infinity && vt_dec2 == s21_neg_infinity) {
    who_is_inf = -1;
  }
  if (vt_dec1 == s21_neg_infinity && vt_dec2 == s21_neg_infinity) {
    who_is_inf = 2;
  }

  return who_is_inf;
}

/**
 * @brief Проверка чисел на отрицательное значение
 * @param dec1 Первое число децимал
 * @param dec2 Второе число децимал
 * @return 0 - оба одного знака, 1 - первое положительное, -1 - второе
 * положительное
 */
int is_negative(s21_decimal *dec1, s21_decimal *dec2) {
  int who_is_negative = 0;
  int sign_dec1 = s21_getsign(dec1);
  int sign_dec2 = s21_getsign(dec2);

  if (!sign_dec1 && sign_dec2) who_is_negative = 1;
  if (sign_dec1 && !sign_dec2) who_is_negative = -1;

  return who_is_negative;
}

/**
 * @brief Проверка скейла чисел и приведение к одному
 * @param dec1 Первое число децимал
 * @param dec2 Второе число децимал
 */
void check_scale(s21_decimal *dec1, s21_decimal *dec2) {
  int scale_dec1 = get_scale(dec1);
  int scale_dec2 = get_scale(dec2);

  if (scale_dec1 != scale_dec2) {
    scale_equalize(dec1, dec2);
  }
}

/**
 * @brief Являются ли числа нулями
 * @param dec1 Первое число децимал
 * @param dec2 Второе число децимал
 * @return 0 - являются (только когда оба),
 * 1 - не являются (по крайней мере одно)
 */
int zero_check(s21_decimal dec1, s21_decimal dec2) {
  int is_zero = FALSE;
  s21_decimal *pt1 = &dec1;
  s21_decimal *pt2 = &dec2;

  if (pt1 && pt2) {
    if (!dec1.bits[0] && !dec2.bits[0] && !dec1.bits[1] && !dec2.bits[1] &&
        !dec1.bits[2] && !dec2.bits[2])
      is_zero = TRUE;
  }
  return is_zero;
}

/**
 * @brief Проверка больше ли первое число, чем второе
 * @param dec1 Первое число децимал
 * @param dec2 Второе число децимал
 * @return 0 - больше, 1 - меньше
 */
int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
  int is_greater = -1;
  if (is_NAN(&dec1, &dec2) == TRUE) is_greater = FALSE;
  if (!zero_check(dec1, dec2) && !is_inf(&dec1, &dec2) &&
      !is_neg_inf(&dec1, &dec2))
    is_greater = FALSE;

  if (is_greater == -1) {
    int who_is_inf = is_inf(&dec1, &dec2);
    if (who_is_inf == 1) is_greater = TRUE;
    if (who_is_inf == -1 || who_is_inf == 2) is_greater = FALSE;

    int who_is_neg_inf = is_neg_inf(&dec1, &dec2);
    if (who_is_neg_inf == -1) is_greater = TRUE;
    if (who_is_neg_inf == 1 || who_is_inf == 2) is_greater = FALSE;
  }

  if (is_greater == -1) {
    int who_is_negative = is_negative(&dec1, &dec2);
    if (who_is_negative == 1) is_greater = TRUE;
    if (who_is_negative == -1) is_greater = FALSE;

    check_scale(&dec1, &dec2);
  }

  for (int i = 95; i >= 0 && is_greater == -1; i--) {
    int bit_dec1 = get_bit(dec1, i);
    int bit_dec2 = get_bit(dec2, i);
    if (bit_dec1 && !bit_dec2) is_greater = TRUE;
    if (bit_dec2 && !bit_dec1) is_greater = FALSE;

    if (is_greater != -1) {
      if (s21_getsign(&dec1) && s21_getsign(&dec2)) is_greater = !is_greater;
    }
  }

  return is_greater;
}

/**
 * @brief Хитрая функция меньше ли первое число, чем второе
 * @param dec1 Первое число децимал
 * @param dec2 Второе число децимал
 * @return 0 - меньше, 1 - больше
 */
int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  return s21_is_greater(dec2, dec1);
}

// /**
//  * @brief Равны ли два числа децимал
//  * @param dec1 Первое число децимал
//  * @param dec2 Второе число децимал
//  * @return 0 - равны, 1 - разные
//  */
int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
  int is_equal = -1;
  if (is_NAN(&dec1, &dec2) == TRUE) is_equal = FALSE;

  if (is_equal == -1) {
    if (zero_check(dec1, dec2) == TRUE) is_equal = TRUE;

    int who_is_inf = is_inf(&dec1, &dec2);
    if (who_is_inf == 1 || who_is_inf == -1) is_equal = FALSE;
    if (who_is_inf == 2) is_equal = TRUE;

    int who_is_neg_inf = is_neg_inf(&dec1, &dec2);
    if (who_is_neg_inf == 1 || who_is_neg_inf == -1) is_equal = FALSE;
    if (who_is_neg_inf == 2) is_equal = TRUE;
  }

  if (is_equal == -1) {
    int who_is_negative = is_negative(&dec1, &dec2);
    if (who_is_negative != 0) is_equal = FALSE;

    check_scale(&dec1, &dec2);
  }

  for (int i = 95; i >= 0 && is_equal == -1; i--) {
    int bit_dec1 = get_bit(dec1, i);
    int bit_dec2 = get_bit(dec2, i);
    if (bit_dec1 != bit_dec2) is_equal = FALSE;
  }

  return (is_equal != FALSE) ? TRUE : FALSE;
}

/**
 * @brief Хитрая - больше или равно первое децимал второму
 * @param dec1 Первое число децимал
 * @param dec2 Второе число децимал
 * @return 0 - больше или равно, 1 - меньше
 */
int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return !(!s21_is_greater(dec1, dec2) || !s21_is_equal(dec1, dec2));
}

/**
 * @brief Хитрая - не равно первое децимал второму
 * @param dec1 Первое число децимал
 * @param dec2 Второе число децимал
 * @return 0 - разные, 1 - равны
 */
// int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {
//   return (!s21_is_equal(dec1, dec2));
// }

/**
 * @brief копирует только 96 бит
 * @param src откуда
 * @param dest куда
 */
void bits_copy(s21_decimal src, s21_decimal *dest) {
  dest->bits[0] = src.bits[0];
  dest->bits[1] = src.bits[1];
  dest->bits[2] = src.bits[2];
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
    *dst /= (int)pow(10, get_scale(&src));
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

/**
 * @brief Деление битов игнорируется степень
 * @param a число децимал
 * @param b число децимал
 * @param buf остаток от деления
 * @return s21_decimal результат деление (целая часть)
 */
s21_decimal div_only_bits(s21_decimal number_1, s21_decimal number_2,
                          s21_decimal *buf) {
  init_struct(buf);
  s21_decimal res = {{0, 0, 0, 0}, 0};
  for (int i = last_bit(number_1); i >= 0; i--) {
    if (get_bit(number_1, i)) set_bit(buf, 0, 1);
    if (s21_is_greater_or_equal(*buf, number_2) == TRUE) {
      *buf = s21_sub(*buf, number_2);
      if (i != 0) offset_left(buf, 1);
      if (get_bit(number_1, i - 1)) set_bit(buf, 0, 1);
      offset_left(&res, 1);
      set_bit(&res, 0, 1);
    } else {
      offset_left(&res, 1);
      if (i != 0) offset_left(buf, 1);
      if ((i - 1) >= 0 && get_bit(number_1, i - 1)) set_bit(buf, 0, 1);
    }
  }
  return res;
}

// int s21_decimal s21_truncate(s21_decimal number_1) {
//   s21_decimal ten = {{10, 0, 0, 0}, s21_usual};
//   s21_decimal res = {{0, 0, 0, 0}, 0};
//   s21_decimal tmp = {{0, 0, 0, 0}, s21_usual};

//   int sign_number_1 = s21_getsign(&number_1);
//   int scale = get_scale(&number_1);

//   int valid_value = (number_1.value_type == s21_usual ? 1 : 0);

//   if (!scale && valid_value) {
//     res = number_1;
//   } else if (valid_value) {
//     for (int i = scale; i > 0; i--) {
//       res = div_only_bits(number_1, ten, &tmp);
//       number_1 = res;
//     }
//   } else {
//     res = number_1;
//   }

//   if (sign_number_1 && valid_value) s21_setsign(&res, 1);

//   return res;
// }

int s21_truncate(s21_decimal value, s21_decimal *result) {
  s21_decimal ten = {{10, 0, 0, 0}, s21_usual}, res = ten, tmp = ten;
  int ret = 0, sign = s21_getsign(&value), scale = get_scale(&value);
  printf("%d %d\n", (char)(value.bits[3] >> 16), scale;

  if (!scale && !value.value_type) {
    res = value;
  } else if (!value.value_type) {
    for (int i = scale; i > 0; i--) {
      res = div_only_bits(value, ten, &tmp);
      value = res;
    }
  } else {   // inf or nan
    res = value;
    ret = 1;
  }

  if (sign && !value.value_type) s21_setsign(&res, 1);
  *result = res;
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

  set_scale(&five, 1);

  if (valid_value) {
    res = trunc;
    if (s21_is_greater_or_equal(buf, five) == TRUE) {
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
  int scale_dec1 = get_scale(&dec1);

  s21_decimal one = {{1, 0, 0, 0}, s21_usual};
  s21_decimal ten = {{10, 0, 0, 0}, s21_usual};
  s21_decimal buf;
  init_struct(&buf);
  for (int i = scale_dec1; i > 0; i--) dec1 = div_only_bits(dec1, ten, &buf);
  set_scale(&dec1, 0);

  if (s21_is_equal(dec1, dec1_copy) == TRUE) valid_value = 0;
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

/**
 * @brief зануляет все биты передаваемой переменной,
 * не трогает скейл и тип
 *
 * @param varPtr указатель на очищаемую переменную
 */
void clear_bits(s21_decimal *varPtr) {
  memset(varPtr->bits, 0, sizeof(varPtr->bits));
}
