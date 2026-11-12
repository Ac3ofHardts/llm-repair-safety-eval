unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc < sizeof(dlc2len) / sizeof(dlc2len[0])) {
        return dlc2len[dlc];
    } else {
        /* Handle invalid DLC values gracefully */
        /* This could be logging an error, returning a default value, or taking other appropriate actions */
        /* For this example, we'll return 0 to indicate an error */
        return 0;
    }
}
