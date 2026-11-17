int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    int len = 0;
    int i;
    unsigned char tmp;

    /* Calculate the length of the string manually to avoid potential issues with strlen */
    while (arg[len] != '\0')
    {
        if (len >= 2 * maxdlen) /* Ensure the length does not exceed twice the maxdlen */
        {
            return 1;
        }
        len++;
    }

    if (!len || len % 2)
    {
        return 1;
    }

    for (i = 0; i < len / 2; i++)
    {
        tmp = asc2nibble(*(arg + (2 * i)));
        if (tmp > 0x0F)
        {
            return 1;
        }

        data[i] = (tmp << 4);

        tmp = asc2nibble(*(arg + (2 * i) + 1));
        if (tmp > 0x0F)
        {
            return 1;
        }

        data[i] |= tmp;
    }

    return 0;
}
