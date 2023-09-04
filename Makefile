SRC = src/main.c \
	src/game.c

OBJ = $(SRC:.c=.o)

#CFLAGS += -Wall -Wextra -Werror -I./include
CFLAGS += -Wall -I include
SDL		= -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer


NAME = bomberman

$(NAME): $(OBJ)
	gcc -o $(NAME) $(OBJ) `sdl2-config --cflags --libs` -lSDL2_image

all:	$(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re