unsigned char can_fd_dlc2len(unsigned char dlc)
{
	if (dlc >= sizeof(dlc2len) / sizeof(dlc2len[0])) {
		return 0; /* Invalid DLC, return 0 or handle error as appropriate */
	}
	return dlc2len[dlc];
}
