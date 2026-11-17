static int decodeHexMessage(struct client *c, char *hex) {
    int l = strlen(hex);
    unsigned char msg[MODES_LONG_MSG_BYTES];
    struct modesMessage mm;
    static struct modesMessage zeroMessage;

    MODES_NOTUSED(c);
    mm = zeroMessage;

    mm.remote = 1;
    mm.signalLevel = 0.0;

    while (l > 0 && isspace(hex[l - 1]))
        hex[--l] = '\0';

    while (l > 0 && isspace(*hex)) {
        hex++;
        l--;
    }

    if (hex[l - 1] != ';')
        return 0;

    switch (hex[0]) {
        case '<':
            mm.signalLevel = ((hexDigitVal(hex[13]) << 4) | hexDigitVal(hex[14])) / 255.0;
            mm.signalLevel = mm.signalLevel * mm.signalLevel;
            hex += 15;
            l -= 16;
            break;
        case '@':
        case '%':
            hex += 13;
            l -= 14;
            break;
        case '*':
        case ':':
            hex++;
            l -= 2;
            break;
        default:
            return 0;
    }

    if (l != (MODEAC_MSG_BYTES * 2) && l != (MODES_SHORT_MSG_BYTES * 2) && l != (MODES_LONG_MSG_BYTES * 2))
        return 0;

    if (Modes.mode_ac == 0 && l == (MODEAC_MSG_BYTES * 2))
        return 0;

    for (int j = 0; j < l; j += 2) {
        int high = hexDigitVal(hex[j]);
        int low = hexDigitVal(hex[j + 1]);

        if (high == -1 || low == -1)
            return 0;

        msg[j / 2] = (high << 4) | low;
    }

    mm.sysTimestampMsg = mstime();

    if (l == (MODEAC_MSG_BYTES * 2)) {
        Modes.stats_current.remote_received_modeac++;
        decodeModeAMessage(&mm, ((msg[0] << 8) | msg[1]));
    } else {
        int result = decodeModesMessage(&mm, msg);
        if (result < 0) {
            if (result == -1)
                Modes.stats_current.remote_rejected_unknown_icao++;
            else
                Modes.stats_current.remote_rejected_bad++;
            return 0;
        } else {
            Modes.stats_current.remote_accepted[mm.correctedbits]++;
        }
    }

    useModesMessage(&mm);
    return 0;
}
