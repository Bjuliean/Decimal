#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (get_scale(value) != 0) {
    int metka = 0;
    s21_decimal one = {{1, 0, 0, 0}};
    if (get_sign(value.bits[3]) == SIGN_MINUS) {
      metka = 1;
      set_sign(&value.bits[3], SIGN_PLUS);
    }
    s21_truncate(value, result);
    if (metka == 1) {
      s21_add(*result, one, result);
      set_sign(&result->bits[3], SIGN_MINUS);
    }
  } else {
    *result = value;
  }
  return OK;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;
  if (get_sign(value.bits[3]) == SIGN_PLUS)
    set_sign(&result->bits[3], SIGN_MINUS);
  else
    set_sign(&result->bits[3], SIGN_PLUS);
  return OK;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int scale = get_scale(value);
  for (int i = 0; i < scale; i++) {
    div10(&value);
  }
  init_decimal(result);
  *result = value;
  return OK;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int metka = 0;
  if (get_sign(value.bits[3]) == SIGN_MINUS) {
    metka = 1;
    set_sign(&value.bits[3], SIGN_PLUS);
  }
  s21_decimal rem_value = value;
  s21_decimal div_value = value;
  s21_decimal five = {{5, 0, 0, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  int scale = get_scale(value);
  if (scale != 0) {
    div10(&div_value);
  }
  for (int i = 1; i < scale; i++) {
    rem_value = div_value;
    div10(&div_value);
  }
  if (scale != 0) {
    rem10(&rem_value);
    set_scale(&value, scale);
    s21_truncate(value, result);
    if (s21_is_equal(rem_value, five)) {
      if (rem_round(value) == 1 && scale > 1) {
        s21_add(*result, one, result);
      } else {
        if (get_bit(div_value.bits[0], 0) == 0) {
          s21_add(*result, one, result);
        }
      }
    } else if (s21_is_greater(rem_value, five)) {
      s21_add(*result, one, result);
    }
  } else {
    *result = value;
  }
  if (metka == 1) {
    set_sign(&result->bits[3], SIGN_MINUS);
  }
  return 0;
}
