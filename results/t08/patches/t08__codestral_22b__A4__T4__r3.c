unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc > 15) {
        /* Invalid DLC value, return 0 as a safe default */
        return 0;
    }

    return dlc2len[dlc];
}
