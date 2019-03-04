
CFLAGS ?= -O2 -Wall -Wextra

all: code128png

code128png: code128png.o code128.o
	$(CC) $^ -lpng -o $@

clean:
	rm -f code128png *.o

format-code:
	astyle *.c *.h

check test:
	./test.sh

.PHONY: clean format-code all check test
