#include "../s21_string.h"


#define MAX_BUFFER_SIZE 1024


void convert_int_to_hex(int n, char *buffer, char flag);
void convert_int_to_oct(int n, char *buffer);
void convert_double_to_e(double n, char *bufferm, char flag);
void convert_int_to_str(int n, char *buffer);
void convert_double_to_str(double n, char *buffer);
void convert_ptr_to_str(unsigned long long int spec_p, char *buffer);
void convert_double_to_g(double n, char *buffer, char flag);

void reverse_str(char *buffer, int j);




int main(void){

    char buffer1[MAX_BUFFER_SIZE];
    char buffer2[MAX_BUFFER_SIZE];

    char c = 'W';
    int d = 123;
    int i = -134;
    double f = 12.985;
    char *s = "Hello World";
    unsigned int u = 1265;
    double g = 0.00123344;
    double e = 12454.3344;
    unsigned int x = 986;
    unsigned int o = 759;
    int n1;
    int n2;
    void *p;

    s21_sprintf(buffer1, "%%double = %%%f,\nint = %i,\ncount = %n,\ndouble e = %e,\ndouble g = %g,\nstring = %s,\nchar = %c,\nint = %u,\nint = %x,\noctal = %o,\ndouble g = %g,\npointer p = %p,\n", f, i, &n1, 0.00234, e * 1000, s, c, u, x, o, g, p);
    
    puts(buffer1);

    printf("\n\n");


    sprintf(buffer2, "%%double = %%%f,\nint = %i,\ncount = %n,\ndouble e = %e,\ndouble g = %g,\nstring = %s,\nchar = %c,\nint = %u,\nint = %x,\noctal = %o,\ndouble g = %g,\npointer p = %p,\n", f, i, &n2, 0.00234, e * 1000, s, c, u, x, o, g, p);

    puts(buffer2);

    printf("%d %d\n", n1, n2);


    return 0;
}



int s21_sprintf(char *str, const char *format, ...){
   
    va_list args;
    va_start(args, format);

    int i = 0;
    int result = 0;
    char flag;

    while(*format){
        if(*format == '%' && *(format + 1) != '%'){
            format++;
            char specifier = *format++;
         
            switch(specifier){
                case 'c':    
                    char c = (char)va_arg(args, int);

                    str[i++] = c;
                    result++;
                    
                    break;
                case 'd':
                case 'i':
                    
                    int n = va_arg(args, int);

                    char buffer_i[MAX_BUFFER_SIZE];
    
                    convert_int_to_str(n, buffer_i);

                    strcpy(str + i, buffer_i);
                    i += strlen(buffer_i);
                    result += strlen(buffer_i);
                    
                    break;
                case 'f':
                    
                    double spec_f = va_arg(args, double);

                    char buffer_f[MAX_BUFFER_SIZE];

                    convert_double_to_str(spec_f, buffer_f);
                    
                    strcpy(str + i, buffer_f);
                    i += strlen(buffer_f);
                    result += strlen(buffer_f);

                    break;
                case 's':      
                    char *s = va_arg(args, char*);
                    
                    strcpy(str + i, s);
                    i += strlen(s);
                    result += strlen(s);

                    break;
                case 'u':
                    unsigned int spec_u = va_arg(args, unsigned int);
                    
                    char buffer_u[MAX_BUFFER_SIZE];

                    convert_int_to_str(spec_u, buffer_u);
                    
                    strcpy(str + i, buffer_u);
                    i += strlen(buffer_u);
                    result += strlen(buffer_u);

                    break; 
                case 'x':
                    flag = 'x';
                case 'X':
                    if(flag != 'x'){
                        flag = 'X';
                    }

                    unsigned int spec_x = va_arg(args, unsigned int);
                
                    char buffer_x[MAX_BUFFER_SIZE];

                    convert_int_to_hex(spec_x, buffer_x, flag);
                    
                    strcpy(str + i, buffer_x);
                    i += strlen(buffer_x);
                    result += strlen(buffer_x);
                    
                    break;
                case 'o':
                    
                    unsigned int spec_o = va_arg(args, unsigned int);
                    
                    char buffer_o[MAX_BUFFER_SIZE];
                    
                    convert_int_to_oct(spec_o, buffer_o);

                    strcpy(str + i, buffer_o);
                    i += strlen(buffer_o);
                    result += strlen(buffer_o);
                    
                    break;
                case 'p':
                    unsigned long long int spec_p = (unsigned long long int)va_arg(args, void*);
                        
                    char buffer[15];

                    convert_ptr_to_str(spec_p, buffer);
                    
                    strcpy(str + i, buffer);
                    i += strlen(buffer);
                    result += strlen(buffer);
                    
                    break;
                case 'g':
                    flag = 'e';
                case 'G':   
                    if(flag != 'e'){
                        flag = 'E';
                    }

                    double spec_g = va_arg(args, double);
                    char buffer_g[MAX_BUFFER_SIZE];

                    convert_double_to_g(spec_g, buffer_g, flag);

                    strcpy(str + i, buffer_g);
                    i += strlen(buffer_g);
                    result += strlen(buffer_g);

                    break;
                case 'e':
                    flag = 'e';
                case 'E':
                    if(flag != 'e'){
                        flag = 'E';
                    }

                    double spec_e = va_arg(args, double);

                    char buffer_e[MAX_BUFFER_SIZE];
                    
                    convert_double_to_e(spec_e, buffer_e, flag);
                    
                    strcpy(str + i, buffer_e);
                    i += strlen(buffer_e);
                    result += strlen(buffer_e);

                    break;
                case 'n':
                    int *pos = va_arg(args, int*);
                    *pos = result;    
                    break;
            }
        }
        else{
            str[i++] = *format;
            if(*format == '%'){
                format++;
            }
            result++;
        }
        format++;
    }
    str[i] = '\0';
    va_end(args);

    return result;
}

