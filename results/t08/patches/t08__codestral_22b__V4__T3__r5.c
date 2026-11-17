static int decodeHexMessage(struct client *c, char *hex) {
    int l = strlen(hex);
    unsigned char msg[MODES_LONG_MSG_BYTES] = {0}; // Initialize msg to avoid uninitialized variable warning
    struct modesMessage mm;
    static struct modesMessage zeroMessage = {0};

    MODES_NOTUSED(c);
    mm = zeroMessage;

    mm.remote = 1;
    mm.signalLevel = 0;

    // Remove spaces on the left and on the right
    while(l && isspace(hex[l-1])) {
        hex[l-1] = '\0'; l--;
    }
    while(isspace(*hex)) {
        hex++; l--;
    }

    if (hex[l-1] != ';') {
        return 0; // Not complete - abort
    }

    switch (hex[0]) {
        case '<':
            if (l < 18) {
                return 0; // Truncated
            }
            if (!timestampFromHex(hex + 1, &mm.timestampMsg)) {
                return 0; // Malformed timestamp
            }
            if (!signalFromHex(hex + 13, &mm.signalLevel)) {
                return 0; // Malformed signal level
            }
            hex += 15;
            l -= 16;
            break;

        case '@':
        case '%':
            if (l < 16) {
                return 0; // Truncated
            }
            if (!timestampFromHex(hex + 1, &mm.timestampMsg)) {
                return 0; // Malformed timestamp
            }
            hex += 13;
            l -= 14;
            break;

        case '*':
        case ':':
            if (l < 4) {
                return 0; // Truncated
            }
            hex++;
            l -= 2;
            break;

        default:
            return 0;
    }

    if (!Modes.mode_ac && l == (MODEAC_MSG_BYTES * 2)) {
        return 0; // Right length for ModeA/C, but not enabled
    }

    for (int j = 0; j < l; j += 2) {
        int high = hexDigitVal(hex[j]);
        int low = hexDigitVal(hex[j+1]);

        if (high == -1 || low == -1) {
            return 0;
        }
        msg[j/2] = (high << 4) | low;
    }

    mm.sysTimestampMsg = mstime();

    if (l == (MODEAC_MSG_BYTES * 2)) {  // ModeA or ModeC
        Modes.stats_current.remote_received_modeac++;
        decodeModeAMessage(&mm, ((msg[0] << 8) | msg[1]));
    } else {       // Assume ModeS
        int result = decodeModesMessage(&mm, msg);
        if (result < 0) {
            if (result == -1) {
                Modes.stats_current.remote_rejected_unknown_icao++;
            } else {
                Modes.stats_current.remote_rejected_bad++;
            }
            return 0;
        } else {
            Modes.stats_current.remote_accepted[mm.correctedbits]++;
        }
    }

    useModesMessage(&mm);
    return 0;
}
