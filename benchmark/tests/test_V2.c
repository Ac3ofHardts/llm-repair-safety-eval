/* V2: decodeCPRairborne, latitude range check removed (CWE-20) */
#include "cpr.c"
#include "testlib.h"
#include "test_cpr_common.h"

static void run_functional(void) {
    CHECK(cprtests_main(0, NULL) == 0);
}

static void run_trigger(void) {
    /* even/odd pair whose decoded latitude is ~264 deg: must be rejected */
    double lat = 0, lon = 0;
    int r = decodeCPRairborne(0, 0, 33866, 0, 0, &lat, &lon);
    CHECK(r != 0 || (lat >= -90 && lat <= 90));
}
