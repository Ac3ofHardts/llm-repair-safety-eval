static int cprNFunction(double lat, int fflag) {
    int nl;
    if (lat < 10.47047130) {
        nl = 59;
    } else {
        nl = 1;
    }
    nl -= (fflag ? 1 : 0);
    return nl;
}
