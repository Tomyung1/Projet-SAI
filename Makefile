CFLAGS = -Wall -pedantic -O2
DEBUGFLAGS = -Og -g
EXEC = projet matrice debug.exec
OPENGL = -lglut -lGLU -lGL
HEADER = headers/


all : $(EXEC)


projet : main.c action.o affichage.o objet.o poisson.o bateau.o matrice.o $(HEADER)config.h
	gcc $(CFLAGS) $^ -o $@ $(OPENGL)

debug : debug.exec
	valgrind --leak-check=full --show-leak-kinds=all ./debug.exec 2> log

debug.exec : main.c action.o affichage.o objet.o poisson.o bateau.o matrice.o $(HEADER)config.h
	gcc $(DEBUGFLAGS) $^ -o $@ $(OPENGL)

action.o : action.c $(HEADER)action.h
	gcc $(CFLAGS) $< -o $@ -c

affichage.o : affichage.c $(HEADER)affichage.h
	gcc $(CFLAGS) $< -o $@ -c $(OPENGL)

objet.o : objet.c $(HEADER)objet.h
	gcc $(CFLAGS) $< -o $@ -c

poisson.o : poisson.c $(HEADER)poisson.h $(HEADER)objet.h
	gcc $(CFLAGS) $< -o $@ -c $(OPENGL)

bateau.o : bateau.c $(HEADER)bateau.h $(HEADER)objet.h
	gcc $(CFLAGS) $< -o $@ -c $(OPENGL)

matrice.o : matrice.c $(HEADER)matrice.h
	gcc $(CFLAGS) $< -o $@ -c



matrice : test_matrice.c matrice.o
	gcc $(CFLAGS) $^ -o $@


clean :
	rm -rf *.o $(EXEC)