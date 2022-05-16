GPP = g++
NAME =  sk_transport
DEBUG_FLAGS =  -std=c17 -Wall -Wextra -I.
LINKER_FLAGS = -Wall
OBJ_DIR = obj
SRC_DIR = src
OBJ = $(OBJ_DIR)/main.o \
	$(OBJ_DIR)/transport.o \
	# $(OBJ_DIR)/vector_entry.o \
	# $(OBJ_DIR)/utilities.o \

OBJ_PATH = ./$(OBJ_DIR)/
SRC_PATH = ./$(SRC_DIR)/


$(NAME): $(OBJ_DIR) $(OBJ)
	$(GPP) $(DEBUG_FLAGS) $(OBJ_PATH)*.o $(LINKER_FLAGS) -o $(NAME)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_PATH)main.o: $(SRC_PATH)main.cpp
	$(GPP) -c -o $@ $<

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp $(SRC_PATH)%.h
	$(GPP) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR)

distclean: clean
	rm $(NAME)

	