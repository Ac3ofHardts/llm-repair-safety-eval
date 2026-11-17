unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    }
    return INVALID_MODE_A; // Replace INVALID_MODE_A with an appropriate invalid mode A value
}
