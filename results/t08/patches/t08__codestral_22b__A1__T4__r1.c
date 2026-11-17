int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
    int len = strlen(arg);
    int i;
    unsigned char tmp;

    /* Check if the input string is empty or has odd length */
    if (!len || len % 2)
        return 1;

    /* Initialize the output data array */
    memset(data, 0, maxdlen);

    /* Iterate over the input string in pairs of characters */
    for (i = 0; i < len / 2; i++) {
        /* Convert the first character of the pair to a nibble */
        tmp = asc2nibble(*(arg + (2 * i)));
        /* Check if the conversion was successful */
        if (tmp > 0x0F)
            return 1;

        /* Shift the nibble to the left and store it in the output array */
        data[i] = (tmp << 4);

        /* Convert the second character of the pair to a nibble */
        tmp = asc2nibble(*(arg + (2 * i) + 1));
        /* Check if the conversion was successful */
        if (tmp > 0x0F)
            return 1;

        /* Combine the two nibbles into a single byte and store it in the output array */
        data[i] |= tmp;
    }

    /* If no errors occurred, return 0 */
    return 0;
}
