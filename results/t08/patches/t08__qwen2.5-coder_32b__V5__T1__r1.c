unsigned modeCToModeA(int modeC)
{
    if (modeC < 0 || modeC >= 4096)
        return INVALID_ALTITUDE;

    modeC += 13;
    if (modeC < 0 || modeC >= 4096)
        return INVALID_ALTITUDE;

    return modeCToATable[modeC];
}
