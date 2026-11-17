static int decodeHexMessage(struct client *c, char *hex) {
    int l = strlen(hex), j;
    unsigned char msg[MODES_LONG_MSG_BYTES];
    struct modesMessage mm;
    static struct modesMessage zeroMessage;
    bool valid = false;

    MODES_NOTUSED(c);
    mm = zeroMessage;

    // Mark messages received over the internet as remote so that we don't try to
    // pass them off as being received by this instance when forwarding them
    mm.remote      = 1;
    mm.signalLevel = 0;

    // Remove spaces on the left and on the right
    while (l && isspace(hex[l - 1])) {
        hex[l - 1] = '\0';
        l--;
    }
    while (isspace(*hex)) {
        hex++;
        l--;
    }

    // Check if the message is complete
    if (hex[l - 1] != ';') {
        return 0;
    }

    // Process different message formats
    switch (hex[0]) {
        case '<':
            if (l < 18) {
                return 0;
            }
            if (!timestampFromHex(hex + 1, &mm.timestampMsg)) {
                return 0;
            }
            if (!signalFromHex(hex + 13, &mm.signalLevel)) {
                return 0;
            }
            hex += 15;
            l -= 16;
            valid = true;
            break;

        case '@':
        case '%':
            if (l < 16) {
                return 0;
            }
            if (!timestampFromHex(hex + 1, &mm.timestampMsg)) {
                return 0;
            }
            hex += 13;
            l -= 14;
            valid = true;
            break;

        case '*':
        case ':':
            if (l < 4) {
                return 0;
            }
            hex++;
            l -= 2;
            valid = true;
            break;

        default:
            return 0;
    }

    if (!valid) {
        return 0;
    }

    if ((Modes.mode_ac == 0) && (l == (MODEAC_MSG_BYTES * 2))) {
        return 0;
    }

    for (j = 0; j < l; j += 2) {
        int high = hexDigitVal(hex[j]);
        int low  = hexDigitVal(hex[j + 1]);

        if (high == -1 || low == -1) {
            return 0;
        }
        msg[j / 2] = (high << 4) | low;
    }

    // Record reception time as the time we read it.
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
