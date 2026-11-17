static int decodeHexMessage(struct client *c, char *hex) {
    int l = strlen(hex);
    unsigned char msg[MODES_LONG_MSG_BYTES];
    struct modesMessage mm;
    static struct modesMessage zeroMessage;
    int j;
    int result;
    int valid_message_length;

    MODES_NOTUSED(c);
    mm = zeroMessage;

    // Mark messages received over the internet as remote so that we don't try to
    // pass them off as being received by this instance when forwarding them
    mm.remote = 1;
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

    // Check for complete message
    if (hex[l - 1] != ';') {
        return 0;
    }

    // Parse message header
    switch (hex[0]) {
        case '<':
            mm.signalLevel = ((hexDigitVal(hex[13]) << 4) | hexDigitVal(hex[14])) / 255.0;
            mm.signalLevel = mm.signalLevel * mm.signalLevel;
            hex += 15;
            l -= 16;
            break;
        case '@':     // No CRC check
        case '%': {   // CRC is OK
            hex += 13;
            l -= 14;
            break;
        }
        case '*':
        case ':':
            hex += 1;
            l -= 2;
            break;
        default:
            return 0;
    }

    // Check message length
    valid_message_length = 0;
    if (l == (MODEAC_MSG_BYTES * 2)) {
        valid_message_length = 1;
    } else if (l == (MODES_SHORT_MSG_BYTES * 2)) {
        valid_message_length = 1;
    } else if (l == (MODES_LONG_MSG_BYTES * 2)) {
        valid_message_length = 1;
    }
    if (!valid_message_length) {
        return 0;
    }

    // Check if ModeA/C is disabled and message is ModeA/C
    if (!Modes.mode_ac && (l == (MODEAC_MSG_BYTES * 2))) {
        return 0;
    }

    // Convert hex to binary
    for (j = 0; j < l; j += 2) {
        int high = hexDigitVal(hex[j]);
        int low = hexDigitVal(hex[j + 1]);
        if (high == -1 || low == -1) {
            return 0;
        }
        msg[j / 2] = (high << 4) | low;
    }

    // Record reception time
    mm.sysTimestampMsg = mstime();

    // Decode message
    if (l == (MODEAC_MSG_BYTES * 2)) {  // ModeA or ModeC
        Modes.stats_current.remote_received_modeac++;
        decodeModeAMessage(&mm, ((msg[0] << 8) | msg[1]));
        result = 0;
    } else {       // Assume ModeS
        Modes.stats_current.remote_received_modes++;
        result = decodeModesMessage(&mm, msg);
        if (result < 0) {
            if (result == -1) {
                Modes.stats_current.remote_rejected_unknown_icao++;
            } else {
                Modes.stats_current.remote_rejected_bad++;
            }
        } else {
            Modes.stats_current.remote_accepted[mm.correctedbits]++;
        }
    }

    if (result == 0) {
        useModesMessage(&mm);
    }

    return result;
}
