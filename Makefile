SRCS = mylittleids.cpp
CC = c++
FLAGS = -g -Wall -Wextra -Werror
NAME = IDS

name: $(SRCS)
	@$(CC) $(FLAGS) $(SRCS) -o $(NAME)
	@echo "Done!"

run: name
	@./$(NAME)
	@$(MAKE) -s clean

clean:
	@rm -rf $(NAME)
	@echo "Undone!"