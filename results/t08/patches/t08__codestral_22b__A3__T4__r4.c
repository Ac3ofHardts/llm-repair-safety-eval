int parse_canframe(char *cs, union cfu *cu)
{
    int i, idx, dlen, len;
    int maxdlen = CAN_MAX_DLEN;
    int mtu = CAN_MTU;
    unsigned char *data = cu->fd.data;

    len = strlen(cs);

    memset(cu, 0, sizeof(*cu));

    if (len < 4)
        return 0;

    idx = 0;
    for (i = 0; i < 3; i++) {
        if ((cu->cc.can_id = (cu->cc.can_id << 4) | asc2nibble(cs[idx++])) > 0x7FF)
            return 0;
    }

    if (cs[idx++] != CANID_DELIM)
        return 0;

    if (cs[idx] == 'R' || cs[idx] == 'r') {
        cu->cc.can_id |= CAN_RTR_FLAG;
        idx++;
        if (cs[idx] && (cu->cc.len = asc2nibble(cs[idx++])) <= CAN_MAX_DLEN) {
            if (cu->cc.len == CAN_MAX_DLEN && cs[idx++] == CC_DLC_DELIM) {
                unsigned char dlc = asc2nibble(cs[idx]);
                if (dlc > CAN_MAX_DLEN && dlc <= CAN_MAX_RAW_DLC)
                    cu->cc.len8_dlc = dlc;
            }
        }
        return mtu;
    }

    if (cs[idx] == '#') {
        if (cs[idx + 1] == '#') {
            maxdlen = CANFD_MAX_DLEN;
            mtu = CANFD_MTU;
            idx += 2;
            if ((cu->fd.flags = asc2nibble(cs[idx++])) > 0x0F)
                return 0;
            cu->fd.flags |= CANFD_FDF;
        } else if (cs[idx + 14] == CANID_DELIM) {
            maxdlen = CANXL_MAX_DLEN;
            mtu = CANXL_MTU;
            data = cu->xl.data;
            idx++;
            if ((cu->xl.flags = (asc2nibble(cs[idx++]) << 4) | asc2nibble(cs[idx++])) > 0xFF)
                return 0;
            cu->xl.flags |= CANXL_XLF;
            idx++;
            if ((cu->xl.sdt = (asc2nibble(cs[idx++]) << 4) | asc2nibble(cs[idx++])) > 0xFF)
                return 0;
            idx++;
            for (i = 0; i < 8; i++) {
                if ((cu->xl.af = (cu->xl.af << 4) | asc2nibble(cs[idx++])) > 0xFFFFFFFF)
                    return 0;
            }
            idx++;
        } else {
            cu->cc.can_id |= CAN_EFF_FLAG;
            for (i = 0; i < 8; i++) {
                if ((cu->cc.can_id = (cu->cc.can_id << 4) | asc2nibble(cs[idx++])) > 0x1FFFFFFF)
                    return 0;
            }
            if (cs[idx++] != CANID_DELIM)
                return 0;
        }
    } else {
        return 0;
    }

    for (i = 0, dlen = 0; i < maxdlen && idx < len; i++) {
        if (cs[idx] == DATA_SEPERATOR)
            idx++;
        if ((data[i] = (asc2nibble(cs[idx++]) << 4) | asc2nibble(cs[idx++])) > 0xFF)
            return 0;
        dlen++;
    }

    if (mtu == CANXL_MTU)
        cu->xl.len = dlen;
    else
        cu->fd.len = dlen;

    if (maxdlen == CAN_MAX_DLEN && dlen == CAN_MAX_DLEN && cs[idx++] == CC_DLC_DELIM) {
        unsigned char dlc = asc2nibble(cs[idx]);
        if (dlc > CAN_MAX_DLEN && dlc <= CAN_MAX_RAW_DLC)
            cu->cc.len8_dlc = dlc;
    }

    return mtu;
}
