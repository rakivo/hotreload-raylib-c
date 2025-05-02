SRCDIR = ./src
BUILDDIR = ./build

BIN = $(BUILDDIR)/out
PLUG_DLL = $(BUILDDIR)/libplug.so

CXX = cc

LIBDIR = thirdparty/raylib/lib
INCDIR = thirdparty/raylib/include

RAYLIB = -lraylib
LDLIBS = -lm $(RAYLIB)
LDFLAGS = -L$(LIBDIR)

CFLAGS = -Wall -Wextra -Werror -pedantic -ggdb -fPIC \
         -Wno-missing-field-initializers \
         -I$(INCDIR) -I$(SRCDIR) \
         -DLIB_PLUG_PATH=\"$(PLUG_DLL)\"

.PHONY: all clean

all: $(BIN)

$(BUILDDIR):
	mkdir -p $@

$(PLUG_DLL): $(SRCDIR)/plug.c $(SRCDIR)/plug.h | $(BUILDDIR)
	$(CXX) -o $@ $(CFLAGS) -shared $< $(LDFLAGS) $(LDLIBS)

$(BIN): $(SRCDIR)/main.c | $(PLUG_DLL)
	$(CXX) -o $@ $(CFLAGS) $< $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PLUG_DLL) $(BIN)
