static int decodeHexMessage(struct client *c, char *hex) {
    int l = strlen(hex);
    unsigned char msg[MODES_LONG_MSG_BYTES];
    struct modesMessage mm;
    static struct modesMessage zeroMessage;
    int j;
    bool valid = true;

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
        valid = false;
    }

    // Turn the message into binary.
    if (valid) {
        switch (hex[0]) {
            case '<':
                if (l < 18) {
                    valid = false;
                } else {
                    valid = timestampFromHex(hex + 1, &mm.timestampMsg);
                    if (valid) {
                        valid = signalFromHex(hex + 13, &mm.signalLevel);
                        if (valid) {
                            hex += 15;
                            l -= 16;
                        }
                    }
                }
                break;

            case '@':     // No CRC check
            case '%':     // CRC is OK
                if (l < 16) {
                    valid = false;
                } else {
                    valid = timestampFromHex(hex + 1, &mm.timestampMsg);
                    if (valid) {
                        hex += 13;
                        l -= 14;
                    }
                }
                break;

            case '*':
            case ':':
                if (l < 4) {
                    valid = false;
                } else {
                    hex++;
                    l -= 2;
                }
                break;

            default:
                valid = false;
        }
    }

    if (valid && (0 == Modes.mode_ac) && (l == (MODEAC_MSG_BYTES * 2))) {
        valid = false;
    }

    if (valid) {
        for (j = 0; j < l; j += 2) {
            int high = hexDigitVal(hex[j]);
            int low = hexDigitVal(hex[j + 1]);

            if (high == -1 || low == -1) {
                valid = false;
                break;
            }
            msg[j / 2] = (high << 4) | low;
        }
    }

    if (valid) {
        // record reception time as the time we read it.
        mm.sysTimestampMsg = mstime();

        if (l == (MODEAC_MSG_BYTES * 2)) {  // ModeA or ModeC
            Modes.stats_current.remote_received_modeac++;
            decodeModeAMessage(&mm, ((msg[0] << 8) | msg[1]));
        } else {       // Assume ModeS
            int result;

            Modes.stats_current.remote_received_modes++;
            result = decodeModesMessage(&mm, msg);
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
    }

    if (valid) {
        useModesMessage(&mm);
    }

    return valid ? 0 : 1;
}
