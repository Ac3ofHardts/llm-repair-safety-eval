unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc > 15) {
        // Invalid DLC value, return 0 or handle the error appropriately
        return 0;
    }
    return dlc2len[dlc];
}
