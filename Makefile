#makefile

CC = g++ -std=c++20
UCFLAGS = -g -O3 -Wall -Wextra
RUCFLAGS := $(root-config --cflags) -I${ROOFITSYS}/include/ -I./include/

LIBS :=  $(root-config --libs) -lTreePlayer -lGpad -lHist -lRIO -lCore -L${ROOFITSYS}/lib/ -lRooFit -lRooFitCore -lGraf -lCling
GLIBS := $(root-config --glibs)


VPATH = ./src/

SRC = Main.cpp\
      Emulator.cpp\
      Utilities.cpp\


INCLUDES = Emulator.h\
           Utilities.h\

    
OBJ = $(patsubst %.cpp,obj/%.o,$(SRC))


all: run


obj/%.o: %.cpp
	@echo "==> Compiling $*"
	@mkdir -p obj/
	@$(CC) -c $< $(UCFLAGS) $(RUCFLAGS) -o $@
   

run: $(OBJ)
	@echo "==> Linking..."
	@$(CC) $^ $(LIBS) $(GLIBS) -o $@


clean:
	@echo "==> Cleaning objects and executable"
	@rm -f obj/*.o
	@rm -f run


uninstall:
	@echo "==> Uninstalling fitter"
	@rm -f obj/*.o obj/*.so obj/*.d obj/*.pcm
	@rm -f ext/*.so ext/*.d ext/*.pcm
	@rm -f run


# $@ references the target file name
# $^ references all of the prerequisite files as a space-separated list
# patsubst function in a makefile is used to perform pattern substitution on a list of strings
# VPATH variable can be used to specify a list of directories where make should look for prerequisite files that are not found in the current directory