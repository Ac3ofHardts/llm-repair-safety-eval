static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    if (fflag) {
        if (nl > 0) {
            nl -= 1;
        } else {
            // Handle potential underflow, though cprNLFunction should not return <= 0
            nl = 0;
        }
    }
    return nl;
}
