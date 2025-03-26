#include "../s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  char *ptr = S21_NULL;
  s21_size_t len_src = s21_strlen(src);
  if (src && trim_chars) {
    char *start_pos = (char *)src;
    char *butt_pos = (char *)src + len_src;

    while (*start_pos != '\0' && s21_strchr(trim_chars, *start_pos))
      start_pos++;

    while (butt_pos != start_pos && s21_strchr(trim_chars, *(butt_pos - 1)))
      butt_pos--;
    ptr = calloc(butt_pos - start_pos + 1, sizeof(char));
    s21_strncpy(ptr, start_pos, butt_pos - start_pos);
  }
  return ptr;
}
