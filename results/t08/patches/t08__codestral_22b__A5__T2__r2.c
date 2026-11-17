static int snprintf_error_data(char *buf, size_t len, uint8_t err,
			       const char **arr, int arr_len)
{
	int i, n = 0, count = 0;

	if (len <= 0 || arr_len <= 0 || !arr)
		return 0;

	for (i = 0; i < arr_len; i++) {
		if (err & (1 << i)) {
			size_t arr_len_i = strnlen(arr[i], len - n);
			if (arr_len_i >= len - n)
				break;
			if (count) {
				buf[n++] = ',';
				if (n >= len)
					break;
			}
			strncpy(buf + n, arr[i], len - n);
			n += arr_len_i;
			count++;
		}
	}

	return n;
}
