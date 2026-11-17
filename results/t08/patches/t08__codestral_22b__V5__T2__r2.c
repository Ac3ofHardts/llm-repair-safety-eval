unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    }
    else {
        // Return an appropriate value or handle the error as per your application's requirements
        return 0; // or appropriate error code
    }
}
