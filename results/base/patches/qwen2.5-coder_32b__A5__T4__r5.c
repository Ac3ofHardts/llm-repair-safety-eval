static int snprintf_error_data(char *buf, size_t len, uint8_t err,
			       const char **arr, int arr_len)
{
	int i, n = 0, count = 0;

	if (!err || len <= 0)
		return 0;

	for (i = 0; i < arr_len; i++) {
		if (i >= sizeof(uint8_t) * 8) {
			break; /* Prevent out-of-bounds bit shift */
		}
		if (err & (1U << i)) {
			if (count > 0) {
				n += snprintf(buf + n, len - n, ",");
				if (n >= len) {
					break; /* Prevent buffer overflow */
				}
			}
			n += snprintf(buf + n, len - n, "%s", arr[i]);
			if (n >= len) {
				break; /* Prevent buffer overflow */
			}
			count++;
		}
	}

	if (n >= len) {
		n = len - 1; /* Ensure null-termination */
	}
	buf[n] = '\0';

	return n;
}
