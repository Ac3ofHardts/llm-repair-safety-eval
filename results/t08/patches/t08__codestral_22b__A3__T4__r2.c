int parse_canframe(char *cs, union cfu *cu) {
    int idx = 0, dlen = 0, maxdlen = CAN_MAX_DLEN, mtu = CAN_MTU;
    __u8 *data = cu->fd.data;
    canid_t tmp;

    memset(cu, 0, sizeof(*cu));

    if (cs[3] == CANID_DELIM) { /* 3 digits SFF */
        for (idx = 0; idx < 3; idx++) {
            if (!isxdigit(cs[idx]))
                return 0;
            cu->cc.can_id |= asc2nibble(cs[idx]) << (2 - idx) * 4;
        }
    } else if (cs[5] == CANID_DELIM) { /* 5 digits CAN XL VCID/PRIO*/
        maxdlen = CANXL_MAX_DLEN;
        mtu = CANXL_MTU;
        data = cu->xl.data;

        for (idx = 0; idx < 5; idx++) {
            if (!isxdigit(cs[idx]))
                return 0;
            cu->xl.prio |= asc2nibble(cs[idx]) << (4 - idx) * 4;
        }

        tmp = (cu->xl.prio << 4) & CANXL_VCID_MASK;
        cu->xl.prio &= CANXL_PRIO_MASK;
        cu->xl.prio |= tmp;
        cu->xl.flags |= CANXL_XLF;
    } else if (cs[8] == CANID_DELIM) { /* 8 digits EFF */
        for (idx = 0; idx < 8; idx++) {
            if (!isxdigit(cs[idx]))
                return 0;
            cu->cc.can_id |= asc2nibble(cs[idx]) << (7 - idx) * 4;
        }
        if (!(cu->cc.can_id & CAN_ERR_FLAG))
            cu->cc.can_id |= CAN_EFF_FLAG;
    } else {
        return 0;
    }

    idx += 4;

    if ((cs[idx] == 'R') || (cs[idx] == 'r')) { /* RTR frame */
        cu->cc.can_id |= CAN_RTR_FLAG;
        if (cs[++idx] && (tmp = asc2nibble(cs[idx++])) <= CAN_MAX_DLEN) {
            cu->cc.len = tmp;
            if ((tmp == CAN_MAX_DLEN) && (cs[idx++] == CC_DLC_DELIM)) {
                tmp = asc2nibble(cs[idx]);
                if ((tmp > CAN_MAX_DLEN) && (tmp <= CAN_MAX_RAW_DLC))
                    cu->cc.len8_dlc = tmp;
            }
        }
        return mtu;
    }

    if (cs[idx] == CANID_DELIM) { /* CAN FD frame escape char '##' */
        maxdlen = CANFD_MAX_DLEN;
        mtu = CANFD_MTU;
        cu->fd.flags |= CANFD_FDF;
        idx += 2;
    } else if (cs[idx + 14] == CANID_DELIM) { /* CAN XL frame '#80:00:11223344#' */
        maxdlen = CANXL_MAX_DLEN;
        mtu = CANXL_MTU;
        data = cu->xl.data;

        if ((cs[idx + 2] != XL_HDR_DELIM) || (cs[idx + 5] != XL_HDR_DELIM))
            return 0;

        for (int i = 0; i < 2; i++) {
            if (!isxdigit(cs[idx++]))
                return 0;
            cu->xl.flags |= asc2nibble(cs[idx - 1]) << (1 - i) * 4;
        }

        idx++;

        for (int i = 0; i < 2; i++) {
            if (!isxdigit(cs[idx++]))
                return 0;
            cu->xl.sdt |= asc2nibble(cs[idx - 1]) << (1 - i) * 4;
        }

        idx++;

        for (int i = 0; i < 8; i++) {
            if (!isxdigit(cs[idx++]))
                return 0;
            cu->xl.af |= asc2nibble(cs[idx - 1]) << (7 - i) * 4;
        }

        idx++;
    }

    for (int i = 0; i < maxdlen; i++) {
        if (cs[idx] == DATA_SEPERATOR)
            idx++;

        if (!isxdigit(cs[idx]))
            return 0;
        data[i] = asc2nibble(cs[idx++]) << 4;

        if (!isxdigit(cs[idx]))
            return 0;
        data[i] |= asc2nibble(cs[idx++]);

        dlen++;
    }

    if (mtu == CANXL_MTU)
        cu->xl.len = dlen;
    else
        cu->fd.len = dlen;

    if ((maxdlen == CAN_MAX_DLEN) && (dlen == CAN_MAX_DLEN) && (cs[idx++] == CC_DLC_DELIM)) {
        unsigned char dlc = asc2nibble(cs[idx]);
        if ((dlc > CAN_MAX_DLEN) && (dlc <= CAN_MAX_RAW_DLC))
            cu->cc.len8_dlc = dlc;
    }

    return mtu;
}
