CFLAGS = -Wall -pedantic -O2
EXEC = projet

all : $(EXEC)

projet : main.c config.h
	gcc $(CFLAGS) $^ -o $@ -lglut -lGLU -lGL -lm

clean :
	rm -rf *.o $(EXEC)
