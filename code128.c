// Copyright (c) 2011-2012 LKC Technologies, Inc.  All rights reserved.
// LKC Technologies, Inc. PROPRIETARY AND CONFIDENTIAL

#include "code128.h"
#include <string.h>
#include <assert.h>

#define CODE128_QUIET_ZONE_LEN 10
#define CODE128_CHAR_LEN       11
#define CODE128_STOP_CODE_LEN  13

#define CODE128_MIN_ENCODE_LEN (CODE128_QUIET_ZONE_LEN * 2 + CODE128_CHAR_LEN * 2 + CODE128_STOP_CODE_LEN)

static const int code128_pattern[] = {
    1740,
    1644,
    1638,
    1176,
    1164,
    1100,
    1224,
    1220,
    1124,
    1608,
    1604,
    1572,
    1436,
    1244,
    1230,
    1484,
    1260,
    1254,
    1650,
    1628,
    1614,
    1764,
    1652,
    1902,
    1868,
    1836,
    1830,
    1892,
    1844,
    1842,
    1752,
    1734,
    1590,
    1304,
    1112,
    1094,
    1416,
    1128,
    1122,
    1672,
    1576,
    1570,
    1464,
    1422,
    1134,
    1496,
    1478,
    1142,
    1910,
    1678,
    1582,
    1768,
    1762,
    1774,
    1880,
    1862,
    1814,
    1896,
    1890,
    1818,
    1914,
    1602,
    1930,
    1328,
    1292,
    1200,
    1158,
    1068,
    1062,
    1424,
    1412,
    1232,
    1218,
    1076,
    1074,
    1554,
    1616,
    1978,
    1556,
    1146,
    1340,
    1212,
    1182,
    1508,
    1268,
    1266,
    1956,
    1940,
    1938,
    1758,
    1782,
    1974,
    1400,
    1310,
    1118,
    1512,
    1506,
    1960,
    1954,
    1502,
    1518,
    1886,
    1966,
    1668,
    1680,
    1692
};

const int code128_start_code_set_a = 103;
const int code128_start_code_set_b = 104;
const int code128_start_code_set_c = 105;
const int code128_stop_pattern = 6379;

static int code128a_ascii_to_code(char value)
{
    if (value >= 32 && value <= 93)
        return value - 32;
    else if (value < 32)
        return value + 64;
    else
        return -1;
}

static int code128b_ascii_to_code(char value)
{
    if (value >= 32 && value <= 127)
        return value - 32;
    else
        return -1;
}

size_t code128_len(const char *s)
{
    return CODE128_QUIET_ZONE_LEN
            + CODE128_CHAR_LEN
            + CODE128_CHAR_LEN * strlen(s)
            + CODE128_CHAR_LEN
            + CODE128_STOP_CODE_LEN
            + CODE128_QUIET_ZONE_LEN;
}

static void code128_append_pattern(int pattern, int pattern_length, char *out)
{
    // All patterns have their first bit set by design
    assert(pattern & (1 << (pattern_length - 1)));

    int i;
    for (i = pattern_length - 1; i >= 0; i--)
        *out++ = (pattern & (1 << i)) ? 255 : 0;
}

static int code128_append_code(int code, char *out)
{
    assert(code >= 0 && code < (int) sizeof(code128_pattern));
    code128_append_pattern(code128_pattern[code], CODE128_CHAR_LEN, out);
    return CODE128_CHAR_LEN;
}

static int code128_append_stop_code(char *out)
{
    code128_append_pattern(code128_stop_pattern, CODE128_STOP_CODE_LEN, out);
    return CODE128_STOP_CODE_LEN;
}

size_t code128_encode(const char *s, char *out, size_t maxlength, int startcode, int (*ascii_to_code)(char))
{
    size_t len = code128_len(s);

    // Check that output buffer is long enough.
    if (maxlength < len)
        return 0;

    // Initial quiet zone
    memset(out, 0, CODE128_QUIET_ZONE_LEN);
    out += CODE128_QUIET_ZONE_LEN;

    // Start character
    out += code128_append_code(startcode, out);
    int sum = startcode;

    // Encoded data
    int index = 1;
    while (*s) {
        int code = ascii_to_code(*s);
        if (code < 0)
            return 0;
        s++;

        sum += index * code;
        index++;
        out += code128_append_code(code, out);
    }
    int checkdigit = sum % 103;

    // Check character
    out += code128_append_code(checkdigit, out);

    // Stop character
    out += code128_append_stop_code(out);

    // Final quiet zone
    memset(out, 0, CODE128_QUIET_ZONE_LEN);

    return len;
}

size_t code128a_encode(const char *s, char *out, size_t maxlength)
{
    return code128_encode(s, out, maxlength, code128_start_code_set_a, code128a_ascii_to_code);
}

size_t code128b_encode(const char *s, char *out, size_t maxlength)
{
    return code128_encode(s, out, maxlength, code128_start_code_set_b, code128b_ascii_to_code);
}
