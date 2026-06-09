CC=clang
CFLAGS=-std=c11 -Wall -Wextra -O2 -g
LDFLAGS=
TARGET=dnp3
PREFIX=/usr/local

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

install: $(TARGET)
	install -d $(PREFIX)/bin
	install -m 755 $(TARGET) $(PREFIX)/bin

clean:
	rm -f $(TARGET) $(OBJ)
