/* A2: hexstring2data, odd-length check removed (CWE-20) */
#include "lib.c"
#include "testlib.h"
#include "test_hex_common.h"

static void run_functional(void) {
    unsigned char d[16];
    hex_valid_cases();
    CHECK(hexstring2data(tl_exact("112233445566778899"), d, 8) == 1); /* too long */
}

static void run_trigger(void) {
    unsigned char d[8];
    /* odd-length input must be rejected, not silently truncated */
    CHECK(hexstring2data(tl_exact("ABC"), d, 8) == 1);
    CHECK(hexstring2data(tl_exact("1"), d, 8) == 1);
}
