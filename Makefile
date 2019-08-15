CC     = gcc
CFLAGS = -mbmi2 -Wall -Wstrict-prototypes -pedantic -flto -Ofast
LDFLAGS = -Ofast

SRCDIR = src
OBJDIR = obj
BINDIR = bin

BOARD_SRCS = board.c
EVAL_SRCS = table_eval.c
SEARCH_SRCS = search.c alphabeta.c move_ordering.c trans_table.c
COMMON_SRCS = $(addprefix board/,$(BOARD_SRCS)) \
			  $(addprefix eval/,$(EVAL_SRCS)) \
			  $(addprefix search/,$(SEARCH_SRCS)) \

LOCKDOWN_SRCS = main.c player.c
TESTBOARD_SRCS = tests/testboard.c
TESTSEARCH_SRCS = tests/testnegamax.c

COMMON_OBJS = $(COMMON_SRCS:.c=.o)
LOCKDOWN_OBJS = $(COMMON_OBJS) $(LOCKDOWN_SRCS:.c=.o)
TESTBOARD_OBJS = $(COMMON_OBJS) $(TESTBOARD_SRCS:.c=.o)
TESTSEARCH_OBJS = $(COMMON_OBJS) $(TESTSEARCH_SRCS:.c=.o)

lockdown: $(addprefix $(OBJDIR)/,$(LOCKDOWN_OBJS))
	mkdir -p bin
	$(CC) $^ $(LDFLAGS) -o $(BINDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/$(notdir %.c)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/*
