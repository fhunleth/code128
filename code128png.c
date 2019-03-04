// Copyright (c) 2015, LKC Technologies, Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer. Redistributions in binary
// form must reproduce the above copyright notice, this list of conditions and
// the following disclaimer in the documentation and/or other materials
// provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
// HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
// OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdio.h>
#include <err.h>
#include <stdlib.h>

#include <png.h>

#include "code128.h"

static void png_error_callback(png_structp png_ptr, const char *msg)
{
    (void) png_ptr;
    errx(EXIT_FAILURE, "libpng: %s", msg);
}

static void png_warning_callback(png_structp png_ptr, const char *msg)
{
    (void) png_ptr;
    warnx("libpng: %s", msg);
}

int main(int argc, char *argv[])
{
    char out[4096];
    int width;
    int height = 40;

    if (argc < 3) {
        printf("%s <output.png> <string to encode>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *str = argv[2];
    width = code128_estimate_len(str);
    width = code128_encode_gs1(str, out, width);

    if (width == 0)
        errx(EXIT_FAILURE, "Invalid characters in string");

    FILE *fp = fopen(argv[1], "wb");
    if (!fp)
        err(EXIT_FAILURE, "can't open output");

    png_structp png_ptr = png_create_write_struct
                          (PNG_LIBPNG_VER_STRING, NULL,
                           png_error_callback, png_warning_callback);
    if (!png_ptr)
        errx(EXIT_FAILURE, "png_create_write_struct");

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr,
                                 (png_infopp)NULL);
        errx(EXIT_FAILURE, "png_create_info_struct");
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        errx(EXIT_FAILURE, "libpng error");
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 1, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_text note;
    memset(&note, 0, sizeof(note));
    note.compression = PNG_TEXT_COMPRESSION_NONE;
    note.key = "gs1-128";
    note.text = argv[1];
    note.text_length = strlen(argv[1]);

    png_set_text(png_ptr, info_ptr, &note, 1);
    png_write_info(png_ptr, info_ptr);
    png_set_packing(png_ptr);
    png_set_invert_mono(png_ptr);

    png_byte *row_pointers[height];
    int i;
    for (i = 0; i < height; i++)
        row_pointers[i] = (png_byte*) out;
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    fclose(fp);
    return 0;
}

