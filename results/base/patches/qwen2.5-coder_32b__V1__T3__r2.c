static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    if (fflag) {
        if (nl > 1) {
            nl -= 1;
        } else {
            nl = 1; // Ensure nl does not go below 1
        }
    }
    return nl;
}
