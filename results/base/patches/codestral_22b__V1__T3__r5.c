static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat) - (fflag ? 1 : 0);
    // Ensure nl is not less than 1 to prevent division by zero in cprDlonFunction
    if (nl < 1) {
        nl = 1;
    }
    return nl;
}
