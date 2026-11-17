int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    if (!arg || !data || maxdlen <= 0)
        return 1;

    int len = strlen(arg);
    if (len == 0 || len > maxdlen * 2 || len % 2 != 0)
        return 1;

    memset(data, 0, maxdlen);

    for (int i = 0; i < len / 2; i++) {
        unsigned char tmp1 = asc2nibble(arg[2 * i]);
        unsigned char tmp2 = asc2nibble(arg[2 * i + 1]);
        if (tmp1 > 0x0F || tmp2 > 0x0F)
            return 1;

        data[i] = (tmp1 << 4) | tmp2;
    }

    return 0;
}
