static int cprNFunction(double lat, int fflag) {
    if (lat < -90.0 || lat > 90.0) {
        return 1; // Return a safe default if latitude is out of range
    }
    int nl = cprNLFunction(lat) - (fflag ? 1 : 0);
    if (nl < 1) {
        nl = 1; // Ensure nl is not less than 1
    }
    return nl;
}
