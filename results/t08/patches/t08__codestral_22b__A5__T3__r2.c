static int snprintf_error_data(char *buf, size_t len, uint8_t err,
                               const char **arr, int arr_len)
{
    int i, n = 0, count = 0;

    if (!err || len <= 0 || arr == NULL || arr_len <= 0)
        return 0;

    for (i = 0; i < arr_len; i++) {
        if (err & (1 << i)) {
            if (count) {
                if (n + 1 < len) {
                    buf[n++] = ',';
                } else {
                    return -1;  // Buffer overflow
                }
            }

            int snprintf_result = snprintf(buf + n, len - n, "%s", arr[i]);
            if (snprintf_result < 0 || snprintf_result >= len - n) {
                return -1;  // Error occurred or buffer overflow
            }

            n += snprintf_result;
            count++;
        }
    }

    return n;
}
