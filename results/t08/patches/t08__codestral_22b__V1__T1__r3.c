static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat) - (fflag ? 1 : 0);
    if (nl < 0) {
        nl = 0; // or any other appropriate value to handle the case when nl becomes negative
    }
    return nl;
}
