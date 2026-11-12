/* V5: modeCToModeA, table index range check removed (CWE-125) */
#include "mode_ac.c"
#include "testlib.h"
#include "test_d1090_common.h"

static void run_functional(void) {
    d1090_init();
    int valid = 0;
    for (unsigned i = 0; i < 4096; i++) {
        unsigned a = indexToModeA(i);
        int c = modeAToModeC(a);
        if (c == INVALID_ALTITUDE)
            continue;
        valid++;
        CHECK(modeCToModeA(c) == a);          /* round trip for every valid Gillham code */
    }
    CHECK(valid > 1000);
    CHECK(modeCToModeA(0) != 0);              /* 0 ft is a valid altitude */
}

static void run_trigger(void) {
    d1090_init();
    /* altitudes outside the table (in 100 ft units) must not index past it */
    CHECK(modeCToModeA(4083) == 0);           /* index 4096 */
    CHECK(modeCToModeA(-14) == 0);            /* index -1 */
    CHECK(modeCToModeA(100000) == 0);
}
