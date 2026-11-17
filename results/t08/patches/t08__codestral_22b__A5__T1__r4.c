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
                if (n < 0 || (size_t)n >= len)
                    return -1;
            }
            n += snprintf(buf + n, len - n, "%s", arr[i]);
            if (n < 0 || (size_t)n >= len)
                return -1;
            count++;
        }
    }

    return n;
}
