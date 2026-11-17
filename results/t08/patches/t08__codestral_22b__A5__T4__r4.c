static int snprintf_error_data(char *buf, size_t len, uint8_t err,
			       const char **arr, int arr_len)
{
	int i, n = 0, count = 0;

	if (!err || len <= 0 || arr_len <= 0 || arr == NULL)
		return 0;

	for (i = 0; i < 8 * sizeof(uint8_t) && i < arr_len; i++) {
		if (err & (1U << i)) {
			if (count) {
				if (n < len) {
					buf[n] = ',';
					n++;
				} else {
					break;
				}
			}

			int arr_len_i = (int)strlen(arr[i]);
			if (arr_len_i < len - n) {
				strncpy(buf + n, arr[i], len - n);
				n += arr_len_i;
			} else {
				break;
			}
			count++;
		}
	}

	return n;
}
