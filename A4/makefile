# Decleration of Variables
CC = g++
CFLAGS = -std=c++11 -stdlib=libc++ -g -Wall -Wno-deprecated
OS = $(shell uname)

# File Names
MAIN = run
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

# Dependencies
DEPS = $(OBJS:.o=.d)

ifeq ($(OS), Darwin)
	LIBS = -framework OpenGL -framework GLUT -stdlib=libc++
else ifeq ($(OS), Linux)
	LIBS = -lglut -lGLU -lGL
else
	CC=x86_64-w64-mingw32-g++
	LIBS = -lglut32 -lglu32 -lopengl32
endif

$(MAIN): $(OBJS)
	@$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(MAIN)

%.o: %.cpp
	@$(CC) -c -MMD -MP $(CFLAGS) $< -o $@
	
clean:
	$(RM) $(MAIN) $(OBJS) -s

-include $(DEPS)
