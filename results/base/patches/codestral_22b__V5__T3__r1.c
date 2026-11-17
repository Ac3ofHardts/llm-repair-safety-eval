unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    } else {
        // Handle the case when modeC is out of bounds
        // This could be an error condition, so appropriate action should be taken
        // For example, return a default value or indicate an error
        return 0; // or any appropriate default value or error handling
    }
}
