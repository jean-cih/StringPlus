#include "../s21_string.h"


int main(void){

    for(int i = 0; i < 200; i++){
        printf("%d %s\n", i, strerror(i));
    }

    return 0;
}


char *s21_strerror(int errnum){

    return NULL;
}
