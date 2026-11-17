int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    int len = strlen(arg);
    int i;
    unsigned char tmp;

    if (len == 0 || len > maxdlen * 2 || len % 2 != 0) {
        return 1;
    }

    memset(data, 0, maxdlen);

    for (i = 0; i < len; i += 2) {
        if (!isxdigit(arg[i]) || !isxdigit(arg[i + 1])) {
            return 1;
        }

        tmp = asc2nibble(arg[i]);
        data[i / 2] = (tmp << 4);

        tmp = asc2nibble(arg[i + 1]);
        data[i / 2] |= tmp;
    }

    return 0;
}
