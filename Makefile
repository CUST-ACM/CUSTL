#TODO(axp) 随便找了个，之后再改
ROOTDIR = .

cc = g++

EXE_DIR = ./bin
CFLAGS = -g -Wall -std=c++11
LFLAGS = 

objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp) $(wildcard test/*.cpp))
executables := $(patsubst %.cpp,%,$(wildcard *.cpp) $(wildcard test/*.cpp))

all : $(objects)
$(objects) :%.o : %.cpp
	@mkdir -p ./bin
	$(cc) -c $(CFLAGS) $< -o $(EXE_DIR)/$(notdir $@)
	$(cc) $(CFLAGS) $< -o $(subst .o, ,$(EXE_DIR)/$(notdir $@)) $(LFLAGS) $(LIBS)
check:
	@./bin/vector_demo
	@./bin/Rb_tree_test
clean:
	@rm -r ./bin
distclean: clean
