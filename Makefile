CFLAGS = -Wall -std=c99
LDFLAGS = -lncursesw


all: manager

file_manager: file_manager.c check_func.c list_builder.c extra.c rendering_ls.c extra_func.c button_processing.c command_line/command_line.c command_line/buffer_save.c rendering_bool.c rendering_help.c
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)















