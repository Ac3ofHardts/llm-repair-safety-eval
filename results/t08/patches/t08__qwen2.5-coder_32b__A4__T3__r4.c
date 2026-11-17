unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc > 15) {
        dlc = 15; // Cap dlc to the maximum valid index
    }
    return dlc2len[dlc];
}
