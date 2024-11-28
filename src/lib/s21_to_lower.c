#include "../s21_string.h"

void *s21_to_lower(const char *str){
    
    char *result = s21_NULL;
    if(str){
        result = (char*)calloc(s21_strlen(str) + 1, sizeof(char));
        int i = 0;
        while(str[i] != '\0'){
            if(str[i] > 64 && str[i] < 91){
                result[i] = str[i] + 32;
            }
            else{
                result[i] = str[i];
            }
            i++;
        }
        result[i] = '\0';
    }

    return result;
}
