BINARIES := cerr deansi escape watchfile

DESTDIR := $(HOME)
bindir := /bin

INSTALL_BINARIES := $(addprefix $(DESTDIR)$(bindir)/,$(BINARIES))

CFLAGS += -O3

all: $(BINARIES)

$(DESTDIR)$(bindir)/%: %
	install --mode=755 $? $(DESTDIR)$(bindir)

install: $(INSTALL_BINARIES)

uninstall:
	rm $(INSTALL_BINARIES)

clean:
	rm -f $(BINARIES)
