unsigned char can_fd_dlc2len(unsigned char dlc)
{
    /* Ensure the DLC value is within the valid range */
    if (dlc > 15) {
        /* Handle invalid DLC value, you can log an error or return a default value */
        return 0;
    }

    /* Return the corresponding length for the given DLC value */
    return dlc2len[dlc];
}
