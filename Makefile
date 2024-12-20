#makefile

CC = g++ -std=c++20
UCFLAGS = -g -O3 -Wall -Wextra -I./include/


VPATH = ./src/

SRC = Main.cpp\
      TowerSums.cpp\
      Utilities.cpp\


INCLUDES = TowerSums.h\
           Utilities.h\

    
OBJ = $(patsubst %.cpp,obj/%.o,$(SRC))


all: run


obj/%.o: %.cpp
	@echo "==> Compiling $*"
	@mkdir -p obj/
	@$(CC) -c $< $(UCFLAGS) -o $@
   

run: $(OBJ)
	@echo "==> Linking..."
	@$(CC) $^ -o $@


clean:
	@echo "==> Cleaning objects and executable"
	@rm -f obj/*.o
	@rm -f run


# $@ references the target file name
# $^ references all of the prerequisite files as a space-separated list
# patsubst function in a makefile is used to perform pattern substitution on a list of strings
# VPATH variable can be used to specify a list of directories where make should look for prerequisite files that are not found in the current directory