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
    p.o.modele = lire_modele("objets/poisson.obj");
    
    // Initialiser les paramètres de déplacement
    p.vitesse_normale = 0.005;  // Vitesse de base
    p.vitesse_fuite = 0.015;    // Vitesse de fuite (3x plus rapide)
    p.vitesse = p.vitesse_normale;
    
    // Direction initiale aléatoire
    p.direction_x = (double)rand() / RAND_MAX * 2.0 - 1.0;  // Entre -1 et 1
    p.direction_y = (double)rand() / RAND_MAX * 2.0 - 1.0;  // Entre -1 et 1
    p.direction_z = ((double)rand() / RAND_MAX * 0.4 - 0.2); // Léger mouvement vertical
    
    // Normaliser le vecteur direction
    double longueur = sqrt(p.direction_x * p.direction_x + 
                         p.direction_y * p.direction_y + 
                         p.direction_z * p.direction_z);
    
    p.direction_x /= longueur;
    p.direction_y /= longueur;
    p.direction_z /= longueur;
    
    
    // État initial
    p.etat_poisson = ETAT_NORMAL;
    p.temps_etat = 0;
    
    return p;
}

void liberer_poisson(poisson p){
    liberer_matrice(p.o.modele);
}


void deplacer_poisson(poisson *p) {
    double dx = p->direction_x * p->vitesse;
    double dy = p->direction_y * p->vitesse; 
    double dz = p->direction_z * p->vitesse;

    translation(&(p -> o.modele), dx, dy, dz);
}


// Déplace le poisson selon sa direction et sa vitesse
void deplacer_poisson_old(poisson *p) {
    // Calculer le déplacement
    double dt = 1.0/60.0; // Assuming 60 FPS or use actual elapsed time 
    double dx = p->direction_x * p->vitesse * dt;
    double dy = p->direction_y * p->vitesse * dt; 
    double dz = p->direction_z * p->vitesse * dt;
    
    // Appliquer le déplacement
    translation(&p->o.modele, dx, dy, dz);
    
    // Vérifier les limites
    double x_pos = 0, y_pos = 0, z_pos = 0;
    
    // On récupère la position du premier point du modèle comme référence
    x_pos = get_mat(p->o.modele, 0, 0);
    y_pos = get_mat(p->o.modele, 1, 0);
    z_pos = get_mat(p->o.modele, 2, 0);
    
    // Limites de la zone de jeu
    double limite_x = 20.0;
    double limite_y = 20.0;
    double limite_z_sup = NIVEAU_MER - 1.0;
    double limite_z_inf = NIVEAU_MER - 8.0;
    
    // Si le poisson atteint une limite, on change sa direction
    if (x_pos > limite_x || x_pos < -limite_x) {
        p->direction_x = -p->direction_x;
        // Rotation pour faire face à la nouvelle direction
        double angle = atan2(p->direction_y, p->direction_x);
        rotation_sur_place(&p->o.modele, angle - M_PI/2, 'z'); // Ajuster selon votre modèle
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
}

// Change aléatoirement la direction du poisson
void changer_direction_poisson(poisson *p) {
    p->direction_x = (double)rand() / RAND_MAX * 2.0 - 1.0;
    p->direction_y = (double)rand() / RAND_MAX * 2.0 - 1.0;
    p->direction_z = ((double)rand() / RAND_MAX * 0.4 - 0.2); // Mouvement vertical limité
    
    // Normaliser
    double longueur = sqrt(p->direction_x * p->direction_x + 
                          p->direction_y * p->direction_y + 
                          p->direction_z * p->direction_z);
    
    p->direction_x /= longueur;
    p->direction_y /= longueur;
    p->direction_z /= longueur;
    
    // Rotation pour faire face à la nouvelle direction
    double angle = atan2(p->direction_y, p->direction_x);
    rotation_z(&p->o.modele, angle - M_PI/2); // Ajuster selon votre modèle
}

// Met le poisson en état de fuite
void mettre_en_fuite(poisson *p) {
    if (p->etat_poisson != ETAT_FUITE) {
        p->etat_poisson = ETAT_FUITE;
        p->vitesse = p->vitesse_fuite;
        p->temps_etat = 0;
        
        // Optionnellement, changer de direction pour fuir
        changer_direction_poisson(p);
    }
}

// Remet le poisson en état normal
void mettre_en_normal(poisson *p) {
    p->etat_poisson = ETAT_NORMAL;
    p->vitesse = p->vitesse_normale;
    p->temps_etat = 0;
}
