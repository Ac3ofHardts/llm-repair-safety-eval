static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat) - (fflag ? 1 : 0);
    if (nl < 0) nl = 0;  // Ensure nl is not negative
    return nl;
}
