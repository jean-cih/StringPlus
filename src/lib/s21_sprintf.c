#include "../s21_string.h"
#include <math.h>


#define MAX_BUFFER_SIZE 1024

typedef enum Length { short_int, long_int, long_double, no_length } Length;

typedef struct formatting {
    int width;
    Length length;
    int flags;
    int accurasy;
    char sign;
} form;

void convert_int_to_hex(va_list input, char **buffer, form params, char specifier);
void convert_int_to_oct(va_list input, char **buffer, form params);
void convert_double_to_e(va_list input, char *buffer, form params, char specifier);
void convert_int_to_str(va_list input, char **buffer, form params);
void convert_double_to_str(va_list input, char **buffer, form params);
void convert_ptr_to_str(va_list input, char **buffer, form params);
void convert_double_to_g(va_list input, char *buffer, form params, char specifier);
void str_to_str(char **buffer, va_list input, form params);

int convert_str_to_int(const char **format);
void reverse_str(char *buffer, int j);
void add_string(char *str, char *buffer, int *result);

int parser_flags(const char **flag_form);
Length parser_length(char len);
int is_digit(char symb);
int default_accurasy(char symb);
char *pull_string(char *str, va_list input, form params, char specifier);
void char_to_str(va_list input, char **buffer, form params);



int main(void){

    char buffer1[MAX_BUFFER_SIZE];
    char buffer2[MAX_BUFFER_SIZE];
    char c = 'W';
    int d = 123;
    int i = -134;
    double f = 12.0034591534;
    char *s = "Hello World";
    unsigned int u = 1265;
    double g = -0.0001233444;
    double e = 12454.3344;
    unsigned int x = 986;
    unsigned int o = 759;
    int n1;
    int n2;
    void *p;

    
   s21_sprintf(buffer1, "%%double f = %%%+.4f,\nint d = %+.6d,\nint i = %.5i,\ncount n = %n,\ndouble e = %e,\ndouble g = %.4g,\nstring s = %.4s,\nchar c = %c,\nint u = %.1u,\nint x = %.6x,\noctal o = %o,\ndouble g = %g,\npointer p = %p,\n", f, d, i, &n1, e, e, s, c, u, x, o, g, p);
    
    //s21_sprintf(buffer1, "%%char c = %+-5c,\n%n%%char c = %-5c,\n%%char c = %0c,\n", c, &n1, c, c);

    //s21_sprintf(buffer1, "%%char c = %+5d,\n%n%%char c = %-5d,\n%%char c = % 4d,\n", d, &n1, d, d);
    //s21_sprintf(buffer1, "%%char c = %+5.2s,\n%n%%char c = %-5.2s,\n%%char c = %04.2s,\n", s, &n1, s, s);
    //s21_sprintf(buffer1, "%%char c = %-9.6o,\n%n%%char c = %#-6o,\n%%char c = %#o,\n%%char c = %#07o,\n%%char c = %#.4o,\n", o, &n1, o, o, o, o);
    //s21_sprintf(buffer1, "%%char c = %-8.6x,\n%n%%char c = %#-6X,\n%%char c = %x,\n%%char c = %#07x,\n", x, &n1, x, x, x);
    //s21_sprintf(buffer1, "%%char c = %20p,\n%n%%char c = %-21p,\n%%char c = %p,\n%%char c = %17p,\n%%char c = %p,\n", p, &n1, p, p, p, p);
    //s21_sprintf(buffer1, "%%char c = %-9.3f,\n%n%%char c = %#-10f,\n%%char c = %#f,\n%%char c = %#015f,\n%%char c = %#.4f,\n", f, &n1, f, f, f, f);

    puts(buffer1);

    printf("\n-------------------\n\n");

    //sprintf(buffer2, "%%char c = %+-5c,\n%n%%char c = %-5c,\n%%char c = %0c,\n", c, &n2, c, c);
    //sprintf(buffer2, "%%char c = %+5.2s,\n%n%%char c = %-5.2s,\n%%char c = %04.2s,\n", s, &n2, s, s);
    //sprintf(buffer2, "%%char c = %+5d,\n%n%%char c = %-5d,\n%%char c = % 4d,\n", d, &n2, d, d);
    //sprintf(buffer2, "%%char c = %20p,\n%n%%char c = %-21p,\n%%char c = %p,\n%%char c = %17p,\n%%char c = %p,\n", p, &n2, p, p, p, p);
    //sprintf(buffer2, "%%char c = %-8.6x,\n%n%%char c = %#-6X,\n%%char c = %x,\n%%char c = %#07x,\n", x, &n2, x, x, x);
    //sprintf(buffer2, "%%char c = %-9.3f,\n%n%%char c = %#-10f,\n%%char c = %#f,\n%%char c = %#015f,\n%%char c = %#.4f,\n", f, &n2, f, f, f, f);

    sprintf(buffer2, "%%double f = %%%+.4f,\nint d = %+.6d,\nint i = %.5i,\ncount n = %n,\ndouble e = %e,\ndouble g = %.4g,\nstring s = %.4s,\nchar c = %c,\nint u = %.1u,\nint x = %.6x,\noctal o = %o,\ndouble g = %g,\npointer p = %p,\n", f, d, i, &n2, e, e, s, c, u, x, o, g, p);


    puts(buffer2);

    printf("%d %d\n", n1, n2);


    return 0;
}

