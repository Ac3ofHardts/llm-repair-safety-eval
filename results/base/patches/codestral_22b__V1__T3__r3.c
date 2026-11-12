static int cprNFunction(double lat, int fflag) {
    int nl = cprNLFunction(lat);
    if (fflag) {
        if (nl > 0) {
            nl--;
        } else {
            // Handle the case where nl is already 0 or negative
            // This could be an error condition or a specific behavior based on your application's requirements
            // For now, I'll leave it as is, but you might want to add error handling or specific behavior here
        }
    }
    return nl;
}
