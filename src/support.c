#include "s21_decimal.h"

int is_overflow(s21_decimal dst1, s21_decimal dst2) {
  int is_ok = 0;
  if (get_scale(dst1) < get_scale(dst2)) {
    for (unsigned int i = 96; i > 96 - (get_scale(dst2) - get_scale(dst1));
         i--) {
      if (get_bit(dst1.bits[i / 32], i % 32) == 1) is_ok = 1;
    }
  } else {
    for (unsigned int i = 96; i > 96 - (get_scale(dst1) - get_scale(dst2));
         i--) {
      if (get_bit(dst2.bits[i / 32], i % 32) == 1) is_ok = 1;
    }
  }
  return is_ok;
}

void shift_left(s21_decimal dst, s21_decimal *res) {
  int pastBit = 0;
  for (int i = 0; i < 96; i++) {
    if (pastBit == 1) set_bit(&res->bits[i / 32], i % 32);
    if (pastBit == 0 || i == 0) del_bit(&res->bits[i / 32], i % 32);
    if (get_bit(dst.bits[i / 32], i % 32) == 1) pastBit = 1;
    if (get_bit(dst.bits[i / 32], i % 32) == 0) pastBit = 0;
  }
}
void shift_right(s21_decimal dst, s21_decimal *res) {
  int pastBit = 0;
  for (int i = 96; i >= 0; i--) {
    if (pastBit == 1) set_bit(&res->bits[i / 32], i % 32);
    if (pastBit == 0 || i == 96) del_bit(&res->bits[i / 32], i % 32);

    if (get_bit(dst.bits[i / 32], i % 32) == 1) pastBit = 1;
    if (get_bit(dst.bits[i / 32], i % 32) == 0) pastBit = 0;
  }
}

int s21_is_greater_or_equal_ABS(s21_decimal value_1, s21_decimal value_2) {
  int is_greater = FALSE;
  if (value_1.bits[2] >= value_2.bits[2]) {
    if (value_1.bits[2] > value_2.bits[2]) is_greater = TRUE;
    if (value_1.bits[1] >= value_2.bits[1]) {
      if (value_1.bits[1] > value_2.bits[1]) is_greater = TRUE;
      if (value_1.bits[0] >= value_2.bits[0]) {
        if (value_1.bits[0] > value_2.bits[0]) is_greater = TRUE;
      }
    }
  }
  if (is_greater == TRUE || s21_is_equal(value_1, value_2) == TRUE)
    is_greater = TRUE;
  return is_greater;
}

void get_point_part(s21_decimal dst, s21_decimal *res) {
  s21_decimal div;
  unsigned int scale = get_scale(dst);
  int divider = pow(10, scale);
  int sign = get_sign(dst.bits[3]);
  if (sign == SIGN_MINUS) set_sign(&dst.bits[3], SIGN_PLUS);
  if (scale != 0) {
    s21_from_int_to_decimal(divider, &div);
    set_scale(&div, scale);
    while (s21_is_greater(dst, div)) {
      s21_sub(dst, div, &dst);
    }
    res->bits[0] = dst.bits[0];
    res->bits[1] = dst.bits[1];
    res->bits[2] = dst.bits[2];
    res->bits[3] = dst.bits[3];
  } else {
    res->bits[0] = 0;
    res->bits[1] = 0;
    res->bits[2] = 0;
    res->bits[3] = 0;
  }
  if (sign == SIGN_MINUS) set_sign(&res->bits[3], SIGN_MINUS);
}

void init_decimal(s21_decimal *dst) {
  for (int i = 0; i <= 31; i++) {
    del_bit(&dst->bits[0], i);
  }
  for (int i = 0; i <= 31; i++) {
    del_bit(&dst->bits[1], i);
  }
  for (int i = 0; i <= 31; i++) {
    del_bit(&dst->bits[2], i);
  }
  for (int i = 0; i <= 31; i++) {
    del_bit(&dst->bits[3], i);
  }
}

