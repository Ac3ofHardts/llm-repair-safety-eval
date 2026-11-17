unsigned modeCToModeA(int modeC)
{
    if (modeC < -13 || modeC >= 4083)
        return 0;

    modeC += 13;
    return modeCToATable[modeC];
}
