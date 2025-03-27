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

matrice lire_modele(char * chemin){
    FILE * f = NULL;
    int nb; /* nombre de sommets */
    double x, y, z;
    int i;
    matrice modele;

    if ((f = fopen(chemin, "r")) == NULL){
        printf("Impossible de charger le modèle 3D");
        exit(EXIT_FAILURE);
    }

    if (fscanf(f, "%d\n", &nb) != 1){
        printf("Erreur lecture du nombre de sommet du modèle 3D\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    modele = creer_matrice(4, nb);
    
    for (i = 0; i < nb; i++){
        if (fscanf(f, "%lf,%lf,%lf\n", &x, &y, &z) != 3){
            printf("Échec lecture coordonnées du modèle 3D\n");
            liberer_matrice(modele);
            exit(EXIT_FAILURE);
        }
        set_mat(modele, 0, i, x);
        set_mat(modele, 1, i, y);
        set_mat(modele, 2, i, z);
        set_mat(modele, 3, i, 1);
    }
    

    fclose(f);

    return modele;
}