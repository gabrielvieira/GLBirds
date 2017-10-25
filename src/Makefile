LIBS = -lGL -lGLU -lglut
# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
# EXE = main.exe
    LIBS = -lopengl32 -lglu32 -lglut32
endif

# OS X
ifeq "$(OSTYPE)" "darwin"
    LIBS = -framework Carbon -framework OpenGL -framework GLUT
endif

prefix=src
bindir=$(prefix)/bin 
CC=g++
# EXEC=../bin/teste
EXEC=test
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
all: exemplo

exemplo: $(OBJ)
	$(CC) -o $(EXEC) $^ $(LIBS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper
clean: 
	@rm -rf *.o *~ 

