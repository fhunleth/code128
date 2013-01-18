// Copyright (c) 2011-2012 LKC Technologies, Inc.  All rights reserved.
// LKC Technologies, Inc. PROPRIETARY AND CONFIDENTIAL

#ifndef CODE128_H
#define CODE128_H

#include <stddef.h>

size_t code128_len(const char *s);
size_t code128a_encode(const char *s, char *out, size_t maxlength);

#endif // CODE128_H
