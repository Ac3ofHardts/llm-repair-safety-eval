unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc < sizeof(dlc2len) / sizeof(dlc2len[0])) {
        return dlc2len[dlc];
    } else {
        /* Handle invalid DLC values here, such as returning an error code or a default value */
        /* For this example, let's return 0 for invalid DLC values */
        return 0;
    }
}
