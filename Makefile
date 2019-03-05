
CC = gcc
CC_OPTS = -Wall -g
CC_LIBS = # Ha ha! None!

IN_FILES = $(wildcard src/*.c)
OUT_FILE = bin/fx-meta

all: $(IN_FILES)
	mkdir -p bin
	$(CC) $(IN_FILES) $(CC_OPTS) `pkg-config --cflags $(CC_LIBS)` `pkg-config --libs $(CC_LIBS)` -o $(OUT_FILE)
