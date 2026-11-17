unsigned modeCToModeA(int modeC)
{
    if (modeC >= -13 && modeC < 4083) {
        modeC += 13;
        return modeCToATable[modeC];
    }
    return INVALID_ALTITUDE;
}
