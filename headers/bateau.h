#ifndef _BATEAU_H_
#define _BATEAU_H_

#include "objet.h"

/* MACRO etat */


typedef struct {
    objet o;
    double vitesse;
    matrice direction;
    matrice hitbox_canne;
} bateau;

bateau creer_bateau();
void liberer_bateau(bateau b);
void deplacer_bateau(bateau *b);
void tourner_bateau(bateau *b, double theta, char sens);

#endif /* _BATEAU_H_ */