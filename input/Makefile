INSTALLFILES = $(wildcard *.dat)

.PHONY: all install

all:

install:
	@echo 'Installing data files for converters...'
	mkdir -p -m 0755 $(DESTDIR)/input
	install -m 0644 $(INSTALLFILES) $(DESTDIR)/input

clean:

