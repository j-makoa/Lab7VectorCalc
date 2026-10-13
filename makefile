# Makefile - CPE-2600 Lab 7 Vector Calculator Updates
#
#   make         build myvectorcalc
#   make run     build and run
#   make clean   remove object files and executable

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g
TARGET  = myvectorcalc
SRCS    = main.c ui.c vectstore.c vect.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Header dependencies: recompile when a header changes
main.o:      main.c ui.h vectstore.h vect.h
ui.o:        ui.c ui.h vect.h vectstore.h
vectstore.o: vectstore.c vectstore.h vect.h
vect.o:      vect.c vect.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
