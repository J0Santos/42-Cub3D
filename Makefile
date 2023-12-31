# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/29 15:00:20 by msousa            #+#    #+#              #
#    Updated: 2022/05/12 02:19:45 by msousa           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS	= -Wall -Wextra -Werror -O3
LINKS		= -lmlx -lm -Llibft -lft
CC			= gcc
RM			= rm -f
INC			= -Iinclude -Ilibft/include
UNAME 	:= ${shell uname}
OBJ			= ${SRC:%.c=obj/%.o}
SRC			= src/main.c src/draw.c src/hooks.c src/helpers.c src/test.c \
					src/parse.c src/parse_1.c src/parse_2.c src/maplines.c src/map.c \
					src/map_1.c src/settings.c src/elements.c src/elements_1.c \
					src/maplines_1.c src/player.c src/raycast.c src/raycast_1.c \
					src/move.c src/point.c src/validation.c src/vector.c src/image.c \
					src/draw_1.c

NAME		= cub3D

ifeq (${UNAME}, Linux)
LINKS 	+= -lbsd -lXext -lX11
endif

ifeq (${UNAME}, Darwin)
LINKS 	+= -framework OpenGL -framework AppKit
CFLAGS 	+= -DOS_MAC
endif

${NAME}:	${OBJ}
					${MAKE} -C libft
					${CC} ${CFLAGS} ${OBJ} ${LINKS} -o $@

obj/%.o:%.c
					@mkdir -p $(dir $@)
					${CC} ${CFLAGS} ${INC} -c $< -o $@

all:			${NAME}

bonus:		all

style:
					-norminette $$( find . -type f \( -name "*.c" -or -name "*.h" \) )

test:			${NAME}
					./test main.sh

check:
					-cat ./src/* | grep TODO

clean:
					${MAKE} clean -C libft
					${RM} -rf obj

fclean:		clean
					${RM} ${NAME}

re: 			fclean all

debug:		INC += -DDEBUG_MODE
debug:		CFLAGS += -g -fsanitize=address
debug:		clean
debug:		all

.PHONY : 	all clean fclean re bonus
