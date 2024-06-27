CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
LIBFT_LIB = libft/libft.a
SRC_CLIENT = src/client.c
SRC_SERVER = src/server.c
NAME_SERVER = server
NAME_CLIENT = client

all: $(NAME_SERVER) $(NAME_CLIENT)

$(NAME_SERVER):
		$(MAKE) -C ./libft
		$(CC) $(CFLAGS) $(SRC_SERVER) $(LIBFT_LIB) -o $(NAME_SERVER)

$(NAME_CLIENT):
		$(MAKE) -C ./libft
		$(CC) $(CFLAGS) $(SRC_CLIENT) $(LIBFT_LIB) -o $(NAME_CLIENT)
		
clean:
		$(RM) $(NAME_SERVER) $(NAME_CLIENT)

fclean: clean
		$(MAKE) fclean -C ./libft

re: clean all

.PHONY: clean