void convert_ptr_to_str(unsigned long long int n, char *buffer){
    int j = 0;                   
    while(n){
        unsigned long long int digit = (n % 16);
        if(digit < 10){
            buffer[j++] = digit + '0';
        }
        else{
            buffer[j++] = digit - 10 + 'a';
        }
        
        n /= 16;
    }
    buffer[j++] = 'x';
    buffer[j++] = '0';

    reverse_str(buffer, j);

    buffer[j] = '\0';
}

void convert_int_to_oct(int n, char *buffer){

    int j = 0;
    while(n){
        buffer[j++] = n % 8 + '0';
        n /= 8;
    }
                           
    reverse_str(buffer, j);

    buffer[j] = '\0';
}

void convert_int_to_hex(int n, char *buffer, char flag){
    
    int j = 0;
    while(n){
        int digit = (n % 16);
        if(digit < 10){
            buffer[j++] = digit + '0';
        }
        else{
            if(flag == 'X'){
                buffer[j++] = digit - 10 + 'A';
            }
            else{
                buffer[j++] = digit - 10 + 'a';
            }
        }
        
        n /= 16;
    }

    reverse_str(buffer, j);
        
    buffer[j] = '\0';
}

void convert_double_to_g(double n, char *buffer, char flag){

    if(fabs(n) >= 1e6){
        convert_double_to_e(n, buffer, flag);
    }
    else{
        int j = 0;
        double frac_part = n - (int)n;
        if(n >= 1){
            int whole_part = (int)n;
            while(whole_part){
                buffer[j++] = whole_part % 10 + '0';
                whole_part /= 10;
            }
                            
            reverse_str(buffer, j);
                        
            buffer[j++] = '.';
            double digit = n - (int)n;
            for(int i = 0; i < 6; i++){                           
                digit *= 10;
                buffer[j++] = (int)digit + '0';
                digit -= (int)digit;
            }
        }
        else{
            buffer[j++] = '0';
            buffer[j++] = '.';
            for(int i = 0; i < 6; i++){                           
                n *= 10;
                buffer[j++] = (int)n + '0';
                n -= (int)n;
            }
        }
        buffer[j] = '\0';
    }
}


void convert_double_to_e(double n, char *buffer, char flag){

    bool positive = true;
    if(n < 0){
        positive = false;
        n *= -1;
    }

    int sign = (n >= 1) ? 1 : -1;

    int prime = 0;
    int j = 0;
    int int_part = (int)n;
    if(int_part == 0){
        while(n < 1){
            n *= 10;
            prime++;
        }
        if(!positive){
            buffer[j++] = '-';
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

        if(!positive){
            buffer[j++] = '-';
        }
        
        reverse_str(buffer, j);
    }
    double frac_part = n - (int)n;
    for(int i = 0; i < 7 - prime; i++){
        frac_part *= 10;
        int digit = (int)frac_part;
        buffer[j++] = digit + '0';
        frac_part -= digit;
    }
    
    buffer[j++] = 'e';
    if(flag == 'E'){
        buffer[j] = 'E';
    }
    buffer[j++] = sign >= 1 ? '+' : '-';
    if(prime >= 10){
        convert_int_to_str(prime, buffer);
        j += 2;
    }
    else{
        buffer[j++] = '0';
        buffer[j++] = prime + '0';
    }
    buffer[j] = '\0';
}

void convert_double_to_str(double n, char *buffer){

    int n_int = (int)n;
    double n_fract = n - n_int;
                    
    int j = 0;
    while(n_int){
        buffer[j++] = n_int % 10 + '0';
        n_int /= 10;
    }

    reverse_str(buffer, j);

    buffer[j++] = '.';
    
    int k = 0;
    while(k < 6){
        n_fract *= 10;
        buffer[j++] = (int)n_fract + '0';
        n_fract -= (int)n_fract;
        k++;
    } 

    buffer[j] = '\0';
}

void convert_int_to_str(int n, char *buffer){

    bool positive = true;
    if(n < 0){
        positive = false;
        n *= -1;
    }

    int j = 0;
    while(n){
        buffer[j++] = n % 10 + '0';
        n /= 10;
    }

    if(!positive){
        buffer[j++] = '-';
    }
    
    reverse_str(buffer, j);

    buffer[j] = '\0';
}

void reverse_str(char *buffer, int j){

    for(int i = 0; i < j / 2; i++){
        char temp = buffer[i];
        buffer[i] = buffer[j - i - 1];
        buffer[j - i - 1] = temp;
    }
}
