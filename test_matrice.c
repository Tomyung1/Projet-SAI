#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "headers/matrice.h"
void afficher_taille(matrice m1){
    double x = get_mat(m1, 0, 0);
    double y = get_mat(m1, 1, 0);
    double z = get_mat(m1, 2, 0);
    double res = sqrt(x * x + y * y + z * z);
    printf("taille = %lf\n", res);
}

int main(){
    matrice m1;
    
    m1 = creer_matrice(4, 1);

    set_mat(m1, 0, 0, -1);
    set_mat(m1, 3, 0, 1);

    afficher_taille(m1);

    rotation_x(&m1, M_PI / 4);

    afficher_taille(m1);

    rotation_y(&m1, 3 * M_PI / 5);

    afficher_taille(m1);

    rotation_z(&m1, 5 * M_PI / 6);

    afficher_taille(m1);

    rotation_x(&m1, M_PI / 4);

    afficher_taille(m1);

    
    liberer_matrice(m1);
    
    exit(EXIT_SUCCESS);
}