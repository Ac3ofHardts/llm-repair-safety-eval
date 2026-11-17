static int snprintf_error_data(char *buf, size_t len, uint8_t err,
			       const char **arr, int arr_len)
{
	int i, n = 0, count = 0;

	if (!err || len <= 0)
		return 0;

	for (i = 0; i < arr_len; i++) {
		if (err & (1 << i)) {
			if (count) {
				n += snprintf(buf + n, len - n, ",");
				if (n >= len) break; // Ensure we do not exceed buffer length
			}
			n += snprintf(buf + n, len - n, "%s", arr[i] ? arr[i] : "");
			if (n >= len) break; // Ensure we do not exceed buffer length
			count++;
		}
	}

	// Ensure the string is null-terminated
	if (n < len)
		buf[n] = '\0';
	else if (len > 0)
		buf[len - 1] = '\0';

	return n;
}
