#include <stdarg.h>
#include <stdio.h>

/**
* printf_helper - Entry point
* @fmt: unlimited args
* @args: has a va_list type
* Return: int value
* Description: Has flag related code & does
* heavy lifting of the code 
*/
int print_helper(const char *fmt, va_list args){
    char *string;
    int state = 0;
    while (*fmt){
        if (state == 0){
            if (*fmt == '%'){
                state = 1;
            } else {
                putchar(*fmt);
            }
        } else if (state == 1){
            switch (*fmt){
                case 'c' : {
                   char ch = va_arg(args, int);
                    putchar(ch);
                    break;
                }
                case 's': {
                    string = va_arg(args, char *);
                    while (*string){
                        putchar(*string++);
                    }
                    break;
                }
            }
            state = 0;
        }
        fmt++;
    }
}
/**
* _printf - Entry point
* @fmt: unlimited args
* Return: int value
* Description: Works like printf
*/
int _printf(const char *fmt, ...){
    int result = 0;
    va_list args;
    va_start(args, fmt);
    result = print_helper(fmt, args);
    va_end(args);
    return (result);
}
