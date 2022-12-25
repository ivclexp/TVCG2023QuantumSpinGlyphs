#!smake
#  @@ Makefile for CMSC 435/634 - Project 1: Simple scene
#  @@ Jian Chen (jichen@umbc.edu)
#

MACHINE= $(shell uname -s)

ifeq ($(MACHINE),Darwin)
	OPENGL_INC= -FOpenGL -I./jcUtil
	OPENGL_LIB= -L./jcUtil -framework OpenGL -framework GLUT -framework Cocoa
else
	OPENGL_INC= -I/usr/include -I./jcUtil
	OPENGL_LIB= -L/usr/lib64  -L./jcUtil
	LLDLIBS = -ljcUtil-1.0 -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lm
endif

CXX=g++
COMPILER_FLAGS= -g

INCLUDE= $(OPENGL_INC)
LIBS= $(OPENGL_LIB) $(LLDLIBS) 

TARGETS = main

SRCS = main.cpp ReadData.cpp ivTrackball.cpp svGlyph.cpp svQDOTDrawing.cpp 

default : $(TARGETS)

OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp
	$(CXX) -c $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

main: main.o  ReadData.o ivTrackball.o svGlyph.o svQDOTDrawing.o 
	$(CXX) $(COMPILER_FLAGS) $(LIBS) main.o ReadData.o  ivTrackball.o svGlyph.o svQDOTDrawing.o -o main

clean:
	rm -f $(OBJS) $(TARGETS) *~ .*.cpp .*.h  .*.swp
