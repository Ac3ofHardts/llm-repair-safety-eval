static int snprintf_error_data(char *buf, size_t len, uint8_t err,
			       const char **arr, int arr_len)
{
	int i, n = 0, count = 0;

	if (len <= 0)
		return 0;

	for (i = 0; i < arr_len; i++) {
		if (err & (1 << i)) {
			if (count > 0)
				n += snprintf(buf + n, len - n, ",");
			n += snprintf(buf + n, len - n, "%s", arr[i]);
			count++;
		}
	}

	return n;
}
