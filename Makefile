C		= gcc
CFLAGS		= -g
TARGET		= master
OBJECTS		= master.o
.SUFFIXES: .c .o

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

.c.o: master.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	/bin/rm -f *.o $(TARGET)
