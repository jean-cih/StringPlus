#include "../s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n){

    unsigned char *s = (unsigned char*)str;
    while(*s != c && n){ 
        s++;
        n--;
    }

    return n != 0 ? (void*)s : s21_NULL;
}
