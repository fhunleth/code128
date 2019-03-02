
CFLAGS ?= -O2 -Wall -Wextra

all: code128png

# MacOS 'brew search zbar' installs /usr/local/include/png.h
#includes = -I/usr/local/include

#test.sh fails when compiled with unsigned char (e.g. default of arm-none-eabi-gcc)
#CFLAGS  += -funsigned-char

#test.sh succeeds when compiled with signed char (e.g. default of gcc)
#CFLAGS  += -fsigned-char

code128png: code128png.o code128.o
	$(CC) $^ $(includes) -lpng -o $@

clean:
	rm -f code128png *.o

format-code:
	astyle *.c *.h

.PHONY: clean format-code all
