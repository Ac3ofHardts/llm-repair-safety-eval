int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    if (arg == NULL || data == NULL || maxdlen <= 0) {
        return 1; /* Invalid arguments */
    }

    int len = strlen(arg);

    if (len == 0 || len % 2 != 0) {
        return 1; /* Invalid input length */
    }

    if (len / 2 > maxdlen) {
        return 1; /* Output buffer is too small */
    }

    for (int i = 0; i < len; i++) {
        if (!((arg[i] >= '0' && arg[i] <= '9') ||
              (arg[i] >= 'A' && arg[i] <= 'F') ||
              (arg[i] >= 'a' && arg[i] <= 'f'))) {
            return 1; /* Invalid character in input */
        }
    }

    for (int i = 0; i < len / 2; i++) {
        unsigned char tmp = asc2nibble(arg[2 * i]);
        if (tmp > 0x0F) {
            return 1; /* Error in conversion */
        }

        data[i] = (tmp << 4);

        tmp = asc2nibble(arg[2 * i + 1]);
        if (tmp > 0x0F) {
            return 1; /* Error in conversion */
        }

        data[i] |= tmp;
    }

    return 0;
}
