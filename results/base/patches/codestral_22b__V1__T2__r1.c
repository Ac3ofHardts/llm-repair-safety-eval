static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    if (fflag) {
        if (nl > 0) nl--;
        else nl = 0; // prevent underflow
    }
    return nl;
}
