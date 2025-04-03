#include <stdio.h>
#include <stdlib.h>

#include "headers/matrice.h"

int main(){
    matrice m1, m2;
    
    m1 = creer_matrice(3, 1);
    m2 = creer_matrice(1, 3);

    if (m1 == NULL || m2 == NULL){
        printf("Erreur création\n");
        exit(EXIT_FAILURE);
    }

    liberer_matrice(m1);
    liberer_matrice(m2);
    
    exit(EXIT_SUCCESS);
}