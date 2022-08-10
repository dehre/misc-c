#pragma once

#ifdef __linux__
#include <stdlib.h>
#include <time.h>
#define Sranddev() srand((unsigned int)time(NULL))
#elif __APPLE__
#include <stdlib.h>
#define Sranddev() sranddev()
#endif
