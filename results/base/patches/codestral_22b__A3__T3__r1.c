int parse_canframe(char *cs, union cfu *cu)
{
    int i, idx, dlen, len;
    int maxdlen = CAN_MAX_DLEN;
    int mtu = CAN_MTU;
    __u8 *data = cu->fd.data;
    canid_t tmp;

    len = strlen(cs);

    memset(cu, 0, sizeof(*cu));

    if (len < 4)
        return 0;

    if (cs[3] == CANID_DELIM) {
        idx = 4;
        for (i = 0; i < 3; i++) {
            if ((tmp = asc2nibble(cs[i])) > 0x0F)
                return 0;
            cu->cc.can_id |= tmp << (2 - i) * 4;
        }
    } else if (cs[5] == CANID_DELIM) {
        idx = 6;
        for (i = 0; i < 5; i++) {
            if ((tmp = asc2nibble(cs[i])) > 0x0F)
                return 0;
            cu->xl.prio |= tmp << (4 - i) * 4;
        }
        tmp = (cu->xl.prio << 4) & CANXL_VCID_MASK;
        cu->xl.prio &= CANXL_PRIO_MASK;
        cu->xl.prio |= tmp;
    } else if (cs[8] == CANID_DELIM) {
        idx = 9;
        for (i = 0; i < 8; i++) {
            if ((tmp = asc2nibble(cs[i])) > 0x0F)
                return 0;
            cu->cc.can_id |= tmp << (7 - i) * 4;
        }
        if (!(cu->cc.can_id & CAN_ERR_FLAG))
            cu->cc.can_id |= CAN_EFF_FLAG;
    } else {
        return 0;
    }

    if ((cs[idx] == 'R') || (cs[idx] == 'r')) {
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

    if (cs[idx] == CANID_DELIM) {
        maxdlen = CANFD_MAX_DLEN;
        mtu = CANFD_MTU;
        if ((tmp = asc2nibble(cs[idx + 1])) > 0x0F)
            return 0;
        cu->fd.flags = tmp;
        cu->fd.flags |= CANFD_FDF;
        idx += 2;
    } else if (cs[idx + 14] == CANID_DELIM) {
        maxdlen = CANXL_MAX_DLEN;
        mtu = CANXL_MTU;
        data = cu->xl.data;
        if ((cs[idx + 2] != XL_HDR_DELIM) || (cs[idx + 5] != XL_HDR_DELIM))
            return 0;
        if ((tmp = asc2nibble(cs[idx++])) > 0x0F)
            return 0;
        cu->xl.flags = tmp << 4;
        if ((tmp = asc2nibble(cs[idx++])) > 0x0F)
            return 0;
        cu->xl.flags |= tmp;
        cu->xl.flags |= CANXL_XLF;
        idx++;
        if ((tmp = asc2nibble(cs[idx++])) > 0x0F)
            return 0;
        cu->xl.sdt = tmp << 4;
        if ((tmp = asc2nibble(cs[idx++])) > 0x0F)
            return 0;
        cu->xl.sdt |= tmp;
        idx++;
        for (i = 0; i < 8; i++) {
            if ((tmp = asc2nibble(cs[idx++])) > 0x0F)
                return 0;
            cu->xl.af |= tmp << (7 - i) * 4;
        }
        idx++;
    }

    for (i = 0, dlen = 0; i < maxdlen; i++) {
        if (cs[idx] == DATA_SEPERATOR)
            idx++;
        if (idx >= len)
            break;
        if ((tmp = asc2nibble(cs[idx++])) > 0x0F)
            return 0;
        data[i] = tmp << 4;
        if ((tmp = asc2nibble(cs[idx++])) > 0x0F)
            return 0;
        data[i] |= tmp;
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
