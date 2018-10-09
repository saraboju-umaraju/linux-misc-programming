GS = -Wall -Wextra -std=c99 -g
CPPFLAGS = -I.
LDFLAGS =

EXEC = hello

all: $(EXEC) po/fr/$(EXEC).mo

$(EXEC): $(EXEC).c 
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LDFLAGS)

po/fr/$(EXEC).mo: po/fr/$(EXEC).po
	msgfmt --output-file=$@ $<

po/fr/$(EXEC).po: po/$(EXEC).pot
	msgmerge --update $@ $<

po/$(EXEC).pot: $(EXEC).c
	xgettext -k_ -j -lC -c -s -o po/hello.pot hello.c

clean:
	@rm -f $(EXEC) po/fr/*.mo *~

.PHONY: clean
