unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc < sizeof(dlc2len) / sizeof(dlc2len[0])) {
        return dlc2len[dlc];
    } else {
        // Handle invalid DLC value, such as returning a default value or setting an error flag
        return 0; // Or some other appropriate action
    }
}
