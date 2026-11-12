/* V4: decodeHexMessage, message-length allowlist removed (CWE-787 stack overflow) */
#include "net_io.c"
#include "testlib.h"
#include "test_d1090_common.h"
#include "test_avr_common.h"

static void run_functional(void) {
    d1090_init();
    avr_valid_cases();
}

static void run_trigger(void) {
    d1090_init();
    /* 30-byte payload for a 14-byte message buffer: must be rejected, never decoded */
    CHECK(avr_feed("*" DF17_HEX "00112233445566778899AABBCCDDEEFF;") == 0);
    CHECK(avr_feed("*8D4B96;") == 0);                      /* 3 bytes: not a Mode S length */
}
