#include "../s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n){
    int flag = 0;
    
    while(n != 0){
        
        if(*str1++ != *str2++){
            flag = 1;
        }
        if(*str1 == '\0' || *str1 == '\0'){
            break;
        }
        n--;
    }

    return flag ? *str1 - *str2 : 0;
}
