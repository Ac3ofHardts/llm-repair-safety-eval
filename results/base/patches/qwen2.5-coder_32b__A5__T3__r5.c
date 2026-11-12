static int snprintf_error_data(char *buf, size_t len, uint8_t err,
			       const char **arr, int arr_len)
{
	int i, n = 0, count = 0;

	if (!err || len <= 0)
		return 0;

	for (i = 0; i < arr_len; i++) {
		if (err & (1 << i)) {
			if (count) {
				if (len - n <= 1)
					break;
				n += snprintf(buf + n, len - n, ",");
			}
			if (len - n <= 1)
				break;
			n += snprintf(buf + n, len - n, "%s", arr[i]);
			count++;
		}
	}

	// Ensure the string is null-terminated
	if (n < len)
		buf[n] = '\0';

	return n;
}
