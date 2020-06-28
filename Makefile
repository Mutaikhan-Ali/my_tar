

NAME = my_tar
SRC = *.c
SANITIZE = -g -fsanitize=address
OBJECT = *.o
all: $(NAME)

$(NAME): $(OBJECT)
	gcc  $(SANITIZE) $(SRC) -o $(NAME)


clean:
	rm -f *.o