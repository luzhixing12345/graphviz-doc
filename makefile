CC=gcc

# CFLAGS=`pkg-config libgvc --cflags` -Wall -g -O2
# -I/usr/include/graphviz

# LDFLAGS=`pkg-config libgvc --libs`
# `sdl2-config --cflags --libs`
# -lgvc -lcgraph -lcdt -lSDL2

RM = rm

.PHONY: clean

CFLAGS = -IE:\\Graphviz\\include

LDFLAGS = -lgvc -lcgraph -lcdt

main: main.c
	$(CC) $^ $(CFLAGS) -LE:\\Graphviz\\lib $(LDFLAGS) -o $@