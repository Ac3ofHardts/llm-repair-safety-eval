/* A3: parse_canframe, delimiter reads without length checks (CWE-125)
 * historical: reverse of upstream cbbad5e (2026-09-20) */
#include "lib.c"
#include "testlib.h"

static void run_functional(void) {
    union cfu cu;

    CHECK(parse_canframe(tl_exact("123#DEADBEEF"), &cu) == CAN_MTU);
    CHECK(cu.cc.can_id == 0x123 && cu.cc.len == 4);
    CHECK(cu.cc.data[0] == 0xDE && cu.cc.data[3] == 0xEF);

    CHECK(parse_canframe(tl_exact("12345678#11"), &cu) == CAN_MTU);
    CHECK(cu.cc.can_id == (0x12345678 | CAN_EFF_FLAG) && cu.cc.len == 1);

    CHECK(parse_canframe(tl_exact("123#11.22.33"), &cu) == CAN_MTU);
    CHECK(cu.cc.len == 3 && cu.cc.data[2] == 0x33);

    CHECK(parse_canframe(tl_exact("123#R"), &cu) == CAN_MTU);
    CHECK(cu.cc.can_id == (0x123 | CAN_RTR_FLAG) && cu.cc.len == 0);

    CHECK(parse_canframe(tl_exact("123#R5"), &cu) == CAN_MTU);
    CHECK(cu.cc.len == 5);

    CHECK(parse_canframe(tl_exact("123##1AABB"), &cu) == CANFD_MTU);
    CHECK(cu.fd.flags == (0x1 | CANFD_FDF) && cu.fd.len == 2 && cu.fd.data[1] == 0xBB);

    CHECK(parse_canframe(tl_exact("45123#81:00:12345678#11223344"), &cu) == CANXL_MTU);
    CHECK(cu.xl.len == 4 && cu.xl.af == 0x12345678 && cu.xl.data[3] == 0x44);

    CHECK(parse_canframe(tl_exact("12#"), &cu) == 0);
    CHECK(parse_canframe(tl_exact("123#GG"), &cu) == 0);
    CHECK(parse_canframe(tl_exact(""), &cu) == 0);
}

static void run_trigger(void) {
    union cfu cu;
    /* short inputs with no delimiter where the old code probed cs[5], cs[8], cs[idx+14] */
    CHECK(parse_canframe(tl_exact("1234"), &cu) == 0);
    CHECK(parse_canframe(tl_exact("12345"), &cu) == 0);
    CHECK(parse_canframe(tl_exact("1234567"), &cu) == 0);
    /* SFF id followed by short data: old XL probe read cs[idx+14] */
    parse_canframe(tl_exact("123#11"), &cu);
}
