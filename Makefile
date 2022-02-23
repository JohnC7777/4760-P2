CC		= gcc
CFLAGS		= -g
TARGET		= chain
OBJECTS		= simplechain.o
.SUFFIXES: .c .o

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

.c.o: simplechain.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	/bin/rm -f *.o $(TARGET)
