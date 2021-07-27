/* date = July 27th 2021 3:29 pm */

#ifndef SDF_assert

#include <stdio.h>
#include <stdlib.h>
#define SDF_assert(x) if (!(x)) { \
printf("Assert Failed on line %d, on file %s\n", __LINE__, __FILE__); \
exit(-42); }
#endif //SDF_ASSERT_H
