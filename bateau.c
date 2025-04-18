#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/objet.h"
#include "headers/bateau.h"
#include "headers/matrice.h"
#include <math.h>

bateau creer_bateau(){
    bateau b;
    
    b.o.etat = E_INITIAL;

    b.o.modele = lire_modele("objets/bateau.obj");

    b.vitesse = 0.005;
    
    b.direction = creer_matrice(4, 1);
    set_mat(b.direction, 0, 0, -1);
    
    return b;
}


void liberer_bateau(bateau b){
    liberer_matrice(b.o.modele);
}

void deplacer_bateau(bateau *b){
    double dx = get_mat((*b).direction, 0, 0) * (b -> vitesse);
    double dy = get_mat((*b).direction, 1, 0) * (b -> vitesse);
    double dz = get_mat((*b).direction, 2, 0) * (b -> vitesse);


    
    translation(&(b -> o.modele), dx, dy, dz);
}

void tourner_bateau(bateau *b, char sens){
    double theta;
    
    switch (sens){
    case 'g': // gauche
        theta = M_PI / 4096;
        break;
    case 'd': // droite
        theta = - M_PI / 4096;
        break;
    default:
        theta = 0;
        break;
    }

    rotation_sur_place(&(b -> o.modele), theta, 'z');
    rotation_sur_place(&(b -> direction), theta, 'z');
}