CC     = gcc
CFLAGS = -g -mbmi2 -Wall -Wstrict-prototypes -ansi -pedantic

SRCDIR = src
OBJDIR = obj
BINDIR = bin

BOARD_SRCS = board.c
EVAL_SRCS = simple_eval.c table_eval.c
SEARCH_SRCS = alphabeta.c
UTIL_SRCS = hash_table.c
COMMON_SRCS = $(addprefix board/,$(BOARD_SRCS)) \
			  $(addprefix eval/,$(EVAL_SRCS)) \
			  $(addprefix search,/$(SEARCH_SRCS)) \
			  $(addprefix util,/$(UTIL_SRCS))

OTHELLO_SRCS = main.c
TESTBOARD_SRCS = tests/testboard.c
TESTSEARCH_SRCS = tests/testnegamax.c

COMMON_OBJS = $(COMMON_SRCS:.c=.o)
OTHELLO_OBJS = $(COMMON_OBJS) $(OTHELLO_SRCS:.c=.o)
TESTBOARD_OBJS = $(COMMON_OBJS) $(TESTBOARD_SRCS:.c=.o)
TESTSEARCH_OBJS = $(COMMON_OBJS) $(TESTSEARCH_SRCS:.c=.o)

othello: $(addprefix $(OBJDIR)/,$(OTHELLO_OBJS))
	mkdir -p bin
	$(CC) $^ -o $(BINDIR)/$@

testboard: $(addprefix $(OBJDIR)/,$(TESTBOARD_OBJS))
	mkdir -p bin
	$(CC) $^ -o $(BINDIR)/$@

testsearch: $(addprefix $(OBJDIR)/,$(TESTSEARCH_OBJS))
	mkdir -p bin
	$(CC) $^ -o $(BINDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/$(notdir %.c)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/*
