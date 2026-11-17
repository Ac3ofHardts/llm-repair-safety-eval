unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc <= 0xF) {
        return dlc2len[dlc];
    } else {
        /* Handle invalid DLC value */
        /* You can return a default value, log an error, or take another appropriate action here */
        return 0; /* For example, returning 0 as a default value */
    }
}
