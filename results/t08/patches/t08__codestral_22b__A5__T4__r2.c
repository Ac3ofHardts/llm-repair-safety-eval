static int snprintf_error_data(char *buf, size_t len, uint8_t err,
                               const char **arr, int arr_len)
{
    int i, n = 0, count = 0;

    if (!err || len <= 0 || arr == NULL || arr_len <= 0)
        return 0;

    for (i = 0; i < arr_len && i < 8; i++) {  // limiting the loop to 8 iterations to avoid overflow
        if (err & (1U << i)) {
            if (count)
                n += snprintf(buf + n, len - n, ",");
            if (n < len && arr[i] != NULL)
                n += snprintf(buf + n, len - n, "%s", arr[i]);
            count++;
        }
    }

    return n;
}