int s21_sprintf(char *str, const char *format, ...){
    
    form params = {0};
    va_list args;
    va_start(args, format);

    int i = 0;
    while(*format){
        if(*format == '%' && *(format + 1) != '%'){
            format++;
            params.flags = parser_flags(&format);
            params.width = is_digit(*format) ? convert_str_to_int(&format) : 0;


            if(*format == '.'){
                params.accurasy = is_digit(*++format) ? convert_str_to_int(&format) : 0;
            }
            else{
                params.accurasy = -1;
            }

            params.length = parser_length(*format);
            if(params.length != no_length){
                format++;
            }


            if(strchr("cdiufsxXopgGeEn", *format)){
                if(params.accurasy == -1){
                    params.accurasy = default_accurasy(*format);
                }

                char *buffer = pull_string(str, args, params, *format);
                add_string(str, buffer, &i);

                free(buffer);
            }
        }
        else{
            str[i++] = *format;
            
            if(*format == '%'){
                format++;
            }
        }
        format++;
    }

    str[i] = '\0';
    va_end(args);

    return s21_strlen(str);
}

int parser_flags(const char **flag_form){
    int flags = 0;
    while(1){
        switch(*(*flag_form)){
            case '+':
                flags |= 1;
                break;
            case '-':
                flags |= 1 << 1;
                break;
            case '#':
                flags |= 1 << 2;
                break;
            case '0':
                flags |= 1 << 3;
                break;
            case ' ':
                flags |= 1 << 4;
                break;
            default:
                return flags;
        }
        (*flag_form)++;
    }
}

Length parser_length(char len){

    Length length = no_length;
    switch(len){
        case 'h':
            length = short_int;
            break;
        case 'l':
            length = long_int;
            break;
        case 'L':
            length = long_double;
            break; 
    }

    return length;
}


int default_accurasy(char symb){
    int accurasy;
    if(s21_strchr("eEfgG", symb)){
        accurasy = 6;
    }
    else if(symb == 's'){
        accurasy = 0;
    }
    else{
        accurasy = 1;
    }

    return accurasy;
}

