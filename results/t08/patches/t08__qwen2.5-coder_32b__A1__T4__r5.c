int hexstring2data(char *arg, unsigned char *data, int maxdlen)
{
	int len = strlen(arg);
	int i;
	unsigned char tmp;
	int result = 1;

	if (len == 0 || len % 2 != 0 || len / 2 > maxdlen)
	{
		result = 1;
	}
	else
	{
		memset(data, 0, maxdlen);
		result = 0;

		for (i = 0; i < len / 2 && result == 0; i++)
		{
			tmp = asc2nibble(*(arg + (2 * i)));
			if (tmp > 0x0F)
			{
				result = 1;
			}
			else
			{
				data[i] = (tmp << 4);
				tmp = asc2nibble(*(arg + (2 * i) + 1));
				if (tmp > 0x0F)
				{
					result = 1;
				}
				else
				{
					data[i] |= tmp;
				}
			}
		}
	}

	return result;
}
