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
    lire_modele("objets/poisson.obj", &p.o.modele, &p.o.hitbox);
    
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

    /* ... voir les limites dans headers/config.h

    // Si le poisson atteint une limite, on change sa direction
    if (x_pos > limite_x || x_pos < -limite_x) {
        p->direction_x = -p->direction_x;
        // Rotation pour faire face à la nouvelle direction
        double angle = atan2(p->direction_y, p->direction_x);
        rotation_z(&p->o.modele, angle - M_PI/2); // Ajuster selon votre modèle
    }
    
    if (y_pos > limite_y || y_pos < -limite_y) {
        p->direction_y = -p->direction_y;
        // Rotation pour faire face à la nouvelle direction
        double angle = atan2(p->direction_y, p->direction_x);
        rotation_z(&p->o.modele, angle - M_PI/2); // Ajuster selon votre modèle
    }
    
    if (z_pos > limite_z_sup || z_pos < limite_z_inf) {
        p->direction_z = -p->direction_z;
    }
    
    // Gestion du temps dans l'état actuel
    p->temps_etat++;
    
    // Changement aléatoire de direction tous les ~100 déplacements en état normal
    if (p->etat_poisson == ETAT_NORMAL && (rand() % 200 == 0)) {
    changer_direction_poisson(p);
}
    
    // Retour à l'état normal après environ 5 secondes de fuite
    if (p->etat_poisson == ETAT_FUITE && p->temps_etat > 300) { // ~5 secondes à 60 FPS
        mettre_en_normal(p);
    }

    */
}


// /!\ combinaison de rotation étrange (alignement devrait résoudre)
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
        
        // Optionnellement, changer de direction pour fuir
        // ... fonction pas aléatoire
    }
}

// Remet le poisson en état normal
void mettre_en_normal(poisson *p) {
    p->etat_poisson = ETAT_NORMAL;
    p->vitesse = p->vitesse_normale;
    p->temps_etat = 0;
}
