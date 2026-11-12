static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    if (fflag) {
        if (nl > 0) {
            nl -= 1;
        } else {
            // Handle the case where nl is less than or equal to 0 when fflag is true
            // This could be an error condition or a specific requirement of the function
            // Handle it appropriately based on the project's requirements
        }
    }
    return nl;
}
