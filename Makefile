OUT := i8080
CC := gcc
SRC := src/i8080.c src/main.c
OBJ = src/i8080.o src/main.o

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) -o $(OUT) $(OBJ)

clean:
	rm -f $(OBJ) $(OUT)


.PHONY: clean