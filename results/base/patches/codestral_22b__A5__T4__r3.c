static int snprintf_error_data(char *buf, size_t len, uint8_t err,
                               const char **arr, int arr_len)
{
    int i, n = 0, count = 0;

    if (!err || len <= 0 || arr_len <= 0)
        return 0;

    for (i = 0; i < 8 * sizeof(err) && i < arr_len; i++) {
        if (err & (1 << i)) {
            if (count) {
                if (n < len)
                    buf[n++] = ',';
                else
                    return n;
            }
            if (arr[i] != NULL) {
                int len_to_copy = (int)strlen(arr[i]);
                if (n + len_to_copy < len) {
                    strncpy(buf + n, arr[i], len_to_copy);
                    n += len_to_copy;
                } else {
                    return n;
                }
            }
            count++;
        }
    }

    return n;
}
