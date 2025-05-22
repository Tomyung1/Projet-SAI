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
    rotation_sur_place(&(b -> o.hitbox), t, 'z');
    rotation_sur_place(&(b -> hitbox_canne), t, 'z');
}
// Détecte s'il y a un obstacle dans la direction du bateau
int detecter_obstacle_devant(bateau *b, obstacle obstacles[], int nb_obstacles) {
    int i;
    double bateau_x = get_mat(b->o.modele, 0, 0);
    double bateau_y = get_mat(b->o.modele, 1, 0);
  
    
    double dir_x = get_mat(b->direction, 0, 0);
    double dir_y = get_mat(b->direction, 1, 0);
    
    // Distance de détection devant le bateau
    double distance_detection = 15.0;
    
    for (i = 0; i < nb_obstacles; i++) {
        double obs_x = get_mat(obstacles[i].o.modele, 0, 0);
        double obs_y = get_mat(obstacles[i].o.modele, 1, 0);
        
        
        // Vecteur du bateau vers l'obstacle
        double vers_obs_x = obs_x - bateau_x;
        double vers_obs_y = obs_y - bateau_y;
        double distance_obs = sqrt(vers_obs_x * vers_obs_x + vers_obs_y * vers_obs_y);
        
        // Si l'obstacle est dans la zone de détection
        if (distance_obs < distance_detection) {
            // Calculer le produit scalaire pour voir si l'obstacle est devant
            double produit_scalaire = (vers_obs_x * dir_x + vers_obs_y * dir_y);
            
            // Si le produit scalaire est positif, l'obstacle est dans la direction du bateau
            if (produit_scalaire > 0) {
                // Calculer l'angle entre la direction et l'obstacle
                double angle_vers_obs = atan2(vers_obs_y, vers_obs_x);
                double angle_bateau = atan2(dir_y, dir_x);
                double diff_angle = fabs(angle_vers_obs - angle_bateau);
                
                // Normaliser l'angle
                if (diff_angle > M_PI) diff_angle = 2 * M_PI - diff_angle;
                
                // Si l'obstacle est dans un cône de 60° devant le bateau
                if (diff_angle < M_PI/3) {
                    return i; // Retourne l'index de l'obstacle détecté
                }
            }
        }
    }
    return -1; // Aucun obstacle détecté
}

// Change la direction du bateau d'un angle donné
void changer_direction_bateau(bateau *b, double angle) {
    rotation_z(&(b->direction), angle);
    rotation_sur_place(&(b->o.modele), angle, 'z');
}

// Fonction principale d'évitement d'obstacles
void eviter_obstacles_bateau(bateau *b, obstacle obstacles[], int nb_obstacles) {
    static int temps_evitement[NB_BATEAUX] = {0}; // Compteur pour chaque bateau
    static int bateau_id = 0; // Simple compteur pour identifier le bateau
    
    // Réduire le temps d'évitement
    if (temps_evitement[bateau_id] > 0) {
        temps_evitement[bateau_id]--;
        bateau_id = (bateau_id + 1) % NB_BATEAUX;
        return;
    }
    
    int obstacle_index = detecter_obstacle_devant(b, obstacles, nb_obstacles);
    
    if (obstacle_index != -1) {
        printf("Bateau %d évite l'iceberg %d!\n", bateau_id, obstacle_index);
        
        // Calculer quelle direction est la meilleure pour éviter
        double bateau_x = get_mat(b->o.modele, 0, 0);
        double bateau_y = get_mat(b->o.modele, 1, 0);
        double obs_x = get_mat(obstacles[obstacle_index].o.modele, 0, 0);
        double obs_y = get_mat(obstacles[obstacle_index].o.modele, 1, 0);
        
        double dir_x = get_mat(b->direction, 0, 0);
        double dir_y = get_mat(b->direction, 1, 0);
        
        // Vecteur perpendiculaire à la direction actuelle (pour aller à gauche ou droite)
        double perp_x = -dir_y;  // Perpendiculaire à gauche
        double perp_y = dir_x;
        
        // Vecteur du bateau vers l'obstacle
        double vers_obs_x = obs_x - bateau_x;
        double vers_obs_y = obs_y - bateau_y;
        
        // Déterminer de quel côté tourner (produit vectoriel)
        double cross_product = vers_obs_x * perp_y - vers_obs_y * perp_x;
        
        // Angle de rotation (45 degrés)
        double angle_evitement = M_PI/4;
        
        if (cross_product > 0) {
            // Tourner à droite
            changer_direction_bateau(b, -angle_evitement);
        } else {
            // Tourner à gauche  
            changer_direction_bateau(b, angle_evitement);
        }
        
        // Définir une durée d'évitement (le bateau gardera cette direction pendant un moment)
        temps_evitement[bateau_id] = 120; 
    }
    
    bateau_id = (bateau_id + 1) % NB_BATEAUX;
}
