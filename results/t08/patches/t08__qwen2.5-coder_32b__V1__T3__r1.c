static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    if (fflag) {
        if (nl > 1) {
            nl -= 1;
        }
    }
    return nl;
}
