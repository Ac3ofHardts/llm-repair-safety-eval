unsigned modeCToModeA(int modeC)
{
    modeC += 13;
    if (modeC >= 0 && modeC < 4096) {
        return modeCToATable[modeC];
    } else {
        // Handle out-of-range modeC values, e.g., return an error code or default value
        // Depending on the specific requirements and context of the codebase
        // This could be modified based on the error handling mechanism used in the project
        return 0; // Placeholder value, replace with appropriate error handling
    }
}
