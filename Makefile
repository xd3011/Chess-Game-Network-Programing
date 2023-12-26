CC := gcc
CFLAGS := -Wall -c
SRCS := gpio.c
OBJS := $(SRCS:.c=.o)
TARGET := gpio
all: client.o server.o
server.o: cchess-server.c
	${CC} -o server cchess-server.c -lpthread
client.o: cchess-client.c
	${CC} -o client cchess-client.c -lpthread
.PHONY: clean
clean:
	rm -f $(OBJS)