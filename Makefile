# -Wno-implicit-fallthrough - the implicit fallthrough is intended so turn off warning
CFLAGS ?= -O2 -Wall -Wextra -Wno-implicit-fallthrough

MAJOR := 1
MINOR := 1
NAME := code128
VERSION := $(MAJOR).$(MINOR)

LIBSO  = lib$(NAME).so
LIBSOM = $(LIBSO).$(MAJOR)
LIBSOV = $(LIBSO).$(VERSION)
LIBA   = lib$(NAME).a


CODE128PNG_INCLUDE_DIRS :=
CODE128PNG_LIBRARY_DIRS :=
CODE128PNG_LIBRARIES := png

CPPFLAGS += $(foreach includedir,$(CODE128PNG_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(CODE128PNG_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(CODE128PNG_LIBRARIES),-l$(library))


all: code128png lib

code128png: code128png.o code128.o
	$(CC) $^ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@

lib: $(LIBSO) $(LIBA)

$(LIBSO): $(LIBSOV)
	ln -s $(LIBSOV) $(LIBSO)

$(LIBSOV): $(NAME).o
	$(CC) $(CFLAGS) -shared -Wl,-soname,$(LIBSOM) -o $(LIBSOV) $(NAME).o

$(LIBA): $(NAME).o
	$(AR) $(ARFLAGS) $(LIBA) $(NAME).o

clean:
	$(RM) -f code128png *.o *.so* *.a

format-code:
	astyle --indent-preproc-block *.c *.h

check test: code128png
	./test.sh

.PHONY: clean format-code all check test
