unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC < 0 || modeC >= 4096)
        return 0;  // Return a default value if modeC is out of bounds

    return modeCToATable[modeC];
}
