#include "../s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n){
    
    s21_size_t size2 = n < s21_strlen(src) ? n : s21_strlen(src);
    s21_size_t size1 = s21_strlen(dest);
    char *result = (char*)calloc(size1 + size2 + 1, sizeof(char));
    for(s21_size_t i = 0; i < size1 + size2; i++){
        if(i < size1){
            result[i] = dest[i]; 
        }
        else{
            result[i] = src[i - size1];
        }
    }
    result[size1 + size2] = '\0';


    return result;
}
