unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    }
    // Handle the case when modeC is out of bounds
    // You can return a default value, log an error, or take appropriate action
    // For this example, let's return a default value of 0
    return 0;
}
