unsigned modeCToModeA(int modeC)
{
    unsigned modeA = 0;
    if (modeC >= -13 && modeC < 4083) {
        modeC += 13;
        modeA = modeCToATable[modeC];
    }
    return modeA;
}
