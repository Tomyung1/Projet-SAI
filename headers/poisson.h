#ifndef _POISSON_H_
#define _POISSON_H_

#include "objet.h"

// États possibles pour le poisson
#define ETAT_NORMAL 0
#define ETAT_FUITE 1

typedef struct {
    objet o;
    double vitesse;      // Vitesse actuelle du poisson
    double vitesse_normale; // Vitesse en état normal
    double vitesse_fuite;   // Vitesse en état de fuite
    double direction_x;   // Direction en X
    double direction_y;   // Direction en Y
    double direction_z;   // Direction en Z (pour la profondeur)
    int etat_poisson;    // État du poisson (NORMAL ou FUITE)
    int temps_etat;      // Compteur pour gérer la durée de l'état
} poisson;

poisson creer_poisson();
void liberer_poisson(poisson p);
void deplacer_poisson(poisson *p);
void changer_direction_poisson(poisson *p);
void mettre_en_fuite(poisson *p);
void mettre_en_normal(poisson *p);

#endif /* _POISSON_H_ */
