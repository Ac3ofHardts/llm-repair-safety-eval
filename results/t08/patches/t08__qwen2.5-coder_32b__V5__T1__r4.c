unsigned modeCToModeA(int modeC)
{
    if (modeC < 13 || modeC >= 4096)
        return INVALID_ALTITUDE;

    modeC += 13;
    return modeCToATable[modeC];
}