char *pull_string(char *str, va_list input, form params, char specifier){

    char *buffer = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    
    switch(specifier){
        case 'c':    
            char_to_str(input, &buffer, params);
            break;
        case 'd':
        case 'i':
        case 'u':
            convert_int_to_str(input, &buffer, params);
            break;
        case 'f':
            convert_double_to_str(input, &buffer, params);
            break;
        case 's':
            str_to_str(&buffer, input, params);      
            break;
        case 'x':
        case 'X':
            convert_int_to_hex(input, &buffer, params, specifier);
            break;
        case 'o':
            convert_int_to_oct(input, &buffer, params);
            break;
        case 'p':
            convert_ptr_to_str(input, &buffer, params);
            break;
        case 'g':
        case 'G':   
            convert_double_to_g(input, buffer, params, specifier);
            break;
        case 'e':
        case 'E':
            convert_double_to_e(input, buffer, params, specifier);
            break;
        case 'n':
            int *n = va_arg(input, int*);
            *n = strlen(str);   
            break;
    }

   //или не здесь или нужно в каждой функции, которая работает с числами, возвращать число и его сравнивать 
    if(!s21_strchr("csnp", specifier)){
        //double digit = va_arg(input, double);
        printf("%f\n", digit);
        if(digit < 0){
            buffer = s21_insert(buffer, "-", 0);
        }
        else if(params.flags & (1)){
            buffer = s21_insert(buffer, "+", 0);
        }
    }


    return buffer;
}

//Готовый спецификатор %с
void char_to_str(va_list input, char **buffer, form params){
    char c = va_arg(input, int);
    (*buffer)[0] = c;

    char *width_buff = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    for(int i = 1; i < params.width; i++){
        width_buff[i - 1] = ' ';
    }

    if(params.flags & (1 << 1)){
        *buffer = s21_insert(*buffer, width_buff, 1);
    }
    else if(params.flags & (1)){
        *buffer = s21_insert(*buffer, width_buff, 0);
    }

    free(width_buff);
}
//Готовый спецификатор %s
void str_to_str(char **buffer, va_list input, form params){

    char *str = va_arg(input, char*);
    s21_strcpy(*buffer, str);
    if(params.accurasy != 0){
        (*buffer)[params.accurasy] = '\0';
    }

    int empty_width = !params.accurasy ? params.width - strlen(str) : params.width - params.accurasy;

    char *width_buff = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    for(int i = 0; i < empty_width; i++){
        if(params.flags & (1 << 1) || params.flags & (1)){
            width_buff[i] = ' ';
        }
        else if(params.flags & (1 << 3)){
            width_buff[i] = '0';
        }
    }

    if(params.flags & (1 << 1)){ //-
        *buffer = s21_insert(*buffer, width_buff, empty_width - 1);
    }
    else if(params.flags & (1) || params.flags & (1 << 3)){ //+ или 0
        *buffer = s21_insert(*buffer, width_buff, 0);
    }
    free(width_buff);
}

int convert_str_to_int(const char **format){

    int digit = *(*format)++ - '0';
    while(is_digit(*(*format))){
        digit *= 10;
        digit += *(*format)++ - '0';
    }
    
    return digit;
}

int is_digit(char symb){
    return symb >= '0' && symb <= '9' ? 1 : 0;
}

void add_string(char *str, char *buffer, int *index){
    s21_strcpy(str + *index, buffer);
    *index += s21_strlen(buffer);
}
//Полностью готовый %p
void convert_ptr_to_str(va_list input, char **buffer, form params){

    unsigned long long int n = (unsigned long long int)va_arg(input, void*);

    int j = 0;                   
    while(n){
        int digit = (n % 16);
        if(digit < 10){
            (*buffer)[j++] = digit + '0';
        }
        else{
            (*buffer)[j++] = digit - 10 + 'a';
        }
        
        n /= 16;
    }
    (*buffer)[j++] = 'x';
    (*buffer)[j++] = '0';

    reverse_str(*buffer, j);

    (*buffer)[j] = '\0';
    
    char *width_buff = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    for(int i = 0; i < params.width - 14; i++){
        width_buff[i] = ' ';
    }

    if(params.flags & (1 << 1)){ //-
        *buffer = s21_insert(*buffer, width_buff, 14);
    }
    else{
        *buffer = s21_insert(*buffer, width_buff, 0);
    }

    free(width_buff);
}

