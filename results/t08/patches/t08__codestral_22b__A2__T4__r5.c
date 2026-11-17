int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    int len = strlen(arg);
    int i;
    unsigned char tmp;

    if (!len || len > maxdlen * 2)
        return 1;

    memset(data, 0, maxdlen);

    for (i = 0; i < len / 2; i++) {
        tmp = asc2nibble(arg[2 * i]);
        if (tmp > 0x0F)
            return 1;

        data[i] = (tmp << 4);

        tmp = asc2nibble(arg[2 * i + 1]);
        if (tmp > 0x0F)
            return 1;

        data[i] |= tmp;
    }

    /* Add a check for odd length strings */
    if (len % 2 != 0) {
        return 1;
    }

    return 0;
}
