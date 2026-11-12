int decodeCPRairborne(int even_cprlat, int even_cprlon,
                      int odd_cprlat, int odd_cprlon,
                      int fflag,
                      double *out_lat, double *out_lon)
{
    double AirDlat0 = 360.0 / 60.0;
    double AirDlat1 = 360.0 / 59.0;
    double lat0 = even_cprlat;
    double lat1 = odd_cprlat;
    double lon0 = even_cprlon;
    double lon1 = odd_cprlon;

    double rlat0, rlat1, rlon;

    // Compute the Latitude Index "j"
    int j = (int) floor(((59 * lat0 - 60 * lat1) / 131072.0) + 0.5);
    rlat0 = AirDlat0 * (cprModInt(j, 60) + lat0 / 131072.0);
    rlat1 = AirDlat1 * (cprModInt(j, 59) + lat1 / 131072.0);

    if (rlat0 >= 270) rlat0 -= 360;
    if (rlat1 >= 270) rlat1 -= 360;

    // Check that both are in the same latitude zone, or abort.
    if (cprNLFunction(rlat0) != cprNLFunction(rlat1))
        return (-1); // positions crossed a latitude zone, try again later

    // Compute ni and the Longitude Index "m"
    if (fflag) { // Use odd packet.
        int ni = cprNFunction(rlat1, 1);
        int m = (int) floor((((lon0 * (cprNLFunction(rlat1) - 1)) -
                              (lon1 * cprNLFunction(rlat1))) / 131072.0) + 0.5);
        rlon = cprDlonFunction(rlat1, 1, 0) * (cprModInt(m, ni) + lon1 / 131072.0);
        *out_lat = rlat1;
    } else {     // Use even packet.
        int ni = cprNFunction(rlat0, 0);
        int m = (int) floor((((lon0 * (cprNLFunction(rlat0) - 1)) -
                              (lon1 * cprNLFunction(rlat0))) / 131072.0) + 0.5);
        rlon = cprDlonFunction(rlat0, 0, 0) * (cprModInt(m, ni) + lon0 / 131072.0);
        *out_lat = rlat0;
    }

    // Renormalize to -180 .. +180
    rlon -= floor((rlon + 180) / 360) * 360;

    *out_lon = rlon;

    return 0;
}
