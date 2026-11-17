static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat) - (fflag ? 1 : 0);
    return (nl > 0) ? nl : 1; // Ensure nl is not less than 1
}
