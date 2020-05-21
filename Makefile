PROGS = unix_client unix_server

all: $(PROGS) $(DOC)

srv: unix_server.c
cli: unix_client.c

.PHONY: clean

clean:
	rm -f *.o socket $(PROGS)