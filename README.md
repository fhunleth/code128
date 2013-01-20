# Trivial Code 128 Barcode Encoder

Almost every other barcode encoder library is
better than this one, but this has the advantage
of having almost no dependencies. Given a text
string, it will return a one dimensional array
where each entry is the pixel value for a row
in the barcode. Repeat the row to make the barcode
taller. That's it.

# Compiling

This tree currently just compiles to a quick unit test. 
To integrate in your application, it's probably easiest
to just copy code128.[ch] to your tree.

