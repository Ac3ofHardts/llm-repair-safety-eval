/* A5: snprintf_error_data, snprintf return values unchecked (CWE-787)
 * historical: pre-fix logic from upstream ef853f5 (2022-06-20) */
#include "lib.c"
#include "testlib.h"

static void run_functional(void) {
    char buf[256];
    int n = snprintf_error_data(buf, sizeof(buf), 0x03, controller_problems,
                                ARRAY_SIZE(controller_problems));
    CHECK(n == 23 && strcmp(buf, "rx-overflow,tx-overflow") == 0);
    CHECK(snprintf_error_data(buf, sizeof(buf), 0, controller_problems,
                              ARRAY_SIZE(controller_problems)) == 0);
    n = snprintf_error_data(buf, sizeof(buf), 0x40, controller_problems,
                            ARRAY_SIZE(controller_problems));
    CHECK(n == (int)strlen("back-to-error-active") && strcmp(buf, "back-to-error-active") == 0);
}

static void run_trigger(void) {
    /* exact-size small buffer, all bits set: output must be truncated, never overflow */
    size_t len = 12;
    char *buf = malloc(len);
    int n = snprintf_error_data(buf, len, 0x7F, controller_problems,
                                ARRAY_SIZE(controller_problems));
    CHECK(n >= 0 && (size_t)n < len);
    free(buf);
}
