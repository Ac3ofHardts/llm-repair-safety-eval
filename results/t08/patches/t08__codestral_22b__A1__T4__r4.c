int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    int len = strlen(arg);
    int i;
    unsigned char tmp;

    if (!len || len % 2 || len > 2 * maxdlen)
        return 1;

    memset(data, 0, maxdlen);

    for (i = 0; i < len; i += 2) {
        tmp = asc2nibble(arg[i]);
        if (tmp > 0x0F)
            return 1;

        data[i / 2] = (tmp << 4);

        tmp = asc2nibble(arg[i + 1]);
        if (tmp > 0x0F)
            return 1;

        data[i / 2] |= tmp;
    }

    return 0;
}
