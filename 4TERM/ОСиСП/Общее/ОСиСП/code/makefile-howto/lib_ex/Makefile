#makefile
CC = gcc
CFLAGS = -std=c11 -g2 -ggdb -pedantic -W -Wall -Wextra 

.SUFFIXES:   
.SUFFIXES: .c .o

DEBUG   = ./build/linux/debug
RELEASE = ./build/linux/release
OUT_DIR = $(DEBUG)
vpath %.c src
vpath %.h src
vpath %.o build/linux/debug

ifeq ($(MODE), release)
  CFLAGS = -std=c11 -pedantic -W -Wall -Wextra -Werror
  OUT_DIR = $(RELEASE)
  vpath %.o build/linux/release
endif


objects =  $(OUT_DIR)/main.o $(OUT_DIR)/lib.o 
#objects =  main.o lib.o 

prog = $(OUT_DIR)/test

all: $(prog) 

$(prog) : $(objects) 
	$(CC) $(CFLAGS) $(objects) -o $@

$(OUT_DIR)/%.o : %.c
	$(CC) -c $(CFLAGS) $^ -o $@

.PHONY: clean 
clean:
	@rm -rf $(DEBUG)/* $(RELEASE)/* test
