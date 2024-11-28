#include "../s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n){

    unsigned char *s1 = (unsigned char*)str1;
    unsigned char *s2 = (unsigned char*)str2;
    while(*s1 == *s2 && n){
        s1++;
        s2++;
        n--;
    }

    return n != 0 ? *s1 - *s2 : 0;
}
