unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc < sizeof(dlc2len) / sizeof(dlc2len[0])) {
        return dlc2len[dlc];
    } else {
        // Handle invalid DLC values here, for example:
        return 0; // Or some other appropriate value or error handling
    }
}
