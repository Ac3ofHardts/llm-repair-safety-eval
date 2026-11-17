unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    }
    // Handle out-of-bounds input
    // You might want to return an error value, a default value, or handle it based on your specific requirements
    return 0; // Or an appropriate error value
}
