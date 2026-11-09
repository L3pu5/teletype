FILES=$(wildcard ./src/*.c)
LINK=/usr/local/lib/raylib/
INCLUDE=/usr/local/include/raylib

all:
	gcc $(FILES) -I$(INCLUDE) -L$(LINK) -lraylib -lm  -o ./bin/teletype 

clean:
	rm ./bin/teletype