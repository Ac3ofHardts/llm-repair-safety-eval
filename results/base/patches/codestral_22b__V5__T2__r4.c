unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    } else {
        // Handle the case when modeC is out of bounds
        // This could be an error condition or a default value, depending on the specific requirements
        // For example, return an error code or a default value
        return 0; // or return an error code, or handle it according to the specific requirements
    }
}
