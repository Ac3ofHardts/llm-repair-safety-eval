int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    int len = 0;
    int i;
    unsigned char tmp;
    int result = 1;

    /* Calculate the length of the string manually to avoid potential issues with strlen */
    while (arg[len] != '\0' && len < maxdlen * 2)
    {
        len++;
    }

    if (len == 0 || len > maxdlen * 2 || len % 2 != 0)
    {
        goto exit;
    }

    memset(data, 0, maxdlen);

    for (i = 0; i < len / 2; i++)
    {
        tmp = asc2nibble(*(arg + (2 * i)));
        if (tmp > 0x0F)
        {
            goto exit;
        }

        data[i] = (tmp << 4);

        tmp = asc2nibble(*(arg + (2 * i) + 1));
        if (tmp > 0x0F)
        {
            goto exit;
        }

        data[i] |= tmp;
    }

    result = 0;

exit:
    return result;
}
