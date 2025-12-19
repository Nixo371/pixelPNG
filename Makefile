SRCDIRS = src
TEST_SRCDIRS = test
INCDIRS = include
BUILDDIR = build

CC = gcc
AR = ar
FLAGS = -Wall -Wextra -Werror $(foreach D, $(INCDIRS), -I$(D))

SRCS = $(foreach D, $(SRCDIRS), $(wildcard $(D)/*.c))
OBJS = $(patsubst %.c, %.o, $(SRCS))

TEST_SRCS = $(foreach D, $(TEST_SRCDIRS), $(wildcard $(D)/*.c))
TEST_OBJS = $(patsubst %.c, %.o, $(TEST_SRCS))

NAME = pixelpng
LIB = $(BUILDDIR)/lib$(NAME).a

all: $(LIB)

$(LIB): $(OBJS) | $(BUILDDIR)
	$(AR) rcs $@ $^

$(BUILDDIR):
	mkdir -p $@

test: $(TEST_OBJS) $(LIB) | $(BUILDDIR)
	$(CC) $^ -L. -L$(BUILDDIR) -lz -o $(BUILDDIR)/test

%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJS) $(TEST_OBJS)

fclean: clean
	rm -rf $(NAME)

.PHONY: all test clean fclean
