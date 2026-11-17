static int decodeHexMessage(struct client *c, char *hex) {
    unsigned char msg[MODES_LONG_MSG_BYTES] = {0};
    struct modesMessage mm = {0};
    int l = strlen(hex);

    MODES_NOTUSED(c);
    mm.remote = 1;
    mm.signalLevel = 0;

    // Remove spaces on the left and on the right
    while (l > 0 && isspace(hex[l - 1])) {
        hex[l - 1] = '\0';
        l--;
    }
    while (l > 0 && isspace(*hex)) {
        hex++;
        l--;
    }

    // Accept only complete messages
    if (l == 0 || hex[l - 1] != ';') {
        return 0;
    }

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

    if (l == MODEAC_MSG_BYTES * 2 && Modes.mode_ac == 0) {
        return 0;
    }

    for (int j = 0; j < l; j += 2) {
        int high = hexDigitVal(hex[j]);
        int low = hexDigitVal(hex[j + 1]);

        if (high == -1 || low == -1) return 0;
        msg[j / 2] = (high << 4) | low;
    }

    mm.sysTimestampMsg = mstime();

    if (l == MODEAC_MSG_BYTES * 2) {
        Modes.stats_current.remote_received_modeac++;
        decodeModeAMessage(&mm, ((msg[0] << 8) | msg[1]));
    } else {
        int result = decodeModesMessage(&mm, msg);
        if (result < 0) {
            Modes.stats_current.remote_rejected_bad++;
            return 0;
        } else {
            Modes.stats_current.remote_accepted[mm.correctedbits]++;
        }
    }

    useModesMessage(&mm);
    return 0;
}
