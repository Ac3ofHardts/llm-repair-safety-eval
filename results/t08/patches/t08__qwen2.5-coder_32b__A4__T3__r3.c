unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc > 15) {
        return 0; // or handle the error as appropriate
    }
    return dlc2len[dlc];
}
