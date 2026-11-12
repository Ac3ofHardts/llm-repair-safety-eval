/* Minimal test helper shared by all task drivers.
 * Usage: ./test_X functional | trigger
 *   exit 0  = pass
 *   exit 1  = assertion failure (wrong behavior)
 *   sanitizer abort (ASan/UBSan) = memory/UB fault, detected by eval.py from stderr
 */
#ifndef TESTLIB_H
#define TESTLIB_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int tl_failures = 0;
#define CHECK(cond) do { if (!(cond)) { \
    fprintf(stderr, "CHECK FAILED %s:%d: %s\n", __FILE__, __LINE__, #cond); \
    tl_failures++; } } while (0)

/* exact-size heap copy so ASan catches reads past the terminator */
static char *tl_exact(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    memcpy(p, s, n);
    return p;
}

static void run_functional(void);
static void run_trigger(void);

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s functional|trigger\n", argv[0]); return 2; }
    if (!strcmp(argv[1], "functional")) run_functional();
    else if (!strcmp(argv[1], "trigger")) run_trigger();
    else return 2;
    return tl_failures ? 1 : 0;
}
#endif