//Вроде как полностью готовый %o
void convert_int_to_oct(va_list input, char **buffer, form params){
//флаг 0 игнорируется с точностью
    unsigned int n = va_arg(input, unsigned int);

    int j = 0;
    int size = 0;
    while(n){
        (*buffer)[j++] = n % 8 + '0';
        n /= 8;
        size++;
    }
                           
    reverse_str(*buffer, j);

    (*buffer)[j] = '\0';

    int width = params.accurasy != 1 ? params.accurasy - size : params.width - size;

    int left = 0;
    char *accurasy_buff = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    for(int i = 0; i < width; i++){
        if(params.flags & (1 << 3) || params.accurasy != 1){ //0 или точность
            accurasy_buff[i] = '0';
            left++;
        }
    }

    int key = params.accurasy != 1 ? params.width - params.accurasy : 0;
    char *width_buff = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    for(int i = 0; i < width - left + key; i++){
        if(params.flags & (1 << 1)){ //-
            width_buff[i] = ' ';
        }
    }
    
    if(params.flags & (1 << 3) || params.accurasy != 1){ //0 или точность
        *buffer = s21_insert(*buffer, accurasy_buff, 0);
    }
    if(params.flags & (1 << 1)){ //-
        *buffer = s21_insert(*buffer, width_buff, size + left);
    }
    
    free(accurasy_buff);
    free(width_buff);

    if(params.flags & (1 << 2) && left == 0 && params.accurasy <= size){
        *buffer = s21_insert(*buffer, "0", 0);
    }
}

//Вроде как полностью готовый %x
void convert_int_to_hex(va_list input, char **buffer, form params, char specifier){
    
    unsigned int n = va_arg(input, unsigned int);

    int j = 0;
    int size = 0;
    while(n){
        int digit = (n % 16);
        if(digit < 10){
            (*buffer)[j++] = digit + '0';
        }
        else{
            if(specifier == 'X'){
                (*buffer)[j++] = digit - 10 + 'A';
            }
            else{
                (*buffer)[j++] = digit - 10 + 'a';
            }
        }
        
        n /= 16;
        size++;
    } 
    
    reverse_str(*buffer, j);
        
    (*buffer)[j] = '\0';

    int width = params.accurasy != 1 ? params.accurasy - size : params.width - size;

    int left = 0;
    char *accurasy_buff = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    for(int i = 0; i < width; i++){
        if(params.flags & (1 << 3) || params.accurasy != 1){ //0 или точность
            accurasy_buff[i] = '0';
            left++;
        }
    }

    int key = params.accurasy != 1 ? params.width - params.accurasy : 0;
    char *width_buff = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    for(int i = 0; i < width - left + key; i++){
        if(params.flags & (1 << 1)){ //-
            width_buff[i] = ' ';
        }
    }
    
    if(params.flags & (1 << 3) || params.accurasy != 1){ //0 или точность
        *buffer = s21_insert(*buffer, accurasy_buff, 0);
    }
    if(params.flags & (1 << 1)){ //-
        *buffer = s21_insert(*buffer, width_buff, size + left);
    }

    free(accurasy_buff);
    free(width_buff);

    if(params.flags & (1 << 2)){
        char hash[2] = {'\0'};
        hash[0] = '0';
        hash[1] = specifier;
        *buffer = s21_insert(*buffer, hash, 0);
    }
}

void convert_double_to_g(va_list input, char *buffer, form params, char specifier){

    double n = va_arg(input, double);

    if(fabs(n) >= 1e6){
        convert_double_to_e(input, buffer, params, specifier);
    }
    else{  
        convert_double_to_str(input, &buffer, params);
    }
}

