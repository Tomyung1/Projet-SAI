#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/matrice.h"


matrice creer_matrice(int n, int m){
    matrice nouv = NULL;
    int i, j, erreur;


    if ((nouv = (matrice) malloc(sizeof(matrice_s))) == NULL){
        return NULL;
    }

    nouv -> n = n;
    nouv -> m = m;
    
    nouv -> mat = NULL;
    if ((nouv -> mat = (double**) malloc(n * sizeof(double*))) == NULL){
        free(nouv);
        return NULL;
    }

    i = 0;
    erreur = 0;

    while (i < m && !erreur){
        nouv -> mat[i] = NULL;
        if ((nouv -> mat[i] = (double*) malloc(m * sizeof(double))) == NULL){
            erreur = 1;
        }
        i++;
    }

    if (erreur){
        for (j = 0; j < i; j++){
            free(nouv -> mat[j]);
        }
        free(nouv -> mat);
        free(nouv);
        return NULL;
    }

    return nouv;
}


void afficher_matrice(matrice m){
    int i, j;

    for (i = 0; i < m->n; i++){

        if (m->n == 1){
            printf("[");
        }
        else if (i == 0){
            printf("⌈");
        }
        else if (i == m->n - 1){
            printf("⌊");
        }
        else {
            printf("|");
        }

        for (j = 0; j < m->m; j++){
            printf("%.3lf ", m -> mat[i][j]);
        }
        
        if (m->n == 1){
            printf("]");
        }
        else if (i == 0){
            printf("⌉");
        }
        else if (i == m->n - 1){
            printf("⌋");
        }
        else {
            printf("|");
        }
        
        printf("\n");
    }

}