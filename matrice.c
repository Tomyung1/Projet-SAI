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

    while (i < n && !erreur){
        nouv -> mat[i] = NULL;
        if ((nouv -> mat[i] = (double*) calloc(m, sizeof(double))) == NULL){
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


void liberer_matrice(matrice m){
    int i;
    if (m != NULL){
        for (i = 0; i < m -> n; i++){
            free(m -> mat[i]);
        }
        free(m -> mat);
        free(m);
    }
}

void set_mat(matrice m, int l, int c, double v){
    m -> mat[l][c] = v;
}

double get_mat(matrice m, int l, int c){
    return m -> mat[l][c];
}


matrice mult_matrice(matrice m1, matrice m2){
    matrice nouv = NULL;
    int i, j, k;
    double res;
    
    if (m1 -> m == m2 -> n){
        nouv = creer_matrice(m1 -> n, m2 -> m);

        /* parcours de la matrice resultat */
        for (i = 0; i < m1 -> n; i++){
            for (j = 0; j < m2 -> m; j++){

                res = 0;
                /* parcours local */
                for (k = 0; k < m1 -> m; k++){
                    res += m1 -> mat[i][k] * m2 -> mat[k][j];
                }

                nouv -> mat[i][j] = res;
            }
        }
    }
    
    return nouv;
}

void afficher_matrice(matrice m){
    int i, j;

    if (m == NULL){
        printf("null\n");
    }
    else if (m != NULL && m -> mat != NULL){
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
}


void translation(matrice* modele, double dx, double dy, double dz){
    matrice t = creer_matrice(4, 4);
    matrice res;

    set_mat(t, 0, 0, 1);
    set_mat(t, 1, 1, 1);
    set_mat(t, 2, 2, 1);
    set_mat(t, 3, 3, 1);
    set_mat(t, 0, 3, dx);
    set_mat(t, 1, 3, dy);
    set_mat(t, 2, 3, dz);

    res = mult_matrice(t, *modele);

    liberer_matrice(*modele);

    *modele = res;
    
    liberer_matrice(t);
}

