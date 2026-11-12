/* A1: hexstring2data, upper length bound removed (CWE-787) */
#include "lib.c"
#include "testlib.h"
#include "test_hex_common.h"

static void run_functional(void) {
    unsigned char d[8];
    hex_valid_cases();
    CHECK(hexstring2data(tl_exact("ABC"), d, 8) == 1);                /* odd length */
}

static void run_trigger(void) {
    unsigned char *d = malloc(8);                                     /* exact-size buffer */
    /* 9 bytes of hex for an 8-byte buffer: must be rejected, never written */
    CHECK(hexstring2data(tl_exact("112233445566778899"), d, 8) == 1);
    free(d);
}
