unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    } else {
        /* Handle out-of-bounds input gracefully */
        return 0; /* Or another appropriate value */
    }
}
