static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    nl -= (fflag != 0) ? 1 : 0;
    return nl;
}
