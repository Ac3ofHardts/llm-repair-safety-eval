unsigned char can_fd_dlc2len(unsigned char dlc)
{
    static const unsigned char dlc2len[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64
    };

    if (dlc >= (sizeof(dlc2len) / sizeof(dlc2len[0]))) {
        /* Handle invalid input by returning a safe value, e.g., 0 */
        return 0;
    }

    return dlc2len[dlc];
}
