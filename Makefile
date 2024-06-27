CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
LIBFT_LIB = libft/libft.a
SRC_CLIENT = src/client.c
SRC_SERVER = src/server.c
NAME = minitalk
NAME_SERVER = server
NAME_CLIENT = client
SRC_CLIENT_BONUS = bonus/client_bonus.c
SRC_SERVER_BONUS = bonus/server_bonus.c
NAME_SERVER_BONUS = server_bonus
NAME_CLIENT_BONUS = client_bonus

all: $(NAME)

$(NAME): $(NAME_SERVER) $(NAME_CLIENT)

bonus: $(NAME_SERVER_BONUS) $(NAME_CLIENT_BONUS)

$(NAME_SERVER):
		$(MAKE) -C ./libft
		$(CC) $(CFLAGS) $(SRC_SERVER) $(LIBFT_LIB) -o $(NAME_SERVER)

$(NAME_CLIENT):
		$(MAKE) -C ./libft
		$(CC) $(CFLAGS) $(SRC_CLIENT) $(LIBFT_LIB) -o $(NAME_CLIENT)

$(NAME_SERVER_BONUS):
		$(MAKE) -C ./libft
		$(CC) $(CFLAGS) $(SRC_SERVER_BONUS) $(LIBFT_LIB) -o $(NAME_SERVER_BONUS)

$(NAME_CLIENT_BONUS):
		$(MAKE) -C ./libft
		$(CC) $(CFLAGS) $(SRC_CLIENT_BONUS) $(LIBFT_LIB) -o $(NAME_CLIENT_BONUS)

clean:
		$(RM) $(NAME_SERVER) $(NAME_CLIENT) $(NAME_SERVER_BONUS) $(NAME_CLIENT_BONUS)

fclean: clean
		$(MAKE) fclean -C ./libft

re: clean all

re_bonus: clean bonus

.PHONY: all bonus clean fclean re re_bonus