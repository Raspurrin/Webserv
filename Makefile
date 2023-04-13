NAME = webserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 

HEADER_DIR = ./header

SRC_DIR = ./src/

OBJ_DIR = ./.build/

LIBS = -lm

SRCS = main.cpp

RM = rm -rf

OBJ = $(SRCS:.cpp=.o)
OBJ_PREF := $(addprefix $(OBJ_DIR), $(OBJ))

$(NAME): $(OBJ_PREF) $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(OBJ_PREF) -I$(HEADER_DIR) $(LIBS) -o $@
	@echo "Success, executable ./$(NAME) created."

$(OBJ_PREF): .build/%.o: src/%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(HEADER_DIR)

all: $(NAME)

run: all
	@./$(NAME)

leaks: all
	@valgrind --leak-check=full ./$(NAME)

clean:
	@$(RM) $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)
	@echo "Success, directory cleaned."

re: fclean all

.PHONY: all clean fclean re run leaks

