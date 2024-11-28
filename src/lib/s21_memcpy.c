#include "../s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n){
    char *dest_cmp = (char*)dest;
    char *src_cmp = (char*)src;

    for(s21_size_t i = 0; i < n; i++){
        dest_cmp[i] = src_cmp[i];
    }

    return dest_cmp;
}
