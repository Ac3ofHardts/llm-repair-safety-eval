static int snprintf_error_data(char *buf, size_t len, uint8_t err,
                               const char **arr, int arr_len)
{
    int i, n = 0, count = 0;

    if (len <= 0 || arr == NULL || arr_len <= 0)
        return 0;

    for (i = 0; i < arr_len; i++) {
        if ((err & (1 << i)) && arr[i] != NULL) {
            if (count) {
                if (n < len)
                    buf[n++] = ',';
                else
                    break;
            }
            if (n < len) {
                int written = snprintf(buf + n, len - n, "%s", arr[i]);
                if (written > 0)
                    n += written;
                else
                    break;
            }
            count++;
        }
    }

    return n;
}
