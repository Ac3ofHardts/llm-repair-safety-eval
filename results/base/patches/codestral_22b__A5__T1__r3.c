static int snprintf_error_data(char *buf, size_t len, uint8_t err,
                               const char **arr, int arr_len)
{
    int i, n = 0, count = 0;

    if (!err || len <= 0)
        return 0;

    for (i = 0; i < arr_len; i++) {
        if (err & (1 << i)) {
            if (count) {
                if (len - n > 1) {
                    buf[n++] = ',';
                    buf[n] = '\0';
                } else {
                    return -1;  // Buffer too small
                }
            }
            int written = snprintf(buf + n, len - n, "%s", arr[i]);
            if (written < 0 || written >= len - n) {
                return -1;  // Error in snprintf or buffer too small
            }
            n += written;
            count++;
        }
    }

    return n;
}
