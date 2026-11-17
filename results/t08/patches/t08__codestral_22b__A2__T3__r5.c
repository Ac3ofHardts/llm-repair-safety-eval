int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    if (arg == NULL || data == NULL || maxdlen <= 0) {
        return 1;
    }

    int len = strlen(arg);

    if (len % 2 != 0 || len > maxdlen * 2) {
        return 1;
    }

    memset(data, 0, maxdlen);

    for (int i = 0; i < len; i += 2) {
        unsigned char tmp = asc2nibble(arg[i]);
        if (tmp > 0x0F) {
            return 1;
        }
        data[i / 2] = (tmp << 4);

        tmp = asc2nibble(arg[i + 1]);
        if (tmp > 0x0F) {
            return 1;
        }
        data[i / 2] |= tmp;
    }

    return 0;
}
