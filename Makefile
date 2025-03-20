CFLAGS = -Wall -pedantic -O2
EXEC = projet

all : $(EXEC)

projet : main.c config.h
	gcc $(CFLAGS) $^ -o $@ -lglut -lGLU -lGL

clean :
	rm -rf *.o $(EXEC)