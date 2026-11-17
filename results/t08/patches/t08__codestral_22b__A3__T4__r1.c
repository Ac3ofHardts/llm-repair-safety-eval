int parse_canframe(char *cs, union cfu *cu)
{
    int i, idx, dlen, len;
    int maxdlen = CAN_MAX_DLEN;
    int mtu = CAN_MTU;
    __u8 *data = cu->fd.data; /* fill CAN CC/FD data by default */
    canid_t tmp;

    len = strlen(cs);

    memset(cu, 0, sizeof(*cu)); /* init CAN CC/FD/XL frame, e.g. LEN = 0 */

    if (len < 4)
        return 0;

    if (cs[3] == CANID_DELIM) { /* 3 digits SFF */
        idx = 4;
        for (i = 0; i < 3; i++) {
            if ((tmp = asc2nibble(cs[i])) > 0x0F)
                return 0;
            cu->cc.can_id |= tmp << (2 - i) * 4;
        }
    } else if (cs[8] == CANID_DELIM) { /* 8 digits EFF */
        idx = 9;
        for (i = 0; i < 8; i++) {
            if ((tmp = asc2nibble(cs[i])) > 0x0F)
                return 0;
            cu->cc.can_id |= tmp << (7 - i) * 4;
        }
        if (!(cu->cc.can_id & CAN_ERR_FLAG)) /* 8 digits but no errorframe?  */
            cu->cc.can_id |= CAN_EFF_FLAG;   /* then it is an extended frame */
    } else
        return 0;

    if ((cs[idx] == 'R') || (cs[idx] == 'r')) { /* RTR frame */
        cu->cc.can_id |= CAN_RTR_FLAG;
        idx++;
        /* check for optional DLC value for CAN 2.0B frames */
        if (cs[idx] && (tmp = asc2nibble(cs[idx++])) <= CAN_MAX_DLEN) {
            cu->cc.len = tmp;
            /* check for optional raw DLC value for CAN 2.0B frames */
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
        if ((tmp = asc2nibble(cs[idx + 1])) > 0x0F)
            return 0;
        cu->fd.flags = tmp;
        cu->fd.flags |= CANFD_FDF; /* dual-use */
        idx += 2;
    }

    for (i = 0, dlen = 0; i < maxdlen; i++) {
        if (cs[idx] == DATA_SEPERATOR) /* skip (optional) separator */
            idx++;

        if (idx >= len) /* end of string => end of data */
            break;

        if ((tmp = asc2nibble(cs[idx++])) > 0x0F)
            return 0;
        data[i] = tmp << 4;
        if ((tmp = asc2nibble(cs[idx++])) > 0x0F)
            return 0;
        data[i] |= tmp;
        dlen++;
    }

    cu->fd.len = dlen;

    /* check for extra DLC when having a Classic CAN with 8 bytes payload */
    if ((maxdlen == CAN_MAX_DLEN) && (dlen == CAN_MAX_DLEN) && (cs[idx++] == CC_DLC_DELIM)) {
        unsigned char dlc = asc2nibble(cs[idx]);
        if ((dlc > CAN_MAX_DLEN) && (dlc <= CAN_MAX_RAW_DLC))
            cu->cc.len8_dlc = dlc;
    }

    return mtu;
}
