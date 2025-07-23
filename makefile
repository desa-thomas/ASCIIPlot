CC = gcc
ODIR = objs
SDIR = src

LIBS = -lncurses
CFLAGS = -g -Wall -Wextra -pedantic -MMD -MP

SRCS = $(wildcard $(SDIR)/*.c)
#pattern substituation: (pattern, replacement, text)
OBJS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRCS))

main.exe: $(OBJS)
	$(CC) $(LIBS) -o $@ $^

-include $(OBJS:.o=.d) #makes it so when headers are updated, recompile

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.d

