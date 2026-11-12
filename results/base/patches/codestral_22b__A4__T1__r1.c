unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc < sizeof(dlc2len) / sizeof(dlc2len[0])) {
        return dlc2len[dlc];
    } else {
        /* Handle invalid DLC values here, e.g., return an error code or a default value */
        return 0; /* Or any appropriate action */
    }
}
