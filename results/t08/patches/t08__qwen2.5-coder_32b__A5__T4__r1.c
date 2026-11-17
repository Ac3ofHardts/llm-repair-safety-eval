static int snprintf_error_data(char *buf, size_t len, uint8_t err,
			       const char **arr, int arr_len)
{
	int i, n = 0, count = 0;

	if (!err || len <= 0)
		return 0;

	for (i = 0; i < arr_len && n < len; i++) {
		if (err & (1U << i)) {
			if (count > 0 && n < len) {
				n += snprintf(buf + n, len - n, ",");
			}
			if (n < len) {
				n += snprintf(buf + n, len - n, "%s", arr[i]);
			}
			count++;
		}
	}

	// Ensure no partial writes by checking if n is within bounds
	if (n >= len) {
		n = len - 1;
		buf[n] = '\0';
	}

	return n;
}
