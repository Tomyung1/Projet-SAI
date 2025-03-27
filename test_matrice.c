#include <stdio.h>
#include <stdlib.h>

#include "headers/matrice.h"

int main(){
    matrice m;

    m = creer_matrice(3, 7);

    m -> mat[1][2] = 2;

    afficher_matrice(m);

    exit(EXIT_SUCCESS);
}