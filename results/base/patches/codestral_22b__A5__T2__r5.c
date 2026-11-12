static int snprintf_error_data(char *buf, size_t len, uint8_t err,
			       const char **arr, int arr_len)
{
	int i, n = 0, count = 0;

	if (!err || len <= 0 || arr_len <= 0 || !arr)
		return 0;

	for (i = 0; i < arr_len && len > 0; i++) {
		if (err & (1 << i)) {
			if (count) {
				if (len > 1) {
					buf[n++] = ',';
					len--;
				} else {
					break;
				}
			}
			int written = snprintf(buf + n, len, "%s", arr[i]);
			if (written < 0 || written >= len) {
				break;
			}
			n += written;
			len -= written;
			count++;
		}
	}

	return n;
}
