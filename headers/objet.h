#ifndef _OBJET_H_
#define _OBJET_H_

#include "matrice.h"

/* états possibles */
#define E_INITIAL 0
#define E_NORMAL 1
#define E_ACTION 2
#define E_FIN 3

typedef struct {
    matrice modele; // colonne d'indice 0 = position (centre)
    int etat;
    matrice hitbox; // deux points définissent un pavé
} objet;

void lire_modele(char * chemin, matrice *modele, matrice *hitbox);

#endif /* _OBJET_H_ */
