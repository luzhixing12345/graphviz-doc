CC=gcc

CFLAGS=`pkg-config libgvc --cflags` -Wall -g -O2
# -I/usr/include/graphviz

LDFLAGS=`pkg-config libgvc --libs`
# `sdl2-config --cflags --libs`
# -lgvc -lcgraph -lcdt -lSDL2

RM = rm

.PHONY: clean

SRCS := $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = $(SRCS:.c=)

all: $(TARGET)

%: %.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

clean:
	rm example? *.png