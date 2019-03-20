CC     = gcc
CFLAGS = -g -Wall -Wstrict-prototypes -ansi -pedantic

SRCDIR = src
OBJDIR = obj
BINDIR = bin

COMMON_SRCS = board/board.c
OTHELLO_SRCS = main.c
TESTBOARD_SRCS = tests/testboard.c

COMMON_OBJS = $(COMMON_SRCS:.c=.o)
OTHELLO_OBJS = $(COMMON_OBJS) $(OTHELLO_SRCS:.c=.o)
TESTBOARD_OBJS = $(COMMON_OBJS) $(TESTBOARD_SRCS:.c=.o)

othello: $(addprefix $(OBJDIR)/,$(OTHELLO_OBJS))
	mkdir -p bin
	$(CC) $^ -o $(BINDIR)/$@

testboard: $(addprefix $(OBJDIR)/,$(TESTBOARD_OBJS))
	mkdir -p bin
	$(CC) $^ -o $(BINDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/$(notdir %.c)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/*
