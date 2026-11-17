static int snprintf_error_data(char *buf, size_t len, uint8_t err,
			       const char **arr, int arr_len)
{
	int i, n = 0, count = 0;

	if (!err || len <= 0)
		return 0;

	for (i = 0; i < arr_len; i++) {
		if (err & (1 << i)) {
			if (count > 0) {
				if (n >= len - 1)
					return n; // No space for comma and null terminator
				buf[n++] = ',';
			}
			size_t written = snprintf(buf + n, len - n, "%s", arr[i]);
			if (written >= len - n)
				return n; // No space for null terminator
			n += written;
			count++;
		}
	}

	return n;
}
