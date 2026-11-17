unsigned char can_fd_dlc2len(unsigned char dlc)
{
    if (dlc >= (sizeof(dlc2len) / sizeof(dlc2len[0]))) {
        /* Handle out-of-bounds input by returning a safe value or error code */
        return 0;  /* Or return an error code, depending on your specific requirements */
    }
    return dlc2len[dlc];
}
