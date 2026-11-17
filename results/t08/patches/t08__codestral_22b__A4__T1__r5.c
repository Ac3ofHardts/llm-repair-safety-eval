unsigned char can_fd_dlc2len(unsigned char dlc)
{
    // Check if dlc is within the valid range of the dlc2len array
    if (dlc < sizeof(dlc2len) / sizeof(dlc2len[0])) {
        return dlc2len[dlc];
    }
    else {
        // Handle invalid dlc value, for example, return 0 or a specific error code
        return 0; // or a suitable error value
    }
}
