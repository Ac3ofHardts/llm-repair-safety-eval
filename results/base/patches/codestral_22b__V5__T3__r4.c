unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    } else {
        // Handle out-of-bounds index
        // You might want to return a default value, log an error, or take other appropriate action
        return 0; // or another appropriate default value
    }
}
