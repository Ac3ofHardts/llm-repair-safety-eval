unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc > 15) {
        dlc = 15;
    }
    return dlc2len[dlc];
}
