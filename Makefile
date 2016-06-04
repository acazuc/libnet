NAME = libnet.a

CC_LINUX = g++ -std=c++14

CC_WINDOWS = x86_64-w64-mingw32-g++ -std=c++14

CC = $(CC_LINUX)

CLFAGS = -Wall -Wextra -Werror -Ofast

INCLUDES_PATH = -Isrcs

SRCS_PATH = srcs/

SRCS_NAME = Buffer.cpp \
			ServerSocket.cpp \
			Socket.cpp \
			Connection.cpp

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_PATH = objs/

OBJS_NAME = $(SRCS_NAME:.cpp=.opp)

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

LIBRARY = -lmysqlcppconn

all: $(NAME)

$(NAME): $(OBJS)
	@echo " - Making $(NAME)"
	@ar -rc $(NAME) $(OBJS)
	@ranlib $(NAME)

$(OBJS_PATH)%.opp: $(SRCS_PATH)%.cpp
	@echo " - Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH)

clean:
	@echo " - Cleaning objs"
	@rm -f $(OBJS)

fclean: clean
	@echo " - Cleaning lib"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
