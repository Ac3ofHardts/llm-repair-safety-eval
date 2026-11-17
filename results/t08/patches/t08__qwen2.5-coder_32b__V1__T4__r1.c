static int cprNFunction(double lat, int fflag) {
    int nl = 0;
    if (lat < 10.47047130) {
        nl = 59;
    } else {
        nl = 1;
    }
    if (fflag) {
        nl -= 1;
    }
    return nl;
}
