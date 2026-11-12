static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    if (fflag) {
        if (nl > 0) {
            nl--;
        } else {
            // Handle the case where fflag is true but nl is already 0 or less
            // This might depend on the specific requirements of the application
            // For now, I'll leave it as is, but it should be reviewed and handled appropriately
        }
    }
    return nl;
}
