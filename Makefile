CFLAGS = -Wall -std=c99

all: rr

rr: file_manager.c check_func.c list_builder.c extra.c
	gcc $(CFLAGS) $^ -o $@












