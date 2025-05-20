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


matrice copier_matrice(matrice m){
    matrice res = creer_matrice(m -> n, m -> m);
    int i, j;

    for (i = 0; i < m -> n; i++){
        for (j = 0; j < m -> m; j++){
            set_mat(res, i, j, get_mat(m, i, j));
        }
    }
    
    return res;
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


/* vvvvvvvvvv NE MARCHE PAS vvvvvvvvvv */


// Retourne le vecteur direction dans l'octant positif
// Si negatif_x est == -1, alors il a été nécessaire de changer le signe
// des x dans le vecteur direction, 1 sinon. pareil pour y et z.
void octant_negatif(matrice* modele, matrice* direction, matrice* hitbox, int* negatif_x, int* negatif_y, int* negatif_z){
  double x, y, z;

  x = get_mat(*direction, 0, 0);
  if (x < 0){
      printf("### 1\n");
      *negatif_x = -1;
      rotation_z(modele, M_PI);
      rotation_z(direction, M_PI);
      rotation_z(hitbox, M_PI);
  } else {
      *negatif_x = 1;
  }
  
  y = get_mat(*direction, 1, 0);
  if (y < 0){
      printf("### 2\n");
      *negatif_y = -1;
      rotation_x(modele, M_PI);
      rotation_x(direction, M_PI);
      rotation_x(hitbox, M_PI);
  } else {
      *negatif_y = 1;
  }
  
  z = get_mat(*direction, 2, 0);
  if (z < 0){
      printf("### 3\n");
      *negatif_z = -1;
      rotation_y(modele, M_PI);
      rotation_y(direction, M_PI);
      rotation_y(hitbox, M_PI);
  } else {
      *negatif_z = 1;
  }
}


void octant_negatif_inverse(matrice* modele, matrice* direction, matrice* hitbox, int negatif_x, int negatif_y, int negatif_z){

    if (negatif_z == -1){
        printf("### 4\n");
        rotation_y(modele, M_PI);
        rotation_y(direction, M_PI);
        rotation_y(hitbox, M_PI);
    }

    if (negatif_y == -1){
        printf("### 5\n");
        rotation_x(modele, M_PI);
        rotation_x(direction, M_PI);
        rotation_x(hitbox, M_PI);
    }

    if (negatif_x == -1){
        printf("### 6\n");
        rotation_z(modele, M_PI);
        rotation_z(direction, M_PI);
        rotation_z(hitbox, M_PI);
    }
}


// ATTENTION, après cette fonction, rotation 'd' -> 'h','g' -> 'b', 'h' -> 'd', 'b' -> 'g'
// ATTETION, FAUX, ROTATION SUR LES X NECESSAIRE SINON ACCUMULE DES ERREURS
// aligne le modèle avec l'axe des x (attention, le résultat n'est pas forcément sur l'axe)
void alignement_x(matrice* modele, matrice* direction, matrice* hitbox, double *theta_x, double *theta_y, int* negatif_x, int* negatif_y, int* negatif_z){
  double x, y, z; // vecteur de direction
  
  octant_negatif(modele, direction, hitbox, negatif_x, negatif_y, negatif_z);

  printf("%d, %d, %d\n", *negatif_x, *negatif_y, *negatif_z);
  
  *theta_x = 0;
  *theta_y = 0;
  
  x = get_mat(*direction, 0, 0);
  y = get_mat(*direction, 1, 0);
  z = get_mat(*direction, 2, 0);

  printf("taille   : %lf\n", sqrt(x * x + y * y + z * z));
  
  // alignement sur le plan 0x 0z
  *theta_x = atan(y/z);  // trigo sur le plan 0y 0z

  rotation_x(modele, *theta_x);
  rotation_x(direction, *theta_x);
  rotation_x(hitbox, *theta_x);
  
  // alignement sur x
  x = get_mat(*direction, 0, 0);
  z = get_mat(*direction, 2, 0);
  
  *theta_y = acos(x); // trigo sur 0x 0z après Rx(theta_x)
  
  rotation_y(modele, *theta_y);
  rotation_y(direction, *theta_y);
  rotation_y(hitbox, *theta_y);
  
  printf("%lf, %lf\n", *theta_x, *theta_y);
}

// annule l'alignement en fonction des paramètres récupérer
void alignement_x_inverse(matrice* modele, matrice* direction, matrice* hitbox, double theta_x, double theta_y, int negatif_x, int negatif_y, int negatif_z){
    rotation_y(modele, -theta_y);
    rotation_y(direction, -theta_y);
    rotation_y(hitbox, -theta_y);
    
    rotation_x(modele, -theta_x);
    rotation_x(direction, -theta_x);
    rotation_x(hitbox, -theta_x);

    octant_negatif_inverse(modele, direction, hitbox, negatif_x, negatif_y, negatif_z);
}


/* ^^^^^^^^^^ NE MARCHE PAS ^^^^^^^^^^ */


void rotation_sur_place(matrice* modele, double theta, char axe){
    double px, py, pz;

    px = get_mat(*modele, 0, 0);
    py = get_mat(*modele, 1, 0);
    pz = get_mat(*modele, 2, 0);

    // translation sur (0, 0, 0)
    translation(modele, -px, -py, -pz);

    // rotation ( /!\ problème si composition )
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
    
    // déplacement à son point d'origine
    translation(modele, px, py, pz);
}


void agrandissement(matrice* modele, double facteur_x, double facteur_y, double facteur_z){
    matrice t = creer_identite(4);
    matrice res;

    set_mat(t, 0, 0, facteur_x);
    set_mat(t, 1, 1, facteur_y);
    set_mat(t, 2, 2, facteur_z);
    
    res = mult_matrice(t, *modele);

    liberer_matrice(*modele);

    *modele = res;

    liberer_matrice(t);
}


void trans_rot_z_alea_tout(matrice* modele, matrice* direction, matrice* hitbox, matrice* hitbox2, double x_min, double x_max, double y_min, double y_max, double z_min, double z_max){
    matrice t = creer_identite(4);
    matrice res;
    double theta, dx, dy, dz;

    theta = rand() / (double)RAND_MAX * 2 * M_PI;
    dx = (rand() / (double)RAND_MAX) * (x_max - x_min) + x_min;
    dy = (rand() / (double)RAND_MAX) * (y_max - y_min) + y_min;
    dz = (rand() / (double)RAND_MAX) * (z_max - z_min) + z_min;

    set_mat(t, 0, 0, cos(theta));
    set_mat(t, 0, 1, -sin(theta));
    set_mat(t, 1, 0, sin(theta));
    set_mat(t, 1, 1, cos(theta));
    set_mat(t, 0, 3, dx);
    set_mat(t, 1, 3, dy);
    set_mat(t, 2, 3, dz);

    // modèle
    if (modele != NULL){
        res = mult_matrice(t, *modele);

        liberer_matrice(*modele);

        *modele = res;
    }
        
    // direction
    if (direction != NULL){
        res = mult_matrice(t, *direction);
    
        liberer_matrice(*direction);

        *direction = res;
    }

    // hitbox
    set_mat(t, 0, 0, 1);   // pas de rotation
    set_mat(t, 0, 1, 0);
    set_mat(t, 1, 0, 0);
    set_mat(t, 1, 1, 1);
    
    if (hitbox != NULL){
        res = mult_matrice(t, *hitbox);
        
        liberer_matrice(*hitbox);
        
        *hitbox = res;
    }

    if (hitbox2 != NULL){
        res = mult_matrice(t, *hitbox2);

        liberer_matrice(*hitbox2);

        *hitbox2 = res;
    }

    liberer_matrice(t);
}