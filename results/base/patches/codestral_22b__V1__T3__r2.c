static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    if (fflag) {
        if (nl > 0) {
            nl -= 1;
        } else {
            // Handle the case where fflag is true but nl is 0 or less
            // This might depend on the specific requirements of your program
            // For now, I'll just return 0, but you might want to handle this differently
            nl = 0;
        }
    }
    return nl;
}
