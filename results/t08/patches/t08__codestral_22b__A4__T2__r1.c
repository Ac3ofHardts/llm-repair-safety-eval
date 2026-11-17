unsigned char can_fd_dlc2len(unsigned char dlc)
{
	if (dlc > 15) {
		return 0;  // Invalid DLC value
	}
	return dlc2len[dlc];
}
