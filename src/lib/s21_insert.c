#include "../s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index){

    char *result = s21_NULL;

    if(s21_strlen(src) > start_index){
        result = (char*)calloc(s21_strlen(src) + s21_strlen(str) + 1, sizeof(char));

        s21_strncpy(result, src, start_index);
        s21_strcat(result, str);
        
        for(int i = 0; i < s21_strlen(src) - start_index; i++){
            result[start_index + s21_strlen(str) + i] = src[start_index + i];
        }
    }

    return result;
}
