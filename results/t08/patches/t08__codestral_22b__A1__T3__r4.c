int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    if (!arg || !data || maxdlen <= 0)
        return 1;

    int len = strlen(arg);
    if (len % 2 != 0)
        return 1;

    int bytes = len / 2;
    if (bytes > maxdlen)
        return 1;

    memset(data, 0, maxdlen);

    for (int i = 0; i < bytes; i++) {
        unsigned char high = asc2nibble(arg[2 * i]);
        unsigned char low = asc2nibble(arg[2 * i + 1]);

        if (high > 0x0F || low > 0x0F)
            return 1;

        data[i] = (high << 4) | low;
    }

    return 0;
}
