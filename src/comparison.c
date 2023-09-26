#include "s21_decimal.h"

int s21_is_less(s21_decimal dst1, s21_decimal dst2) {
  int res = 0;
  if (s21_is_greater(dst1, dst2) == FALSE && s21_is_equal(dst1, dst2) == FALSE)
    res = TRUE;
  else
    res = FALSE;
  return res;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int is_greater = 0;
  eq_scale(&value_1, &value_2);
  if (get_sign(value_1.bits[3]) == SIGN_PLUS &&
      get_sign(value_2.bits[3]) == SIGN_MINUS)
    is_greater = TRUE;
  else if (get_sign(value_1.bits[3]) == SIGN_MINUS &&
           get_sign(value_2.bits[3]) == SIGN_PLUS)
    is_greater = FALSE;
  else if (get_sign(value_1.bits[3]) == SIGN_MINUS &&
           get_sign(value_2.bits[3]) == SIGN_MINUS) {
    set_sign(&value_1.bits[3], SIGN_PLUS);
    set_sign(&value_2.bits[3], SIGN_PLUS);
    if (s21_is_greater(value_1, value_2) == TRUE)
      is_greater = FALSE;
    else
      is_greater = TRUE;
    set_sign(&value_1.bits[3], SIGN_MINUS);
    set_sign(&value_2.bits[3], SIGN_MINUS);
  } else if (get_sign(value_1.bits[3]) == SIGN_PLUS &&
             get_sign(value_2.bits[3] == SIGN_PLUS)) {
    if (value_1.bits[2] >= value_2.bits[2]) {
      if (value_1.bits[2] > value_2.bits[2])
        is_greater = TRUE;
      else {
        if (value_1.bits[1] >= value_2.bits[1]) {
          if (value_1.bits[1] > value_2.bits[1])
            is_greater = TRUE;
          else {
            if (value_1.bits[0] > value_2.bits[0]) {
              is_greater = TRUE;
            } else
              is_greater = FALSE;
          }
        } else
          is_greater = FALSE;
      }
    } else
      is_greater = FALSE;
    if (get_sign(value_1.bits[3]) == SIGN_MINUS)
      is_greater = pow(0, is_greater);
  }
  return is_greater;
}

int s21_is_not_equal(s21_decimal dst1, s21_decimal dst2) {
  int res = 0;
  if (s21_is_equal(dst1, dst2) == FALSE)
    res = TRUE;
  else
    res = FALSE;
  return res;
}

int s21_is_greater_or_equal(s21_decimal dst1, s21_decimal dst2) {
  int res = 0;
  if (s21_is_greater(dst1, dst2) == TRUE || s21_is_equal(dst1, dst2) == TRUE)
    res = TRUE;
  else
    res = FALSE;
  return res;
}

int s21_is_less_or_equal(s21_decimal dst1, s21_decimal dst2) {
  int res = 0;
  if (s21_is_less(dst1, dst2) == TRUE || s21_is_equal(dst1, dst2) == TRUE)
    res = TRUE;
  else
    res = FALSE;
  return res;
}

int s21_is_equal(s21_decimal dst1, s21_decimal dst2) {
  int res = 0;
  if (dst1.bits[0] == dst2.bits[0]) {
    if (dst1.bits[1] == dst2.bits[1]) {
      if (dst1.bits[2] == dst2.bits[2]) {
        if (dst1.bits[3] == dst2.bits[3]) {
          res = TRUE;
        } else {
          res = FALSE;
        }
      } else {
        res = FALSE;
      }
    } else {
      res = FALSE;
    }
  } else {
    res = FALSE;
  }
  return res;
}