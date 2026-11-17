int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    int len = 0;
    int i;
    unsigned char tmp;

    /* Calculate the length of the string manually to avoid security issues */
    while (arg[len] != '\0')
    {
        len++;
    }

    /* Check for zero length or odd length */
    if (len == 0 || len % 2 != 0)
    {
        return 1;
    }

    /* Ensure the length does not exceed maxdlen */
    if (len / 2 > maxdlen)
    {
        return 1;
    }

    /* Initialize data array to zero */
    for (i = 0; i < maxdlen; i++)
    {
        data[i] = 0;
    }

    /* Process each pair of characters */
    for (i = 0; i < len / 2; i++)
    {
        tmp = asc2nibble(arg[2 * i]);
        if (tmp > 0x0F)
        {
            return 1;
        }

        data[i] = (tmp << 4);

        tmp = asc2nibble(arg[2 * i + 1]);
        if (tmp > 0x0F)
        {
            return 1;
        }

        data[i] |= tmp;
    }

    return 0;
}
