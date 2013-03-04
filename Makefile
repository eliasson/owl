CFLAGS = -std=c99 -Wall -I/usr/local/include/
LDLIBS = -lpthread -lspotify -levent
LDFLAGS = -Wl,-rpath,/usr/local/lib -L/usr/local/lib/

SOURCES = src/logging.c src/utils.c src/queue.c src/owl.c src/spotify.c
TEST_SOURCES = src/logging.c src/utils.c src/queue.c test/test.c

.SILENT: owl test unittest clean clean-test

all: clean owl

update: clean owl test

owl:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(SOURCES) $(LDFLAGS) -o $@ $(LDLIBS)

test: clean-test unittest

unittest:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(TEST_SOURCES) $(LDFLAGS) -o $@ $(LDLIBS)
	./unittest

clean:
	rm -f *.o owl
	rm -rf .settings .cache
	rm -f unittest
	
clean-test:
	rm -f unittest
