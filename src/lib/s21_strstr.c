#include "../s21_string.h"

char *s21_strstr(const char *haystack, const char *needle){

    s21_size_t i = 0, j = 0;
    for(; *haystack != '\0' && needle[j] != '\0'; haystack++){
        for(j = 0; haystack[i] == needle[j]; i++, j++){
            ;
        }
    }
        
    if(needle[i] == '\0'){
        haystack--;
    }
    
    return needle[i] == '\0' ? (char*)haystack : s21_NULL;
}
