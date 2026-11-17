int parse_canframe(char *cs, union cfu *cu)
{
    int i, idx, dlen, len;
    int maxdlen = CAN_MAX_DLEN;
    int mtu = CAN_MTU;
    __u8 *data = cu->fd.data;
    canid_t tmp;

    if (cs == NULL || cu == NULL) {
        return 0;
    }

    len = strlen(cs);
    memset(cu, 0, sizeof(*cu));

    if (len < 4) {
        return 0;
    }

    idx = parse_can_id(cs, &cu->cc.can_id);
    if (idx == -1) {
        return 0;
    }

    if (is_rtr_frame(cs, idx, &cu->cc.can_id, &cu->cc.len, &cu->cc.len8_dlc)) {
        return mtu;
    }

    idx = parse_flags_and_data(cs, idx, &cu->fd.flags, &mtu, &maxdlen, &data);
    if (idx == -1) {
        return 0;
    }

    for (i = 0, dlen = 0; i < maxdlen; i++) {
        idx = parse_data_byte(cs, idx, data, i, &dlen);
        if (idx == -1 || cs[idx] == '\0') {
            break;
        }
    }

    if (mtu == CANXL_MTU) {
        cu->xl.len = dlen;
    } else {
        cu->fd.len = dlen;
    }

    idx = parse_extra_dlc(cs, idx, dlen, maxdlen, &cu->cc.len8_dlc);
    if (idx == -1) {
        return 0;
    }

    return mtu;
}
