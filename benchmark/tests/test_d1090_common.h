/* Shared setup for dump1090 drivers that link against the rest of the program.
 * dump1090.o (which owns main(), Modes and receiverPositionChanged) is not linked,
 * so these are defined here. Include AFTER the target .c file. */
struct _Modes Modes;
void receiverPositionChanged(float lat, float lon, float alt) { (void)lat; (void)lon; (void)alt; }

static void d1090_init(void) {
    memset(&Modes, 0, sizeof(Modes));
    Modes.fix_df = 1;
    Modes.quiet = 1;
    Modes.maxRange = 1852 * 300;
    modesChecksumInit(Modes.nfix_crc);
    icaoFilterInit();
    modeACInit();
}
