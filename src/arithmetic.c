#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int ret = 0;
  if (is_overflow(value_1, value_2) == 1) {
    init_decimal(result);
    return INF;
  }
  if (get_sign(value_1.bits[3]) == get_sign(value_2.bits[3])) {
    init_decimal(result);
    eq_scale(&value_1, &value_2);
    unsigned int mem_por = 0;
    for (int i = 0; i < 96; i++) {
      if (get_bit(value_1.bits[i / 32], i % 32) == 1 ||
          get_bit(value_2.bits[i / 32], i % 32) == 1) {
        if (mem_por == 1 && i == 95) {
          if (get_sign(value_1.bits[3]) == SIGN_MINUS)
            ret = 2;
          else
            ret = 1;
          break;
        }
        if (get_bit(value_1.bits[i / 32], i % 32) == 1 &&
            get_bit(value_2.bits[i / 32], i % 32) == 1) {
          if (mem_por == 1) {
            if (i == 95) {
              if (get_sign(value_1.bits[3]) == SIGN_MINUS)
                ret = 2;
              else
                ret = 1;
              break;
            }
            set_bit(&(result->bits[i / 32]), i % 32);
          } else {
            del_bit(&result->bits[i / 32], i % 32);
            mem_por = 1;
          }
        } else if ((get_bit(value_1.bits[i / 32], i % 32) == 0 ||
                    get_bit(value_2.bits[i / 32], i % 32) == 0)) {
          if (mem_por == 1) {
            del_bit(&result->bits[i / 32], i % 32);
          } else
            set_bit(&(result->bits[i / 32]), i % 32);
        }
      } else {
        if (mem_por == 1) {
          set_bit(&result->bits[i / 32], i % 32);
          mem_por = 0;
        } else {
          del_bit(&(result->bits[i / 32]), i % 32);
        }
      }
    }
    set_sign(&result->bits[3], get_sign(value_1.bits[3]));
    set_scale(result, get_scale(value_1));
  } else {
    if (get_sign(value_1.bits[3]) == SIGN_MINUS) {
      set_sign(&value_1.bits[3], SIGN_PLUS);
      if (s21_is_greater_or_equal(value_1, value_2)) {
        set_sign(&value_1.bits[3], SIGN_MINUS);
        set_sign(&value_2.bits[3], SIGN_MINUS);
        s21_sub(value_1, value_2, result);
      } else {
        set_sign(&value_1.bits[3], SIGN_PLUS);
        set_sign(&value_2.bits[3], SIGN_PLUS);
        s21_sub(value_2, value_1, result);
      }
    } else {
      set_sign(&value_2.bits[3], SIGN_PLUS);
      if (s21_is_greater_or_equal(value_1, value_2)) {
        set_sign(&value_1.bits[3], SIGN_PLUS);
        set_sign(&value_2.bits[3], SIGN_PLUS);
        s21_sub(value_1, value_2, result);
      } else {
        set_sign(&value_1.bits[3], SIGN_MINUS);
        set_sign(&value_2.bits[3], SIGN_MINUS);
        s21_sub(value_2, value_1, result);
      }
    }
  }
  return ret;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (get_sign(value_1.bits[3]) == get_sign(value_2.bits[3])) {
    init_decimal(result);
    if (is_overflow(value_1, value_2) == 1) {
      init_decimal(result);
      return INF;
    }
    eq_scale(&value_1, &value_2);
    unsigned int mem_por = 0;
    if (s21_is_greater_or_equal_ABS(value_1, value_2)) {
      for (int i = 0; i < 96; i++) {
        if (get_bit(value_1.bits[i / 32], i % 32) == 1) {
          if (get_bit(value_2.bits[i / 32], i % 32) == 1) {
            if (mem_por == 1)
              set_bit(&result->bits[i / 32], i % 32);
            else
              del_bit(&result->bits[i / 32], i % 32);
          } else {
            if (mem_por == 0)
              set_bit(&result->bits[i / 32], i % 32);
            else {
              del_bit(&result->bits[i / 32], i % 32);
              mem_por = 0;
            }
          }
        } else {
          if (get_bit(value_2.bits[i / 32], i % 32) == 1) {
            if (mem_por == 1)
              del_bit(&result->bits[i / 32], i % 32);
            else {
              set_bit(&result->bits[i / 32], i % 32);
              mem_por = 1;
            }
          } else {
            if (mem_por == 1)
              set_bit(&result->bits[i / 32], i % 32);
            else
              del_bit(&result->bits[i / 32], i % 32);
          }
        }
      }
      set_sign(&result->bits[3], get_sign(value_1.bits[3]));
      set_scale(result, get_scale(value_1));
    } else {
      if (get_sign(result->bits[3]) == SIGN_PLUS) {
        set_sign(&value_1.bits[3], SIGN_MINUS);
        set_sign(&value_2.bits[3], SIGN_MINUS);
      } else {
        set_sign(&value_1.bits[3], SIGN_PLUS);
        set_sign(&value_2.bits[3], SIGN_PLUS);
      }
      s21_sub(value_2, value_1, result);
    }
  } else {
    if (get_sign(value_1.bits[3]) == SIGN_MINUS) {
      set_sign(&value_2.bits[3], SIGN_MINUS);
      s21_add(value_1, value_2, result);
    } else {
      set_sign(&value_2.bits[3], SIGN_PLUS);
      s21_add(value_1, value_2, result);
    }
  }
  return OK;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (is_overflow(value_1, value_2) == 1) {
    init_decimal(result);
    return INF;
  }
  clear_code(&value_1, &value_2, result);
  int ret = OK;
  set_scale(result, get_scale(value_1) * 2);
  int buffer[96] = {0};
  int i = 95;
  int j = 95;
  while (get_bit(value_1.bits[i / 32], 32 - (i % 32)) != 1 && i != 0) {
    i--;
  }
  while (get_bit(value_1.bits[j / 32], 32 - (j % 32)) != 1 && j != 0) {
    j--;
  }
  if (i + j >= 96) {
    if (get_sign(result->bits[3]) == SIGN_MINUS) {
      ret = MIN_INF;
    } else
      ret = INF;
  } else {
    for (int cnt_2_bit = 0; cnt_2_bit <= j; cnt_2_bit++) {
      for (int cnt_1_bit = 0; cnt_1_bit <= i; cnt_1_bit++) {
        buffer[cnt_1_bit + cnt_2_bit] +=
            get_bit(value_1.bits[cnt_1_bit / 32], cnt_1_bit % 32) *
            get_bit(value_2.bits[cnt_2_bit / 32], cnt_2_bit % 32);
      }
    }
    for (int i = 0; i < 96; i++) {
      if (buffer[i] > 1) {
        buffer[i + 1] += buffer[i] / 2;
        buffer[i] = buffer[i] % 2;
      }
    }
    for (int i = 0; i < 96; i++) {
      if (buffer[i] == 1) set_bit(&result->bits[i / 32], i % 32);
    }
    if (!is_dec_not_nul(value_1) || !is_dec_not_nul(value_2)) {
      init_decimal(result);
    }
  }
  return ret;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (is_overflow(value_1, value_2) == 1) {
    init_decimal(result);
    return INF;
  }
  clear_code(&value_1, &value_2, result);
  set_scale(result, 0);
  int ret = OK;
  int metka = 0;
  if (!is_dec_not_nul(value_1)) {
    ret = 1;
    metka = 1;
  }
  if (!is_dec_not_nul(value_2)) {
    ret = 3;
    metka = 1;
  }
  int scale_2 = 0;
  int buf_azer[96] = {0};
  int buf_azer_count = 0;
  float mem = 0.0;
  s21_decimal copy_delit = value_2;
  while (metka == 0.0) {
    if (s21_is_less(value_1, value_2)) {
      metka = 1;
    } else {
      if (s21_is_greater(copy_delit, value_1)) {
        shift_right(copy_delit, &copy_delit);
        scale_2--;
        set_bit(&result->bits[scale_2 / 32], scale_2 % 32);
        s21_sub(value_1, copy_delit, &value_1);
        copy_delit = value_2;
        scale_2 = 0;
      } else {
        shift_left(copy_delit, &copy_delit);
        scale_2++;
      }
    }
  }
  if (is_dec_not_nul(value_1)) {
    for (int y = 0; y < 20; y++) {
      while (s21_is_less(value_1, value_2) && y < 20) {
        value_1.bits[0] = value_1.bits[0] << 1;
        buf_azer_count++;
        y++;
      }
      s21_sub(value_1, value_2, &value_1);
      buf_azer[buf_azer_count] = 1;
    }
    for (int i = 1; i <= 8; i++) {
      mem += buf_azer[i] * pow(2.0, -i);
    }
    s21_decimal af_zer;
    s21_from_float_to_decimal(mem, &af_zer);
    set_sign(&af_zer.bits[3], get_sign(result->bits[3]));
    s21_add(*result, af_zer, result);
  }
  return ret;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (is_overflow(value_1, value_2) == 1) {
    init_decimal(result);
    return INF;
  }
  clear_code(&value_1, &value_2, result);
  int ret = OK;
  int metka = 0;
  if (!is_dec_not_nul(value_1)) {
    ret = 1;
    metka = 1;
  }
  if (!is_dec_not_nul(value_2)) {
    ret = 3;
    metka = 1;
  }
  int scale_2 = 0;
  s21_decimal copy_delit = value_2;
  while (metka == 0.0) {
    if (s21_is_less(value_1, value_2)) {
      metka = 1;
    } else {
      if (s21_is_greater(copy_delit, value_1)) {
        shift_right(copy_delit, &copy_delit);
        scale_2--;
        set_bit(&result->bits[scale_2 / 32], scale_2 % 32);
        s21_sub(value_1, copy_delit, &value_1);
        copy_delit = value_2;
        scale_2 = 0;
      } else {
        shift_left(copy_delit, &copy_delit);
        scale_2++;
      }
    }
  }
  set_sign(&value_1.bits[3], get_sign(result->bits[3]));
  *result = value_1;
  set_scale(result, get_scale(value_1));
  return ret;
}
