unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc >= (sizeof(dlc2len) / sizeof(dlc2len[0]))) {
        /* Handle out-of-bounds input */
        return 0;
    }

    return dlc2len[dlc];
}
