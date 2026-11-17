unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    } else {
        // Handle out-of-bounds input gracefully
        // You might want to log an error or return a default value
        return 0; // or another appropriate value
    }
}
