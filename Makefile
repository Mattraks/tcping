LDFLAGS+=
CFLAGS+=-O3 -Wall

VERSION=0.3

OBJS=io.o error.o main.o tcp.o

all: tcping

tcping: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o tcping
	strip tcping

install: tcping
	cp tcping $(DESTDIR)/usr/bin

clean:
	rm -f $(OBJS) core tcping

package: clean
	# source package
	rm -rf tcping-$(VERSION)*
	mkdir tcping-$(VERSION)
	cp *.c *.h Makefile readme.txt license.txt tcping-$(VERSION)
	tar czf tcping-$(VERSION).tgz tcping-$(VERSION)
	rm -rf tcping-$(VERSION)
