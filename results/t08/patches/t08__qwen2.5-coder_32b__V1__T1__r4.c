static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    if (fflag) nl--;
    return nl;
}
