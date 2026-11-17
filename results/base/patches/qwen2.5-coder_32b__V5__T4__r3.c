unsigned modeCToModeA(int modeC)
{
    unsigned result = 0;
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        result = modeCToATable[modeC];
    }
    return result;
}
