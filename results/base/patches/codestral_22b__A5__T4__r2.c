static int snprintf_error_data(char *buf, size_t len, uint8_t err,
                               const char **arr, int arr_len)
{
    int i, n = 0, count = 0;

    if (!err || len <= 0 || arr_len <= 0)
        return 0;

    for (i = 0; i < arr_len && i < 8; i++) {
        if (err & (1 << i)) {
            if (count) {
                if (n < len) {
                    buf[n] = ',';
                    n++;
                }
            }
            if (n < len) {
                int written = snprintf(buf + n, len - n, "%s", arr[i]);
                if (written > 0) {
                    n += written;
                    count++;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }

    return n;
}
