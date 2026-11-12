static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat) - (fflag ? 1 : 0);
    // Ensure nl is within a safe range to prevent potential integer overflow or underflow
    if (nl < 1) {
        nl = 1;
    } else if (nl > 59) {
        nl = 59;
    }
    return nl;
}
