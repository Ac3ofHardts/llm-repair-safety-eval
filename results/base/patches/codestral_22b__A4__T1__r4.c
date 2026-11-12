unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc < sizeof(dlc2len) / sizeof(dlc2len[0])) {
        return dlc2len[dlc];
    } else {
        // Handle invalid DLC values here, such as returning an error code or a default value
        // For this example, I'll return 0 as a placeholder
        return 0;
    }
}
