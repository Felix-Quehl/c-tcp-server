SHELL := /bin/bash

output_path = ./bin
sources_path = ./sources
headers_path = ./headers
name = a.out
target = $(output_path)/$(name)

CC = clang
CFLAGS = -std=c89 -Weverything -Wall -Wextra -Werror -Wpedantic -I$(headers_path)
LDFLAGS = -I$(headers_path) -lpthread

src = $(shell find $(sources_path) -name '*.c')
obj = $(src:.c=.o)

trash = $(obj) $(target)

all: debug

debug: CFLAGS += -g
debug: LDFLAGS += -g
debug: $(target)
$(target): $(output_path) $(obj)
	$(CC) $(LDFLAGS) -o "$(target)" $(obj)

$(output_path):
	mkdir -p $(output_path)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(trash)

.ONESHELL: