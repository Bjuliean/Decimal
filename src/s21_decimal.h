#ifndef _S21_DECIMAL_H_
#define _S21_DECIMAL_H_

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define is_fin(x) __builtin_isfinite(x)
#define is_nan(x) __builtin_isnan(x)
#define is_inf(x) __builtin_isinf(x)

enum func_state {
  OK = 0,
  INF = 1,
  MIN_INF = 2,
  DIV_BY_ZERO = 3,
  SIGN_MINUS = 4,
  SIGN_PLUS = 5,
  ERR = 6
};

enum comp_state { FALSE = 0, TRUE = 1 };

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef union {
  float num;
  unsigned int bits;
} translated_float;

// support
int get_bit(unsigned int num, int bit);
void set_bit(unsigned int *num, int bit);
void del_bit(unsigned int *num, int bit);
int get_sign(unsigned int num);
void set_sign(unsigned int *num, int sign);
unsigned int translate_float_to_int(float src);
int get_float_exp(unsigned int src);
void debug(unsigned int num);
void init_decimal(s21_decimal *dst);
void mul10(s21_decimal *dst);
unsigned int get_scale(s21_decimal dst);
void eq_scale(s21_decimal *dst1, s21_decimal *dst2);
void set_scale(s21_decimal *dst, unsigned int scale);
void del_scale(s21_decimal *dst);
void get_point_part(s21_decimal dst, s21_decimal *res);
void shift_left(s21_decimal dst, s21_decimal *res);
void shift_right(s21_decimal dst, s21_decimal *res);
void clear_code(s21_decimal *value_1, s21_decimal *value_2,
                s21_decimal *result);
int is_dec_not_nul(s21_decimal value);
int s21_is_greater_or_equal_ABS(s21_decimal value_1, s21_decimal value_2);
int is_overflow(s21_decimal dst1, s21_decimal dst2);
void div10(s21_decimal *value);
void rem10(s21_decimal *value);
int rem_round(s21_decimal value);

// comparison;

int s21_is_less(s21_decimal dst1, s21_decimal dst2);
int s21_is_equal(s21_decimal dst1, s21_decimal dst2);
int s21_is_less_or_equal(s21_decimal dst1, s21_decimal dst2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal dst1, s21_decimal dst2);
int s21_is_not_equal(s21_decimal dst1, s21_decimal dst2);

// converter
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);

// another
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);

// arithmetic
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

#endif  //  _S21_DECIMAL_H_
