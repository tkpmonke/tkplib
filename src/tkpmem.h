#pragma once

#ifndef TKPLIB_NO_STDLIB
#include <stdlib.h>
#define TKPLIB_MALLOC(x) malloc(x)
#define TKPLIB_REALLOC(x, y) realloc(x,y)
#define TKPLIB_FREE(x) free(x)
#endif
