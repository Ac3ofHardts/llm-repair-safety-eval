unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc < sizeof(dlc2len) / sizeof(dlc2len[0])) {
        return dlc2len[dlc];
    } else {
        /* Handle invalid DLC values gracefully */
        /* This could be an error condition or a default value, depending on the requirements */
        return 0; /* Or some other appropriate value */
    }
}
