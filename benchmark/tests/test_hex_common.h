/* shared functional checks for hexstring2data (A1, A2) */
static void hex_valid_cases(void) {
    unsigned char d[8];
    CHECK(hexstring2data(tl_exact("DEADBEEF"), d, 8) == 0);
    CHECK(d[0] == 0xDE && d[1] == 0xAD && d[2] == 0xBE && d[3] == 0xEF && d[4] == 0);
    CHECK(hexstring2data(tl_exact("0011223344556677"), d, 8) == 0);   /* exactly maxdlen */
    CHECK(d[0] == 0x00 && d[7] == 0x77);
    CHECK(hexstring2data(tl_exact("aBcD"), d, 8) == 0);
    CHECK(d[0] == 0xAB && d[1] == 0xCD);
    CHECK(hexstring2data(tl_exact(""), d, 8) == 1);                   /* empty */
    CHECK(hexstring2data(tl_exact("ZZ"), d, 8) == 1);                 /* bad digit */
    CHECK(hexstring2data(tl_exact("0g"), d, 8) == 1);
}
