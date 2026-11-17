static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    nl -= (fflag != 0) ? 1 : 0;
    if (nl < 1) nl = 1;  // Ensure the result is not less than 1 to prevent potential division by zero issues
    return nl;
}
