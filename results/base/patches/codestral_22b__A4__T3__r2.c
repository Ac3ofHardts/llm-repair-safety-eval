unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc >= sizeof(dlc2len) / sizeof(dlc2len[0])) {
        return 0;  // or handle the error appropriately
    }
    return dlc2len[dlc];
}
