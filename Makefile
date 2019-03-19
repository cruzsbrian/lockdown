CC     = gcc
CFLAGS = -g -Wall -Wstrict-prototypes -ansi -pedantic

SRCDIR = src
OBJDIR = obj
BINDIR = bin

COMMON_SRCS = main.c

COMMON_OBJS = $(COMMON_SRCS:.c=.o)

othello: $(addprefix $(OBJDIR)/,$(COMMON_OBJS))
	mkdir -p bin
	$(CC) $^ -o $(BINDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/$(notdir %.c)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BINDIR)/* $(OBJDIR)/*
