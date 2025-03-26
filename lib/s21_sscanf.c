#include "../s21_string.h"
// Функция sscanf() используется для считывания отформатированного ввода из
// строки. функция sscanf() считывает ввод из строки и сохраняет его в другой
// строке. Возвращаемое значение В случае успешного выполнения функция
// возвращает количество успешно прочитанных значений. В случае сбоя ввода до
// успешного считывания каких-либо данных возвращается значение EOF.
// проверим

int s21_sscanf(const char *str, const char *format, ...) {
  int result = 0;
  va_list arguments;
  form params = {0};
  va_start(arguments, format);
  int count_for_n = 0, count_char = 0;
  while (*format && count_char != -1) {
    count_char = 0;
    if (*format == '%' && *(format + 1) != '%') {
      format++;  // Переход к следующему символу после '%'
      params.width = 0;
      if (iss_digit(*format)) params.width = convert_str_to_int_sc(&format);
      params.length = parser_length_sc(*format);  // h,l,L
      if (params.length != no_length) format++;
      if (s21_strchr("cs", *format)) {
        count_char = scanf_cs(str, arguments, params, format, &result);
      } else if (s21_strchr("di", *format)) {
        count_char = scanf_di(str, arguments, params, &result);
      } else if (s21_strchr("ouxXp", *format)) {
        count_char = scanf_unsig(str, arguments, params, format, &result);
      } else if (s21_strchr("eEgGf", *format)) {
        count_char = scanf_float(str, arguments, params, &result);
      } else if (s21_strchr("n", *format)) {
        *va_arg(arguments, int *) = count_for_n;
      }
      if (count_char > 0) str = str + count_char;
      count_for_n += count_char;
      format++;
    } else if (*format == ' ') {
      format++;
      if (*str == ' ') {
        str++;
        count_for_n++;
      }
    } else {  // пропускаем одинаковые символы без %
      if (*format != '%' && *format != *str) break;
      format++;
      str++;
      count_for_n++;
    }
  }
  va_end(arguments);
  if (result == 0) result = -1;
  return result;
}

Length parser_length_sc(char len) {
  Length length = no_length;
  switch (len) {
    case 'h':
      length = short_int;
      break;
    case 'l':
      length = long_int;
      break;
    case 'L':
      length = long_double;
      break;
  }
  return length;
}

int iss_digit(char symb) { return symb >= '0' && symb <= '9' ? 1 : 0; }

int is_space(char symb) {
  return (symb == ' ' || symb == '\n' || symb == '\t' || symb == '\0') ? 1 : 0;
}

int convert_str_to_int_sc(const char **format) {
  int digit = *(*format)++ - '0';
  while (iss_digit(*(*format))) {
    digit *= 10;
    digit += *(*format)++ - '0';
  }

  return digit;
}

long long my_atoi(const char **str, int *count, int width) {
  long long result = 0;
  int sign = 1;  // Переменная для хранения знака числа
  int max = 0;  // переполнения
  int len;
  (width > 0) ? (len = width) : (len = -1);
  // Обрабатываем знак числа
  if (*(*str) == '-') {
    sign = -1;
    *str = *str + 1;
    *count += 1;
  } else if (*(*str) == '+') {
    sign = 1;
    *str = *str + 1;
    *count += 1;
  }
  // Преобразуем строку в число
  while (iss_digit(*(*str)) && len != 0) {
    if ((sign == -1) && (result != 0)) {
      result = -1 * result;
      sign = 1;
    }
    if (result < 0 && max == 0) {
      if (result < ((-9223372036854775807 + (*(*str) - '0')) / 10)) {
        result = -9223372036854775807;
        max = 1;
      } else {
        result = result * 10 - (*(*str) - '0');
      }
    } else if (max == 0) {
      if (result > ((9223372036854775807 - (*(*str) - '0')) / 10)) {
        result = 9223372036854775807;
        max = 1;
      } else
        result = result * 10 + (*(*str) - '0');
    }
    *str = *str + 1;
    *count += 1;
    len--;
  }
  return result;  // Возвращаем результат с учетом знака
}

unsigned long long my_atoi_base(const char **str, unsigned base, int *count,
                                int width) {
  unsigned long long result = 0;
  int bbase = (int)base;
  int len;
  (width > 0) ? (len = width) : (len = -1);
  while (len != 0 &&
         ((**str >= '0' && **str <= ('0' + bbase - 1)) ||
          (base > 10 && ((**str >= 'A' && **str <= ('A' + bbase - 10)) ||
                         (**str >= 'a' && **str <= ('a' + bbase - 10)))))) {
    char base_char;
    (**str >= 'a')
        ? (base_char = 'a' - 10)
        : ((**str >= 'A') ? (base_char = 'A' - 10) : (base_char = '0'));
    if (result > ((MAXUNSIG - (*(*str) - base_char)) / base)) {
      result = MAXUNSIG;
    } else
      result = result * base + (*(*str) - base_char);
    *str = *str + 1;
    *count += 1;
    len--;
  }
  return result;
}

