CFLAGS = -Wall -pedantic -O2
EXEC = projet matrice
OPENGL = -lglut -lGLU -lGL

all : $(EXEC)


projet : main.c matrice.o action.o affichage.o headers/config.h
	gcc $(CFLAGS) $^ -o $@ $(OPENGL)

action.o : action.c headers/action.h
	gcc $(CFLAGS) $< -o $@ -c

affichage.o : affichage.c headers/affichage.h
	gcc $(CFLAGS) $< -o $@ $(OPENGL) -c

matrice.o : matrice.c headers/matrice.h
	gcc $(CFLAGS) $< -o $@ -c


matrice : test_matrice.c matrice.o
	gcc $(CFLAGS) $^ -o $@


clean :
	rm -rf *.o $(EXEC)