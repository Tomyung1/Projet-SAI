#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "headers/config.h"
#include "headers/objet.h"
#include "headers/poisson.h"
#include "headers/matrice.h"

poisson creer_poisson(){
    poisson p;

    p.o.etat = E_INITIAL;
    lire_modele("objets/poisson.obj", &p.o.modele, &p.o.hitbox, NULL);
    
    // Initialiser les paramètres de déplacement
    p.vitesse_normale = 0.001;  // Vitesse de base
    p.vitesse_fuite = 0.015;    // Vitesse de fuite (3x plus rapide)
    p.vitesse = p.vitesse_normale;

    p.direction = creer_matrice(4, 1);  // 4 ème coord = 0 pour empêcher les translations
    set_mat(p.direction, 0, 0, -1);
    
    // État initial
    p.etat_poisson = ETAT_NORMAL;
    p.temps_etat = 0;
    
    return p;
}

void liberer_poisson(poisson p){
    liberer_matrice(p.o.modele);
    liberer_matrice(p.o.hitbox);
    liberer_matrice(p.direction);
}


void deplacer_poisson(poisson *p) {
    double dx = get_mat((*p).direction, 0, 0) * (p->vitesse);
    double dy = get_mat((*p).direction, 1, 0) * (p->vitesse); 
    double dz = get_mat((*p).direction, 2, 0) * (p->vitesse);

    translation(&(p -> o.modele), dx, dy, dz);
    translation(&(p -> o.hitbox), dx, dy, dz);

    // Gestion du temps dans l'état actuel
    p->temps_etat++;
    
    // Changement aléatoire de direction
    if (p->etat_poisson == ETAT_NORMAL && (rand() % 500 == 0)) {
        changer_direction_poisson(p);
    }
    
    // Retour à l'état normal après environ 3 secondes de fuite
    if (p->etat_poisson == ETAT_FUITE && p->temps_etat > 180) { 
        mettre_en_normal(p);
        changer_direction_poisson(p); // Changer de direction après la fuite
    }

    // Gestion des limites du monde - rebond sur les bords
    double pos_x = get_mat(p->o.modele, 0, 0);
    double pos_y = get_mat(p->o.modele, 1, 0);
    double pos_z = get_mat(p->o.modele, 2, 0);
    
    if (pos_x > LIMITE_MAX_X - 5 || pos_x < LIMITE_MIN_X + 5) {
        set_mat(p->direction, 0, 0, -get_mat(p->direction, 0, 0));
    }
    
    if (pos_y > LIMITE_MAX_Y - 5 || pos_y < LIMITE_MIN_Y + 5) {
        set_mat(p->direction, 1, 0, -get_mat(p->direction, 1, 0));
    }
    
    if (pos_z > NIVEAU_MER - 1 || pos_z < NIVEAU_MER - 10) {
        set_mat(p->direction, 2, 0, -get_mat(p->direction, 2, 0));
    }
}


void tourner_poisson(poisson *p, double theta, char sens) {
    
    switch (sens){
    case 'g':   // gauche
        rotation_sur_place(&(p -> o.modele), theta, 'z');
        rotation_z(&(p -> direction), theta);
        break;
    case 'd':   // droite
        rotation_sur_place(&(p -> o.modele), -theta, 'z');
        rotation_z(&(p -> direction), -theta);
        break;
    case 'h':   // haut
        rotation_sur_place(&(p -> o.modele), theta, 'y');
        rotation_y(&(p -> direction), theta);
        break; 
    case 'b':   // bas
        rotation_sur_place(&(p -> o.modele), -theta, 'y');
        rotation_y(&(p -> direction), -theta);
        break;
    default:
        break;
    }
}

// Met le poisson en état de fuite
void mettre_en_fuite(poisson *p) {
    if (p->etat_poisson != ETAT_FUITE) {
        p->etat_poisson = ETAT_FUITE;
        p->vitesse = p->vitesse_fuite;
        p->temps_etat = 0;
        

    }
}

// Remet le poisson en état normal
void mettre_en_normal(poisson *p) {
    p->etat_poisson = ETAT_NORMAL;
    p->vitesse = p->vitesse_normale;
    p->temps_etat = 0;
}
// Change aléatoirement la direction du poisson
void changer_direction_poisson(poisson *p) {
    double theta_y = (rand() / (double)RAND_MAX) * 2 * M_PI; // Rotation horizontale
    double theta_x = (rand() / (double)RAND_MAX - 0.5) * M_PI/3; // Rotation verticale limitée
    
    // Nouvelle direction normalisée
    set_mat(p->direction, 0, 0, cos(theta_y) * cos(theta_x));
    set_mat(p->direction, 1, 0, sin(theta_y) * cos(theta_x));
    set_mat(p->direction, 2, 0, sin(theta_x));
}

// Inverse la direction du poisson (pour les collisions)
void inverser_direction_poisson(poisson *p) {
    set_mat(p->direction, 0, 0, -get_mat(p->direction, 0, 0));
    set_mat(p->direction, 1, 0, -get_mat(p->direction, 1, 0));
    set_mat(p->direction, 2, 0, -get_mat(p->direction, 2, 0));
}
