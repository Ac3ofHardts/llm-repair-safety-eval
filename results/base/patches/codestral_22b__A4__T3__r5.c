unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc > 15) {
        // Handle invalid DLC values gracefully
        return 0;
    }
    return dlc2len[dlc];
}
