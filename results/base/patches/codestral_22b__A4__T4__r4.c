unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc < (sizeof(dlc2len) / sizeof(dlc2len[0]))) {
        return dlc2len[dlc];
    } else {
        /* Handle invalid DLC values gracefully */
        /* This could be an error code, a default value, or a safe value */
        /* For safety, return 0 as it is a valid length but not the expected one */
        return 0;
    }
}
