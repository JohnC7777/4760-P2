GCC = gcc
CFLAGS = -g -Wall -Wshadow
SRC = $(wildcard *.c)
TAR = $(SRC:.c=.o)
EXEC = $(SRC:.c=)
LOGS = $(wildcard logfile.*)

.PHONY: all clean

all: $(TAR)

%.o: %.c
	$(GCC) $(CFLAGS) -c $<
	$(GCC)  -o $* $@

clean:
	rm -f $(TAR)
	rm -f $(EXEC)
	rm -f $(LOGS)
	rm -f cstest


#C		= gcc
#CFLAGS		= -g
#TARGET		= master slave
#OBJECTS		= master.o slave.o
#.SUFFIXES: .c .o

#$(TARGET): $(OBJECTS)
#	$(CC) -o $@ $(OBJECTS)

#.c.o: master.c
#	$(CC) $(CFLAGS) -c $<
	
#.c.o: slave.c
#	$(CC) $(CFLAGS) -c $<

#.PHONY: clean
#clean:
#	/bin/rm -f *.o $(TARGET)
