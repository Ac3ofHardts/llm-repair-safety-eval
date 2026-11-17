static int snprintf_error_data(char *buf, size_t len, uint8_t err,
                               const char **arr, int arr_len)
{
    int i, n = 0, count = 0;

    if (!err || len <= 0 || arr == NULL || arr_len <= 0)
        return 0;

    for (i = 0; i < arr_len; i++) {
        if (arr[i] == NULL)
            continue;

        if (err & (1 << i)) {
            if (count) {
                if (n + 1 < len)
                    buf[n++] = ',';
                else
                    break;
            }
            int len_to_copy = strlen(arr[i]);
            if (n + len_to_copy < len) {
                memcpy(buf + n, arr[i], len_to_copy);
                n += len_to_copy;
            } else {
                memcpy(buf + n, arr[i], len - n - 1);
                buf[len - 1] = '\0';
                n = len;
                break;
            }
            count++;
        }
    }

    if (n < len)
        buf[n] = '\0';

    return n;
}
