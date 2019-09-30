CC=gcc

SRC = $(wildcard *.c) $(wildcard */*.c)
OBJ = $(addprefix ./,$(notdir $(SRC:.c=.o)))

CFLAGS = -I ../../include -std=gnu99 -Wall -Werror -Wno-unused -g 
LFLAGS = -g -L../../ ../../libage.a -lncurses

OUT = test

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $@


%.o: %.c
	$(CC) $< -c $(CFLAGS) -o $@

	
clean:
	rm *.o $(OUT)