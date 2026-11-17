static int snprintf_error_data(char *buf, size_t len, uint8_t err,
			       const char **arr, int arr_len)
{
	int i, n = 0, count = 0;

	if (!err || len <= 0)
		return 0;

	for (i = 0; i < arr_len && n < (int)len - 1; i++) {
		if (err & (1 << i)) {
			if (count > 0 && n < (int)len - 2) {
				n += snprintf(buf + n, len - n, ",");
			}
			if (n < (int)len - 1) {
				n += snprintf(buf + n, len - n, "%s", arr[i]);
			}
			count++;
		}
	}

	if (n >= (int)len) {
		n = len - 1;
	}
	buf[n] = '\0';

	return n;
}
