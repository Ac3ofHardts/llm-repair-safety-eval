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
    while (l && isspace(hex[l-1])) {
        hex[l-1] = '\0';
        l--;
    }
    while (isspace(*hex)) {
        hex++;
        l--;
    }

    // Check if the message is complete
    if (hex[l-1] != ';') {
        valid = false;
        goto end;
    }

    // Turn the message into binary.
    switch (hex[0]) {
        case '<':
            if (l < 18) {
                valid = false;
                break;
            }
            if (!timestampFromHex(hex + 1, &mm.timestampMsg)) {
                valid = false;
                break;
            }
            if (!signalFromHex(hex + 13, &mm.signalLevel)) {
                valid = false;
                break;
            }
            hex += 15;
            l -= 16;
            valid = true;
            break;

        case '@':
        case '%':
            if (l < 16) {
                valid = false;
                break;
            }
            if (!timestampFromHex(hex + 1, &mm.timestampMsg)) {
                valid = false;
                break;
            }
            hex += 13;
            l -= 14;
            valid = true;
            break;

        case '*':
        case ':':
            if (l < 4) {
                valid = false;
                break;
            }
            hex++;
            l -= 2;
            valid = true;
            break;

        default:
            valid = false;
            break;
    }

    if (!valid) {
        goto end;
    }

    if ((0 == Modes.mode_ac) && (l == (MODEAC_MSG_BYTES * 2))) {
        valid = false;
        goto end;
    }

    for (j = 0; j < l; j += 2) {
        int high = hexDigitVal(hex[j]);
        int low  = hexDigitVal(hex[j+1]);

        if (high == -1 || low == -1) {
            valid = false;
            goto end;
        }
        msg[j/2] = (high << 4) | low;
    }

    valid = true;

    // record reception time as the time we read it.
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
            valid = false;
        } else {
            Modes.stats_current.remote_accepted[mm.correctedbits]++;
        }
    }

end:
    if (valid) {
        useModesMessage(&mm);
    }
    return valid ? 0 : 1;
}
