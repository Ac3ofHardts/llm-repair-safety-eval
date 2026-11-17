static int decodeHexMessage(struct client *c, char *hex) {
    int l = strlen(hex), j;
    unsigned char msg[MODES_LONG_MSG_BYTES] = {0};
    struct modesMessage mm;

    // Initialize mm to zero
    memset(&mm, 0, sizeof(mm));

    // Mark messages as remote
    mm.remote = 1;

    // Trim spaces from the input
    char *end;
    while (isspace(*hex))
        hex++;
    end = hex + strlen(hex) - 1;
    while (end > hex && isspace(*end))
        end--;
    *(end + 1) = '\0';

    // Check if the message is complete
    if (*end != ';')
        return 0;

    // Process the message based on its first character
    switch(*hex) {
        case '<':
            mm.signalLevel = ((hexDigitVal(hex[13]) << 4) | hexDigitVal(hex[14])) / 255.0;
            mm.signalLevel *= mm.signalLevel;
            hex += 15;
            break;
        case '@':
        case '%':
            hex += 13;
            break;
        case '*':
        case ':':
            hex++;
            break;
        default:
            return 0;
    }

    // Check message length
    l = strlen(hex);
    if (l != (MODEAC_MSG_BYTES * 2) && l != (MODES_SHORT_MSG_BYTES * 2) && l != (MODES_LONG_MSG_BYTES * 2))
        return 0;
    if (l == (MODEAC_MSG_BYTES * 2) && Modes.mode_ac == 0)
        return 0;

    // Convert hexadecimal message to binary
    for (j = 0; j < l; j += 2) {
        int high = hexDigitVal(hex[j]);
        int low = hexDigitVal(hex[j + 1]);
        if (high == -1 || low == -1)
            return 0;
        msg[j / 2] = (high << 4) | low;
    }

    // Decode the message
    mm.sysTimestampMsg = mstime();
    if (l == (MODEAC_MSG_BYTES * 2)) {
        Modes.stats_current.remote_received_modeac++;
        decodeModeAMessage(&mm, ((msg[0] << 8) | msg[1]));
    } else {
        int result = decodeModesMessage(&mm, msg);
        if (result < 0) {
            if (result == -1)
                Modes.stats_current.remote_rejected_unknown_icao++;
            else
                Modes.stats_current.remote_rejected_bad++;
            return 0;
        }
        Modes.stats_current.remote_accepted[mm.correctedbits]++;
    }

    useModesMessage(&mm);
    return 0;
}
