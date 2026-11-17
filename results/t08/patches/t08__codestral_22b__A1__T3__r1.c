int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    if (!arg || !data || maxdlen <= 0)
        return 1;

    int len = strlen(arg);

    if (len % 2 != 0 || len / 2 > maxdlen)
        return 1;

    memset(data, 0, maxdlen);

    for (int i = 0; i < len / 2; i++) {
        unsigned char tmp_high = asc2nibble(*(arg + (2 * i)));
        unsigned char tmp_low = asc2nibble(*(arg + (2 * i) + 1));

        if (tmp_high > 0x0F || tmp_low > 0x0F)
            return 1;

        data[i] = (tmp_high << 4) | tmp_low;
    }

    return 0;
}
