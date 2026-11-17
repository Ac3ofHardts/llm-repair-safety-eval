int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    int len = 0;
    int i;
    unsigned char tmp;

    /* Calculate the length of the string manually to avoid using strlen */
    while (arg[len] != '\0')
    {
        len++;
    }

    /* Check for empty string or odd length */
    if (!len || (len % 2) != 0)
    {
        return 1;
    }

    /* Ensure the length of the data does not exceed maxdlen */
    if (len / 2 > maxdlen)
    {
        return 1;
    }

    memset(data, 0, maxdlen);

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