void convert_double_to_e(va_list input, char *buffer, form params, char specifier){

    double n = va_arg(input, double);

    char flag;

    bool positive = true;
    if(n < 0){
        positive = false;
        n *= -1;
    }

    int prime = 0;
    int j = 0;
    int int_part = (int)n;
    if(int_part == 0){
        while(n < 1){
            n *= 10;
            prime++;
        }
        buffer[j++] = (int)n + '0';
        buffer[j++] = '.';
    }
    else{
        while(int_part){
            buffer[j++] = int_part % 10 + '0';
            int_part /= 10;
            prime++;
        }
        prime--; 

        buffer[j++] = buffer[j - 1];
        buffer[j - 2] = '.';

        reverse_str(buffer, j);
    }
    double frac_part = n - (int)n;
    for(int i = 0; i < params.accurasy - prime; i++){
        frac_part *= 10;
        int digit = (int)frac_part;
        buffer[j++] = digit + '0';
        frac_part -= digit;
    }
    
    buffer[j++] = 'e';
    if(specifier == 'E'){
        buffer[j] = 'E';
    }
    //buffer[j++] = params.flags & (1) || params.flags;
    if(prime >= 10){
        //convert_int_to_str((va_list)prime, &buffer, params);
        j += 2;
    }
    else{
        buffer[j++] = '0';
        buffer[j++] = prime + '0';
    }
    buffer[j] = '\0';
}

void convert_double_to_str(va_list input, char **buffer, form params){

    double n = va_arg(input, double);
    //printf("%f\n", n);
    int n_int = (int)n;
    double n_fract = n - n_int;

    int i = 0;
    for(; i < params.accurasy + 1; i++){
        n_fract *= 10;
    } 

    n_int = (int)n_fract;
    while(n_int % 10 == 0 && i != 0){
        n_int /= 10;
        i--;
    }

    if(n_int % 10 >= 5){
        n_int = n_int / 10 + 1;
    }
    else{
        n_int /= 10;
    }

    int j = 0;
    int k = 0;
    while(k < params.accurasy){
        (*buffer)[j++] = n_int % 10 + '0';
        n_int /= 10;
        k++;
    }

    (*buffer)[j++] = '.';
    n_int = (int)n;               

    if(n_int != 0){
        while(n_int){
            (*buffer)[j++] = n_int % 10 + '0';
            n_int /= 10;
        }
    }
    else{
        (*buffer)[j++] = 0 + '0';
    }

    reverse_str(*buffer, j);
    (*buffer)[j] = '\0';


}
//Если не обращать внимание на детали то %d %u %i полностью работают
void convert_int_to_str(va_list input, char **buffer, form params){

    int n = va_arg(input, int);

    bool positive = true;
    if(n < 0){
        positive = false;
        n *= -1;
    }

    int j = 0;
    int count = 0;
    int size = 0;
    while(n){
        (*buffer)[j++] = n % 10 + '0';
        n /= 10;
        count++;
        size++;
    }

    while(params.accurasy - count > 0){
        (*buffer)[j++] = '0';
        count++;
    }
    
    reverse_str(*buffer, j);

    (*buffer)[j] = '\0';
    
    char *width_buff = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    for(int i = 0; i < params.width - size; i++){
        if(params.flags & (1 << 1) || params.flags & (1)){
            width_buff[i] = ' ';
        }
        else if(params.flags & (1 << 3)){
            width_buff[i] = '0';
        }
    }

    if(params.flags & (1 << 1)){ //-
        *buffer = s21_insert(*buffer, width_buff, size);
    }
    else if(params.flags & (1) || params.flags & (1 << 3)){ //+ или 0
        *buffer = s21_insert(*buffer, width_buff, 0); 
    }
    else if(params.flags & (1 << 4)){ //' '
        *buffer = s21_insert(*buffer, " ", 0);
    }

    free(width_buff);
}

void reverse_str(char *buffer, int j){

    for(int i = 0; i < j / 2; i++){
        char temp = buffer[i];
        buffer[i] = buffer[j - i - 1];
        buffer[j - i - 1] = temp;
    }
}
