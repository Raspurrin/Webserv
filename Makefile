NAME = webserv
CC = c++
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror -g -std=c++98
DEBUG = -fsanitize=address

HEADER_DIR = ./header
SRC_DIR = ./src/
OBJ_DIR = ./.build/
LIBS = -lm

SRCS =	error.cpp \
		main.cpp \
		parsing.cpp \
		Request.class.cpp \
		Response.class.cpp \
		Route.class.cpp \
		Server.class.cpp

OBJ = $(SRCS:.cpp=.o)
OBJ_PREF := $(addprefix $(OBJ_DIR), $(OBJ))

$(NAME): $(OBJ_PREF) $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(OBJ_PREF) -I$(HEADER_DIR) $(DEBUG) $(LIBS) -o $@
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

