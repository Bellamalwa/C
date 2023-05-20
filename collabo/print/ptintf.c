#include <stdarg.h>
#include <stdio.h>

void print_helper(const char *fmt, va_list args){
    int state = 0;
    while (*fmt){
        if (state = 0){
            if (*fmt == '%'){
                state = 1;
            } else {
                putchar(*fmt);
            }
        } else if (state == 1){
            switch(*fmt){
                
j            }
        }
    }
}




 make -s
./main.c:5:15: error: expected expression
    char ch = '';
              ^
1 error generated.
make: *** [Makefile:10: main] Error 1
exit status 2
 



 make -s
 ./main
 