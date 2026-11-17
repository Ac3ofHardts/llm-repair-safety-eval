int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    if (arg == NULL || data == NULL) {
        return 1; // Error: null pointers are not allowed
    }

    int len = strlen(arg);
    int i;
    unsigned char tmp;

    if (len == 0 || len > maxdlen * 2) {
        return 1; // Error: invalid input length
    }

    memset(data, 0, maxdlen);

    for (i = 0; i < len / 2; i++) {
        tmp = asc2nibble(*(arg + (2 * i)));
        if (tmp > 0x0F) {
            return 1; // Error: invalid hex character
        }

        data[i] = (tmp << 4);

        tmp = asc2nibble(*(arg + (2 * i) + 1));
        if (tmp > 0x0F) {
            return 1; // Error: invalid hex character
        }

        data[i] |= tmp;
    }

    return 0;
}
