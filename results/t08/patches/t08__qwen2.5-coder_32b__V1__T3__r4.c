static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    return nl > 0 ? nl - fflag : 1;
}
