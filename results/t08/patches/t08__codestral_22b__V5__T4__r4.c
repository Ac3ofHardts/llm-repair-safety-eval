unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC < 0 || modeC >= 4096)
        return 0; /* or a suitable default value or error handling */

    return modeCToATable[modeC];
}
