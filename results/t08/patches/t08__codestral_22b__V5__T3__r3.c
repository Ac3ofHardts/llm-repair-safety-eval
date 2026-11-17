unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    } else {
        // Handle the error appropriately, such as returning a special value or an error code
        // For example, you can return a default value or 0, depending on your application's requirements
        return 0; // or return a special value indicating an error
    }
}
