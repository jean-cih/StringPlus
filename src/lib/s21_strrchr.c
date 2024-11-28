#include "../s21_string.h"

char *s21_strrchr(const char *str, int c){

    char *s = s21_NULL;
    while(*str != '\0'){
        if(*str == c){
            s = (char*)str;
        }
        str++;
    }

    return s;
}
