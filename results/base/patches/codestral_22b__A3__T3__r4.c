int parse_canframe(char *cs, union cfu *cu)
{
    if (cs == NULL || cu == NULL)
        return 0;

    int i, idx, dlen, len;
    int maxdlen = CAN_MAX_DLEN;
    int mtu = CAN_MTU;
    __u8 *data = cu->fd.data;

    len = strlen(cs);

    memset(cu, 0, sizeof(*cu));

    if (len < 4)
        return 0;

    if (cs[3] == CANID_DELIM) {
        idx = 4;
        for (i = 0; i < 3; i++) {
            if (!isxdigit(cs[i]))
                return 0;
            cu->cc.can_id |= (unsigned char)strtol(&cs[i], NULL, 16) << (2 - i) * 4;
        }
    } else if (cs[5] == CANID_DELIM) {
        idx = 6;
        for (i = 0; i < 5; i++) {
            if (!isxdigit(cs[i]))
                return 0;
            cu->xl.prio |= (unsigned char)strtol(&cs[i], NULL, 16) << (4 - i) * 4;
        }
        unsigned char tmp = (cu->xl.prio << 4) & CANXL_VCID_MASK;
        cu->xl.prio &= CANXL_PRIO_MASK;
        cu->xl.prio |= tmp;
    } else if (cs[8] == CANID_DELIM) {
        idx = 9;
        for (i = 0; i < 8; i++) {
            if (!isxdigit(cs[i]))
                return 0;
            cu->cc.can_id |= (unsigned char)strtol(&cs[i], NULL, 16) << (7 - i) * 4;
        }
        if (!(cu->cc.can_id & CAN_ERR_FLAG))
            cu->cc.can_id |= CAN_EFF_FLAG;
    } else {
        return 0;
    }

    if ((cs[idx] == 'R') || (cs[idx] == 'r')) {
        cu->cc.can_id |= CAN_RTR_FLAG;
        if (cs[++idx] && (isxdigit(cs[idx]))) {
            cu->cc.len = (unsigned char)strtol(&cs[idx], NULL, 16);
            if ((cu->cc.len == CAN_MAX_DLEN) && (cs[++idx] == CC_DLC_DELIM) && isxdigit(cs[idx])) {
                unsigned char dlc = (unsigned char)strtol(&cs[idx], NULL, 16);
                if ((dlc > CAN_MAX_DLEN) && (dlc <= CAN_MAX_RAW_DLC))
                    cu->cc.len8_dlc = dlc;
            }
        }
        return mtu;
    }

    if (cs[idx] == CANID_DELIM) {
        maxdlen = CANFD_MAX_DLEN;
        mtu = CANFD_MTU;
        if (!isxdigit(cs[idx + 1]))
            return 0;
        cu->fd.flags = (unsigned char)strtol(&cs[idx + 1], NULL, 16);
        cu->fd.flags |= CANFD_FDF;
        idx += 2;
    } else if (cs[idx + 14] == CANID_DELIM) {
        maxdlen = CANXL_MAX_DLEN;
        mtu = CANXL_MTU;
        data = cu->xl.data;
        if ((cs[idx + 2] != XL_HDR_DELIM) || (cs[idx + 5] != XL_HDR_DELIM))
            return 0;
        if (!isxdigit(cs[idx]) || !isxdigit(cs[idx + 1]))
            return 0;
        cu->xl.flags = (unsigned char)strtol(&cs[idx], NULL, 16) << 4;
        idx += 2;
        if (!isxdigit(cs[idx]))
            return 0;
        cu->xl.flags |= (unsigned char)strtol(&cs[idx], NULL, 16);
        cu->xl.flags |= CANXL_XLF;
        idx += 2;
        if (!isxdigit(cs[idx]) || !isxdigit(cs[idx + 1]))
            return 0;
        cu->xl.sdt = (unsigned char)strtol(&cs[idx], NULL, 16) << 4;
        idx += 2;
        if (!isxdigit(cs[idx]) || !isxdigit(cs[idx + 1]))
            return 0;
        cu->xl.sdt |= (unsigned char)strtol(&cs[idx], NULL, 16);
        idx += 2;
        for (i = 0; i < 8; i++) {
            if (!isxdigit(cs[idx]))
                return 0;
            cu->xl.af |= (unsigned char)strtol(&cs[idx], NULL, 16) << (7 - i) * 4;
            idx += 1;
        }
        idx += 1;
    }

    for (i = 0, dlen = 0; i < maxdlen; i++) {
        if (cs[idx] == DATA_SEPERATOR)
            idx++;

        if (idx >= len)
            break;

        if (!isxdigit(cs[idx]) || !isxdigit(cs[idx + 1]))
            return 0;
        data[i] = (unsigned char)strtol(&cs[idx], NULL, 16) << 4;
        idx += 2;
        data[i] |= (unsigned char)strtol(&cs[idx], NULL, 16);
        idx += 1;
        dlen++;
    }

    if (mtu == CANXL_MTU)
        cu->xl.len = dlen;
    else
        cu->fd.len = dlen;

    if ((maxdlen == CAN_MAX_DLEN) && (dlen == CAN_MAX_DLEN) && (cs[idx++] == CC_DLC_DELIM) && isxdigit(cs[idx])) {
        unsigned char dlc = (unsigned char)strtol(&cs[idx], NULL, 16);
        if ((dlc > CAN_MAX_DLEN) && (dlc <= CAN_MAX_RAW_DLC))
            cu->cc.len8_dlc = dlc;
    }

    return mtu;
}
