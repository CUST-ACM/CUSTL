#TODO(axp) 随便找了个，之后再改
## Update at 2018-05-12 by F_TD5X(jhx)
ROOTDIR = .

EXE_DIR = ./bin
CFLAGS = -g -Wall -std=c++14
LFLAGS = 
INCLUDE = header
LIBS = 

objects := $(patsubst %.cpp,%.o,$(wildcard ./test/*.cpp))
executables := $(patsubst %.cpp,%,$(wildcard ./test/*.cpp))

all : $(objects)
$(objects) :%.o : %.cpp
	@mkdir -p bin
	$(CXX) -c $(CFLAGS) $< -I $(INCLUDE) -o $(EXE_DIR)/$(notdir $@)
	$(CXX) $(CFLAGS) $< -I $(INCLUDE) -o $(subst .o, ,$(EXE_DIR)/$(notdir $@)) $(LFLAGS) $(LIBS)
check:
	@bin/vector_demo
	@bin/deque_demo
	@bin/list_demo
	@bin/Rb_tree_test
clean:
	@rm -r bin
distclean: clean
