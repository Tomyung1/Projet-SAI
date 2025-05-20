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

    lire_modele("objets/bateau.obj", &b.o.modele, &b.o.hitbox, &b.hitbox_canne);

    b.vitesse = 0.005;
    
    b.direction = creer_matrice(4, 1);
    set_mat(b.direction, 0, 0, -1);
    
    return b;
}


void liberer_bateau(bateau b){
    liberer_matrice(b.o.modele);
    liberer_matrice(b.o.hitbox);
    liberer_matrice(b.direction);
    liberer_matrice(b.hitbox_canne);
}

void deplacer_bateau(bateau *b){
    double dx = get_mat((*b).direction, 0, 0) * (b -> vitesse);
    double dy = get_mat((*b).direction, 1, 0) * (b -> vitesse);
    double dz = get_mat((*b).direction, 2, 0) * (b -> vitesse);

    
    translation(&(b -> o.modele), dx, dy, dz);
    translation(&(b -> o.hitbox), dx, dy, dz);
    translation(&(b -> hitbox_canne), dx, dy, dz);
}

void tourner_bateau(bateau *b, double theta, char sens){
    double t;
    
    switch (sens){
    case 'g': // gauche
        t = theta;
        break;
    case 'd': // droite
        t = -theta;
        break;
    default:
        t = 0;
        break;
    }

    rotation_sur_place(&(b -> o.modele), t, 'z');
    rotation_sur_place(&(b -> direction), t, 'z');
}