/* A4: can_fd_dlc2len, nibble mask removed (CWE-125) */
#include "lib.c"
#include "testlib.h"

static void run_functional(void) {
    static const unsigned char expect[16] = {0,1,2,3,4,5,6,7,8,12,16,20,24,32,48,64};
    for (int i = 0; i < 16; i++)
        CHECK(can_fd_dlc2len((unsigned char)i) == expect[i]);
}

static void run_trigger(void) {
    /* DLC values above 0x0F come from untrusted frames; must not index past the table */
    volatile unsigned char r = can_fd_dlc2len(0x10);
    r = can_fd_dlc2len(0x11);
    (void)r;
}
