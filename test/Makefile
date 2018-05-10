#TODO(axp) 随便找了个，之后再改
ROOTDIR = .

cc = g++

EXE_DIR = ./bin
CFLAGS = -g -Wall
LFLAGS = 

objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
executables := $(patsubst %.cpp,%,$(wildcard *.cpp))

all : $(objects)
$(objects) :%.o : %.cpp
	@mkdir -p ./bin
	$(cc) -c $(CFLAGS) $< -o $@
	$(cc) $(CFLAGS) $< -o $(subst .o, ,$(EXE_DIR)/$@) $(LFLAGS) $(LIBS)
test:
	@./bin/Rb_tree_test
clean:
	@rm -f *.o rm -f $(executables)
	@rm -rf ./bin 
distclean: clean
