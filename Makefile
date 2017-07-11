CC=gcc
CFLAGS=
LDFLAGS=
SOURCES=src/main.c src/cdindex.c src/graph.c src/utility.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bin/cdindex

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

c.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm src/*o
