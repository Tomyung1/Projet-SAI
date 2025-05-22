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
    matrice direction;
    int etat_poisson;    // État du poisson (NORMAL ou FUITE)
    int temps_etat;      // Compteur pour gérer la durée de l'état
} poisson;

poisson creer_poisson();
void liberer_poisson(poisson p);
void deplacer_poisson(poisson *p);
void tourner_poisson(poisson *p, double theta, char sens);
// void changer_direction_poisson(poisson *p);
void mettre_en_fuite(poisson *p);
void mettre_en_normal(poisson *p);
void changer_direction_poisson(poisson *p);
void inverser_direction_poisson(poisson *p);

#endif /* _POISSON_H_ */