void debug(unsigned int num) {
  static int testNum = 1;
  printf("#####################################TEST %d\n\n", testNum);
  for (int i = 31; i >= 0; i--) {
    printf("№%d", i);
    if (i == 31 || i == 23) {
      printf("|");
      if (i == 23) printf("(1)");
    }
    printf("\t");
  }
  printf("\n");
  for (int i = 31; i >= 0; i--) {
    if (get_bit(num, i) == 1) {
      printf("1");
      printf("\t");
    } else {
      printf("0");
      printf("\t");
    }
  }
  printf("\n\n");
  printf("For copy: ");
  for (int i = 31; i >= 0; i--) {
    if (get_bit(num, i) == 1) {
      printf("1");
    } else {
      printf("0");
    }
  }
  printf("  ||  %u", num);
  testNum++;
  printf("\n");
  printf("###########################################\n\n");
}

unsigned int translate_float_to_int(float src) {
  return ((translated_float){.num = src}).bits;
}

int get_float_exp(unsigned int src) {
  int res = 0;
  int discharge = 7;
  for (int i = 30; i >= 23; i--) {
    if (get_bit(src, i) == 1) {
      res += pow(2, discharge);
    }
    discharge--;
  }
  return res;
}

void set_sign(unsigned int *num, int sign) {
  int mask = 1;
  mask = mask << 31;
  if (sign == SIGN_PLUS) {
    mask = ~mask;
    *num = *num & mask;
  }
  if (sign == SIGN_MINUS) {
    *num = *num | mask;
  }
}

int get_sign(unsigned int num) {
  int mask = 1;
  mask = mask << 31;
  mask = mask & num;
  return mask == 0 ? SIGN_PLUS : SIGN_MINUS;
}

void del_bit(unsigned int *num, int bit) {
  int mask = 1;
  mask = mask << bit;
  mask = ~mask;
  *num = *num & mask;
}

void set_bit(unsigned int *num, int bit) {
  int mask = 1;
  mask = mask << bit;
  *num = *num | mask;
}

void set_bitBack(unsigned int *num, int bit) { *num = *num >> bit; }

int get_bit(unsigned int num, int bit) {
  int res = 0;
  int mask = 1;
  mask = mask << bit;
  res = num & mask;
  return res == 0 ? 0 : 1;
}

void eq_scale(s21_decimal *dst1, s21_decimal *dst2) {
  unsigned int scale1 = get_scale(*dst1);
  unsigned int scale2 = get_scale(*dst2);
  if (scale1 > scale2) {
    for (int i = scale1 - scale2; i > 0; i--) {
      mul10(dst2);
    }
    set_scale(dst2, scale1);
  }
  if (scale1 < scale2) {
    for (int i = scale2 - scale1; i > 0; i--) {
      mul10(dst1);
    }
    set_scale(dst1, scale2);
  }
}

void del_scale(s21_decimal *dst) {
  for (int i = 16; i <= 23; i++) {
    del_bit(&dst->bits[3], i);
  }
}

void set_scale(s21_decimal *dst, unsigned int scale) {
  int scaleIndex = 0;
  del_scale(dst);
  for (int i = 16; i <= 23; i++) {
    if (get_bit(scale, scaleIndex) == 1) {
      set_bit(&dst->bits[3], i);
    } else {
      del_bit(&dst->bits[3], i);
    }
    scaleIndex++;
  }
}

unsigned int get_scale(s21_decimal dst) {
  unsigned int scale = 0;
  int index = 0;
  for (int i = 16; i <= 23; i++) {
    if (get_bit(dst.bits[3], i) == 1) {
      scale += pow(2, index);
    }
    index++;
  }
  return scale;
}

void mul10(s21_decimal *dst) {
  s21_decimal dst_copy, dst_copy2, dst_result;
  init_decimal(&dst_copy);
  init_decimal(&dst_copy2);
  init_decimal(&dst_result);
  for (int j = 0; j < 3; j++) {
    dst_copy.bits[j] = dst->bits[j];
    dst_copy2.bits[j] = dst->bits[j];
  }
  for (int i = 0; i < 9; i++) {
    s21_add(dst_copy, dst_copy2, &dst_result);
    for (int j = 0; j < 3; j++) {
      dst_copy.bits[j] = dst_result.bits[j];
    }
  }
  for (int j = 0; j < 3; j++) {
    dst->bits[j] = dst_result.bits[j];
  }
}

