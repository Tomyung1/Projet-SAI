CFLAGS = -Wall -pedantic -O2
EXEC = projet matrice

all : $(EXEC)

projet : main.c matrice.o config.h
	gcc $(CFLAGS) $^ -o $@ -lglut -lGLU -lGL

matrice.o : matrice.c matrice.h
	gcc $(CFLAGS) $< -o $@ -c

matrice : test_matrice.c matrice.o
	gcc $(CFLAGS) $^ -o $@

clean :
	rm -rf *.o $(EXEC)