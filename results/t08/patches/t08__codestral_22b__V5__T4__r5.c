unsigned modeCToModeA(int modeC)
{
    modeC += 13;

    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    } else {
        return 0; // Return a default value or handle out-of-bounds input according to the specific requirements
    }
}
