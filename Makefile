NAME = webserv
CC = c++
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror -std=c++98 -O3 -D DEBUG=0
DFLAGS = -Wall -Wextra -Werror -std=c++98 -g -D DEBUG=1

HEADER_DIR = ./header
SRC_DIR = ./src/
OBJ_DIR = ./.build/
LIBS = -lm

SRCS =	main.cpp \
		Request.class.cpp \
		error.cpp \
		Response.class.cpp \
		ServerConfig.class.cpp \
		ServerManager.class.cpp \
		Client.class.cpp \
		ParsingConfig.class.cpp \
		Cgi.class.cpp \
		utils.cpp

OBJ = $(SRCS:.cpp=.o)
OBJ_PREF := $(addprefix $(OBJ_DIR), $(OBJ))

$(NAME): $(OBJ_PREF) $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(OBJ_PREF) -I$(HEADER_DIR) $(LIBS) -o $@
	@echo "Success, executable ./$(NAME) created."

$(OBJ_PREF): .build/%.o: src/%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(HEADER_DIR)

all: $(NAME)

re: fclean all

debug:	CFLAGS = $(DFLAGS)
debug:	all ./$(NAME)

run: all
	@./$(NAME)

leaks: all
	@valgrind --leak-check=full ./$(NAME)

clean:
	@$(RM) $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)
	@echo "Success, directory cleaned."

.PHONY: all clean fclean re run leaks debug
