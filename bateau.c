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
int detecter_obstacle_devant(bateau *b, obstacle *obstacles, int nb_obstacles) {
    int i;
    double bateau_x = get_mat(b->o.modele, 0, 0);
    double bateau_y = get_mat(b->o.modele, 1, 0);
    
    double dir_x = get_mat(b->direction, 0, 0);
    double dir_y = get_mat(b->direction, 1, 0);
    
    // Distance de détection réduite pour éviter la détection trop précoce
    double distance_detection = 12.0;
    
    for (i = 0; i < nb_obstacles; i++) {
        double obs_x = get_mat(obstacles[i].o.modele, 0, 0);
        double obs_y = get_mat(obstacles[i].o.modele, 1, 0);
        
        double vers_obs_x = obs_x - bateau_x;
        double vers_obs_y = obs_y - bateau_y;
        double distance_obs = sqrt(vers_obs_x * vers_obs_x + vers_obs_y * vers_obs_y);
        
        if (distance_obs < distance_detection) {
            double produit_scalaire = (vers_obs_x * dir_x + vers_obs_y * dir_y);
            
            if (produit_scalaire > 0) {
                double angle_vers_obs = atan2(vers_obs_y, vers_obs_x);
                double angle_bateau = atan2(dir_y, dir_x);
                double diff_angle = fabs(angle_vers_obs - angle_bateau);
                
                if (diff_angle > M_PI) diff_angle = 2 * M_PI - diff_angle;
                
               
                if (diff_angle < M_PI/4) { 
                    return i;
                }
            }
        }
    }
    return -1;
}
// Change la direction du bateau d'un angle donné
void changer_direction_bateau(bateau *b, double angle) {
    rotation_z(&(b->direction), angle);
    rotation_sur_place(&(b->o.modele), angle, 'z');
}

// Fonction principale d'évitement d'obstacles 
void eviter_obstacles_bateau(bateau *b, obstacle *obstacles, int nb_obstacles) {
    static int temps_evitement[NB_BATEAUX] = {0}; 
    static double direction_originale_x[NB_BATEAUX] = {0};
    static double direction_originale_y[NB_BATEAUX] = {0};
    static int phase_evitement[NB_BATEAUX] = {0}; // 0: normal, 1: contournement, 2: retour
    static int bateau_id = 0;
    
    // Vérifier les limites du monde 
    double bateau_x = get_mat(b->o.modele, 0, 0);
    double bateau_y = get_mat(b->o.modele, 1, 0);
    
    double marge = 10.0;
    
    // Gestion des limites du monde 
    if (bateau_x > LIMITE_MAX_X - marge || bateau_x < LIMITE_MIN_X + marge ||
        bateau_y > LIMITE_MAX_Y - marge || bateau_y < LIMITE_MIN_Y + marge) {
        
        double vers_centre_x = -bateau_x;
        double vers_centre_y = -bateau_y;
        double longueur = sqrt(vers_centre_x * vers_centre_x + vers_centre_y * vers_centre_y);
        
        if (longueur > 0) {
            set_mat(b->direction, 0, 0, vers_centre_x / longueur);
            set_mat(b->direction, 1, 0, vers_centre_y / longueur);
            temps_evitement[bateau_id] = 60;
            phase_evitement[bateau_id] = 0; 
        }
        
        bateau_id = (bateau_id + 1) % NB_BATEAUX;
        return;
    }
    
    // Si on est en cours d'évitement
    if (temps_evitement[bateau_id] > 0) {
        temps_evitement[bateau_id]--;
        
        // Phase de contournement
        if (phase_evitement[bateau_id] == 1) {
            // Continuer à tourner graduellement pendant le contournement
            double petit_angle = M_PI / 180.0; 
            double dir_actuelle_x = get_mat(b->direction, 0, 0);
            double dir_actuelle_y = get_mat(b->direction, 1, 0);
            
            // Rotation graduelle
            double nouveau_x = dir_actuelle_x * cos(petit_angle) - dir_actuelle_y * sin(petit_angle);
            double nouveau_y = dir_actuelle_x * sin(petit_angle) + dir_actuelle_y * cos(petit_angle);
            
            set_mat(b->direction, 0, 0, nouveau_x);
            set_mat(b->direction, 1, 0, nouveau_y);
            
            // Passer à la phase de retour après la moitié du temps
            if (temps_evitement[bateau_id] <= 45) {
                phase_evitement[bateau_id] = 2;
            }
        }
        // Phase de retour vers la direction originale
        else if (phase_evitement[bateau_id] == 2) {
            // Interpoler progressivement vers la direction originale
            double dir_actuelle_x = get_mat(b->direction, 0, 0);
            double dir_actuelle_y = get_mat(b->direction, 1, 0);
            
            double facteur = 0.05; // Vitesse de retour vers la direction originale
            double nouveau_x = dir_actuelle_x + (direction_originale_x[bateau_id] - dir_actuelle_x) * facteur;
            double nouveau_y = dir_actuelle_y + (direction_originale_y[bateau_id] - dir_actuelle_y) * facteur;
            
            // Normaliser
            double longueur = sqrt(nouveau_x * nouveau_x + nouveau_y * nouveau_y);
            if (longueur > 0) {
                set_mat(b->direction, 0, 0, nouveau_x / longueur);
                set_mat(b->direction, 1, 0, nouveau_y / longueur);
            }
        }
        
        // Fin de l'évitement
        if (temps_evitement[bateau_id] == 0) {
            phase_evitement[bateau_id] = 0; // Retour à la normale
        }
        
        bateau_id = (bateau_id + 1) % NB_BATEAUX;
        return;
    }
    
    // Détection d'obstacle
    int obstacle_index = detecter_obstacle_devant(b, obstacles, nb_obstacles);
    
    if (obstacle_index != -1) {
        printf("Bateau %d commence l'évitement de l'iceberg %d!\n", bateau_id, obstacle_index);
        
        // Sauvegarder la direction originale
        direction_originale_x[bateau_id] = get_mat(b->direction, 0, 0);
        direction_originale_y[bateau_id] = get_mat(b->direction, 1, 0);
        
        // Calculer de quel côté contourner 
        double obs_x = get_mat(obstacles[obstacle_index].o.modele, 0, 0);
        double obs_y = get_mat(obstacles[obstacle_index].o.modele, 1, 0);
        
        double vers_obs_x = obs_x - bateau_x;
        double vers_obs_y = obs_y - bateau_y;
        
        double dir_x = get_mat(b->direction, 0, 0);
        double dir_y = get_mat(b->direction, 1, 0);
        
        // Produit vectoriel pour déterminer le côté
        double cross_product = vers_obs_x * dir_y - vers_obs_y * dir_x;
        
        // Tourner immédiatement de 45° du bon côté
        double angle_initial = (cross_product > 0) ? M_PI/4 : -M_PI/4;
        changer_direction_bateau(b, angle_initial);
        
        // Démarrer la phase de contournement
        phase_evitement[bateau_id] = 1;
        temps_evitement[bateau_id] = 90; 
    }
    
    bateau_id = (bateau_id + 1) % NB_BATEAUX;
}