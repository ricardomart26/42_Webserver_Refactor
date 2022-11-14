NAME = server

CC = c++ -std=c++98 -pedantic -fsanitize=address -g

SRCS =	$(shell find ./src -name '*.cpp')
OBJS = $(addsuffix .o, $(basename $(SRCS)))
INCLUDES = $(addprefix -I, $(shell find inc -type d))

all: $(NAME)

%.o:%.cpp
	$(CC) $(INCLUDES) -c -o $@ $<

$(NAME): $(OBJS)
	$(CC) -o $@  $^

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(NAME) clean

re: clean all

r:
	make && make clean && clear && ./$(NAME)

.PHONY: all clean re
