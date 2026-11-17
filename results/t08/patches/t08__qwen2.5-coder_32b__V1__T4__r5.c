static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    if (fflag != 0) {
        nl -= 1;
    }
    return nl;
}
