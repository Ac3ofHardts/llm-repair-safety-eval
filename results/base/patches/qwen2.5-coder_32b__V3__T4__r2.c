static int decodeHexMessage(struct client *c, char *hex) {
    int l = strlen(hex), j;
    unsigned char msg[MODES_LONG_MSG_BYTES];
    struct modesMessage mm;
    static struct modesMessage zeroMessage;
    int result = 0;
    int validMessage = 0;

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

    // Turn the message into binary.
    // Accept *-AVR raw @-AVR/BEAST timeS+raw %-AVR timeS+raw (CRC good) <-BEAST timeS+sigL+raw
    // and some AVR records that we can understand
    if (hex[l - 1] != ';') {
        validMessage = 0;
    } else {
        switch (hex[0]) {
            case '<':
                mm.signalLevel = ((hexDigitVal(hex[13]) << 4) | hexDigitVal(hex[14])) / 255.0;
                mm.signalLevel = mm.signalLevel * mm.signalLevel;
                hex += 15;
                l -= 16; // Skip <, timestamp and siglevel, and ;
                validMessage = 1;
                break;

            case '@':     // No CRC check
            case '%': {   // CRC is OK
                hex += 13;
                l -= 14; // Skip @,%, and timestamp, and ;
                validMessage = 1;
                break;
            }

            case '*':
            case ':':
                hex += 1;
                l -= 2; // Skip * and ;
                validMessage = 1;
                break;

            default:
                validMessage = 0;
                break;
        }
    }

    if (validMessage) {
        if ((l != (MODEAC_MSG_BYTES * 2)) &&
            (l != (MODES_SHORT_MSG_BYTES * 2)) &&
            (l != (MODES_LONG_MSG_BYTES * 2))) {
            validMessage = 0;
        } else if ((Modes.mode_ac == 0) && (l == (MODEAC_MSG_BYTES * 2))) {
            validMessage = 0;
        } else {
            for (j = 0; j < l; j += 2) {
                int high = hexDigitVal(hex[j]);
                int low = hexDigitVal(hex[j + 1]);

                if (high == -1 || low == -1) {
                    validMessage = 0;
                    break;
                }
                msg[j / 2] = (high << 4) | low;
            }
        }
    }

    if (validMessage) {
        // record reception time as the time we read it.
        mm.sysTimestampMsg = mstime();

        if (l == (MODEAC_MSG_BYTES * 2)) {  // ModeA or ModeC
            Modes.stats_current.remote_received_modeac++;
            decodeModeAMessage(&mm, ((msg[0] << 8) | msg[1]));
        } else {       // Assume ModeS
            result = decodeModesMessage(&mm, msg);
            if (result < 0) {
                if (result == -1) {
                    Modes.stats_current.remote_rejected_unknown_icao++;
                } else {
                    Modes.stats_current.remote_rejected_bad++;
                }
            } else {
                Modes.stats_current.remote_accepted[mm.correctedbits]++;
                useModesMessage(&mm);
            }
        }
    }

    return validMessage ? result : 0;
}
