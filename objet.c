#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/objet.h"
#include "headers/matrice.h"



/* lit un fichier .obj, puis crée et initialise la matrice modele
   format fichier .obj ( /!\ ordre important des sommets pour l'affichage) :
   nb_sommet
   x,y,z
   x',y',z'
   ...

   ordonné comme les 8-arbres pour les pavés droits
 */

void lire_modele(char * chemin, matrice *modele, matrice *hitbox, matrice *hitbox2){
    FILE * f = NULL;
    int n; /* nombre de sommets */
    double x, y, z;
    int i, f1, f2, f3;

    if ((f = fopen(chemin, "r")) == NULL){
        printf("Impossible de charger le modèle 3D\n");
        exit(EXIT_FAILURE);
    }

    if (fscanf(f, "%d\n", &n) != 1){
        printf("Erreur lecture du nombre de sommet du modèle 3D\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    /* poistion + n points */
    *modele = creer_matrice(4, n + 1);

    /* La position */
    set_mat(*modele, 0, 0, 0);
    set_mat(*modele, 1, 0, 0);
    set_mat(*modele, 2, 0, 0);
    set_mat(*modele, 3, 0, 1);
    
    /* Les points */
    for (i = 1; i < n + 1; i++){
        if (fscanf(f, "%lf,%lf,%lf\n", &x, &y, &z) != 3){
            printf("Échec lecture coordonnées du modèle 3D\n");
            liberer_matrice(*modele);
            exit(EXIT_FAILURE);
        }
        set_mat(*modele, 0, i, x);
        set_mat(*modele, 1, i, y);
        set_mat(*modele, 2, i, z);
        set_mat(*modele, 3, i, 1);
    }


    /* on ignore les règles pour les facettes */
    if (fscanf(f, "%d\n", &n) != 1){
        fprintf(stderr, "Erreur lecture modèle 3D\n");
        fclose(f);
        liberer_matrice(*modele);
        exit(EXIT_FAILURE);        
    }
    for (i = 0; i < n; i++){
        if (fscanf(f, "%d,%d,%d\n", &f1, &f2, &f3) != 3){
            fprintf(stderr, "Erreur lecture modèle 3D\n");
            fclose(f);
            liberer_matrice(*modele);
            exit(EXIT_FAILURE);
        }
    }

    /* lecture de la hitbox */
    if (fscanf(f, "%d\n", &n) != 1){  // nombre de points pour la hitbox (2 pour un pavé)
        fprintf(stderr, "Erreur lecture modèle 3D\n");
        fclose(f);
        exit(EXIT_FAILURE);        
    }
    
    *hitbox = creer_matrice(4, n);
    
    for (i = 0; i < n; i++){
        if (fscanf(f, "%lf,%lf,%lf\n", &x, &y, &z) != 3){
            fprintf(stderr, "Erreur lecture modèle 3D\n");
            fclose(f);
            liberer_matrice(*modele);
            liberer_matrice(*hitbox);
            exit(EXIT_FAILURE);
        }
        set_mat(*hitbox, 0, i, x);
        set_mat(*hitbox, 1, i, y);
        set_mat(*hitbox, 2, i, z);
        set_mat(*hitbox, 3, i, 1);
    }

    /* lecture d'une deuxième hitbox si elle existe*/

    if (fscanf(f, "%d\n", &n) == 1){ // nombre de points pour la deuxième hitbox (2 pour un pavé)
        *hitbox2 = creer_matrice(4, n);

        for (i = 0; i < n; i++) {
            if (fscanf(f, "%lf,%lf,%lf\n", &x, &y, &z) != 3){
                fprintf(stderr, "Erreur lecture modèle 3D\n");
                fclose(f);
                liberer_matrice(*modele);
                liberer_matrice(*hitbox);
                liberer_matrice(*hitbox2);
                exit(EXIT_FAILURE);
            }
            set_mat(*hitbox2, 0, i, x);
            set_mat(*hitbox2, 1, i, y);
            set_mat(*hitbox2, 2, i, z);
            set_mat(*hitbox2, 3, i, 1);
        }
    }
    
    
    fclose(f);
}