#ifndef _BATEAU_H_
#define _BATEAU_H_

#include "objet.h"
#include "obstacle.h"  
/* MACRO etat */


typedef struct {
    objet o;
    double vitesse;
    matrice direction;
    matrice hitbox_canne;
    int score_poissons; // nombre de poissons pêchés
} bateau;

bateau creer_bateau();
void liberer_bateau(bateau b);
void deplacer_bateau(bateau *b);
void petit_retour_arrière(bateau *b);
void tourner_bateau(bateau *b, double theta, char sens);
void eviter_obstacles_bateau(bateau *b, obstacle obstacles[], int nb_obstacles);
void changer_direction_bateau(bateau *b, double angle);
int detecter_obstacle_devant(bateau *b, obstacle obstacles[], int nb_obstacles);

#endif /* _BATEAU_H_ */
