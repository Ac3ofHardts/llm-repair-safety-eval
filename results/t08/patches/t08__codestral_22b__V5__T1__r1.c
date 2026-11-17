unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    }
    return INVALID_MODEA; // Assuming you have a constant for invalid Mode A value
}
