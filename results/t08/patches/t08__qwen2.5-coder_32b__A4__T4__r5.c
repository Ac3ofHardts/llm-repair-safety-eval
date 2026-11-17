unsigned char can_fd_dlc2len(unsigned char dlc)
{
    unsigned char length = 0;
    if (dlc < sizeof(dlc2len))
    {
        length = dlc2len[dlc];
    }
    return length;
}
