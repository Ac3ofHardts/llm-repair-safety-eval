static int snprintf_error_data(char *buf, size_t len, uint8_t err,
			       const char **arr, int arr_len)
{
	int i, n = 0, count = 0;

	if (!err || len <= 0 || arr == NULL || arr_len <= 0)
		return 0;

	if (arr_len > 8 * sizeof(err))
		arr_len = 8 * sizeof(err);

	for (i = 0; i < arr_len; i++) {
		if (err & (1 << i)) {
			if (count)
				n += snprintf(buf + n, len - n, ",");
			if (arr[i] != NULL)
				n += snprintf(buf + n, len - n, "%s", arr[i]);
			count++;
		}
	}

	return n;
}
