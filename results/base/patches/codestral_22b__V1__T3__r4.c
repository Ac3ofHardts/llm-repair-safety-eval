static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    if (fflag) {
        if (nl > 0) {
            nl--;
        } else {
            // Handle error or invalid case when fflag is true and nl is not greater than 0
            // This could be logging an error, returning a default value, or throwing an exception, depending on the context
        }
    }
    return nl;
}
