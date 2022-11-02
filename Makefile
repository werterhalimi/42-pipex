NAME = pipex
LIBFT = libft
CFLAGS = -Wall -Wextra -Werror -lm -D MAIN
CC = gcc
SRC_PATH = ./
OPTIONS = -I./libft -g3 -fsanitize=address
SRC = main.c
SRCS = $(addprefix $(SRC_PATH),$(SRC))
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
			make -C $(LIBFT)
			mv ./libft/libft.a libft.a
			ar rcs libft.a ${OBJS} 


%.o: %.c
	$(CC) -Wall -Wextra -Werror -D BUFFER_SIZE=5 -c $< -o $@ -g 

$(NAME): $(OBJS) $(LIBFT)
	$(CC)  -Ilibft -L. -lft $(OBJS) -o $(NAME) -g 

unit: 
	gcc main.c libft/*.c -I libft/libft.h -D BUFFER_SIZE=5 -D UNIT -g -o unit
	./unit
clean:	
		rm -f $(OBJS)
		make -C $(LIBFT) clean

fclean:	clean
		rm -f $(NAME)
		make -C $(LIBFT) fclean

re:		fclean all

.PHONY: re fclean clean all $(LIBFT) $(NAME) $(MINILIB) unit