int scanf_cs(const char *str, va_list arguments, form params,
             const char *format, int *result) {
  int count_char = 0;
  int count_probel = 0;
  // int len;
  if (*format == 'c') { /* символ */
    if (str[0]) {
      char *c_new = va_arg(arguments, char *);
      count_char++;
      *c_new = *str++;
      *result = *result + 1;
    }
  } else { /* строка */
    while (is_space(*str)) {
      str++;
      count_probel++;
    }
    // char *start = (char *)str;
    if ((params.length == long_int) && (*format == 's')) {
      wchar_t *s_new = va_arg(arguments, wchar_t *);
      while ((*str != '\0' && *str != ' ' && *str != '\n') &&
             (params.width == 0 || count_char < params.width)) {
        *s_new++ = *str++;
        count_char++;
      }
      *s_new = '\0';
      *result = *result + 1;
    } else {
      char *s_new = va_arg(arguments, char *);
      while ((*str != '\0' && *str != ' ' && *str != '\n') &&
             (params.width == 0 || count_char < params.width)) {
        *s_new++ = *str++;
        count_char++;
      }
      *s_new = '\0';
      *result = *result + 1;
    }
  }
  if (count_char == 0)
    count_char = -1;
  else
    count_char = count_char + count_probel;
  return count_char;
}

int scanf_di(const char *str, va_list arguments, form params, int *result) {
  int count_char = 0, old_count = 0, bad = 0;
  int base = 10;

  while (is_space(*str)) {
    str++;  // Пропускаем пробелы в строке
    count_char++;
  }
  if (str[0] == '0' && str[1] == 'x') {
    str = str + 2;
    count_char = count_char + 2;
    base = 16;
  }
  long long data;
  old_count = count_char;
  if (base == 16) {
    data = my_atoi_base(&str, base, &count_char, params.width);
  } else
    data = my_atoi(&str, &count_char, params.width);
  if (count_char == old_count) bad = 1;
  if (params.length == short_int) {
    short int *d = va_arg(arguments, short int *);
    if (bad == 0) *d = (short)data;
  } else if (params.length == long_double) {
    long long *d = va_arg(arguments, long long *);
    if (bad == 0) *d = data;
  } else if (params.length == long_int) {
    long *d = va_arg(arguments, long *);
    if (bad == 0) *d = (long)data;
  } else {
    int *d = va_arg(arguments, int *);
    if (bad == 0) *d = (int)data;
  }
  if (count_char > 0 && bad == 0) *result = *result + 1;
  return count_char;
}

int scanf_unsig(const char *str, va_list arguments, form params,
                const char *format, int *result) {
  int count_char = 0;
  int base = 10;
  while (is_space(*str)) {
    str++;  // Пропускаем пробелы в строке
    count_char++;
  }
  switch (*format) {
    case 'o':
      base = 8;
      break;
    case 'x':
    case 'X':
    case 'p':
      base = 16;
      if (str[0] == '0' && str[1] == 'x') {
        str = str + 2;
        count_char = count_char + 2;
      }
      break;
    default:
      base = 10;
      break;
  }
  unsigned long long data = my_atoi_base(&str, base, &count_char, params.width);
  if (*format == 'p') {
    unsigned long int *p_void = va_arg(arguments, void *);
    *p_void = (unsigned long)data;
  } else if (params.length == short_int) {
    short unsigned *d = va_arg(arguments, short unsigned *);
    *d = (short)data;
  } else if (params.length == long_double) {
    unsigned long long *d = va_arg(arguments, unsigned long long *);
    *d = data;
  } else if (params.length == long_int) {
    long unsigned *d = va_arg(arguments, unsigned long *);
    *d = (long unsigned)data;
  } else {
    unsigned *d = va_arg(arguments, unsigned *);
    *d = (unsigned)data;
  }

  if (count_char == 0)
    count_char = -1;
  else
    *result = *result + 1;
  return count_char;
}

int scanf_float(const char *str, va_list arguments, form params, int *result) {
  int count_char = 0;
  int count_probel = 0;
  char znak = ' ';
  int bad = 0;
  // int nan = 0x7F800001;
  while (is_space(*str)) {
    str++;  // Пропускаем пробелы в строке
    count_probel++;
  }
  if (*str == '+' || *str == '-') {
    znak = *str;
    str++;
    count_char++;
  }
  long double data = (long double)my_atoi(
      &str, &count_char, params.width);  // Преобразуем строку в целое число
  int old_count = count_char;
  if (count_char == 0) bad = -1;
  count_char = 0;
  if (*str == '.') {
    str++;
    long double data2 = (long double)my_atoi(&str, &count_char, params.width);
    if (count_char == 0) bad = -1;
    old_count = old_count + count_char + 1;
    while (count_char > 0) {
      data2 = data2 / 10;
      count_char = count_char - 1;
    }
    data += data2;
  }
  if (znak == '-' && bad == 0) (data = data * -1);
  znak = '+';
  if (*str == 'e' || *str == 'E') {
    str++;
    old_count++;
    if (*str == '+' || *str == '-') {
      znak = *str;
      str++;
      old_count++;
    }
    count_char = 0;
    long long data3 = my_atoi(&str, &count_char, params.width);
    if (count_char == 0) bad = -1;
    old_count = old_count + count_char;
    if (params.length == long_double) {
      if (data3 > 4932) bad = 1;
    } else {
      if (data3 > 308) bad = 1;
    }
    while (data3 > 0 && bad == 0) {
      (znak == '-') ? (data = data / 10) : (data = data * 10);
      data3 -= 1;
    }
    if (bad == 1 && znak == '-') {
      data = 0.0;
      bad = -1;
    }
  }
  if (params.length == long_double) {
    long double *d = va_arg(arguments, long double *);
    if (bad == 0) *d = (long double)data;
    if (bad == 1) *d = 1.0 / 0.0;
  } else {
    float *d = va_arg(arguments, float *);
    if (bad == 0) *d = (float)data;
    if (bad == 1) *d = 1.0 / 0.0;
  }
  if (old_count > 0 && bad == 0) *result = *result + 1;
  count_char = old_count + count_probel;
  return count_char;
}
