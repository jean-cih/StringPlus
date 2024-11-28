#include "../s21_string.h"

void *s21_trim(const char *src, const char *trim_chars){

    char *result = s21_NULL;
    int i = 0;
    if(src){
        if(trim_chars){
            for(; strchr(trim_chars, src[i]) && src[i] != '\0'; i++)
                ;
        }

        result = (char*)calloc(s21_strlen(src + i) + 1, sizeof(char));

        s21_strcpy(result, src + i);
        if(trim_chars){
            i = s21_strlen(result);
            for(i = i > 0 ? i - 1 : 0; s21_strchr(trim_chars, result[i]) && result[i] != '\0'; i--){
                result[i] = '\0';
            }
        }
        result[s21_strlen(result)] = '\0';
    }

    return result;
}
