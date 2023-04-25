#include <stdio.h>
#include <stdarg.h>

int _printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    int count = 0;
    char c;
    int d;
    double f;
    char *s;

    while ((c = *(format++)) != '\0') {
        if (c == '%') {
            c = *(format++);
            switch (c) {
                case 'd':
                    d = va_arg(args, int);
                    count += printf("%d", d);
                    break;
                case 'f':
                    f = va_arg(args, double);
                    count += printf("%f", f);
                    break;
                case 'c':
                    d = va_arg(args, int);
                    count += printf("%c", d);
                    break;
                case 's':
                    s = va_arg(args, char *);
                    count += printf("%s", s);
                    break;
                case '%':
                    count += putchar('%');
                    break;
                default:
                    count += putchar('%');
                    count += putchar(c);
                    break;
            }
        } else {
            count += putchar(c);
        }
    }

    va_end(args);
    return count;
}
