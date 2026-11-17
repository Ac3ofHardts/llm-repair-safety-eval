unsigned char can_fd_dlc2len(unsigned char dlc)
{
    unsigned char len = 0;
    if (dlc < sizeof(dlc2len)) {
        len = dlc2len[dlc];
    }
    return len;
}
