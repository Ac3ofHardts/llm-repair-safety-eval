unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    } else {
        // Handle the case when modeC is out of bounds
        // You might want to log an error, return a default value, or take appropriate action
        // For now, returning 0 as a placeholder
        return 0;
    }
}
