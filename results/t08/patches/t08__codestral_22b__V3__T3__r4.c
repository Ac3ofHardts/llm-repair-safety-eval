static int decodeHexMessage(struct client *c, char *hex) {
    int l = strlen(hex), j;
    unsigned char msg[MODES_LONG_MSG_BYTES] = {0}; // Initialize the array to all zeros
    struct modesMessage mm;
    static struct modesMessage zeroMessage = {0}; // Initialize the struct to all zeros

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
        return (0); // not complete - abort
    }

    switch(hex[0]) {
        case '<': {
            mm.signalLevel = ((hexDigitVal(hex[13])<<4) | hexDigitVal(hex[14])) / 255.0;
            mm.signalLevel = mm.signalLevel * mm.signalLevel;
            hex += 15; l -= 16;
            break;
        }
        case '@':
        case '%': {
            hex += 13; l -= 14;
            break;
        }
        case '*':
        case ':': {
            hex++; l-=2;
            break;
        }
        default: {
            return (0); // Unknown message type
        }
    }

    if ( (l != (MODEAC_MSG_BYTES * 2)) && (l != (MODES_SHORT_MSG_BYTES * 2)) && (l != (MODES_LONG_MSG_BYTES * 2)) ) {
        return (0); // Invalid message length
    }

    if ( (0 == Modes.mode_ac) && (l == (MODEAC_MSG_BYTES * 2)) ) {
        return (0); // ModeA/C not enabled
    }

    for (j = 0; j < l; j += 2) {
        int high = hexDigitVal(hex[j]);
        int low  = hexDigitVal(hex[j+1]);

        if (high == -1 || low == -1) {
            return 0; // Invalid hexadecimal digit
        }
        msg[j/2] = (high << 4) | low;
    }

    mm.sysTimestampMsg = mstime();

    if (l == (MODEAC_MSG_BYTES * 2)) {
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
    return (0);
}
