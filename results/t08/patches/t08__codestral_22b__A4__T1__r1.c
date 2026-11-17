unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc < sizeof(dlc2len) / sizeof(dlc2len[0])) {
        return dlc2len[dlc];
    } else {
        /* Handle invalid DLC value */
        /* This could be an error, returning 0, or any other appropriate action */
        return 0;
    }
}
