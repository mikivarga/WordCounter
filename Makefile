NAME = counter

SRC_DIR =	./src/
OBJ_DIR =	./obj/
INC_DIR =	./inc/

CF_FLAGS	:=	-Wall
LD_FLAGS	:=	-lpthread

HEADER_FLAGS	:=	-I $(INC_DIR)

CC:=gcc

SRC =	parse_words.c words.c tree.c main.c

OBJ =	$(addprefix $(OBJ_DIR), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	@echo $(NAME) ": Source compilling..."
	@$(CC) $(OBJ) $(LD_FLAGS) -o $(NAME)
	@echo "Executable "$(NAME)" made."

$(OBJ_DIR)%.o: $(SRC_DIR)/%.c
	@$(CC) -c $< -o $@ $(CC_FLAGS) $(HEADER_FLAGS) 

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
	@echo "Make re done!"
