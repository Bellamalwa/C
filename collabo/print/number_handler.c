#include "main.h"
#include <stdint.h>
#include <stdio.h>

void unsigned_number_to_string(uint64_t number, int base, char *buffer) {
  char buf[65]; /*Memory Space */
  int i = 0, currentNo = 0, digit = 0;
  if (number == 0) {
    *buffer++ = '0';
    *buffer = 0;
    return;
  }

  for (i = 0; i < 65; i++) {
    buf[i] = 0;
  }

  currentNo = 0;
  while (number) {
    digit = number % base;
    if (digit >= 10) {
      buf[currentNo++] = 'a' + (digit - 10);
    } else {
      /*NOTE: Has the right value but gets left out */
      buf[currentNo++] = '0' + digit;
    }
    number /= base;
  }

  for (i = currentNo - 1; i >= 0; i--) {
    *buffer++ = buf[i];
  }
  *buffer = buf[0];
  *buffer = 0;
}

void number_to_string(int64_t number, int base, char *buffer) {
  if (number < 0) {
    *buffer++ = '-';
    number = -number;
  }
  unsigned_number_to_string(number, base, buffer);
}
