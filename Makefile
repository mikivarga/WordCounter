NAME := counter

SRC_DIR := ./src/
OBJ_DIR := ./obj/
INC_DIR := ./inc/

CC_FLAGS :=	-Wall -g
LD_FLAGS :=	-lpthread

HEADER_FLAGS :=	-I $(INC_DIR)

CC:=gcc

SRC = main.c tree.c parser.c

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LD_FLAGS) -o $(NAME)

$(OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: %.c
	$(CC) -c $< -o $@ $(CC_FLAGS) $(HEADER_FLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)

re: fclean all

vpath %.c $(SRC_DIR)

.PHONY:all clean fclean re
