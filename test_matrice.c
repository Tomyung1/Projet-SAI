#include <stdio.h>
#include <stdlib.h>

#include "headers/matrice.h"

int main(){
    matrice m1, m2, m;
    
    m1 = creer_matrice(3, 1);
    m2 = creer_matrice(1, 3);

    if (m1 == NULL || m2 == NULL){
        printf("Erreur création\n");
        exit(EXIT_FAILURE);
    }

    affecter_matrice(m1, 0, 0, 1);
    affecter_matrice(m1, 1, 0, 2);
    affecter_matrice(m1, 2, 0, 3);
    
    affecter_matrice(m2, 0, 0, 1);
    affecter_matrice(m2, 0, 1, 2);
    affecter_matrice(m2, 0, 2, 3);

    m = mult_matrice(m1, m2);

    
    afficher_matrice(m1);
    printf("\n");
    afficher_matrice(m2);
    printf("\n");
    afficher_matrice(m);

    liberer_matrice(m1);
    liberer_matrice(m2);
    if (m != NULL) {
        liberer_matrice(m);
    }
    
    exit(EXIT_SUCCESS);
}