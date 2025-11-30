# Makefile portable para LRU
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = pvv

all: $(TARGET)

$(TARGET): pvv.c
	$(CC) $(CFLAGS) -o $(TARGET) pvv.c functions.c

clean:
	del /f /q $(TARGET).exe 2>nul || rm -f $(TARGET)

.PHONY: all clean
