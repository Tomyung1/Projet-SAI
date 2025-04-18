#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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


matrice creer_identite(int n){
    matrice nouv = creer_matrice(n, n);
    int i;
    
    for (i = 0; i < n; i++){
        set_mat(nouv, i, i, 1);
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
    matrice t = creer_identite(4);
    matrice res;

    set_mat(t, 0, 3, dx);
    set_mat(t, 1, 3, dy);
    set_mat(t, 2, 3, dz);

    res = mult_matrice(t, *modele);

    liberer_matrice(*modele);

    *modele = res;
    
    liberer_matrice(t);
}


/* sens trigo en regardant l'origine depuis (1, 0, 0) */
void rotation_x(matrice* modele, double theta){
    matrice t = creer_identite(4);
    matrice res;

    set_mat(t, 1, 1, cos(theta));
    set_mat(t, 1, 2, -sin(theta));
    set_mat(t, 2, 1, sin(theta));
    set_mat(t, 2, 2, cos(theta));
    
    res = mult_matrice(t, *modele);

    liberer_matrice(*modele);

    *modele = res;

    liberer_matrice(t);
}

/* sens trigo en regardant l'origine depuis (0, 1, 0) */
void rotation_y(matrice* modele, double theta){
    matrice t = creer_identite(4);
    matrice res;

    set_mat(t, 0, 0, cos(theta));
    set_mat(t, 0, 2, sin(theta));
    set_mat(t, 2, 0, -sin(theta));
    set_mat(t, 2, 2, cos(theta));
    
    res = mult_matrice(t, *modele);

    liberer_matrice(*modele);

    *modele = res;

    liberer_matrice(t);
}



/* sens trigo en regardant l'origine depuis (0, 0, 1) (vu du dessus) */
void rotation_z(matrice* modele, double theta){
    matrice t = creer_identite(4);
    matrice res;

    set_mat(t, 0, 0, cos(theta));
    set_mat(t, 0, 1, -sin(theta));
    set_mat(t, 1, 0, sin(theta));
    set_mat(t, 1, 1, cos(theta));
    
    res = mult_matrice(t, *modele);

    liberer_matrice(*modele);

    *modele = res;

    liberer_matrice(t);
}

void rotation_sur_place(matrice* modele, double theta, char axe){
    double px, py, pz;

    px = get_mat(*modele, 0, 0);
    py = get_mat(*modele, 1, 0);
    pz = get_mat(*modele, 2, 0);

    translation(modele, -px, -py, -pz);
    switch (axe){
    case 'x':
        rotation_x(modele, theta);
        break;
    case 'y':
        rotation_y(modele, theta);
        break;
    case 'z':
        rotation_z(modele, theta);
        break;
    default:
        break;
    }
    translation(modele, px, py, pz);
}

void trans_rot_z_alea(matrice* modele, double x_min, double x_max, double y_min, double y_max, double z_min, double z_max){
    matrice t = creer_identite(4);
    matrice res;
    double theta, dx, dy, dz;

    theta = rand() / (double)RAND_MAX * 2 * M_PI;
    dx = (rand() / (double)RAND_MAX) * (x_max - x_min) + x_min;
    dy = (rand() / (double)RAND_MAX) * (y_max - y_min) + y_min;
    dz = (rand() / (double)RAND_MAX) * (z_max - z_min) + z_min;

    printf("angle = %lf, dx = %lf, dy = %lf, dz = %lf\n", theta, dx, dy, dz);
    
    set_mat(t, 0, 0, cos(theta));
    set_mat(t, 0, 1, -sin(theta));
    set_mat(t, 1, 0, sin(theta));
    set_mat(t, 1, 1, cos(theta));
    set_mat(t, 0, 3, dx);
    set_mat(t, 1, 3, dy);
    set_mat(t, 2, 3, dz);

    // modèle
    res = mult_matrice(t, *modele);

    liberer_matrice(*modele);

    *modele = res;

    liberer_matrice(t);
}

void trans_rot_z_alea_2(matrice* modele, matrice* direction, double x_min, double x_max, double y_min, double y_max, double z_min, double z_max){
    matrice t = creer_identite(4);
    matrice res1, res2;
    double theta, dx, dy, dz;

    theta = rand() / (double)RAND_MAX * 2 * M_PI;
    dx = (rand() / (double)RAND_MAX) * (x_max - x_min) + x_min;
    dy = (rand() / (double)RAND_MAX) * (y_max - y_min) + y_min;
    dz = (rand() / (double)RAND_MAX) * (z_max - z_min) + z_min;

    printf("angle = %lf, dx = %lf, dy = %lf, dz = %lf\n", theta, dx, dy, dz);
    
    set_mat(t, 0, 0, cos(theta));
    set_mat(t, 0, 1, -sin(theta));
    set_mat(t, 1, 0, sin(theta));
    set_mat(t, 1, 1, cos(theta));
    set_mat(t, 0, 3, dx);
    set_mat(t, 1, 3, dy);
    set_mat(t, 2, 3, dz);

    // modèle
    res1 = mult_matrice(t, *modele);

    liberer_matrice(*modele);

    *modele = res1;

    // direction
    res2 = mult_matrice(t, *direction);
    
    liberer_matrice(*direction);

    *direction = res2;
    

    liberer_matrice(t);
}
