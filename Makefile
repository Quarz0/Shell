#
# Makefile
#
# Shell
#


# compiler to use
CC = gcc

# flags to pass compiler
CFLAGS = -ggdb -std=c99 -Wall -Werror

# name for executable
EXE = shell

# space-separated list of header files
HDRS = command_parser.h commands.h environment.h file_processing.h trie.h variables.h

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS =

# space-separated list of source files
SRCS = command_parser.c commands.c environment.c file_processing.c trie.c variables.c main.c

# automatically generated list of object files
OBJS = $(SRCS:.c=.o)


# default target
$(EXE): $(OBJS) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# dependencies 
$(OBJS): $(HDRS) Makefile

# housekeeping
clean:
	rm -f core $(EXE) *.o
