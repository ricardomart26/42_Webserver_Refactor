NAME = server

CC = c++ -Wall -Wextra -Werror -std=c++98 -pedantic -fsanitize=address -g

SRCS =	$(shell find ./src -name '*.cpp')
OBJS = $(addsuffix .o, $(basename $(SRCS)))
INCLUDES = $(addprefix -I, $(shell find inc -type d))

all: $(NAME)

%.o:%.cpp
	$(CC) -c $(INCLUDES) -o $@ $<

$(NAME): $(OBJS) 
	$(CC) -o $@  $^

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: clean all

r:
	make && clear && ./$(NAME) conf_example/web_files.conf

m:
	make fclean && clear

.PHONY: all clean re
