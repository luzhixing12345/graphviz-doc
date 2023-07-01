
.PHONY: all

DOT_FILES := $(wildcard *.gv)
PICTURES = $(DOT_FILES:.gv=.png)
all: $(PICTURES)

%.png: %.gv
	dot -Tpng $^ -o $@