int is_dec_not_nul(s21_decimal value) {
  int res = 0;
  for (int i = 0; i < 96; i++) {
    if (get_bit(value.bits[i / 32], i % 32) == 1) res = 1;
  }
  return res;
}

void clear_code(s21_decimal *value_1, s21_decimal *value_2,
                s21_decimal *result) {
  init_decimal(result);
  eq_scale(value_1, value_2);
  set_scale(result, get_scale(*value_1));
  // if (get_sign(value_1->bits[3]) == SIGN_MINUS ||
  //     get_sign(value_2->bits[3]) == SIGN_MINUS) {
  //   if (get_sign(value_1->bits[3]) == SIGN_MINUS &&
  //       get_sign(value_2->bits[3]) == SIGN_MINUS)
  //     set_sign(&result->bits[3], SIGN_PLUS);
  //   else
  //     set_sign(&result->bits[3], SIGN_MINUS);
  // }
  if (get_sign(value_1->bits[3]) == get_sign(value_2->bits[3])) {
    set_sign(&result->bits[3], SIGN_PLUS);
  } else
    set_sign(&result->bits[3], SIGN_MINUS);
  set_sign(&value_1->bits[3], SIGN_PLUS);
  set_sign(&value_2->bits[3], SIGN_PLUS);
}

int rem_round(s21_decimal value) {
  int metka = 0;
  s21_decimal zero = {{0, 0, 0, 0}};
  int scale = get_scale(value);
  for (int i = 1; i < scale; i++) {
    rem10(&value);
    if (s21_is_not_equal(value, zero)) {
      metka = 1;
    }
  }
  return metka;
}

void div10(s21_decimal *value) {
  int metka = 0;
  del_scale(value);
  if (get_sign(value->bits[3]) == SIGN_MINUS) {
    set_sign(&value->bits[3], SIGN_PLUS);
    metka = 1;
  }
  s21_decimal q = *value;
  s21_decimal n = *value;
  s21_decimal nine = {{9, 0, 0, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  shift_right(q, &q);
  shift_right(q, &q);
  shift_right(n, &n);
  s21_add(q, n, &q);
  s21_decimal buf = q;

  for (int i = 4; i <= 16; i *= 2) {
    for (int z = i; z > 0; z--) {
      shift_right(buf, &buf);
    }
    s21_add(q, buf, &q);
    buf = q;
  }
  for (int i = 0; i < 3; i++) {
    shift_right(q, &q);
  }
  s21_decimal rem = q;
  shift_left(rem, &buf);
  shift_left(buf, &buf);
  s21_add(rem, buf, &rem);
  shift_left(rem, &rem);
  init_decimal(&buf);
  s21_sub(*value, rem, &buf);
  if (s21_is_greater(buf, nine)) {
    s21_add(q, one, &q);
  }
  *value = q;
  if (metka == 1) {
    set_sign(&value->bits[3], SIGN_MINUS);
  }
}

void rem10(s21_decimal *value) {
  del_scale(value);
  s21_decimal q = *value;
  s21_decimal n = *value;
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal eleven = {{11, 0, 0, 0}};
  s21_decimal zero = {{0, 0, 0, 0}};
  shift_right(q, &q);
  shift_right(q, &q);
  shift_right(n, &n);
  s21_add(q, n, &q);
  s21_decimal buf = q;
  for (int i = 4; i <= 16; i *= 2) {
    for (int z = i; z > 0; z--) {
      shift_right(buf, &buf);
    }
    s21_add(q, buf, &q);
    buf = q;
  }
  for (int i = 0; i < 3; i++) {
    shift_right(q, &q);
  }
  s21_decimal rem = q;
  shift_left(rem, &buf);
  shift_left(buf, &buf);
  s21_add(rem, buf, &rem);
  shift_left(rem, &rem);
  init_decimal(&buf);
  s21_sub(*value, rem, &buf);
  set_sign(&buf.bits[3], SIGN_PLUS);
  if (s21_is_equal(buf, ten)) {
    buf = zero;
  }
  if (s21_is_equal(buf, eleven)) {
    buf = one;
  }
  *value = buf;
}
