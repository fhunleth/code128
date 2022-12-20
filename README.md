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

## Compiling and testing

The main library, `code128.[ch]`, doesn't have any dependencies. The test
program depends on `libpng`, so make sure to install that first. On
Debian/Ubuntu, run:

```sh
sudo apt install libpng-dev
```

On Windows, with MSYS2, run:
```sh
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-libpng
```

Details on how to install and setup MSYS2 environment on Windows can be found
[here](https://code.visualstudio.com/docs/languages/cpp) and
[here](https://www.freecodecamp.org/news/how-to-install-c-and-cpp-compiler-on-windows/) .


Then run `make`. The result is a test program that creates `png` files of
barcode data passed on the commandline.

On Windows, to build the libraries and the test program for different architectures,
you should open Powershell and run:

```sh
mingw-32-make.exe all
```


The regression tests require [zbar](http://zbar.sourceforge.net/). Install
`zbar` on Debian/Ubuntu by:

```sh
sudo apt install zbar-tools
```

Or on OSX:

```sh
brew install zbar
```

Then run `make check`.

## Integrating

If you're using C or C++, the easiest way of integrating this code into your
program is to just copy code128.[ch] to your tree. If you're not using C,
then calling `code128png` from your app may be an option as well.
