int decodeCPRairborne(int even_cprlat, int even_cprlon,
                      int odd_cprlat, int odd_cprlon,
                      int fflag,
                      double *out_lat, double *out_lon)
{
    double AirDlat0 = 360.0 / 60.0;
    double AirDlat1 = 360.0 / 59.0;
    double lat0 = (double)even_cprlat;
    double lat1 = (double)odd_cprlat;
    double lon0 = (double)even_cprlon;
    double lon1 = (double)odd_cprlon;

    double rlat, rlon;
    int j;
    int ni;
    int m;
    int ret = -1;

    // Compute the Latitude Index "j"
    j = (int) floor(((59 * lat0 - 60 * lat1) / 131072.0) + 0.5);
    double rlat0 = AirDlat0 * (cprModInt(j, 60) + lat0 / 131072.0);
    double rlat1 = AirDlat1 * (cprModInt(j, 59) + lat1 / 131072.0);

    if (rlat0 >= 270.0) rlat0 -= 360.0;
    if (rlat1 >= 270.0) rlat1 -= 360.0;

    // Check that both are in the same latitude zone, or abort.
    if (cprNLFunction(rlat0) == cprNLFunction(rlat1)) {
        if (fflag) { // Use odd packet.
            ni = cprNFunction(rlat1, 1);
            m = (int) floor((((lon0 * (cprNLFunction(rlat1) - 1)) -
                              (lon1 * cprNLFunction(rlat1))) / 131072.0) + 0.5);
            rlon = cprDlonFunction(rlat1, 1, 0) * (cprModInt(m, ni) + lon1 / 131072.0);
            rlat = rlat1;
        } else {     // Use even packet.
            ni = cprNFunction(rlat0, 0);
            m = (int) floor((((lon0 * (cprNLFunction(rlat0) - 1)) -
                              (lon1 * cprNLFunction(rlat0))) / 131072.0) + 0.5);
            rlon = cprDlonFunction(rlat0, 0, 0) * (cprModInt(m, ni) + lon0 / 131072.0);
            rlat = rlat0;
        }

        // Renormalize to -180 .. +180
        rlon -= floor((rlon + 180.0) / 360.0) * 360.0;

        *out_lat = rlat;
        *out_lon = rlon;
        ret = 0;
    }

    return ret;
}
