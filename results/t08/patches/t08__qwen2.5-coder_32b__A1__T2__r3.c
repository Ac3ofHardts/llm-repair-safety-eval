int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
	int len = strlen(arg);
	int i;
	unsigned char tmp;

	if (!len || len % 2 || len / 2 > maxdlen)
		return 1;

	memset(data, 0, maxdlen);

	for (i = 0; i < len / 2; i++) {
		tmp = asc2nibble(*(arg + (2 * i)));
		if (tmp > 0x0F)
			return 1;

		data[i] = (tmp << 4);

		tmp = asc2nibble(*(arg + (2 * i) + 1));
		if (tmp > 0x0F)
			return 1;

		data[i] |= tmp;
	}

	return 0;
}
