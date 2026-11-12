/* Reuse the upstream dump1090 CPR test suite as the functional test. */
#define main cprtests_main
#include "cprtests.c"
#undef main
