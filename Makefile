CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c99 -g
TARGET  = pvv
SRCS    = pvv.c functions.c
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c functions.h structures.h commands.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	del /Q $(TARGET).exe $(OBJS) *.exe *.o 2>nul || true
else
	rm -f $(TARGET) $(OBJS) *.exe *.o
endif

rebuild: clean all

.PHONY: all clean rebuild
