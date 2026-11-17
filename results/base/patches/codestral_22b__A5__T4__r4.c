static int snprintf_error_data(char *buf, size_t len, uint8_t err,
                               const char **arr, int arr_len)
{
    int i, n = 0, count = 0;

    if (!err || len <= 0 || arr_len <= 0)
        return 0;

    for (i = 0; i < arr_len && i < 8; i++) { /* Limit loop to 8 iterations to avoid potential infinite loop */
        if (err & (1 << i)) {
            if (count) {
                if (n < len) {
                    buf[n++] = ',';
                } else {
                    return -1; /* Buffer overflow */
                }
            }
            if (n < len) {
                n += snprintf(buf + n, len - n, "%s", arr[i]);
            } else {
                return -1; /* Buffer overflow */
            }
            count++;
        }
    }

    return n;
}
