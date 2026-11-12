/* V3: decodeHexMessage, AVR record header read without length checks (CWE-125)
 * historical: pre-fix logic from upstream dump1090 65cd5fe (2021-12-09) */
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
    /* truncated '<' records: signal-level digits are at [13..14] and must not be read */
    CHECK(avr_feed("<;") == 0);
    CHECK(avr_feed("<0123;") == 0);
    CHECK(avr_feed("<0123456789;") == 0);
}
