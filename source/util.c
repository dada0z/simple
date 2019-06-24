#include "util.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
char *substring(char *begin, char *end) {
  size_t length = end - begin + 1;
  char *str = (char *)malloc(length);
  memset(str, 0, length);
  strncpy(str, begin, length - 1);
  return str;
}