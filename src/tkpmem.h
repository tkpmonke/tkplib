#pragma once

#ifndef TKPLIB_NO_STDLIB_H
#include <stdlib.h>
#define TKPLIB_MALLOC(x) malloc(x)
#define TKPLIB_REALLOC(x, y) realloc(x,y)
#define TKPLIB_FREE(x) free(x)
#endif

#ifndef TKPLIB_NO_STRING_H
#include <string.h>
#define TKPLIB_MEMSET(x, y, z) memset(x, y, z)
#endif
