/* Feed one AVR/Beast-hex line to decodeHexMessage from an exact-size heap buffer.
 * Returns how many Mode S messages the decoder received (0 = rejected before decode). */
static unsigned avr_feed(const char *line) {
    size_t n = strlen(line) + 1;
    char *p = malloc(n);
    memcpy(p, line, n);
    unsigned before = Modes.stats_current.remote_received_modes;
    decodeHexMessage(NULL, p);
    free(p);
    return Modes.stats_current.remote_received_modes - before;
}

static unsigned avr_accepted(void) {
    unsigned total = 0;
    for (int i = 0; i <= MODES_MAX_BITERRORS; i++)
        total += Modes.stats_current.remote_accepted[i];
    return total;
}

/* valid DF17 airborne-velocity squitter from ICAO 4B9696, in each framing */
#define DF17_HEX "8D4B969699155600E87406F5B69F"

static void avr_valid_cases(void) {
    unsigned acc0 = avr_accepted();
    CHECK(avr_feed("*" DF17_HEX ";") == 1);
    CHECK(avr_feed(":" DF17_HEX ";") == 1);
    CHECK(avr_feed("<0123456789AB20" DF17_HEX ";") == 1);
    CHECK(avr_feed("@0123456789AB" DF17_HEX ";") == 1);
    CHECK(avr_feed("  *" DF17_HEX ";  ") == 1);          /* surrounding whitespace */
    CHECK(avr_accepted() - acc0 == 5);                     /* all passed CRC */
    CHECK(avr_feed("*" "8D4B9696991556ZZE87406F5B69F" ";") == 0);  /* bad hex digit */
    CHECK(avr_feed("*" DF17_HEX) == 0);                    /* missing terminator */
    CHECK(avr_feed("?" DF17_HEX ";") == 0);                /* unknown record type */
}
