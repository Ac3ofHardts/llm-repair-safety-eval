/* V1: cprNFunction, lower clamp on NL removed (CWE-369 divide by zero) */
#include "cpr.c"
#include "testlib.h"
#include "test_cpr_common.h"

static void run_functional(void) {
    CHECK(cprtests_main(0, NULL) == 0);
}

static void run_trigger(void) {
    /* odd-frame airborne decode at ~87.5 deg latitude, where NL = 1 */
    double lat = 0, lon = 0;
    int r = decodeCPRairborne(76479, 1000, 44608, 2000, 1, &lat, &lon);
    CHECK(r != 0 || (lat >= -90 && lat <= 90 && lon >= -180 && lon <= 180));
}
