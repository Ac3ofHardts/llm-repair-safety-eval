static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    return fflag ? nl - 1 : nl;
}
