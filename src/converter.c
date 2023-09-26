#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  unsigned int scale = 0;
  uint64_t buffer = 0;
  scale = get_scale(src);
  scale = pow(10, scale);
  buffer = ((src.bits[0]) + (src.bits[1] * 4294967296) +
            (src.bits[2] * 4294967296 * 4294967296)) /
           scale;
  if (buffer <= 2147483647 && get_sign(src.bits[3]) == SIGN_PLUS)
    *dst = buffer;
  else if (buffer <= 2147483648 && get_sign(src.bits[3]) == SIGN_MINUS)
    *dst = -buffer;
  else
    return INF;
  return OK;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  unsigned int scale = get_scale(src);
  long double res = 0;
  for (int i = 0; i < 96; i++) {
    if (get_bit(src.bits[i / 32], i % 32) == 1) {
      res += pow(2, i);
    }
  }
  res /= pow(10, scale);
  if (get_sign(src.bits[3]) == SIGN_MINUS) res = -res;
  *dst = (float)res;
  return OK;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  init_decimal(dst);
  if (is_inf(src) || is_nan(src)) return INF;
  if (src < 0) set_sign(&dst->bits[3], SIGN_MINUS);
  int scale = 0;
  unsigned int res = 0;
  while (src != (int)src) {
    src *= 10;
    scale++;
  }
  set_scale(dst, scale);
  unsigned int bits = translate_float_to_int(src);
  int exp = get_float_exp(bits) - 127;
  for (int i = 23; i >= 0; i--) {
    if (get_bit(bits, i) == 1 || i == 23) res += pow(2, exp);
    exp--;
  }
  dst->bits[0] = res;
  return OK;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  init_decimal(dst);
  if (src < 0) {
    set_sign(&dst->bits[3], SIGN_MINUS);
    src = -src;
  } else {
    set_sign(&dst->bits[3], SIGN_PLUS);
  }
  dst->bits[0] = src;
  return OK;
}
