static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat) - (fflag ? 1 : 0);
    if (nl < 0) {
        nl = 0; // or handle the error as per your requirement
    }
    return nl;
}
