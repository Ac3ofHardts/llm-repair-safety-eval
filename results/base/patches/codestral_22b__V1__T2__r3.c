static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    if (fflag) {
        if (nl > 0) nl--;
        else return -1; // Handle error or invalid input
    }
    return nl;
}
