static int decodeHexMessage(struct client *c, char *hex) {
    int l = strlen(hex), j;
    unsigned char msg[MODES_LONG_MSG_BYTES];
    struct modesMessage mm;
    static struct modesMessage zeroMessage;

    MODES_NOTUSED(c);
    mm = zeroMessage;

    // Mark messages received over the internet as remote so that we don't try to
    // pass them off as being received by this instance when forwarding them
    mm.remote      =    1;
    mm.signalLevel =    0;

    // Remove spaces on the left and on the right
    while(l && isspace(hex[l-1])) {
        hex[l-1] = '\0'; l--;
    }
    while(isspace(*hex)) {
        hex++; l--;
    }

    // Check if the message is complete
    if (hex[l-1] != ';') {
        Modes.stats_current.remote_rejected_bad++;
        return 0;
    }

    // Process the message based on its type
    switch(hex[0]) {
        case '<': {
            if (l < 16) {
                Modes.stats_current.remote_rejected_bad++;
                return 0;
            }
            mm.signalLevel = ((hexDigitVal(hex[13])<<4) | hexDigitVal(hex[14])) / 255.0;
            mm.signalLevel = mm.signalLevel * mm.signalLevel;
            hex += 15; l -= 16; // Skip <, timestamp and siglevel, and ;
            break;}

        case '@':
        case '%': {
            if (l < 14) {
                Modes.stats_current.remote_rejected_bad++;
                return 0;
            }
            hex += 13; l -= 14; // Skip @,%, and timestamp, and ;
            break;}

        case '*':
        case ':': {
            if (l < 2) {
                Modes.stats_current.remote_rejected_bad++;
                return 0;
            }
            hex++; l-=2; // Skip * and ;
            break;}

        default: {
            Modes.stats_current.remote_rejected_unknown_icao++;
            return 0; // We don't know what this is, so abort
            break;}
    }

    // Check the length of the message
    if ( (l != (MODEAC_MSG_BYTES * 2))
      && (l != (MODES_SHORT_MSG_BYTES * 2))
      && (l != (MODES_LONG_MSG_BYTES * 2)) )
    {
        Modes.stats_current.remote_rejected_bad++;
        return 0; // Message is too short or long
    }

    if ( (0 == Modes.mode_ac)
      && (l == (MODEAC_MSG_BYTES * 2)) )
    {
        Modes.stats_current.remote_rejected_bad++;
        return 0; // Right length for ModeA/C, but not enabled
    }

    // Convert the message to binary
    for (j = 0; j < l; j += 2) {
        int high = hexDigitVal(hex[j]);
        int low  = hexDigitVal(hex[j+1]);

        if (high == -1 || low == -1) {
            Modes.stats_current.remote_rejected_bad++;
            return 0;
        }
        msg[j/2] = (high << 4) | low;
    }

    // Record reception time
    mm.sysTimestampMsg = mstime();

    // Decode the message based on its length
    if (l == (MODEAC_MSG_BYTES * 2)) {
        Modes.stats_current.remote_received_modeac++;
        decodeModeAMessage(&mm, ((msg[0] << 8) | msg[1]));
    } else {
        int result;

        Modes.stats_current.remote_received_modes++;
        result = decodeModesMessage(&mm, msg);
        if (result < 0) {
            if (result == -1)
                Modes.stats_current.remote_rejected_unknown_icao++;
            else
                Modes.stats_current.remote_rejected_bad++;
            return 0;
        } else {
            Modes.stats_current.remote_accepted[mm.correctedbits]++;
        }
    }

    useModesMessage(&mm);
    return 0;
}
