unsigned char can_fd_dlc2len(unsigned char dlc)
{
    return (dlc < sizeof(dlc2len)) ? dlc2len[dlc] : 0;
}
