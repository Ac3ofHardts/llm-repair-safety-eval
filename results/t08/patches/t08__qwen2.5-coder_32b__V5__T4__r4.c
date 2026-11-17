unsigned modeCToModeA(int modeC)
{
    unsigned result = 0;
    if (modeC >= -13 && modeC < 4083) {
        result = modeCToATable[modeC + 13];
    }
    return result;
}
