LINK=/usr/local/lib/raylib/

all:
	gcc ./teletype.c -o teletype -L$(LINK) -lraylib