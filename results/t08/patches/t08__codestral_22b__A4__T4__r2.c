unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc > 15) {
        /* DLC value is out of valid range, return 0 as an error indication */
        return 0;
    }
    return dlc2len[dlc];
}
