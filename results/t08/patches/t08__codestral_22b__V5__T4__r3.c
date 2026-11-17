unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    }
    else {
        /* Return a default value or handle the error as per MISRA C:2012 guidelines */
        return 0;
    }
}
