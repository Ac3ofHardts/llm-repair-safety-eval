static int cprNFunction(double lat, int fflag) {
    int nl;

    if (lat < -90.0 || lat > 90.0) {
        /* Handle invalid input */
        return -1;
    }

    nl = cprNLFunction(lat);
    if (nl > 0) {
        nl -= (fflag ? 1 : 0);
    }

    /* Ensure nl is always greater than 0 */
    if (nl <= 0) {
        return 1;
    }

    return nl;
}
