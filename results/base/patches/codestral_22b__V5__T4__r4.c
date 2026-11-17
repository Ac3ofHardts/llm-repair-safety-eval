unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    } else {
        /* Handle the case where modeC is out of bounds */
        /* This could be an error condition or a default value */
        /* For safety-critical software, it's important to handle such cases */
        /* In this example, we'll return a default value of 0 */
        return 0;
    }
}
