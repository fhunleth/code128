# GS1-128/Code 128 barcode encoder

This is a simple [GS1-128](http://en.wikipedia.org/wiki/GS1-128) barcode
encoder library. The reason to choose it over others is simply that it is
written in pure C code and has no external dependencies. That's right - it
doesn't even depend on a graphics library. As a bonus, the barcodes produced
by this program will automatically use the Code 128 A, B, or C modes based
on what generates the shortest barcode.

Here's how to use:

```C
    const char *str = "[FNC1] 00 12345678 0000000001";
    size_t barcode_length = code128_estimate_len(str);
    char *barcode_data = (char *) malloc(barcode_length);
    int i;

    barcode_length = code128_encode_gs1(str, barcode_data, barcode_length);

    /* barcode_length is now the actual number of "bars". */
    for (i = 0; i < barcode_length; i++) {
        if (barcode_data[i])
            draw_vertical_bar_at_column(i);
    }
```

Once you get `barcode_data`, each byte corresponds to whether a vertical
line should be drawn. If the byte is 0xff, then draw a line. If the byte is
0x00, then don't.

## Compiling

To build on Linux, just run `make`. The result is a test program that creates
`png` files of barcode data passed on the commandline.

To verify that nothing went wrong, run `./test.sh` to try encoding barcodes
and decoding them with a 3rd party tool. You'll need to install zbar-tools.

## Integrating

If you're using C or C++, the easiest way of integrating this code into your
program is to just copy code128.[ch] to your tree. If you're not using C,
then calling `code128png` from your app may not be too difficult.

