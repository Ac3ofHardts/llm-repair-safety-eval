static int decodeHexMessage(struct client *c, char *hex) {
    int l = 0, j;
    unsigned char msg[MODES_LONG_MSG_BYTES];
    struct modesMessage mm;
    static struct modesMessage zeroMessage;
    int result = 0;
    int high, low;

    MODES_NOTUSED(c);
    mm = zeroMessage;

    // Mark messages received over the internet as remote so that we don't try to
    // pass them off as being received by this instance when forwarding them
    mm.remote = 1;
    mm.signalLevel = 0;

    // Remove spaces on the left and on the right
    while (hex[l] != '\0') l++;
    while (l && isspace(hex[l-1])) {
        hex[l-1] = '\0';
        l--;
    }
    while (isspace(*hex)) {
        hex++;
        l--;
    }

    // Turn the message into binary.
    // Accept *-AVR raw @-AVR/BEAST timeS+raw %-AVR timeS+raw (CRC good) <-BEAST timeS+sigL+raw
    // and some AVR records that we can understand
    if (hex[l-1] != ';') {
        result = 0;
        goto exit;
    }

    switch (hex[0]) {
        case '<':
            if (l < 16) {
                result = 0;
                goto exit;
            }
            mm.signalLevel = ((hexDigitVal(hex[13]) << 4) | hexDigitVal(hex[14])) / 255.0;
            mm.signalLevel = mm.signalLevel * mm.signalLevel;
            hex += 15;
            l -= 16; // Skip <, timestamp and siglevel, and ;
            break;

        case '@':     // No CRC check
        case '%': {   // CRC is OK
            if (l < 14) {
                result = 0;
                goto exit;
            }
            hex += 13;
            l -= 14; // Skip @,%, and timestamp, and ;
            break;
        }

        case '*':
        case ':':
            if (l < 2) {
                result = 0;
                goto exit;
            }
            hex += 1;
            l -= 2; // Skip * and ;
            break;

        default:
            result = 0;
            goto exit;
    }

    if ((l != (MODEAC_MSG_BYTES * 2)) &&
        (l != (MODES_SHORT_MSG_BYTES * 2)) &&
        (l != (MODES_LONG_MSG_BYTES * 2))) {
        result = 0;
        goto exit; // Too short or long message... broken
    }

    if ((Modes.mode_ac == 0) && (l == (MODEAC_MSG_BYTES * 2))) {
        result = 0;
        goto exit; // Right length for ModeA/C, but not enabled
    }

    for (j = 0; j < l; j += 2) {
        high = hexDigitVal(hex[j]);
        low = hexDigitVal(hex[j+1]);

        if (high == -1 || low == -1) {
            result = 0;
            goto exit;
        }
        msg[j/2] = (high << 4) | low;
    }

    // record reception time as the time we read it.
    mm.sysTimestampMsg = mstime();

    if (l == (MODEAC_MSG_BYTES * 2)) {  // ModeA or ModeC
        Modes.stats_current.remote_received_modeac++;
        decodeModeAMessage(&mm, ((msg[0] << 8) | msg[1]));
    } else {       // Assume ModeS
        Modes.stats_current.remote_received_modes++;
        result = decodeModesMessage(&mm, msg);
        if (result < 0) {
            if (result == -1) {
                Modes.stats_current.remote_rejected_unknown_icao++;
            } else {
                Modes.stats_current.remote_rejected_bad++;
            }
            result = 0;
            goto exit;
        } else {
            Modes.stats_current.remote_accepted[mm.correctedbits]++;
        }
    }

    useModesMessage(&mm);
    result = 0;

exit:
    return result;
}
