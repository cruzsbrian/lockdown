CC     = gcc
CFLAGS = -mbmi2 -Wall -Wstrict-prototypes -pedantic -flto -Ofast
LDFLAGS = -Ofast

SRCDIR = src
OBJDIR = obj
BINDIR = bin

BOARD_SRCS = board.c
EVAL_SRCS = table_eval.c
SEARCH_SRCS = search.c bestnode.c alphabeta.c endgame.c move_ordering.c trans_table.c
OPENING_SRCS = opening.c
COMMON_SRCS = $(addprefix board/,$(BOARD_SRCS)) \
			  $(addprefix eval/,$(EVAL_SRCS)) \
			  $(addprefix search/,$(SEARCH_SRCS)) \
			  $(addprefix opening/,$(OPENING_SRCS)) \

LOCKDOWN_SRCS = main.c player.c
TEST_SRCS = endgame_test.c
ANALYSIS_SRCS = $(addprefix analysis/,build_eval.c)

COMMON_OBJS = $(COMMON_SRCS:.c=.o)
LOCKDOWN_OBJS = $(COMMON_OBJS) $(LOCKDOWN_SRCS:.c=.o)
TEST_OBJS = $(COMMON_OBJS) $(TEST_SRCS:.c=.o)
ANALYSIS_OBJS = $(COMMON_OBJS) $(ANALYSIS_SRCS:.c=.o)

lockdown: $(addprefix $(OBJDIR)/,$(LOCKDOWN_OBJS))
	mkdir -p bin
	$(CC) $^ $(LDFLAGS) -o $(BINDIR)/$@

test: $(addprefix $(OBJDIR)/,$(TEST_OBJS))
	mkdir -p bin
	$(CC) $^ $(LDFLAGS) -o $(BINDIR)/$@

build_eval: $(addprefix $(OBJDIR)/,$(ANALYSIS_OBJS))
	mkdir -p bin
	$(CC) $^ $(LDFLAGS) -o $(BINDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/$(notdir %.c)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/*
