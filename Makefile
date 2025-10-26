SRCDIRS = src
TEST_SRCDIRS = test
INCDIRS = include

CC = gcc
AR = ar
FLAGS = -Wall -Wextra -Werror $(foreach D, $(INCDIRS), -I$(D))

SRCS = $(foreach D, $(SRCDIRS), $(wildcard $(D)/*.c))
OBJS = $(patsubst %.c, %.o, $(SRCS))

TEST_SRCS = $(foreach D, $(TEST_SRCDIRS), $(wildcard $(D)/*.c))
TEST_OBJS = $(patsubst %.c, %.o, $(TEST_SRCS))

NAME = pixelpng

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) rcs lib$@.a $^

test: $(TEST_OBJS)
	$(CC) $^ -L. -l$(NAME) -o out

%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

.PHONY: all test clean fclean
