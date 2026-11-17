unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC < 0 || modeC >= 4096)
        return 0;

    return modeCToATable[modeC];
}
