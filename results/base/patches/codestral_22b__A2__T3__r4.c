int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    if (!arg || !data)
        return 1;

    int len = strlen(arg);

    if (len > maxdlen * 2)
        return 1;

    memset(data, 0, maxdlen);

    for (int i = 0; i < len / 2; i++) {
        unsigned char tmp = asc2nibble(*(arg + (2 * i)));
        if (tmp > 0x0F)
            return 1;

        data[i] = (tmp << 4);

        tmp = asc2nibble(*(arg + (2 * i) + 1));
        if (tmp > 0x0F)
            return 1;

        data[i] |= tmp;
    }

    return 0;
}
