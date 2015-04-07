#!/bin/sh

# Code128 unit test
#
# This test uses zbarimg to check the encoding. The tool doesn't
# report the FNC1 code, so the tests are lacking, but most other
# characters used in real barcodes are covered.
#
# To install zbarimg on Debian/Ubuntu, run:
#
#     sudo apt-get install zbar-tools
#

TEST_STRINGS="A AA AAA aaaa AaAa abcd ABCD\
              0 01 012 0123 01234 012345 A0B1C2\
              abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ\
              !%&()*+,-./ :;<=>?@ [\\]^_ {|}~\
              A11B22C33D44E55f66G77h88I99J00 \
	      a1b22c333d4444e55555f666666g7777777h88888888i999999999j0000000000 \
              112012120721R00012
	      "

	      #a1b22c333d4444e55555f666666g7777777h88888888i999999999j0000000000a1b22c333d4444e55555f666666g7777777h88888888i999999999j0000000000 \
# zbarimg is buggy. The following strings have to be tested manually.
ZBARIMG_BROKE_STRINGS="a aa 1234"

for str in $TEST_STRINGS; do
    if ! ./code128png test.png "${str}"; then
        echo "Encode of '${str}' failed."
        exit 1
    fi
     
    result=$(zbarimg -q --raw test.png)
    if [ "${result}" != "${str}" ]; then
        echo "Verification of '${str}' failed. Got '${result}'"
        exit 1
    fi
done
rm -f test.png

echo "Success"

