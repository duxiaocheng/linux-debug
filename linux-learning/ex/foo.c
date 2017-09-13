#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "foo.h"

int myprintf(const char *fmt, ...)
{
   int ret = -1;
   va_list list;
   char *parg = NULL;

   // format variable arguments
   va_start(list, fmt);
   vasprintf(&parg, fmt, list);
   va_end(list);
  
   ret = printf("[libfoo] %s", parg);
   free(parg);
   return ret;
}

