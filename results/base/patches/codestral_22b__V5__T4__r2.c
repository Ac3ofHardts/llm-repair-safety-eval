unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    } else {
        /* Handle the error according to the MISRA C:2012 guidelines for safety-critical software */
        /* For example, you could return a default value or set a flag to indicate an error */
        return 0; /* Or another appropriate value */
    }
}
