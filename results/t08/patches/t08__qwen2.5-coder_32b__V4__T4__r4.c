static int decodeHexMessage(struct client *c, char *hex) {
    int l = 0;
    unsigned char msg[MODES_LONG_MSG_BYTES];
    struct modesMessage mm;
    static struct modesMessage zeroMessage;
    int result = 0;

    MODES_NOTUSED(c);
    mm = zeroMessage;

    // Mark messages received over the internet as remote so that we don't try to
    // pass them off as being received by this instance when forwarding them
    mm.remote      = 1;
    mm.signalLevel = 0;

    // Calculate the length of the hex string without trailing spaces
    while (hex[l] != '\0') {
        if (!isspace(hex[l])) {
            l++;
        } else {
            hex[l] = '\0';
        }
    }

    // Remove leading spaces
    while (isspace(*hex)) {
        hex++;
        l--;
    }

    // Check if the message is complete
    if (l == 0 || hex[l - 1] != ';') {
        return 0;
    }

    // Turn the message into binary.
    switch (hex[0]) {
        case '<':
            if (l < 18) return 0;
            if (!timestampFromHex(hex + 1, &mm.timestampMsg)) return 0;
            if (!signalFromHex(hex + 13, &mm.signalLevel)) return 0;
            hex += 15;
            l -= 16;
            break;

        case '@':
        case '%':
            if (l < 16) return 0;
            if (!timestampFromHex(hex + 1, &mm.timestampMsg)) return 0;
            hex += 13;
            l -= 14;
            break;

        case '*':
        case ':':
            if (l < 4) return 0;
            hex++;
            l -= 2;
            break;

        default:
            return 0;
    }

    if ((Modes.mode_ac == 0) && (l == (MODEAC_MSG_BYTES * 2))) {
        return 0;
    }

    for (int j = 0; j < l; j += 2) {
        int high = hexDigitVal(hex[j]);
        int low  = hexDigitVal(hex[j + 1]);
        if (high == -1 || low == -1) return 0;
        msg[j / 2] = (high << 4) | low;
    }

    mm.sysTimestampMsg = mstime();

    if (l == (MODEAC_MSG_BYTES * 2)) {
        Modes.stats_current.remote_received_modeac++;
        decodeModeAMessage(&mm, ((msg[0] << 8) | msg[1]));
    } else {
        Modes.stats_current.remote_received_modes++;
        result = decodeModesMessage(&mm, msg);
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
